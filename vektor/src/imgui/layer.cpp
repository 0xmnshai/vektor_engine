
#include <imgui_internal.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <GLFW/glfw3.h>

#include "core/core.hpp"
#include "imgui/layer.hpp"
#include "events/event.hpp"
#include "application/application.hpp"
#include "imgui/layer.hpp"

namespace vektor::imgui
{
    Layer::Layer() : layer::Layer("ImGuiLayer")
    {
    }

    imgui::Layer::~Layer()
    {
    }

    // void imgui::Layer::setTheme()
    // {
    //     // ImGuiStyle &style = ImGui::GetStyle();

    //     // style.Alpha = 1.0f;
    //     // style.DisabledAlpha = 1.0f;
    //     // style.WindowPadding = ImVec2(12.0f, 12.0f);
    //     // style.WindowRounding = 11.5f;
    //     // style.WindowBorderSize = 0.0f;
    //     // style.WindowMinSize = ImVec2(20.0f, 20.0f);
    //     // style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
    //     // style.WindowMenuButtonPosition = ImGuiDir_Right;
    //     // style.ChildRounding = 0.0f;
    //     // style.ChildBorderSize = 1.0f;
    //     // style.PopupRounding = 0.0f;
    //     // style.PopupBorderSize = 1.0f;
    //     // style.FramePadding = ImVec2(20.0f, 3.400000095367432f);
    //     // style.FrameRounding = 11.89999961853027f;
    //     // style.FrameBorderSize = 0.0f;
    //     // style.ItemSpacing = ImVec2(4.300000190734863f, 5.5f);
    //     // style.ItemInnerSpacing = ImVec2(7.099999904632568f, 1.799999952316284f);
    //     // style.CellPadding = ImVec2(12.10000038146973f, 9.199999809265137f);
    //     // style.IndentSpacing = 0.0f;
    //     // style.ColumnsMinSpacing = 4.900000095367432f;
    //     // style.ScrollbarSize = 11.60000038146973f;
    //     // style.ScrollbarRounding = 15.89999961853027f;
    //     // style.GrabMinSize = 3.700000047683716f;
    //     // style.GrabRounding = 20.0f;
    //     // style.TabRounding = 0.0f;
    //     // style.TabBorderSize = 0.0f;
    //     // // style.TabMinWidthForCloseButton = 0.0f;
    //     // style.ColorButtonPosition = ImGuiDir_Right;
    //     // style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
    //     // style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

