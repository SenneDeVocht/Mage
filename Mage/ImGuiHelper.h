#pragma once

namespace Mage
{
	class ImGuiHelper
	{
	public:
		static void InitImGui(SDL_Window* pWindow);

	private:
		static void SetCustomStyle();
	};
}
