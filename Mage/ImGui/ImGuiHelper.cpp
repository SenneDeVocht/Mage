#include "Mage/MagePCH.h"
#include "ImGuiHelper.h"

#include "imgui_internal.h"
#include "backends/imgui_impl_opengl2.h"
#include "backends/imgui_impl_sdl.h"
#include "IconsFontAwesome.h"
#include "Mage/ResourceManagement/Texture2D.h"

void Mage::ImGuiHelper::InitImGui(SDL_Window* pWindow)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	SetCustomStyle();

	ImGui_ImplSDL2_InitForOpenGL(pWindow, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL2_Init();
}

ImFont* Mage::ImGuiHelper::GetFontBold()
{
	const ImGuiIO& io = ImGui::GetIO();
	return io.Fonts->Fonts[1];
}

ImFont* Mage::ImGuiHelper::GetFontItalic()
{
	const ImGuiIO& io = ImGui::GetIO();
	return io.Fonts->Fonts[2];
}

ImFont* Mage::ImGuiHelper::GetFontBoldItalic()
{
	const ImGuiIO& io = ImGui::GetIO();
	return io.Fonts->Fonts[3];
}

bool Mage::ImGuiHelper::SDLColorPicker(const char* label, SDL_Color* pColor, int imGuiColorEditFlags)
{
	glm::vec4 color = { pColor->r / 255.f, pColor->g / 255.f, pColor->b / 255.f, pColor->a / 255.f };
	const bool edited = ImGui::ColorEdit4(label, &color.x, imGuiColorEditFlags);
	*pColor = SDL_Color{
		static_cast<uint8_t>(color.r * 255),
		static_cast<uint8_t>(color.g * 255),
		static_cast<uint8_t>(color.b * 255),
		static_cast<uint8_t>(color.a * 255)
	};

	return edited;
}

bool Mage::ImGuiHelper::ToggleButton(const char* label, bool active, const glm::vec2& size)
{
    if (active)
    	ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);

	const bool clicked = ImGui::Button(label, { size.x, size.y });

	if (active)
		ImGui::PopStyleColor();

	return clicked;
}

bool Mage::ImGuiHelper::RadioToggleButton(const char* label, int* v, int v_button, const glm::vec2& size)
{
	const bool pressed = ToggleButton(label, *v == v_button, size);
	if (pressed)
		*v = v_button;
	return pressed;
}

void Mage::ImGuiHelper::ItemLabel(const char* title, ItemLabelAlignment alignment)
{
	// Code from https://github.com/ocornut/imgui/issues/3469#issuecomment-691845667

	ImGuiWindow* window = ImGui::GetCurrentWindow();
	const ImVec2 lineStart = ImGui::GetCursorScreenPos();
	const ImGuiStyle& style = ImGui::GetStyle();
	float fullWidth = ImGui::GetContentRegionAvail().x;
	float itemWidth = ImGui::CalcItemWidth() + style.ItemSpacing.x;
	ImVec2 textSize = ImGui::CalcTextSize(title);
	ImRect textRect;
	textRect.Min = ImGui::GetCursorScreenPos();
	if (alignment == ItemLabelAlignment::Right)
		textRect.Min.x = textRect.Min.x + itemWidth;
	textRect.Max = textRect.Min;
	textRect.Max.x += fullWidth - itemWidth;
	textRect.Max.y += textSize.y;

	ImGui::SetCursorScreenPos(textRect.Min);

	ImGui::AlignTextToFramePadding();
	// Adjust text rect manually because we render it directly into a drawlist instead of using public functions.
	textRect.Min.y += window->DC.CurrLineTextBaseOffset;
	textRect.Max.y += window->DC.CurrLineTextBaseOffset;

    ImGui::ItemSize(textRect);
	if (ImGui::ItemAdd(textRect, window->GetID(title)))
	{
        ImGui::RenderText(textRect.Min, title);
	}
	if (alignment == ItemLabelAlignment::Left)
	{
		ImGui::SetCursorScreenPos(ImVec2{ textRect.Max.x, textRect.Max.y - textSize.y + window->DC.CurrLineTextBaseOffset });
		ImGui::SameLine();
	}
	else if (alignment == ItemLabelAlignment::Right)
		ImGui::SetCursorScreenPos(lineStart);
}

void Mage::ImGuiHelper::Component(const char* name, void const* id, bool* enabled, const std::function<void()>& extraUIFunction)
{
	ImGui::PushID(id);

	bool open;

	// TODO: Make alignment cleaner

	if (enabled != nullptr)
	{
		ImGui::BeginTable("header", 2);
		ImGui::TableSetupColumn("headerCol");
		ImGui::TableSetupColumn("enableCol", ImGuiTableColumnFlags_WidthFixed);

		ImGui::TableNextColumn();
		open = ImGui::CollapsingHeader(name);

		ImGui::TableNextColumn();
		ImGui::Checkbox("", enabled);

		ImGui::EndTable();
	}
    else
    {
		ImGui::BeginTable("header", 2, ImGuiTableFlags_NoPadInnerX);
		ImGui::TableSetupColumn("headerCol");
		ImGui::TableSetupColumn("enableCol", ImGuiTableColumnFlags_WidthFixed);

		ImGui::TableNextColumn();
		open = ImGui::CollapsingHeader(name);

		ImGui::TableNextColumn();

		ImGui::EndTable();
    }

	if (open)
		extraUIFunction();

	ImGui::PopID();
}