    //     // style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    //     // style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.2745098173618317f, 0.3176470696926117f, 0.4509803950786591f, 1.0f);
    //     // style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
    //     // style.Colors[ImGuiCol_ChildBg] = ImVec4(0.09250493347644806f, 0.100297249853611f, 0.1158798336982727f, 1.0f);
    //     // style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
    //     // style.Colors[ImGuiCol_Border] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
    //     // style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
    //     // style.Colors[ImGuiCol_FrameBg] = ImVec4(0.1120669096708298f, 0.1262156516313553f, 0.1545064449310303f, 1.0f);
    //     // style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
    //     // style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
    //     // style.Colors[ImGuiCol_TitleBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
    //     // style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
    //     // style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
    //     // style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
    //     // style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
    //     // style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
    //     // style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
    //     // style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
    //     // style.Colors[ImGuiCol_CheckMark] = ImVec4(0.9725490212440491f, 1.0f, 0.4980392158031464f, 1.0f);
    //     // style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.971993625164032f, 1.0f, 0.4980392456054688f, 1.0f);
    //     // style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0f, 0.7953379154205322f, 0.4980392456054688f, 1.0f);
    //     // style.Colors[ImGuiCol_Button] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
    //     // style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.1821731775999069f, 0.1897992044687271f, 0.1974248886108398f, 1.0f);
    //     // style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.1545050293207169f, 0.1545048952102661f, 0.1545064449310303f, 1.0f);
    //     // style.Colors[ImGuiCol_Header] = ImVec4(0.1414651423692703f, 0.1629818230867386f, 0.2060086131095886f, 1.0f);
    //     // style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.1072951927781105f, 0.107295036315918f, 0.1072961091995239f, 1.0f);
    //     // style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
    //     // style.Colors[ImGuiCol_Separator] = ImVec4(0.1293079704046249f, 0.1479243338108063f, 0.1931330561637878f, 1.0f);
    //     // style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
    //     // style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
    //     // style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.1459212601184845f, 0.1459220051765442f, 0.1459227204322815f, 1.0f);
    //     // style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.9725490212440491f, 1.0f, 0.4980392158031464f, 1.0f);
    //     // style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.999999463558197f, 1.0f, 0.9999899864196777f, 1.0f);
    //     // style.Colors[ImGuiCol_Tab] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
    //     // style.Colors[ImGuiCol_TabHovered] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
    //     // style.Colors[ImGuiCol_TabActive] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
    //     // style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
    //     // style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.1249424293637276f, 0.2735691666603088f, 0.5708154439926147f, 1.0f);
    //     // style.Colors[ImGuiCol_PlotLines] = ImVec4(0.5215686559677124f, 0.6000000238418579f, 0.7019608020782471f, 1.0f);
    //     // style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.03921568766236305f, 0.9803921580314636f, 0.9803921580314636f, 1.0f);
    //     // style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.8841201663017273f, 0.7941429018974304f, 0.5615870356559753f, 1.0f);
    //     // style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.9570815563201904f, 0.9570719599723816f, 0.9570761322975159f, 1.0f);
    //     // style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
    //     // style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
    //     // style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    //     // style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
    //     // style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
    //     // style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.9356134533882141f, 0.9356129765510559f, 0.9356223344802856f, 1.0f);
    //     // style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
    //     // style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.266094446182251f, 0.2890366911888123f, 1.0f, 1.0f);
    //     // style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
    //     // style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 0.501960813999176f);
    //     // style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 0.501960813999176f);

    //     ImGuiStyle &style = ImGui::GetStyle();

    //     // -- Geometry --
    //     style.WindowPadding = ImVec2(10.0f, 10.0f);
    //     style.WindowRounding = 5.0f;
    //     style.FramePadding = ImVec2(5.0f, 4.0f);
    //     style.FrameRounding = 3.0f;
    //     style.ItemSpacing = ImVec2(8.0f, 6.0f);
    //     style.ItemInnerSpacing = ImVec2(6.0f, 6.0f);
    //     style.IndentSpacing = 25.0f;
    //     style.ScrollbarSize = 12.0f;
    //     style.ScrollbarRounding = 9.0f;
    //     style.GrabMinSize = 10.0f;
    //     style.GrabRounding = 3.0f;
    //     style.TabRounding = 4.0f;
    //     style.WindowBorderSize = 1.0f;
    //     style.ChildBorderSize = 1.0f;
    //     style.PopupBorderSize = 1.0f;
    //     style.FrameBorderSize = 1.0f;
    //     style.TabBorderSize = 0.0f;

    //     // -- Colors --
    //     ImVec4 *colors = style.Colors;

    //     // Basics
    //     colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
    //     colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    //     colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    //     colors[ImGuiCol_ChildBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    //     colors[ImGuiCol_PopupBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    //     colors[ImGuiCol_Border] = ImVec4(0.25f, 0.25f, 0.25f, 0.50f);
    //     colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

    //     // Inputs / Frames
    //     colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    //     colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    //     colors[ImGuiCol_FrameBgActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);

    //     // Title Bar
    //     colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
    //     colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
    //     colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    //     colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);

    //     // Scrollbar
    //     colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    //     colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    //     colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    //     colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);

    //     // Checkmark
    //     colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);

    //     // Sliders / Buttons (High Contrast White)
    //     colors[ImGuiCol_SliderGrab] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    //     colors[ImGuiCol_SliderGrabActive] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);

    //     colors[ImGuiCol_Button] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    //     colors[ImGuiCol_ButtonHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
    //     colors[ImGuiCol_ButtonActive] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);

    //     // Headers
    //     colors[ImGuiCol_Header] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    //     colors[ImGuiCol_HeaderHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
    //     colors[ImGuiCol_HeaderActive] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);

