#include "Mage/MagePCH.h"
#include "Renderer.h"

#include "Mage/Scenegraph/SceneManager.h"
#include "Mage/ResourceManagement/Texture2D.h"

#include "imgui.h"
#include "backends/imgui_impl_opengl2.h"
#include "backends/imgui_impl_sdl.h"
#include "Mage/Components/CameraComponent.h"
#include "Mage/Components/Transform.h"
#include "Mage/Scenegraph/GameObject.h"

int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void Mage::Renderer::Init(SDL_Window* window)
{
	m_Window = window;
	m_Renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_Renderer == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}
}

void Mage::Renderer::SetCamera(CameraComponent* pCamera)
{
	m_pCamera = pCamera;
}


void Mage::Renderer::Render() const
{
	// Clear last frame
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_Renderer);

	// Render everything
	if (m_pCamera != nullptr)
		SceneManager::GetInstance().Render();

	// Render ImGui
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	// Display
	SDL_RenderPresent(m_Renderer);

	// Additional ImGui viewports
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

void Mage::Renderer::Destroy()
{
	if (m_Renderer != nullptr)
	{
		SDL_DestroyRenderer(m_Renderer);
		m_Renderer = nullptr;
	}
}

void Mage::Renderer::RenderTexture(const Texture2D& texture, float dstX, float dstY) const
{
	int width, height;
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &width, &height);

	RenderPartialTexture(texture, 0, 0, width, height, dstX, dstY);
}

void Mage::Renderer::RenderPartialTexture(const Texture2D& texture, int srcX, int srcY, int srcW, int srcH, float dstX, float dstY) const
{
	// Source Rect (Part of texture)
	SDL_Rect src{};
	src.x = srcX;
	src.y = srcY;
	src.w = srcW;
	src.h = srcH;

	// Destination Rect (Part of screen)
	int windowWidth, windowHeight;
	SDL_GetWindowSize(m_Window, &windowWidth, &windowHeight);


	float pivotRelToCamX = dstX - m_pCamera->GetGameObject()->GetTransform()->GetWorldPosition().x;
	float pivotRelToCamY = -dstY + m_pCamera->GetGameObject()->GetTransform()->GetWorldPosition().y;


	SDL_Rect dst{};
	dst.x = static_cast<int>((pivotRelToCamX - texture.GetPivot().x * srcW / texture.GetPixelsPerUnit()) * windowWidth  / m_pCamera->GetWidth()  + windowWidth / 2);
	dst.y = static_cast<int>((pivotRelToCamY - (1 - texture.GetPivot().y) * srcH / texture.GetPixelsPerUnit()) * windowHeight / m_pCamera->GetHeight() + windowHeight / 2);
	dst.w = static_cast<int>(srcW / texture.GetPixelsPerUnit() * windowWidth / m_pCamera->GetWidth());
	dst.h = static_cast<int>(srcH / texture.GetPixelsPerUnit() * windowHeight / m_pCamera->GetHeight());

	// Render
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dst);
}