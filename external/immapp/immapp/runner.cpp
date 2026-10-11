#include "immapp.h"
#include "vd_path/vd_path.h"

#ifdef IMGUI_BUNDLE_WITH_IMPLOT
#include "implot/implot.h"
#endif
#ifdef IMGUI_BUNDLE_WITH_IMPLOT3D
#include "implot3d/implot3d.h"
#endif
#ifdef IMGUI_BUNDLE_WITH_IMFILEDIALOG
#include "bundle_integration/ImFileDialogTextureHelper.h"
#endif

#ifdef IMGUI_BUNDLE_WITH_TEXT_INSPECT
#include "imgui_tex_inspect/imgui_tex_inspect.h"
#include "imgui_tex_inspect/backends/tex_inspect_opengl.h"
#endif
#include "hello_imgui/hello_imgui.h"
#include "hello_imgui/internal/functional_utils.h"
#ifdef IMGUI_BUNDLE_WITH_IMMVISION
#include "immvision/immvision.h"
#endif

#ifdef IMGUI_BUNDLE_WITH_IMGUI_NODE_EDITOR
std::function<void()> FnResetImGuiNodeEditorId; // may be bound from pybind_imgui_node_editor.cpp
void UpdateNodeEditorColorsFromImguiColors();
#endif

#include <chrono>
#include <cassert>
#include <filesystem>


// Private API used by ImGuiTexInspect (not mentioned in headers!)
namespace HelloImGui { std::string GlslVersion(); }


namespace ImmApp
{

    struct ImmAppContext
    {
#ifdef IMGUI_BUNDLE_WITH_IMPLOT
        ImPlotContext* ownedImPlot = nullptr;
#endif
#ifdef IMGUI_BUNDLE_WITH_IMPLOT3D
        ImPlot3DContext* ownedImPlot3D = nullptr;
#endif
        bool markdownInitialized = false;
#ifdef IMGUI_BUNDLE_WITH_IMGUI_NODE_EDITOR
        std::optional<ax::NodeEditor::EditorContext *> _NodeEditorContext;
        ax::NodeEditor::Config _NodeEditorConfig;
#endif

#ifdef IMGUI_BUNDLE_WITH_TEXT_INSPECT
        ImGuiTexInspect::Context * _ImGuiTextInspect_Context = nullptr;
#endif
    };

    ImmAppContext gImmAppContext;

    // Only one instance of `Renderer` can exist at a time
    // ---------------------------------------------------
    static int gRendererInstanceCount = 0;
    static AddOnsParams gAddOnsParamsAtSetup;

    static void Priv_TearDown();