    //     // Separators
    //     colors[ImGuiCol_Separator] = ImVec4(0.30f, 0.30f, 0.30f, 0.50f);
    //     colors[ImGuiCol_SeparatorHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.78f);
    //     colors[ImGuiCol_SeparatorActive] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);

    //     // Resize Grip
    //     colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.26f, 0.26f, 0.25f);
    //     colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.50f, 0.50f, 0.50f, 0.67f);
    //     colors[ImGuiCol_ResizeGripActive] = ImVec4(0.70f, 0.70f, 0.70f, 0.95f);

    //     // Tabs
    //     colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    //     colors[ImGuiCol_TabHovered] = ImVec4(0.30f, 0.30f, 0.30f, 0.80f);
    //     colors[ImGuiCol_TabActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    //     colors[ImGuiCol_TabUnfocused] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    //     colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);

    //     // Plotting
    //     colors[ImGuiCol_PlotLines] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
    //     colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    //     colors[ImGuiCol_PlotHistogram] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
    //     colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

    //     // Tables
    //     colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
    //     colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
    //     colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
    //     colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    //     colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);

    //     // Navigation / Focus
    //     colors[ImGuiCol_TextSelectedBg] = ImVec4(0.60f, 0.60f, 0.60f, 0.35f);
    //     colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    //     colors[ImGuiCol_NavHighlight] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    //     colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    //     colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    //     colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
    // }

    void imgui::Layer::setTheme()
    {
        ImGuiStyle &style = ImGui::GetStyle();
        ImVec4 *colors = style.Colors;

        // --- Geometry & Padding ---
        style.WindowPadding = ImVec2(16.0f, 16.0f);  // More padding for a "spacious" feel
        style.FramePadding = ImVec2(10.0f, 6.0f);    // Taller inputs
        style.ItemSpacing = ImVec2(10.0f, 10.0f);    // Space between items
        style.ItemInnerSpacing = ImVec2(8.0f, 8.0f); // Space between label and item
        style.IndentSpacing = 22.0f;
        style.ScrollbarSize = 14.0f;
        style.GrabMinSize = 12.0f;

        // --- Rounding (Modern, soft corners) ---
        style.WindowRounding = 8.0f;
        style.ChildRounding = 6.0f;
        style.FrameRounding = 6.0f; // Soft inputs/buttons
        style.PopupRounding = 6.0f;
        style.ScrollbarRounding = 9.0f; // Pill shape
        style.GrabRounding = 6.0f;
        style.TabRounding = 6.0f;

        // --- Borders ---
        style.WindowBorderSize = 0.0f; // Clean, no outer border
        style.ChildBorderSize = 1.0f;
        style.PopupBorderSize = 1.0f;
        style.FrameBorderSize = 0.0f; // Flat look
        style.TabBorderSize = 0.0f;

        // --- Colors (Monochrome / Matte Dark) ---

        // Text
        colors[ImGuiCol_Text] = ImVec4(0.92f, 0.92f, 0.92f, 1.00f); // Off-white
        colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);

