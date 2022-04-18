#pragma once
#include "Mage/Singleton.h"

namespace Mage
{
	class CameraComponent;
	class Texture2D;

	// Simple RAII wrapper for the SDL renderer
	class Renderer final : public Singleton<Renderer>
	{
	public:
		void Init(SDL_Window* window);
		void Render() const;
		void Destroy();
		
		void SetCamera(CameraComponent* pCamera);

		void RenderTexture(const Texture2D& texture, float dstX, float dstY) const;
		void RenderPartialTexture(const Texture2D& texture, int srcX, int srcY, int srcW, int srcH, float dstX, float dstY) const;

		const SDL_Color& GetBackgroundColor() const { return m_ClearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_ClearColor = color; }

	private:
		friend class Singleton<Renderer>;
		Renderer() = default;

		CameraComponent* m_pCamera{};

		SDL_GLContext m_Context;
		SDL_Window* m_pWindow{};
		SDL_Color m_ClearColor{};
	};
}