    // Establish add-on ownership and install cleanup for both Hello exits; caller guards partial failure.
    static void Priv_Setup(HelloImGui::RunnerParams& runnerParams, const AddOnsParams& passedAddOnsParams, bool& acquired)
    {
        // Check if we are already running
#ifdef IMGUI_BUNDLE_BUILD_PYTHON
        // For python, we forgive the user for not calling TearDown() before calling Renderer()
        // since it might be due to a Python exception, which might be recoverable, if running
        // in a Python REPL, a notebook, or Pyodide.
        if (gRendererInstanceCount > 0)
        {
            printf("ImmApp: calling TearDown() prior to Setup (probable prior exception in Python).\n");
            Priv_TearDown();
        }
#else
        if (gRendererInstanceCount > 0)
        throw std::runtime_error("Only one instance of `ImmApp::Renderer` can exist at a time.");
#endif
        gAddOnsParamsAtSetup = passedAddOnsParams;
        AddOnsParams& addOnsParams = gAddOnsParamsAtSetup;
        gRendererInstanceCount++;
        acquired = true; // Only this setup attempt may release the newly established session.

        // App cleanup precedes add-on cleanup, and both precede Hello's context teardown.
        // A throwing app callback must not prevent owned add-ons from being released.
        auto withOwnedCleanup = [](HelloImGui::VoidFunction callback) {
            return [callback]() {
                std::exception_ptr failure;
                try { if (callback) callback(); } catch (...) { failure = std::current_exception(); }
                try { Priv_TearDown(); } catch (...) { if (!failure) failure = std::current_exception(); }
                if (failure) std::rethrow_exception(failure);
            };
        };
        runnerParams.callbacks.BeforeExit = withOwnedCleanup(runnerParams.callbacks.BeforeExit);
        runnerParams.callbacks.BeforeExit_OnException = withOwnedCleanup(runnerParams.callbacks.BeforeExit_OnException);

        // create implot context if required
#ifdef IMGUI_BUNDLE_WITH_IMPLOT
        if (addOnsParams.withImplot)
            gImmAppContext.ownedImPlot = ImPlot::CreateContext();
#endif
        // create implot3d context if required
#ifdef IMGUI_BUNDLE_WITH_IMPLOT3D
        if (addOnsParams.withImplot3d)
            gImmAppContext.ownedImPlot3D = ImPlot3D::CreateContext();
#endif

#ifdef IMGUI_BUNDLE_WITH_IMGUI_NODE_EDITOR
        // create imgui_node_editor context if required
        if (addOnsParams.withNodeEditor || addOnsParams.withNodeEditorConfig.has_value())
        {
            addOnsParams.withNodeEditor = true;
            if (addOnsParams.withNodeEditorConfig.has_value())
                gImmAppContext._NodeEditorConfig = addOnsParams.withNodeEditorConfig.value();

            // Replace settings file name if default
            if (gImmAppContext._NodeEditorConfig.SettingsFile == "NodeEditor.json")
            {
                gImmAppContext._NodeEditorConfig.SettingsFile = NodeEditorSettingsLocation(runnerParams);
            }

            gImmAppContext._NodeEditorContext = ax::NodeEditor::CreateEditor(&gImmAppContext._NodeEditorConfig);
            ax::NodeEditor::SetCurrentEditor(gImmAppContext._NodeEditorContext.value());

            runnerParams.callbacks.BeforeExit = HelloImGui::SequenceFunctions(
                FnResetImGuiNodeEditorId,
                runnerParams.callbacks.BeforeExit
            );

            // Update node editor colors from imgui colors
            auto fnUpdateNodeEditorColorsFromImguiColors = [&]
            {
                if (addOnsParams.updateNodeEditorColorsFromImguiColors)
                    UpdateNodeEditorColorsFromImguiColors();
            };
            // Once at startup
            runnerParams.callbacks.SetupImGuiStyle = HelloImGui::SequenceFunctions(
                runnerParams.callbacks.SetupImGuiStyle,
                fnUpdateNodeEditorColorsFromImguiColors
            );
            // Once every frame. We choose a relatively unused callback to avoid
            // situations where a user would forget to chain the callbacks.
            runnerParams.callbacks.BeforeImGuiRender = HelloImGui::SequenceFunctions(
                runnerParams.callbacks.BeforeImGuiRender,
                fnUpdateNodeEditorColorsFromImguiColors
            );
        }
#endif

        // load markdown fonts if needed
        if (addOnsParams.withMarkdown || addOnsParams.withMarkdownOptions.has_value())
        {
            if (!addOnsParams.withMarkdownOptions.has_value())
                addOnsParams.withMarkdownOptions = ImGuiMd::MarkdownOptions();
            ImGuiMd::InitializeMarkdown(addOnsParams.withMarkdownOptions.value());
            gImmAppContext.markdownInitialized = true;

            runnerParams.callbacks.LoadAdditionalFonts = HelloImGui::SequenceFunctions(
                runnerParams.callbacks.LoadAdditionalFonts,
                ImGuiMd::GetFontLoaderFunction());
        }

#ifdef IMGUI_BUNDLE_WITH_IMFILEDIALOG
        ImFileDialogSetupTextureLoader();
#endif

#ifdef IMGUI_BUNDLE_WITH_TEXT_INSPECT
        if (addOnsParams.withTexInspect)
        {
            // Modify post-init: Init ImGuiTexInspect
            {
                auto fn_ImGuiTextInspect_Init = [&runnerParams, &addOnsParams](){
                    if (runnerParams.rendererBackendType == HelloImGui::RendererBackendType::OpenGL3)
                    {
                        ImGuiTexInspect::ImplOpenGL3_Init(HelloImGui::GlslVersion().c_str());
                        ImGuiTexInspect::Init();
                        gImmAppContext._ImGuiTextInspect_Context = ImGuiTexInspect::CreateContext();
                    }
                    else
                    {
                        addOnsParams.withTexInspect = false;
                        fprintf(stderr, "ImGuiTexInspect is only supported with OpenGL renderer!");
                    }
                };
                runnerParams.callbacks.PostInit = HelloImGui::SequenceFunctions(
                    fn_ImGuiTextInspect_Init,
                    runnerParams.callbacks.PostInit
                );
            }

            // Modify before-exit: DeInit ImGuiTexInspect
            {
                auto fn_ImGuiTextInspect_DeInit = [&addOnsParams](){
                    if (gImmAppContext._ImGuiTextInspect_Context)
                    {
                        ImGuiTexInspect::Shutdown();
                        ImGuiTexInspect::DestroyContext(gImmAppContext._ImGuiTextInspect_Context);
                        gImmAppContext._ImGuiTextInspect_Context = nullptr;
                        ImGuiTexInspect::ImplOpenGl3_Shutdown();
                    }
                };
                runnerParams.callbacks.BeforeExit = HelloImGui::SequenceFunctions(
                    fn_ImGuiTextInspect_DeInit,
                    runnerParams.callbacks.BeforeExit
                );
                runnerParams.callbacks.BeforeExit_OnException = HelloImGui::SequenceFunctions(
                    fn_ImGuiTextInspect_DeInit,
                    runnerParams.callbacks.BeforeExit_OnException
                );
            }
        }
#endif

#ifdef IMGUI_BUNDLE_WITH_IMMVISION
        // Clear ImmVision cache, before OpenGl is uninitialized
        runnerParams.callbacks.BeforeExit = HelloImGui::SequenceFunctions(
            runnerParams.callbacks.BeforeExit,
            ImmVision::ClearTextureCache);
        runnerParams.callbacks.BeforeExit_OnException = HelloImGui::SequenceFunctions(
            runnerParams.callbacks.BeforeExit_OnException,
            ImmVision::ClearTextureCache);
#endif
    }