        // Window & Backgrounds
        colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f); // Deep dark grey
        colors[ImGuiCol_ChildBg] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);  // Slightly lighter
        colors[ImGuiCol_PopupBg] = ImVec4(0.12f, 0.12f, 0.12f, 0.98f);
        colors[ImGuiCol_Border] = ImVec4(0.25f, 0.25f, 0.25f, 0.50f); // Subtle borders
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

        // Frames (Inputs, Checkboxes)
        colors[ImGuiCol_FrameBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);

        // Title Bar
        colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f); // Matches WindowBg for seamless look
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);

        // Scrollbar
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.00f); // Transparent track
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);

        // Checkmark / Radio (High Contrast White)
        colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

        // Sliders
        colors[ImGuiCol_SliderGrab] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f); // White when dragging

        // Buttons
        colors[ImGuiCol_Button] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f); // Flash white on click

        // Headers (Collapsing Headers, Selectables, Menu Items)
        colors[ImGuiCol_Header] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);

        // Separators
        colors[ImGuiCol_Separator] = ImVec4(0.30f, 0.30f, 0.30f, 0.50f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.78f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);

        // Resize Grip
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f); // Hidden usually
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);

        // Tabs
        colors[ImGuiCol_Tab] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f); // Matches bg
        colors[ImGuiCol_TabHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
        colors[ImGuiCol_TabActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f); // Slightly distinct
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);

        // Docking
        colors[ImGuiCol_DockingPreview] = ImVec4(0.60f, 0.60f, 0.60f, 0.30f);
        colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);

        // Plotting (Graphs)
        colors[ImGuiCol_PlotLines] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

        // Tables
        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
        colors[ImGuiCol_TableBorderStrong] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
        colors[ImGuiCol_TableBorderLight] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.03f); // Very subtle stripe

        // Navigation
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.60f, 0.60f, 0.60f, 0.35f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
        colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    }

    void imgui::Layer::onAttach()
    {
        ImGui::CreateContext();

        ImGui::StyleColorsDark();
        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        ImGuiStyle &style = ImGui::GetStyle();

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        float fontSize = 18.0f;
        ImFontConfig config;
        config.OversampleH = 2;
        config.OversampleV = 2;
        io.Fonts->AddFontDefault(&config);

        setTheme();

        Application &app = Application::getInstance();
        GLFWwindow *window = static_cast<GLFWwindow *>(app.getWindow().getNativeWindow());

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void imgui::Layer::onDetach()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void imgui::Layer::begin()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
        dockspace_flags |= ImGuiDockNodeFlags_PassthruCentralNode;

        // i am  using the ImGuiWindowFlags_NoBackground flag to ensure
        // the parent window of the dockspace is transparent
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBackground;

        // Setup simple full-screen dockspace
        const ImGuiViewport *viewport = ImGui::GetMainViewport();

        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", nullptr, window_flags);
        ImGui::PopStyleVar();
        ImGui::PopStyleVar(2);

        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

        ImGui::End();
    }

    void imgui::Layer::onRender()
    {
        setTheme();

        static bool show_demo = true;
        if (show_demo)
            ImGui::ShowDemoWindow(&show_demo);
    }

    void imgui::Layer::end()
    {
        ImGuiIO &io = ImGui::GetIO();
        Application &app = Application::getInstance();

        float w = static_cast<float>(app.getWindow().getWidth());
        float h = static_cast<float>(app.getWindow().getHeight());

        io.DisplaySize = ImVec2(w, h);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow *backup_current_context = glfwGetCurrentContext();

            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();

            glfwMakeContextCurrent(backup_current_context);
        }

        glViewport(0, 0, static_cast<int>(w), static_cast<int>(h));
    }

    void imgui::Layer::onEvent(event::Event &event)
    {
        event::EventDispatcher dispatcher(event);

        dispatcher.dispatch<event::MouseButtonPressedEvent>(VEKTOR_BIND_EVENT_FN(imgui::Layer::onMouseButtonPressedEvent));
        dispatcher.dispatch<event::MouseButtonReleasedEvent>(VEKTOR_BIND_EVENT_FN(imgui::Layer::onMouseButtonReleasedEvent));
        dispatcher.dispatch<event::MouseMovedEvent>(VEKTOR_BIND_EVENT_FN(imgui::Layer::onMouseMovedEvent));
        dispatcher.dispatch<event::MouseScrolledEvent>(VEKTOR_BIND_EVENT_FN(imgui::Layer::onMouseScrolledEvent));

        dispatcher.dispatch<event::KeyPressedEvent>(VEKTOR_BIND_EVENT_FN(imgui::Layer::onKeyPressedEvent));
        dispatcher.dispatch<event::KeyReleasedEvent>(VEKTOR_BIND_EVENT_FN(imgui::Layer::onKeyReleasedEvent));
        dispatcher.dispatch<event::KeyTypedEvent>(VEKTOR_BIND_EVENT_FN(imgui::Layer::onKeyTypedEvent));

        dispatcher.dispatch<event::WindowResizeEvent>(VEKTOR_BIND_EVENT_FN(imgui::Layer::onWindowResizeEvent));
        dispatcher.dispatch<event::WindowMovedEvent>(VEKTOR_BIND_EVENT_FN(imgui::Layer::onWindowMovedEvent));
        dispatcher.dispatch<event::WindowFocusEvent>(VEKTOR_BIND_EVENT_FN(imgui::Layer::onWindowFocusEvent));
        dispatcher.dispatch<event::WindowLostFocusEvent>(VEKTOR_BIND_EVENT_FN(imgui::Layer::onWindowLostFocusEvent));

        dispatcher.dispatch<event::AppTickEvent>(VEKTOR_BIND_EVENT_FN(imgui::Layer::onAppTickEvent));
        dispatcher.dispatch<event::AppUpdateEvent>(VEKTOR_BIND_EVENT_FN(imgui::Layer::onAppUpdateEvent));
        dispatcher.dispatch<event::AppRenderEvent>(VEKTOR_BIND_EVENT_FN(imgui::Layer::onAppRenderEvent));
    }

    // Mouse events
    bool imgui::Layer::onMouseButtonPressedEvent(event::MouseButtonPressedEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        io.MouseDown[event.getMouseButton()] = true;
        return false;
    }

    bool imgui::Layer::onMouseButtonReleasedEvent(event::MouseButtonReleasedEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        io.MouseDown[event.getMouseButton()] = false;
        return false;
    }

    bool imgui::Layer::onMouseMovedEvent(event::MouseMovedEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        io.MousePos = ImVec2(event.getX(), event.getY());
        return false;
    }

    bool imgui::Layer::onMouseScrolledEvent(event::MouseScrolledEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        io.MouseWheel += event.getYOffset();
        io.MouseWheelH += event.getXOffset();
        return false;
    }

    // Keyboard events
    bool imgui::Layer::onKeyPressedEvent(event::KeyPressedEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        io.AddKeyEvent(ImGuiKey_LeftCtrl, ImGui::IsKeyDown(ImGuiKey_LeftCtrl));
        io.AddKeyEvent(ImGuiKey_RightCtrl, ImGui::IsKeyDown(ImGuiKey_RightCtrl));
        io.AddKeyEvent(ImGuiKey_LeftShift, ImGui::IsKeyDown(ImGuiKey_LeftShift));
        io.AddKeyEvent(ImGuiKey_RightShift, ImGui::IsKeyDown(ImGuiKey_RightShift));
        return false;
    }

    bool imgui::Layer::onKeyReleasedEvent(event::KeyReleasedEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        io.AddKeyEvent(ImGuiKey_LeftCtrl, false);
        io.AddKeyEvent(ImGuiKey_RightCtrl, false);
        io.AddKeyEvent(ImGuiKey_LeftShift, false);
        io.AddKeyEvent(ImGuiKey_RightShift, false);
        return false;
    }

    bool imgui::Layer::onKeyTypedEvent(event::KeyTypedEvent &event)
    {
        return false;
        // ImGuiIO &io = ImGui::GetIO();
        // io.AddInputCharacter(event.getKeyCode());
        // return true;
    }

    // Window events
    bool imgui::Layer::onWindowResizeEvent(event::WindowResizeEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        io.DisplaySize = ImVec2(event.getWidth(), event.getHeight());
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
        glViewport(0, 0, event.getWidth(), event.getHeight());
        return false;
    }

    bool imgui::Layer::onWindowMovedEvent(event::WindowMovedEvent &event)
    {
        return false;
    }

    bool imgui::Layer::onWindowFocusEvent(event::WindowFocusEvent &event)
    {
        return false;
    }

    bool imgui::Layer::onWindowLostFocusEvent(event::WindowLostFocusEvent &event)
    {
        return false;
    }

    // App events
    bool imgui::Layer::onAppTickEvent(event::AppTickEvent &event)
    {
        return false;
    }

    bool imgui::Layer::onAppUpdateEvent(event::AppUpdateEvent &event)
    {
        return false;
    }

    bool imgui::Layer::onAppRenderEvent(event::AppRenderEvent &event)
    {
        return false;
    }
}