#include "Mage/MagePCH.h"
#include "Renderer.h"

#include <functional>

#include "Mage/Scenegraph/SceneManager.h"
#include "Mage/ResourceManagement/Texture2D.h"

#include "imgui.h"
#include "backends/imgui_impl_opengl2.h"
#include "backends/imgui_impl_sdl.h"
#include "Mage/Components/CameraComponent.h"
#include "Mage/Components/Transform.h"
#include "Mage/Scenegraph/GameObject.h"

#pragma region Implementation

namespace Mage {
	class GLRenderer::GLRendererImpl final
	{
	public:
		GLRendererImpl(SDL_Window* window);
		~GLRendererImpl();
        
        GLRendererImpl(const GLRendererImpl& other) = delete;
        GLRendererImpl(GLRendererImpl&& other) noexcept = delete;
        GLRendererImpl& operator=(const GLRendererImpl& other) = delete;
        GLRendererImpl& operator=(GLRendererImpl&& other) noexcept = delete;

		void SetCamera(CameraComponent* pCamera);
		void SetBackgroundColor(const SDL_Color& color);

	    void Render();

		void RenderPolygonOutline(const std::vector<glm::vec2>& positions, bool closed, const glm::vec4& color, float layer);
	    void RenderPolygonFilled(const std::vector<glm::vec2>& positions, const glm::vec4& color, float layer);
		void RenderTexture(const Texture2D* texture, const glm::vec2& position, float rotation, const glm::vec2& scale, float layer);
		void RenderPartialTexture(const Texture2D* texture, int srcX, int srcY, int srcW, int srcH, const glm::vec2& position, float rotation, const glm::vec2& scale, float layer);

	private:
		void ActuallyRenderPolygonOutline(const std::vector<glm::vec2>& positions, bool closed, const glm::vec4& color) const;
		void ActuallyRenderPolygonFilled(const std::vector<glm::vec2>& positions, const glm::vec4& color) const;
		void ActuallyRenderTexture(const Texture2D* texture, const glm::vec2& position, float rotation, const glm::vec2& scale) const;
		void ActuallyRenderPartialTexture(const Texture2D* texture, int srcX, int srcY, int srcW, int srcH, const glm::vec2& position, float rotation, const glm::vec2& scale) const;

		struct RenderCommand
		{
			std::function<void()> Command;
			float Layer;
		};

		std::vector<RenderCommand> m_RenderCommands;

		Mage::CameraComponent* m_pCamera{};

		SDL_GLContext m_Context;
		SDL_Window* m_pWindow{};
		SDL_Color m_ClearColor{};
	};
}

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

Mage::GLRenderer::GLRendererImpl::GLRendererImpl(SDL_Window* window)
    : m_pWindow{ window }
{
	m_Context = SDL_GL_CreateContext(m_pWindow);

	if (glewInit() != GLEW_OK)
		return;

	// VSync
	SDL_GL_SetSwapInterval(1);

	// Set the Projection matrix to the identity matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set up a two-dimensional orthographic viewing region.
	int windowWidth, windowHeight;
	SDL_GetWindowSize(m_pWindow, &windowWidth, &windowHeight);
	glOrtho(0, windowWidth, 0, windowHeight, -1, 1); // y from bottom to top

	// Set the viewport to the client window area
	// The viewport is the rectangular region of the window where the image is drawn.
	glViewport(0, 0, windowWidth, windowHeight);

	// Set the Modelview matrix to the identity matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Enable color blending and use alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Mage::GLRenderer::GLRendererImpl::~GLRendererImpl()
{
	SDL_GL_DeleteContext(m_Context);
}

void Mage::GLRenderer::GLRendererImpl::SetCamera(CameraComponent* pCamera)
{
	m_pCamera = pCamera;
}

void Mage::GLRenderer::GLRendererImpl::SetBackgroundColor(const SDL_Color& color)
{
    m_ClearColor = color;
}

void Mage::GLRenderer::GLRendererImpl::Render()
{
	// CLEAR
	//------
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// GAME WINDOW
	//------------
#pragma region GameWindow
	if (m_pCamera != nullptr)
	{
		if (m_pCamera->IsEnabled())
		{
			// Render game
			m_RenderCommands.clear();
			SceneManager::GetInstance().Render();
			std::stable_sort(
				m_RenderCommands.begin(), m_RenderCommands.end(),
				[](const RenderCommand& a, const RenderCommand& b)
				{
					return a.Layer < b.Layer;
				});

			for (auto& renderCommand : m_RenderCommands)
				renderCommand.Command();

			// Render Gizmos
#ifdef _DEBUG
			m_RenderCommands.clear();
			SceneManager::GetInstance().RenderGizmos();
			std::stable_sort(
				m_RenderCommands.begin(), m_RenderCommands.end(),
				[](const RenderCommand& a, const RenderCommand& b)
				{
					return a.Layer < b.Layer;
				});

			for (auto& renderCommand : m_RenderCommands)
				renderCommand.Command();
#endif
		}
	}
#pragma endregion

	// IMGUI
	//------
#pragma region ImGui
	// Render
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_pWindow);
	ImGui::NewFrame();

	SceneManager::GetInstance().DrawImGui();

	ImGui::Render();

	// Display
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	SDL_GL_SwapWindow(m_pWindow);

	// Additional ImGui viewports
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
		const SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();

		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();

		SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
	}