    // Idempotently release actual owned initialization stages, never another caller's context.
    static void Priv_TearDown()
    {
        if (gRendererInstanceCount == 0) return;
        gRendererInstanceCount = 0;
        std::exception_ptr failure;
        auto attempt = [&](auto&& action) {
            try { action(); } catch (...) { if (!failure) failure = std::current_exception(); }
        };
#ifdef IMGUI_BUNDLE_WITH_IMPLOT
        if (gImmAppContext.ownedImPlot)
        {
            auto owned = gImmAppContext.ownedImPlot;
            gImmAppContext.ownedImPlot = nullptr;
            attempt([&] { ImPlot::DestroyContext(owned); });
        }
#endif
#ifdef IMGUI_BUNDLE_WITH_IMPLOT3D
        if (gImmAppContext.ownedImPlot3D)
        {
            auto owned = gImmAppContext.ownedImPlot3D;
            gImmAppContext.ownedImPlot3D = nullptr;
            attempt([&] { ImPlot3D::DestroyContext(owned); });
        }
#endif
#ifdef IMGUI_BUNDLE_WITH_IMGUI_NODE_EDITOR
        if (gImmAppContext._NodeEditorContext.has_value())
        {
            auto owned = *gImmAppContext._NodeEditorContext;
            gImmAppContext._NodeEditorContext.reset();
            attempt([&] { ax::NodeEditor::DestroyEditor(owned); });
        }
#endif
        if (gImmAppContext.markdownInitialized)
        {
            gImmAppContext.markdownInitialized = false;
            attempt([&] { ImGuiMd::DeInitializeMarkdown(); });
        }
        if (failure) std::rethrow_exception(failure);
    }

