#include "Mage/MagePCH.h"
#include "Renderer.h"

#include "Mage/Scenegraph/SceneManager.h"
#include "Mage/ResourceManagement/Texture2D.h"

#include "imgui.h"
#include "backends/imgui_impl_opengl2.h"
#include "Mage/Components/CameraComponent.h"
#include "Mage/Components/Transform.h"
#include "Mage/Scenegraph/GameObject.h"
#include "Mage/Engine/GameSettings.h"

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
	m_pWindow = window;
	m_pRenderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_pRenderer == nullptr)
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
	SDL_SetRenderDrawColor(m_pRenderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_pRenderer);

	// Render everything
	if (m_pCamera != nullptr)
	{
		if (m_pCamera->IsEnabled())
			SceneManager::GetInstance().Render();
	}

	// Render ImGui
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	// Display
	SDL_RenderPresent(m_pRenderer);

	// Additional ImGui viewports
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

void Mage::Renderer::Destroy()
{
	if (m_pRenderer != nullptr)
	{
		SDL_DestroyRenderer(m_pRenderer);
		m_pRenderer = nullptr;
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
	SDL_GetWindowSize(m_pWindow, &windowWidth, &windowHeight);


	float pivotRelToCamX = dstX - m_pCamera->GetGameObject()->GetTransform()->GetWorldPosition().x;
	float pivotRelToCamY = -dstY + m_pCamera->GetGameObject()->GetTransform()->GetWorldPosition().y;


	SDL_Rect dst{};
	dst.x = static_cast<int>((pivotRelToCamX - texture.GetPivot().x * srcW / texture.GetPixelsPerUnit()) * windowWidth  / m_pCamera->GetSize().x  + windowWidth / 2);
	dst.y = static_cast<int>((pivotRelToCamY - (1 - texture.GetPivot().y) * srcH / texture.GetPixelsPerUnit()) * windowHeight / m_pCamera->GetSize().y + windowHeight / 2);
	dst.w = static_cast<int>(srcW / texture.GetPixelsPerUnit() * windowWidth / m_pCamera->GetSize().x);
	dst.h = static_cast<int>(srcH / texture.GetPixelsPerUnit() * windowHeight / m_pCamera->GetSize().y);

	// Render
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dst);
}