#pragma endregion
}

void Mage::GLRenderer::GLRendererImpl::RenderPolygonOutline(const std::vector<glm::vec2>& positions, bool closed, const glm::vec4& color, float layer)
{
	m_RenderCommands.push_back({ std::bind(&GLRendererImpl::ActuallyRenderPolygonOutline, this, positions, closed, color), layer });
}

void Mage::GLRenderer::GLRendererImpl::RenderPolygonFilled(const std::vector<glm::vec2>& positions, const glm::vec4& color, float layer)
{
	m_RenderCommands.push_back({ std::bind(&GLRendererImpl::ActuallyRenderPolygonFilled, this, positions, color), layer });
}

void Mage::GLRenderer::GLRendererImpl::RenderTexture(const Texture2D* texture, const glm::vec2& position, float rotation, const glm::vec2& scale, float layer)
{
	m_RenderCommands.push_back({ std::bind(&GLRendererImpl::ActuallyRenderTexture, this, texture, position, rotation, scale), layer });
}

void Mage::GLRenderer::GLRendererImpl::RenderPartialTexture(const Texture2D* texture, int srcX, int srcY, int srcW, int srcH, const glm::vec2& position, float rotation, const glm::vec2& scale, float layer)
{
	m_RenderCommands.push_back({ std::bind(&GLRendererImpl::ActuallyRenderPartialTexture, this, texture, srcX, srcY, srcW, srcH, position, rotation, scale), layer });
}

void Mage::GLRenderer::GLRendererImpl::ActuallyRenderPolygonOutline(const std::vector<glm::vec2>& positions, bool closed, const glm::vec4& color) const
{
	if (positions.size() <= 1)
		return;

	// Draw
	glPushMatrix();

	    // First translate [0, 0] to the center of the screen
	    int windowWidth, windowHeight;
	    SDL_GetWindowSize(m_pWindow, &windowWidth, &windowHeight);
	    glTranslatef((float)windowWidth / 2.f, (float)windowHeight / 2.f, 0.0f);

	    // Scale to camera size
	    const auto camSize = m_pCamera->GetSize() * m_pCamera->GetGameObject()->GetTransform()->GetWorldScale();
	    glScalef(windowWidth / camSize.x, windowHeight / camSize.y, 1.0f);

	    // Translate to camera position
	    const auto camPos = m_pCamera->GetGameObject()->GetTransform()->GetWorldPosition();
	    glTranslatef(-camPos.x, -camPos.y, 0.0f);

	    // Rotate with camera
	    glRotatef(m_pCamera->GetGameObject()->GetTransform()->GetWorldRotation(), 0.0f, 0.0f, 1.0f);

	    glBegin(GL_LINES);

	    	glColor4f(color.r, color.g, color.b, color.a);

	        for (size_t i{ 0 }; i < positions.size() - 1; ++i)
	        {
				const auto pos1 = positions[i];
				const auto pos2 = positions[i + 1];

		        glVertex2f(pos1.x, pos1.y);
		        glVertex2f(pos2.x, pos2.y);
	        }

	        if (closed)
	        {
				glVertex2f(positions.back().x, positions.back().y);
		        glVertex2f(positions[0].x, positions[0].y);
	        }

	    glEnd();

	glPopMatrix();
}

void Mage::GLRenderer::GLRendererImpl::ActuallyRenderPolygonFilled(const std::vector<glm::vec2>& positions, const glm::vec4& color) const
{
	// Draw
	glPushMatrix();

	    // First translate [0, 0] to the center of the screen
	    int windowWidth, windowHeight;
	    SDL_GetWindowSize(m_pWindow, &windowWidth, &windowHeight);
	    glTranslatef((float)windowWidth / 2.f, (float)windowHeight / 2.f, 0.0f);

	    // Scale to camera size
	    const auto camSize = m_pCamera->GetSize() * m_pCamera->GetGameObject()->GetTransform()->GetWorldScale();
	    glScalef(windowWidth / camSize.x, windowHeight / camSize.y, 1.0f);

	    // Translate to camera position
	    const auto camPos = m_pCamera->GetGameObject()->GetTransform()->GetWorldPosition();
	    glTranslatef(-camPos.x, -camPos.y, 0.0f);

	    // Rotate with camera
	    glRotatef(m_pCamera->GetGameObject()->GetTransform()->GetWorldRotation(), 0.0f, 0.0f, 1.0f);

	    glBegin(GL_QUADS);

	        glColor4f(color.r, color.g, color.b, color.a);

	        for (const auto& position : positions)
	        {
		        glVertex2f(position.x, position.y);
	        }

	    glEnd();

	glPopMatrix();
}