void Mage::ImGuiHelper::Texture(const Texture2D* texture)
{
	const float availableWidth = ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x;
	constexpr float availableHeight = 50.f;

	const float scaleFactorX = availableWidth / texture->GetWidth();
	const float scaleFactorY = availableHeight / texture->GetHeight();

	const float scaleFactor = std::min(scaleFactorX, scaleFactorY);

	const auto borderCol = ImGui::GetStyle().Colors[ImGuiCol_Border];
	ImGui::Image(
		(void*)(intptr_t)texture->GetGLTexture(),
		{ texture->GetWidth() * scaleFactor, texture->GetHeight() * scaleFactor },
		{ 0, 0 }, { 1, 1 },
		{ 1, 1, 1, 1 },
		borderCol);
}

void Mage::ImGuiHelper::SetCustomStyle()
{
	// FONT
	//-----
	ImGuiIO& io = ImGui::GetIO();

	ImFontConfig config;
	config.MergeMode = true;
	config.GlyphMinAdvanceX = 13.0f; // Use if you want to make the icon monospaced
	static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };

	io.FontDefault = io.Fonts->AddFontFromFileTTF("Data/Fonts/Montserrat-Medium.ttf", 16.f);
	io.Fonts->AddFontFromFileTTF("Data/Fonts/fa-solid-900.ttf", 13.0f, &config, icon_ranges);

	io.Fonts->AddFontFromFileTTF("Data/Fonts/Montserrat-Bold.ttf", 16.f);
	io.Fonts->AddFontFromFileTTF("Data/Fonts/fa-solid-900.ttf", 13.0f, &config, icon_ranges);

	io.Fonts->AddFontFromFileTTF("Data/Fonts/Montserrat-Italic.ttf", 16.f);
	io.Fonts->AddFontFromFileTTF("Data/Fonts/fa-solid-900.ttf", 13.0f, &config, icon_ranges);

	io.Fonts->AddFontFromFileTTF("Data/Fonts/Montserrat-BoldItalic.ttf", 16.f);
	io.Fonts->AddFontFromFileTTF("Data/Fonts/fa-solid-900.ttf", 13.0f, &config, icon_ranges);

	// COLORS
	//-------
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
	colors[ImGuiCol_Border] = ImVec4(0.50f, 0.50f, 0.50f, 0.50f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 0.00f, 0.00f, 0.95f);
	colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 0.00f, 0.00f, 0.95f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.75f, 0.00f, 0.00f, 0.95f);
	colors[ImGuiCol_Button] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
	colors[ImGuiCol_Header] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
	colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.41f, 0.42f, 0.44f, 1.00f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.75f, 0.00f, 0.00f, 0.95f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.29f, 0.30f, 0.31f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.75f, 0.00f, 0.00f, 0.95f);
	colors[ImGuiCol_Tab] = ImVec4(0.08f, 0.08f, 0.09f, 0.83f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.33f, 0.34f, 0.36f, 0.83f);
	colors[ImGuiCol_TabActive] = ImVec4(0.23f, 0.23f, 0.24f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.08f, 0.08f, 0.09f, 0.83f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.23f, 0.23f, 0.24f, 1.00f);
	colors[ImGuiCol_DockingPreview] = ImVec4(0.98f, 0.26f, 0.26f, 0.70f);
	colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.18f, 0.18f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.73f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.88f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
	colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
	colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
	colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(0.92f, 0.11f, 0.11f, 1.00f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);


	// SIZES
	//------
	auto& style = ImGui::GetStyle();

	// Main
	style.WindowPadding = ImVec2(8, 8);
	style.FramePadding = ImVec2(4, 4);
	style.CellPadding = ImVec2(4, 2);
	style.ItemSpacing = ImVec2(8, 4);
	style.ItemInnerSpacing = ImVec2(4, 4);
	style.TouchExtraPadding = ImVec2(0, 0);
	style.IndentSpacing = 21.f;
	style.ScrollbarSize = 14.f;
	style.GrabMinSize = 10.f;

	// Borders
	style.WindowBorderSize = 0.f;
	style.ChildBorderSize = 1.f;
	style.PopupBorderSize = 1.f;
	style.FrameBorderSize = 0.f;
	style.TabBorderSize = 0.f;

	// Rounding
	style.WindowRounding = 0.f;
	style.ChildRounding = 0.f;
	style.FrameRounding = 3.f;
	style.PopupRounding = 0.f;
	style.ScrollbarRounding = 9.f;
	style.GrabRounding = 3.f;
	style.LogSliderDeadzone = 4.f;
	style.TabRounding = 4.f;

	// Alignment
	style.WindowTitleAlign = ImVec2(0.f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_None;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.f, 0.f);

	// Safe Area Padding
	style.DisplaySafeAreaPadding = ImVec2(3, 3);
}
