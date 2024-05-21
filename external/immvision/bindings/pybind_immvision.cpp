// Part of ImGui Bundle - MIT License - Copyright (c) 2022-2024 Pascal Thomet - https://github.com/pthom/imgui_bundle
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#ifndef IMGUI_BUNDLE_WITH_IMMVISION
void py_init_module_immvision(pybind11::module& m)
{
    m.def("immvision_not_available", []() { return std::string("immvision_not_available: imgui_bundle was compiled without immvision"); });
}
#else

#include "immvision/immvision.h"
#include "immvision/internal/cv/cv_drawing_utils.h"
#include "opencv2/core/core.hpp"
#include "cvnp/cvnp.h"

void pydef_cvnp(pybind11::module& m);

namespace py = pybind11;


// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  AUTOGENERATED CODE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// <litgen_glue_code>  // Autogenerated code below! Do not edit!

// </litgen_glue_code> // Autogenerated code end
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  AUTOGENERATED CODE END !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

namespace ImmVision {
    namespace ImageDrawing
    {
        cv::Mat _DrawLutGraph(const std::vector<double>& x, const std::vector<double>& y, cv::Size size);
    }
}


void py_init_module_immvision(py::module& m)
{
    pydef_cvnp(m);

    m.def("_draw_lut_graph", ImmVision::ImageDrawing::_DrawLutGraph);

    using namespace ImmVision;

    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  AUTOGENERATED CODE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // <litgen_pydef> // Autogenerated code below! Do not edit!
    ////////////////////    <generated_from:immvision.h>    ////////////////////
    py::enum_<ImmVision::ColorMapStatsTypeId>(m, "ColorMapStatsTypeId", py::arithmetic(), "Are we using the stats on the full image, on the Visible ROI, or are we using Min/Max values")    // immvision.h:27
        .value("from_full_image", ImmVision::ColorMapStatsTypeId::FromFullImage, "")
        .value("from_visible_roi", ImmVision::ColorMapStatsTypeId::FromVisibleROI, "");


    auto pyClassColormapScaleFromStatsData =
        py::class_<ImmVision::ColormapScaleFromStatsData>    // immvision.h:34
            (m, "ColormapScaleFromStatsData", " Scale the Colormap according to the Image  stats\n\nIMMVISION_API_STRUCT")
        .def(py::init<>([](
        ImmVision::ColorMapStatsTypeId ColorMapStatsType = ImmVision::ColorMapStatsTypeId(), double NbSigmas = 1.5, bool UseStatsMin = false, bool UseStatsMax = false)
        {
            auto r = std::make_unique<ImmVision::ColormapScaleFromStatsData>();
            r->ColorMapStatsType = ColorMapStatsType;
            r->NbSigmas = NbSigmas;
            r->UseStatsMin = UseStatsMin;
            r->UseStatsMax = UseStatsMax;
            return r;
        })
        , py::arg("color_map_stats_type") = ImmVision::ColorMapStatsTypeId(), py::arg("nb_sigmas") = 1.5, py::arg("use_stats_min") = false, py::arg("use_stats_max") = false
        )
        .def_readwrite("color_map_stats_type", &ImmVision::ColormapScaleFromStatsData::ColorMapStatsType, "Are we using the stats on the full image, the visible ROI, or are we using Min/Max values")    // immvision.h:37
        .def_readwrite("nb_sigmas", &ImmVision::ColormapScaleFromStatsData::NbSigmas, "If stats active (either on ROI or on Image), how many sigmas around the mean should the Colormap be applied")    // immvision.h:40
        .def_readwrite("use_stats_min", &ImmVision::ColormapScaleFromStatsData::UseStatsMin, "If ColorMapScaleType==ColorMapStatsType::FromMinMax, then ColormapScaleMin will be calculated from the matrix min value instead of a sigma based value")    // immvision.h:43
        .def_readwrite("use_stats_max", &ImmVision::ColormapScaleFromStatsData::UseStatsMax, "If ColorMapScaleType==ColorMapStatsType::FromMinMax, then ColormapScaleMax will be calculated from the matrix min value instead of a sigma based value")    // immvision.h:45
        ;


    auto pyClassColormapSettingsData =
        py::class_<ImmVision::ColormapSettingsData>    // immvision.h:50
            (m, "ColormapSettingsData", " Colormap Settings (useful for matrices with one channel, in order to see colors mapping float values)\n\nIMMVISION_API_STRUCT")
        .def(py::init<>([](
        std::string Colormap = "None", double ColormapScaleMin = 0., double ColormapScaleMax = 1., ImmVision::ColormapScaleFromStatsData ColormapScaleFromStats = ImmVision::ColormapScaleFromStatsData(), std::string internal_ColormapHovered = "")
        {
            auto r = std::make_unique<ImmVision::ColormapSettingsData>();
            r->Colormap = Colormap;
            r->ColormapScaleMin = ColormapScaleMin;
            r->ColormapScaleMax = ColormapScaleMax;
            r->ColormapScaleFromStats = ColormapScaleFromStats;
            r->internal_ColormapHovered = internal_ColormapHovered;
            return r;
        })
        , py::arg("colormap") = "None", py::arg("colormap_scale_min") = 0., py::arg("colormap_scale_max") = 1., py::arg("colormap_scale_from_stats") = ImmVision::ColormapScaleFromStatsData(), py::arg("internal_colormap_hovered") = ""
        )
        .def_readwrite("colormap", &ImmVision::ColormapSettingsData::Colormap, " Colormap, see available Colormaps with AvailableColormaps()\n Work only with 1 channel matrices, i.e len(shape)==2")    // immvision.h:54
        .def_readwrite("colormap_scale_min", &ImmVision::ColormapSettingsData::ColormapScaleMin, "")    // immvision.h:61
        .def_readwrite("colormap_scale_max", &ImmVision::ColormapSettingsData::ColormapScaleMax, "")    // immvision.h:62
        .def_readwrite("colormap_scale_from_stats", &ImmVision::ColormapSettingsData::ColormapScaleFromStats, " If ColormapScaleFromStats.ActiveOnFullImage or ColormapScaleFromStats.ActiveOnROI,\n then ColormapScaleMin/Max are ignored, and the scaling is done according to the image stats.\n ColormapScaleFromStats.ActiveOnFullImage is True by default")    // immvision.h:67
        .def_readwrite("internal_colormap_hovered", &ImmVision::ColormapSettingsData::internal_ColormapHovered, "Internal value: stores the name of the Colormap that is hovered by the mouse")    // immvision.h:71
        ;


    auto pyClassMouseInformation =
        py::class_<ImmVision::MouseInformation>    // immvision.h:76
            (m, "MouseInformation", " Contains information about the mouse inside an image\n\nIMMVISION_API_STRUCT")
        .def(py::init<>([](
        bool IsMouseHovering = false, cv::Point2d MousePosition = cv::Point2d(-1., -1.), cv::Point MousePosition_Displayed = cv::Point(-1, -1))
        {
            auto r = std::make_unique<ImmVision::MouseInformation>();
            r->IsMouseHovering = IsMouseHovering;
            r->MousePosition = MousePosition;
            r->MousePosition_Displayed = MousePosition_Displayed;
            return r;
        })
        , py::arg("is_mouse_hovering") = false, py::arg("mouse_position") = cv::Point2d(-1., -1.), py::arg("mouse_position_displayed") = cv::Point(-1, -1)
        )
        .def_readwrite("is_mouse_hovering", &ImmVision::MouseInformation::IsMouseHovering, "Is the mouse hovering the image")    // immvision.h:79
        .def_readwrite("mouse_position", &ImmVision::MouseInformation::MousePosition, " Mouse position in the original image/matrix\n This position is given with float coordinates, and will be (-1., -1.) if the mouse is not hovering the image")    // immvision.h:83
        .def_readwrite("mouse_position_displayed", &ImmVision::MouseInformation::MousePosition_Displayed, " Mouse position in the displayed portion of the image (the original image can be zoomed,\n and only show a subset if it may be shown).\n This position is given with integer coordinates, and will be (-1, -1) if the mouse is not hovering the image")    // immvision.h:87
        ;


    auto pyClassImageParams =
        py::class_<ImmVision::ImageParams>    // immvision.h:96
            (m, "ImageParams", " Set of display parameters and options for an Image\n\nIMMVISION_API_STRUCT")
        .def(py::init<>([](
        bool RefreshImage = false, cv::Size ImageDisplaySize = cv::Size(), cv::Matx33d ZoomPanMatrix = cv::Matx33d::eye(), std::string ZoomKey = "", ImmVision::ColormapSettingsData ColormapSettings = ImmVision::ColormapSettingsData(), std::string ColormapKey = "", bool PanWithMouse = true, bool ZoomWithMouseWheel = true, bool CanResize = true, bool IsColorOrderBGR = true, int SelectedChannel = -1, bool ShowSchoolPaperBackground = true, bool ShowAlphaChannelCheckerboard = true, bool ShowGrid = true, bool DrawValuesOnZoomedPixels = true, bool ShowImageInfo = true, bool ShowPixelInfo = true, bool ShowZoomButtons = true, bool ShowOptionsPanel = false, bool ShowOptionsInTooltip = false, bool ShowOptionsButton = true, std::vector<cv::Point> WatchedPixels = std::vector<cv::Point>(), bool AddWatchedPixelOnDoubleClick = true, bool HighlightWatchedPixels = true, ImmVision::MouseInformation MouseInfo = ImmVision::MouseInformation())
        {
            auto r = std::make_unique<ImmVision::ImageParams>();
            r->RefreshImage = RefreshImage;
            r->ImageDisplaySize = ImageDisplaySize;
            r->ZoomPanMatrix = ZoomPanMatrix;
            r->ZoomKey = ZoomKey;
            r->ColormapSettings = ColormapSettings;
            r->ColormapKey = ColormapKey;
            r->PanWithMouse = PanWithMouse;
            r->ZoomWithMouseWheel = ZoomWithMouseWheel;
            r->CanResize = CanResize;
            r->IsColorOrderBGR = IsColorOrderBGR;
            r->SelectedChannel = SelectedChannel;
            r->ShowSchoolPaperBackground = ShowSchoolPaperBackground;
            r->ShowAlphaChannelCheckerboard = ShowAlphaChannelCheckerboard;
            r->ShowGrid = ShowGrid;
            r->DrawValuesOnZoomedPixels = DrawValuesOnZoomedPixels;
            r->ShowImageInfo = ShowImageInfo;
            r->ShowPixelInfo = ShowPixelInfo;
            r->ShowZoomButtons = ShowZoomButtons;
            r->ShowOptionsPanel = ShowOptionsPanel;
            r->ShowOptionsInTooltip = ShowOptionsInTooltip;
            r->ShowOptionsButton = ShowOptionsButton;
            r->WatchedPixels = WatchedPixels;
            r->AddWatchedPixelOnDoubleClick = AddWatchedPixelOnDoubleClick;
            r->HighlightWatchedPixels = HighlightWatchedPixels;
            r->MouseInfo = MouseInfo;
            return r;
        })
        , py::arg("refresh_image") = false, py::arg("image_display_size") = cv::Size(), py::arg("zoom_pan_matrix") = cv::Matx33d::eye(), py::arg("zoom_key") = "", py::arg("colormap_settings") = ImmVision::ColormapSettingsData(), py::arg("colormap_key") = "", py::arg("pan_with_mouse") = true, py::arg("zoom_with_mouse_wheel") = true, py::arg("can_resize") = true, py::arg("is_color_order_bgr") = true, py::arg("selected_channel") = -1, py::arg("show_school_paper_background") = true, py::arg("show_alpha_channel_checkerboard") = true, py::arg("show_grid") = true, py::arg("draw_values_on_zoomed_pixels") = true, py::arg("show_image_info") = true, py::arg("show_pixel_info") = true, py::arg("show_zoom_buttons") = true, py::arg("show_options_panel") = false, py::arg("show_options_in_tooltip") = false, py::arg("show_options_button") = true, py::arg("watched_pixels") = std::vector<cv::Point>(), py::arg("add_watched_pixel_on_double_click") = true, py::arg("highlight_watched_pixels") = true, py::arg("mouse_info") = ImmVision::MouseInformation()
        )
        .def_readwrite("refresh_image", &ImmVision::ImageParams::RefreshImage, " Refresh Image: images textures are cached. Set to True if your image matrix/buffer has changed\n (for example, for live video images)")    // immvision.h:111
        .def_readwrite("image_display_size", &ImmVision::ImageParams::ImageDisplaySize, " Size of the displayed image (can be different from the matrix size)\n If you specify only the width or height (e.g (300, 0), then the other dimension\n will be calculated automatically, respecting the original image w/h ratio.")    // immvision.h:120
        .def_readwrite("zoom_pan_matrix", &ImmVision::ImageParams::ZoomPanMatrix, "ZoomPanMatrix can be created using MakeZoomPanMatrix to create a view centered around a given point")    // immvision.h:127
        .def_readwrite("zoom_key", &ImmVision::ImageParams::ZoomKey, "If displaying several images, those with the same ZoomKey will zoom and pan together")    // immvision.h:129
        .def_readwrite("colormap_settings", &ImmVision::ImageParams::ColormapSettings, "\n Colormap Settings (useful for matrices with one channel, in order to see colors mapping float values)\n\n ColormapSettings stores all the parameter concerning the Colormap")    // immvision.h:135
        .def_readwrite("colormap_key", &ImmVision::ImageParams::ColormapKey, "If displaying several images, those with the same ColormapKey will adjust together")    // immvision.h:137
        .def_readwrite("pan_with_mouse", &ImmVision::ImageParams::PanWithMouse, "")    // immvision.h:142
        .def_readwrite("zoom_with_mouse_wheel", &ImmVision::ImageParams::ZoomWithMouseWheel, "")    // immvision.h:143
        .def_readwrite("can_resize", &ImmVision::ImageParams::CanResize, "Can the image widget be resized by the user")    // immvision.h:146
        .def_readwrite("is_color_order_bgr", &ImmVision::ImageParams::IsColorOrderBGR, "Color Order: RGB or RGBA versus BGR or BGRA (Note: by default OpenCV uses BGR and BGRA)")    // immvision.h:149
        .def_readwrite("selected_channel", &ImmVision::ImageParams::SelectedChannel, "\n Image display options\n\n if SelectedChannel >= 0 then only this channel is displayed")    // immvision.h:155
        .def_readwrite("show_school_paper_background", &ImmVision::ImageParams::ShowSchoolPaperBackground, "Show a \"school paper\" background grid")    // immvision.h:157
        .def_readwrite("show_alpha_channel_checkerboard", &ImmVision::ImageParams::ShowAlphaChannelCheckerboard, "show a checkerboard behind transparent portions of 4 channels RGBA images")    // immvision.h:159
        .def_readwrite("show_grid", &ImmVision::ImageParams::ShowGrid, "Grid displayed when the zoom is high")    // immvision.h:161
        .def_readwrite("draw_values_on_zoomed_pixels", &ImmVision::ImageParams::DrawValuesOnZoomedPixels, "Pixel values show when the zoom is high")    // immvision.h:163
        .def_readwrite("show_image_info", &ImmVision::ImageParams::ShowImageInfo, "\n Image display options\n\n Show matrix type and size")    // immvision.h:169
        .def_readwrite("show_pixel_info", &ImmVision::ImageParams::ShowPixelInfo, "Show pixel values")    // immvision.h:171
        .def_readwrite("show_zoom_buttons", &ImmVision::ImageParams::ShowZoomButtons, "Show buttons that enable to zoom in/out (the mouse wheel also zoom)")    // immvision.h:173
        .def_readwrite("show_options_panel", &ImmVision::ImageParams::ShowOptionsPanel, "Open the options panel")    // immvision.h:175
        .def_readwrite("show_options_in_tooltip", &ImmVision::ImageParams::ShowOptionsInTooltip, "If set to True, then the option panel will be displayed in a transient tooltip window")    // immvision.h:177
        .def_readwrite("show_options_button", &ImmVision::ImageParams::ShowOptionsButton, "If set to False, then the Options button will not be displayed")    // immvision.h:179
        .def_readwrite("watched_pixels", &ImmVision::ImageParams::WatchedPixels, "\n Watched Pixels\n\n List of Watched Pixel coordinates")    // immvision.h:185
        .def_readwrite("add_watched_pixel_on_double_click", &ImmVision::ImageParams::AddWatchedPixelOnDoubleClick, "Shall we add a watched pixel on double click")    // immvision.h:187
        .def_readwrite("highlight_watched_pixels", &ImmVision::ImageParams::HighlightWatchedPixels, "Shall the watched pixels be drawn on the image")    // immvision.h:189
        .def_readwrite("mouse_info", &ImmVision::ImageParams::MouseInfo, "Mouse position information. These values are filled after displaying an image")    // immvision.h:192
        ;
    // #ifdef IMMVISION_SERIALIZE_JSON
    //

    m.def("image_params_to_json",    // immvision.h:198
        ImmVision::ImageParamsToJson, py::arg("params"));

    m.def("fill_image_params_from_json",    // immvision.h:199
        ImmVision::FillImageParamsFromJson, py::arg("json"), py::arg("params"));

    m.def("image_params_from_json",    // immvision.h:200
        ImmVision::ImageParamsFromJson, py::arg("json"));
    // #endif
    //

    m.def("factor_image_params_display_only",    // immvision.h:204
        ImmVision::FactorImageParamsDisplayOnly, "Create ImageParams that display the image only, with no decoration, and no user interaction");

    m.def("make_zoom_pan_matrix",    // immvision.h:208
        ImmVision::MakeZoomPanMatrix,
        py::arg("zoom_center"), py::arg("zoom_ratio"), py::arg("displayed_image_size"),
        "Create a zoom/pan matrix centered around a given point of interest");

    m.def("make_zoom_pan_matrix_scale_one",    // immvision.h:214
        ImmVision::MakeZoomPanMatrix_ScaleOne, py::arg("image_size"), py::arg("displayed_image_size"));

    m.def("make_zoom_pan_matrix_full_view",    // immvision.h:219
        ImmVision::MakeZoomPanMatrix_FullView, py::arg("image_size"), py::arg("displayed_image_size"));

    m.def("image",    // immvision.h:259
        ImmVision::Image,
        py::arg("label"), py::arg("mat"), py::arg("params"),
        " Display an image, with full user control: zoom, pan, watch pixels, etc.\n\n :param label\n     A legend that will be displayed.\n     Important notes:\n         - With ImGui and ImmVision, widgets *must* have a unique Ids.\n           For this widget, the id is given by this label.\n           Two widgets (for example) two images *cannot* have the same label or the same id!\n           (you can use ImGui::PushID / ImGui::PopID to circumvent this, or add suffixes with ##)\n\n           If they do, they might not refresh correctly!\n           To circumvent this, you can:\n              - Call `ImGui::PushID(\"some_unique_string\")` at the start of your function,\n                and `ImGui::PopID()` at the end.\n              - Or modify your label like this:\n                  \"MyLabel##some_unique_id\"\n                  (the part after \"##\" will not be displayed but will be part of the id)\n        - To display an empty legend, use \"##_some_unique_id\"\n\n :param mat\n     An image you want to display, under the form of an OpenCV matrix. All types of dense matrices are supported.\n\n :param params\n     Complete options (as modifiable inputs), and outputs (mouse position, watched pixels, etc)\n     @see ImageParams structure.\n     The ImageParams may be modified by this function: you can extract from them\n     the mouse position, watched pixels, etc.\n     Important note:\n         ImageParams is an input-output parameter, passed as a pointer.\n         Its scope should be wide enough so that it is preserved from frame to frame.\n         !! If you cannot zoom/pan in a displayed image, extend the scope of the ImageParams !!\n\n - This function requires that both imgui and OpenGL were initialized.\n   (for example, use `imgui_runner.run`for Python,  or `HelloImGui::Run` for C++)");

    m.def("image_display",    // immvision.h:307
        ImmVision::ImageDisplay,
        py::arg("label_id"), py::arg("mat"), py::arg("image_display_size") = cv::Size(), py::arg("refresh_image") = false, py::arg("show_options_button") = false, py::arg("is_bgr_or_bgra") = true,
        " ImageDisplay: Only, display the image, with no user interaction (by default)\n\n Parameters:\n :param label_id\n     A legend that will be displayed.\n     Important notes:\n         - With ImGui and ImmVision, widgets must have a unique Ids. For this widget, the id is given by this label.\n           Two widgets (for example) two images *cannot* have the same label or the same id!\n           If they do, they might not refresh correctly!\n           To circumvent this, you can modify your label like this:\n              \"MyLabel##some_unique_id\"    (the part after \"##\" will not be displayed but will be part of the id)\n        - To display an empty legend, use \"##_some_unique_id\"\n        - if your legend is displayed (i.e. it does not start with \"##\"),\n          then the total size of the widget will be larger than the imageDisplaySize.\n\n :param mat:\n     An image you want to display, under the form of an OpenCV matrix. All types of dense matrices are supported.\n\n :param imageDisplaySize:\n     Size of the displayed image (can be different from the mat size)\n     If you specify only the width or height (e.g (300, 0), then the other dimension\n     will be calculated automatically, respecting the original image w/h ratio.\n\n :param refreshImage:\n     images textures are cached. Set to True if your image matrix/buffer has changed\n     (for example, for live video images)\n\n :param showOptionsButton:\n     If True, show an option button that opens the option panel.\n     In that case, it also becomes possible to zoom & pan, add watched pixel by double-clicking, etc.\n\n :param isBgrOrBgra:\n     set to True if the color order of the image is BGR or BGRA (as in OpenCV, by default)\n\n :return:\n      The mouse position in `mat` original image coordinates, as double values.\n      (i.e. it does not matter if imageDisplaySize is different from mat.size())\n      It will return (-1., -1.) if the mouse is not hovering the image.\n\n      Note: use ImGui::IsMouseDown(mouse_button) (C++) or imgui.is_mouse_down(mouse_button) (Python)\n            to query more information about the mouse.\n\n Note: this function requires that both imgui and OpenGL were initialized.\n       (for example, use `imgui_runner.run`for Python,  or `HelloImGui::Run` for C++)\n");

    m.def("image_display_resizable",    // immvision.h:319
        ImmVision::ImageDisplayResizable,
        py::arg("label_id"), py::arg("mat"), py::arg("size") = py::none(), py::arg("refresh_image") = false, py::arg("resizable") = true, py::arg("show_options_button") = false, py::arg("is_bgr_or_bgra") = true,
        " ImageDisplayResizable: display the image, with no user interaction (by default)\n The image can be resized by the user (and the new size will be stored in the size parameter, if provided)\n The label will not be displayed (but it will be used as an id, and must be unique)");

    m.def("available_colormaps",    // immvision.h:332
        ImmVision::AvailableColormaps, " Return the list of the available color maps\n Taken from https://github.com/yuki-koyama/tinycolormap, thanks to Yuki Koyama");

    m.def("clear_texture_cache",    // immvision.h:339
        ImmVision::ClearTextureCache, " Clears the internal texture cache of immvision (this is done automatically at exit time)\n\n Note: this function requires that both imgui and OpenGL were initialized.\n       (for example, use `imgui_runner.run`for Python,  or `HelloImGui::Run` for C++)");

    m.def("get_cached_rgba_image",    // immvision.h:344
        ImmVision::GetCachedRgbaImage,
        py::arg("label"),
        " Returns the RGBA image currently displayed by ImmVision::Image or ImmVision::ImageDisplay\n Note: this image must be currently displayed. This function will return the transformed image\n (i.e with ColorMap, Zoom, etc.)");

    m.def("version_info",    // immvision.h:347
        ImmVision::VersionInfo, "Return immvision version info");


    m.def("inspector_add_image",    // immvision.h:368
        ImmVision::Inspector_AddImage, py::arg("image"), py::arg("legend"), py::arg("zoom_key") = "", py::arg("colormap_key") = "", py::arg("zoom_center") = cv::Point2d(), py::arg("zoom_ratio") = -1., py::arg("is_color_order_bgr") = true);

    m.def("inspector_show",    // immvision.h:378
        ImmVision::Inspector_Show);

    m.def("inspector_clear_images",    // immvision.h:380
        ImmVision::Inspector_ClearImages);
    ////////////////////    </generated_from:immvision.h>    ////////////////////


    ////////////////////    <generated_from:cv_drawing_utils.h>    ////////////////////

    { // <namespace CvDrawingUtils>
        py::module_ pyNsCvDrawingUtils = m.def_submodule("cv_drawing_utils", "namespace CvDrawingUtils");
        py::enum_<ImmVision::CvDrawingUtils::Colors>(pyNsCvDrawingUtils, "Colors", py::arithmetic(), "")    // cv_drawing_utils.h:9
            .value("black", ImmVision::CvDrawingUtils::Colors::Black, "")
            .value("red", ImmVision::CvDrawingUtils::Colors::Red, "")
            .value("green", ImmVision::CvDrawingUtils::Colors::Green, "")
            .value("blue", ImmVision::CvDrawingUtils::Colors::Blue, "")
            .value("white", ImmVision::CvDrawingUtils::Colors::White, "")
            .value("yellow", ImmVision::CvDrawingUtils::Colors::Yellow, "")
            .value("cyan", ImmVision::CvDrawingUtils::Colors::Cyan, "")
            .value("violet", ImmVision::CvDrawingUtils::Colors::Violet, "")
            .value("orange", ImmVision::CvDrawingUtils::Colors::Orange, "");


        pyNsCvDrawingUtils.def("colors_to_scalar",    // cv_drawing_utils.h:22
            ImmVision::CvDrawingUtils::ColorsToScalar,
            py::arg("value"),
            "(private API)");

        pyNsCvDrawingUtils.def("black",    // cv_drawing_utils.h:24
            ImmVision::CvDrawingUtils::Black, "(private API)");

        pyNsCvDrawingUtils.def("red",    // cv_drawing_utils.h:27
            ImmVision::CvDrawingUtils::Red, "(private API)");

        pyNsCvDrawingUtils.def("green",    // cv_drawing_utils.h:30
            ImmVision::CvDrawingUtils::Green, "(private API)");

        pyNsCvDrawingUtils.def("blue",    // cv_drawing_utils.h:33
            ImmVision::CvDrawingUtils::Blue, "(private API)");

        pyNsCvDrawingUtils.def("white",    // cv_drawing_utils.h:36
            ImmVision::CvDrawingUtils::White, "(private API)");

        pyNsCvDrawingUtils.def("yellow",    // cv_drawing_utils.h:39
            ImmVision::CvDrawingUtils::Yellow, "(private API)");

        pyNsCvDrawingUtils.def("cyan",    // cv_drawing_utils.h:42
            ImmVision::CvDrawingUtils::Cyan, "(private API)");

        pyNsCvDrawingUtils.def("violet",    // cv_drawing_utils.h:45
            ImmVision::CvDrawingUtils::Violet, "(private API)");

        pyNsCvDrawingUtils.def("orange",    // cv_drawing_utils.h:48
            ImmVision::CvDrawingUtils::Orange, "(private API)");

        pyNsCvDrawingUtils.def("line",    // cv_drawing_utils.h:52
            ImmVision::CvDrawingUtils::line,
            py::arg("image"), py::arg("a"), py::arg("b"), py::arg("color"), py::arg("thickness") = 1,
            "(private API)");

        pyNsCvDrawingUtils.def("ellipse",    // cv_drawing_utils.h:58
            ImmVision::CvDrawingUtils::ellipse,
            py::arg("image"), py::arg("center"), py::arg("size"), py::arg("color"), py::arg("angle") = 0., py::arg("start_angle") = 0., py::arg("end_angle") = 360., py::arg("thickness") = 1,
            "(private API)");

        pyNsCvDrawingUtils.def("circle",    // cv_drawing_utils.h:67
            ImmVision::CvDrawingUtils::circle,
            py::arg("image"), py::arg("center"), py::arg("radius"), py::arg("color"), py::arg("thickness") = 1,
            "(private API)");

        pyNsCvDrawingUtils.def("rectangle",    // cv_drawing_utils.h:73
            ImmVision::CvDrawingUtils::rectangle,
            py::arg("image"), py::arg("pt1"), py::arg("pt2"), py::arg("color"), py::arg("fill") = false, py::arg("thickness") = 1,
            "(private API)");

        pyNsCvDrawingUtils.def("rectangle_size",    // cv_drawing_utils.h:81
            ImmVision::CvDrawingUtils::rectangle_size,
            py::arg("img"), py::arg("pt"), py::arg("size"), py::arg("color"), py::arg("fill") = false, py::arg("thickness") = 1,
            "(private API)");

        pyNsCvDrawingUtils.def("text",    // cv_drawing_utils.h:88
            ImmVision::CvDrawingUtils::text,
            py::arg("img"), py::arg("position"), py::arg("msg"), py::arg("color"), py::arg("center_around_point") = false, py::arg("add_cartouche") = false, py::arg("font_scale") = 0.4, py::arg("thickness") = 1,
            "(private API)");

        pyNsCvDrawingUtils.def("cross_hole",    // cv_drawing_utils.h:97
            ImmVision::CvDrawingUtils::cross_hole,
            py::arg("img"), py::arg("position"), py::arg("color"), py::arg("size") = 2., py::arg("size_hole") = 2., py::arg("thickness") = 1,
            "(private API)");

        pyNsCvDrawingUtils.def("draw_named_feature",    // cv_drawing_utils.h:104
            ImmVision::CvDrawingUtils::draw_named_feature,
            py::arg("img"), py::arg("position"), py::arg("name"), py::arg("color"), py::arg("add_cartouche") = false, py::arg("size") = 3., py::arg("size_hole") = 2., py::arg("thickness") = 1, py::arg("font_scale") = 0.4,
            "(private API)");

        pyNsCvDrawingUtils.def("draw_transparent_pixel",    // cv_drawing_utils.h:114
            ImmVision::CvDrawingUtils::draw_transparent_pixel,
            py::arg("img_rgba"), py::arg("position"), py::arg("color"), py::arg("alpha"),
            "(private API)");

        pyNsCvDrawingUtils.def("draw_grid",    // cv_drawing_utils.h:121
            ImmVision::CvDrawingUtils::draw_grid,
            py::arg("img_rgba"), py::arg("line_color"), py::arg("alpha"), py::arg("x_spacing"), py::arg("y_spacing"), py::arg("x_start"), py::arg("y_start"), py::arg("x_end"), py::arg("y_end"),
            "(private API)");

        pyNsCvDrawingUtils.def("stack_images_vertically",    // cv_drawing_utils.h:130
            ImmVision::CvDrawingUtils::stack_images_vertically,
            py::arg("img1"), py::arg("img2"),
            "(private API)");

        pyNsCvDrawingUtils.def("stack_images_horizontally",    // cv_drawing_utils.h:131
            ImmVision::CvDrawingUtils::stack_images_horizontally,
            py::arg("img1"), py::arg("img2"),
            "(private API)");

        pyNsCvDrawingUtils.def("make_alpha_channel_checkerboard_image",    // cv_drawing_utils.h:133
            ImmVision::CvDrawingUtils::make_alpha_channel_checkerboard_image,
            py::arg("size"), py::arg("square_size") = 30,
            "(private API)");

        pyNsCvDrawingUtils.def("overlay_alpha_image_precise",    // cv_drawing_utils.h:138
            ImmVision::CvDrawingUtils::overlay_alpha_image_precise,
            py::arg("background_rgb_or_rgba"), py::arg("overlay_rgba"), py::arg("alpha"),
            "(private API)");

        pyNsCvDrawingUtils.def("converted_to_rgba_image",    // cv_drawing_utils.h:141
            ImmVision::CvDrawingUtils::converted_to_rgba_image,
            py::arg("input_mat"), py::arg("is_bgr_or_bgra"),
            "(private API)");
    } // </namespace CvDrawingUtils>
    ////////////////////    </generated_from:cv_drawing_utils.h>    ////////////////////

    // </litgen_pydef> // Autogenerated code end
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  AUTOGENERATED CODE END !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}

#endif // #else / #ifndef IMGUI_BUNDLE_WITH_IMMVISION