    // Run one owned session; all recoverable setup/frame failures release add-ons and preserve the original error.
    void Run(HelloImGui::RunnerParams& runnerParams, const AddOnsParams& addOnsParams)
    {
        bool acquired = false;
        try
        {
            Priv_Setup(runnerParams, addOnsParams, acquired);
            HelloImGui::Run(runnerParams);
            Priv_TearDown();
        }
        catch (...)
        {
            // Preserve the original setup/frame failure even if final cleanup also fails.
            if (acquired)
                try { Priv_TearDown(); } catch (...) { fprintf(stderr, "ImmApp: failure during exception cleanup\n"); }
            throw;
        }
    }

    void Run(const HelloImGui::SimpleRunnerParams& simpleParams, const AddOnsParams& addOnsParams)
    {
        HelloImGui::RunnerParams runnerParams = simpleParams.ToRunnerParams();
        Run(runnerParams, addOnsParams);
    }


    void Run(
        // HelloImGui::SimpleRunnerParams below:
        const VoidFunction& guiFunction,
        const std::string& windowTitle,
        bool windowSizeAuto,
        bool windowRestorePreviousGeometry,
        const ScreenSize& windowSize,
        float fpsIdle,

        // ImGuiBundle_AddOnsParams below:
        bool withImplot,
        bool withImplot3d,
        bool withMarkdown,
        bool withNodeEditor,
        bool withTexInspect,
#ifdef IMGUI_BUNDLE_WITH_IMGUI_NODE_EDITOR
        const std::optional<NodeEditorConfig>& withNodeEditorConfig,
#endif
        const std::optional<ImGuiMd::MarkdownOptions> & withMarkdownOptions
    )
    {
        HelloImGui::SimpleRunnerParams simpleRunnerParams;
        simpleRunnerParams.guiFunction = guiFunction;
        simpleRunnerParams.windowTitle = windowTitle;
        simpleRunnerParams.windowSizeAuto = windowSizeAuto;
        simpleRunnerParams.windowRestorePreviousGeometry = windowRestorePreviousGeometry;
        simpleRunnerParams.windowSize = windowSize;
        simpleRunnerParams.fpsIdle = fpsIdle;

        AddOnsParams addOnsParams;
        addOnsParams.withImplot = withImplot;
        addOnsParams.withImplot3d = withImplot3d;
        addOnsParams.withMarkdown = withMarkdown;
        addOnsParams.withNodeEditor = withNodeEditor;
        addOnsParams.withTexInspect = withTexInspect;
#ifdef IMGUI_BUNDLE_WITH_IMGUI_NODE_EDITOR
        addOnsParams.withNodeEditorConfig = withNodeEditorConfig;
#endif
        addOnsParams.withMarkdownOptions = withMarkdownOptions;

        Run(simpleRunnerParams, addOnsParams);
    }


    void RunWithMarkdown(
        // HelloImGui::SimpleRunnerParams below:
        const VoidFunction& guiFunction,
        const std::string& windowTitle,
        bool windowSizeAuto,
        bool windowRestorePreviousGeometry,
        const ScreenSize& windowSize,
        float fpsIdle,

        // ImGuiBundle_AddOnsParams below:
        bool withImplot,
        bool withImplot3d,
        bool withNodeEditor,
        bool withTexInspect,
#ifdef IMGUI_BUNDLE_WITH_IMGUI_NODE_EDITOR
        const std::optional<NodeEditorConfig>& withNodeEditorConfig,
#endif
        const std::optional<ImGuiMd::MarkdownOptions> & withMarkdownOptions
    )
    {
        HelloImGui::SimpleRunnerParams simpleRunnerParams;
        simpleRunnerParams.guiFunction = guiFunction;
        simpleRunnerParams.windowTitle = windowTitle;
        simpleRunnerParams.windowSizeAuto = windowSizeAuto;
        simpleRunnerParams.windowRestorePreviousGeometry = windowRestorePreviousGeometry;
        simpleRunnerParams.windowSize = windowSize;
        simpleRunnerParams.fpsIdle = fpsIdle;

        AddOnsParams addOnsParams;
        addOnsParams.withImplot = withImplot;
        addOnsParams.withImplot3d = withImplot3d;
        addOnsParams.withMarkdown = true;
        addOnsParams.withNodeEditor = withNodeEditor;
        addOnsParams.withTexInspect = withTexInspect;
#ifdef IMGUI_BUNDLE_WITH_IMGUI_NODE_EDITOR
        addOnsParams.withNodeEditorConfig = withNodeEditorConfig;
#endif
        addOnsParams.withMarkdownOptions = withMarkdownOptions;

        Run(simpleRunnerParams, addOnsParams);
    }

