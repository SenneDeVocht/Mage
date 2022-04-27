#pragma once

namespace Mage
{
	class CameraComponent;
	class Texture2D;

	class Renderer
	{
	public:
		virtual ~Renderer() = default;
		
		virtual void Render() const = 0;

		virtual void SetCamera(CameraComponent* pCamera) = 0;
		virtual void SetBackgroundColor(const SDL_Color& color) = 0;

		virtual void RenderTexture(const Texture2D& texture, const glm::vec2& position, float rotation, const glm::vec2& scale) const = 0;
		virtual void RenderPartialTexture(const Texture2D& texture, int srcX, int srcY, int srcW, int srcH, const glm::vec2& position, float rotation, const glm::vec2& scale) const = 0;
	};

	class NullRenderer final : public Renderer
	{
	public:
		void Render() const override {}

		void SetCamera(CameraComponent*) override {}
		void SetBackgroundColor(const SDL_Color&) override {}

		void RenderTexture(const Texture2D&, const glm::vec2&, float, const glm::vec2&) const override {}
		void RenderPartialTexture(const Texture2D&, int, int, int, int, const glm::vec2&, float, const glm::vec2&) const override {}
	};

	class GLRenderer final : public Renderer
	{
	public:
		GLRenderer(SDL_Window* window);
		~GLRenderer() override;

        GLRenderer(const GLRenderer& other) = delete;
        GLRenderer(GLRenderer&& other) noexcept = delete;
        GLRenderer& operator=(const GLRenderer& other) = delete;
        GLRenderer& operator=(GLRenderer&& other) noexcept = delete;

		void Render() const override;
		
		void SetCamera(CameraComponent* pCamera) override;
		void SetBackgroundColor(const SDL_Color& color) override { m_ClearColor = color; }

		void RenderTexture(const Texture2D& texture, const glm::vec2& position, float rotation, const glm::vec2& scale) const override;
		void RenderPartialTexture(const Texture2D& texture, int srcX, int srcY, int srcW, int srcH, const glm::vec2& position, float rotation, const glm::vec2& scale) const override;

	private:
		CameraComponent* m_pCamera{};

		SDL_GLContext m_Context;
		SDL_Window* m_pWindow{};
		SDL_Color m_ClearColor{};
	};
}
