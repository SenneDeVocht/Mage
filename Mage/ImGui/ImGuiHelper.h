#pragma once
#include <functional>

struct ImFont;

namespace Mage
{
	class ImGuiHelper
	{
	public:
		static void InitImGui(SDL_Window* pWindow);

		static ImFont* GetFontBold();
		static ImFont* GetFontItalic();
		static ImFont* GetFontBoldItalic();

		enum class ItemLabelAlignment { Left, Right };
		static void ItemLabel(const char* title, ItemLabelAlignment alignment);
		
		static bool SDLColorPicker(const char* label, SDL_Color* pColor, int imGuiColorEditFlags = 0);

		static void Component(const char* name, void const* id, bool* enabled, const std::function<void()>& extraUIFunction);

	private:
		static void SetCustomStyle();
	};
}