    float EmSize()
    {
        return HelloImGui::EmSize();
    }
    ImVec2 EmToVec2(float x, float y)
    {
        return HelloImGui::EmToVec2(x, y);
    }
    ImVec2 EmToVec2(ImVec2 v)
    {
        return HelloImGui::EmToVec2(v);
    }
    float EmSize(float nbLines)
    {
        return HelloImGui::EmSize(nbLines);
    }

    ImVec2 PixelsToEm(ImVec2 pixels)
    {
        return HelloImGui::PixelsToEm(pixels);
    }

    float  PixelSizeToEm(float pixelSize)
    {
        return HelloImGui::PixelSizeToEm(pixelSize);
    }


#ifdef IMGUI_BUNDLE_WITH_IMGUI_NODE_EDITOR
    ax::NodeEditor::EditorContext* DefaultNodeEditorContext()
    {
        if (!gImmAppContext._NodeEditorContext.has_value())
            throw std::runtime_error("No current node editor context\n"
                                     "    Did you set with_node_editor_config when calling ImmApp::Run()?");
        return *gImmAppContext._NodeEditorContext;
    }

    ax::NodeEditor::Config* DefaultNodeEditorConfig()
    {
        return &gImmAppContext._NodeEditorConfig;
    }

    // NodeEditorSettingsLocation returns the path to the json file for the node editor settings.
    std::string NodeEditorSettingsLocation(const HelloImGui::RunnerParams& runnerParams)
    {
        std::string iniLocation = HelloImGui::IniSettingsLocation(runnerParams);
        // iniLocation is of the form path/to/your/app.ini
        // => we replace it with path/to/your/app_node_editor.json
        std::string jsonLocation = iniLocation;
        jsonLocation.replace(jsonLocation.size() - 4, 4, ".node_editor.json");
        return jsonLocation;
    }


    // HasNodeEditorSettings returns true if the json file for the node editor settings exists.
    bool HasNodeEditorSettings(const HelloImGui::RunnerParams& runnerParams)
    {
        std::string filename = NodeEditorSettingsLocation(runnerParams);
        if (filename.empty())
            return false;
        const auto admitted = vdas::path::require(vdas::path::parseUtf8(filename, "immapp.node-settings"));
        return vdas::path::require(vdas::path::exists(admitted));
    }

    // DeleteNodeEditorSettings deletes the json file for the node editor settings.
    void DeleteNodeEditorSettings(const HelloImGui::RunnerParams& runnerParams)
    {
        std::string filename = NodeEditorSettingsLocation(runnerParams);
        if (filename.empty())
            return;
        const auto admitted = vdas::path::require(vdas::path::parseUtf8(filename, "immapp.node-settings"));
        if (!vdas::path::require(vdas::path::exists(admitted)))
            return;
        bool success = vdas::path::require(vdas::path::remove(admitted));
        IM_ASSERT(success && "Failed to delete ini file %s");
    }

#endif


// ========================= ManualRender ====================================================

namespace ManualRender  // namespace ImmApp::ManualRender
{
    // Enumeration to track the current state of the ManualRenderer
    enum class RendererStatus
    {
        NotInitialized,
        Initialized,
    };
    RendererStatus sCurrentStatus = RendererStatus::NotInitialized;