void Mage::GLRenderer::GLRendererImpl::ActuallyRenderTexture(const Texture2D* texture, const glm::vec2& position, float rotation, const glm::vec2& scale) const
{
	ActuallyRenderPartialTexture(texture, 0, 0, texture->GetWidth(), texture->GetHeight(), position, rotation, scale);
}

void Mage::GLRenderer::GLRendererImpl::ActuallyRenderPartialTexture(const Texture2D* texture, int srcX, int srcY, int srcW, int srcH, const glm::vec2& position, float rotation, const glm::vec2& scale) const
{
	// Source Rect (Part of texture)
	SDL_FRect src{};
	src.x = (float)srcX;
	src.y = (float)srcY;
	src.w = (float)srcW;
	src.h = (float)srcH;

	// Destination Rect (Part of world)
	SDL_FRect dst{};
	dst.x = -texture->GetPivot().x * srcW / texture->GetPixelsPerUnit();
	dst.y = -texture->GetPivot().y * srcH / texture->GetPixelsPerUnit();
	dst.w = srcW / (float)texture->GetPixelsPerUnit();
	dst.h = srcH / (float)texture->GetPixelsPerUnit();

	// Render
	// Determine texture coordinates
	const float uvLeft = src.x / texture->GetWidth();
	const float uvRight = (src.x + src.w) / texture->GetWidth();
	const float uvTop = (src.y - src.h) / texture->GetHeight();
	const float uvBottom = src.y / texture->GetHeight();

	// Determine vertex coordinates
	const float vertexLeft{ (float)dst.x };
	const float vertexBottom{ (float)dst.y };
	const float vertexRight{ vertexLeft + dst.w };
	const float vertexTop{ vertexBottom + dst.h };

	// Tell opengl which texture we will use
	glBindTexture(GL_TEXTURE_2D, texture->GetGLTexture());
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	// Draw
	glPushMatrix();

	// First translate [0, 0] to the center of the screen
	int windowWidth, windowHeight;
	SDL_GetWindowSize(m_pWindow, &windowWidth, &windowHeight);
	glTranslatef((float)windowWidth / 2.f, (float)windowHeight / 2.f, 0.0f);

	// Scale to camera size
	const auto camSize = m_pCamera->GetSize() * m_pCamera->GetGameObject()->GetTransform()->GetWorldScale();
	glScalef(windowWidth / camSize.x, windowHeight / camSize.y, 1.0f);

	// Translate to camera position
	const auto camPos = m_pCamera->GetGameObject()->GetTransform()->GetWorldPosition();
	glTranslatef(-camPos.x, -camPos.y, 0.0f);

	// Rotate with camera
	glRotatef(m_pCamera->GetGameObject()->GetTransform()->GetWorldRotation(), 0.0f, 0.0f, 1.0f);

	// Transform texture
	glTranslatef(position.x, position.y, 0.0f);
	glRotatef(rotation, 0.0f, 0.0f, 1.0f);
	glScalef(scale.x, scale.y, 1.0f);

	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);

	glTexCoord2f(uvLeft, uvBottom);
	glVertex2f(vertexLeft, vertexBottom);

	glTexCoord2f(uvLeft, uvTop);
	glVertex2f(vertexLeft, vertexTop);

	glTexCoord2f(uvRight, uvTop);
	glVertex2f(vertexRight, vertexTop);

	glTexCoord2f(uvRight, uvBottom);
	glVertex2f(vertexRight, vertexBottom);

	glEnd();
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

#pragma endregion

Mage::GLRenderer::GLRenderer(SDL_Window* window)
    : m_pImpl{ std::make_unique<GLRendererImpl>(window) }
{}

Mage::GLRenderer::~GLRenderer() = default;

void Mage::GLRenderer::Render()
{
	m_pImpl->Render();
}

void Mage::GLRenderer::SetCamera(CameraComponent* pCamera)
{
	m_pImpl->SetCamera(pCamera);
}

void Mage::GLRenderer::SetBackgroundColor(const SDL_Color& color)
{
	m_pImpl->SetBackgroundColor(color);
}

void Mage::GLRenderer::RenderPolygonOutline(const std::vector<glm::vec2>& positions, bool closed, const glm::vec4& color, float layer)
{
	m_pImpl->RenderPolygonOutline(positions, closed, color, layer);
}

void Mage::GLRenderer::RenderPolygonFilled(const std::vector<glm::vec2>& positions, const glm::vec4& color, float layer)
{
	m_pImpl->RenderPolygonFilled(positions, color, layer);
}

void Mage::GLRenderer::RenderTexture(const Texture2D* texture, const glm::vec2& position, float rotation, const glm::vec2& scale, float layer)
{
	m_pImpl->RenderTexture(texture, position, rotation, scale, layer);
}

void Mage::GLRenderer::RenderPartialTexture(const Texture2D* texture, int srcX, int srcY, int srcW, int srcH, const glm::vec2& position, float rotation, const glm::vec2& scale, float layer)
{
	m_pImpl->RenderPartialTexture(texture, srcX, srcY, srcW, srcH, position, rotation, scale, layer);
}