    // Changes the current status to Initialized if it was NotInitialized,
    // otherwise raises an error (assert or exception)
    void TrySwitchToInitialized()
    {
        if (sCurrentStatus == RendererStatus::Initialized)
            throw std::runtime_error("ImmApp::ManualRender is already initialized. Call TearDown() first.");
        sCurrentStatus = RendererStatus::Initialized;
    }

    // Changes the current status to NotInitialized if it was Initialized,
    // otherwise raises an error (assert or exception)
    void TrySwitchToNotInitialized()
    {
        if (sCurrentStatus == RendererStatus::NotInitialized)
            IM_ASSERT(false && "ImmApp::ManualRender::TearDown() cannot be called while not initialized.");
        sCurrentStatus = RendererStatus::NotInitialized;
    }


    void SetupFromRunnerParams(const HelloImGui::RunnerParams& runnerParams, const AddOnsParams& addOnsParams)
    {
        const auto previousStatus = sCurrentStatus;
        TrySwitchToInitialized();
        HelloImGui::RunnerParams runnerParamsCopy = runnerParams;
        bool acquired = false;
        try
        {
            Priv_Setup(runnerParamsCopy, addOnsParams, acquired);
            HelloImGui::ManualRender::SetupFromRunnerParams(runnerParamsCopy);
        }
        catch (...)
        {
            if (acquired)
                try { Priv_TearDown(); } catch (...) { fprintf(stderr, "ImmApp: failure during manual setup cleanup\n"); }
            sCurrentStatus = previousStatus;
            throw;
        }
    }

    void SetupFromSimpleRunnerParams(const HelloImGui::SimpleRunnerParams& simpleParams, const AddOnsParams& addOnsParams)
    {
        HelloImGui::RunnerParams runnerParams = simpleParams.ToRunnerParams();
        SetupFromRunnerParams(runnerParams, addOnsParams);
    }

    void SetupFromGuiFunction(
        const VoidFunction& guiFunction,
        const std::string& windowTitle,
        bool windowSizeAuto,
        bool windowRestorePreviousGeometry,
        const ScreenSize& windowSize,
        float fpsIdle,

        // AddOnsParams below:
        bool withImplot,
        bool withImplot3d,
        bool withMarkdown,
        bool withNodeEditor,
        bool withTexInspect,
#ifdef IMGUI_BUNDLE_WITH_IMGUI_NODE_EDITOR
        const std::optional<NodeEditorConfig>& withNodeEditorConfig,
#endif
        const std::optional<ImGuiMd::MarkdownOptions> & withMarkdownOptions
    )
    {
        HelloImGui::SimpleRunnerParams simpleRunnerParams;
        simpleRunnerParams.guiFunction = guiFunction;
        simpleRunnerParams.windowTitle = windowTitle;
        simpleRunnerParams.windowSizeAuto = windowSizeAuto;
        simpleRunnerParams.windowRestorePreviousGeometry = windowRestorePreviousGeometry;
        simpleRunnerParams.windowSize = windowSize;
        simpleRunnerParams.fpsIdle = fpsIdle;

        AddOnsParams addOnsParams;
        addOnsParams.withImplot = withImplot;
        addOnsParams.withImplot3d = withImplot3d;
        addOnsParams.withMarkdown = true;
        addOnsParams.withNodeEditor = withNodeEditor;
        addOnsParams.withTexInspect = withTexInspect;
#ifdef IMGUI_BUNDLE_WITH_IMGUI_NODE_EDITOR
        addOnsParams.withNodeEditorConfig = withNodeEditorConfig;
#endif
        addOnsParams.withMarkdownOptions = withMarkdownOptions;

        SetupFromSimpleRunnerParams(simpleRunnerParams, addOnsParams);
    }

    void Render()
    {
        HelloImGui::ManualRender::Render();
    }

    void TearDown()
    {
        TrySwitchToNotInitialized();
        HelloImGui::ManualRender::TearDown();
        Priv_TearDown();
    }
} // namespace ManualRender

} // namespace ImmApp
