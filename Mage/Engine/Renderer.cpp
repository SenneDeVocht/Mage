#include "Mage/MagePCH.h"
#include "Renderer.h"

#include "Mage/Scenegraph/SceneManager.h"
#include "Mage/ResourceManagement/Texture2D.h"

#include "imgui.h"
#include "backends/imgui_impl_opengl2.h"
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
	m_pWindow = window;
	
	m_Context = SDL_GL_CreateContext(m_pWindow);

	if (glewInit() != GLEW_OK)
		return;

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

void Mage::Renderer::SetCamera(CameraComponent* pCamera)
{
	m_pCamera = pCamera;
}

void Mage::Renderer::Render() const
{
	// Clear last frame
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Render everything
	if (m_pCamera != nullptr)
	{
		if (m_pCamera->IsEnabled())
			SceneManager::GetInstance().Render();
	}
	
	// Render ImGui
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	// Display
	SDL_GL_SwapWindow(m_pWindow);

	// Additional ImGui viewports
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
		SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
	}
}

void Mage::Renderer::Destroy()
{
	SDL_GL_DeleteContext(m_Context);
}

void Mage::Renderer::RenderTexture(const Texture2D& texture, float dstX, float dstY) const
{
	RenderPartialTexture(texture, 0, 0, texture.GetWidth(), texture.GetHeight(), dstX, dstY);
}

void Mage::Renderer::RenderPartialTexture(const Texture2D& texture, int srcX, int srcY, int srcW, int srcH, float dstX, float dstY) const
{
	// Source Rect (Part of texture)
	SDL_FRect src{};
	src.x = (float)srcX;
	src.y = (float)srcY;
	src.w = (float)srcW;
	src.h = (float)srcH;

	// Destination Rect (Part of world)
	SDL_FRect dst{};
	dst.x = dstX - texture.GetPivot().x * srcW / texture.GetPixelsPerUnit();
	dst.y = dstY - texture.GetPivot().y * srcH / texture.GetPixelsPerUnit();
	dst.w = srcW / (float)texture.GetPixelsPerUnit();
	dst.h = srcH / (float)texture.GetPixelsPerUnit();

	// Render
	// Determine texture coordinates
	const float uvLeft = src.x / texture.GetWidth();
	const float uvRight = (src.x + src.w) / texture.GetWidth();
	const float uvTop = (src.y - src.h) / texture.GetHeight();
	const float uvBottom = src.y / texture.GetHeight();

	// Determine vertex coordinates
	const float vertexLeft{ (float)dst.x };
	const float vertexBottom{ (float)dst.y };
	const float vertexRight{ vertexLeft + dst.w };
	const float vertexTop{ vertexBottom + dst.h };	

	// Tell opengl which texture we will use
	glBindTexture(GL_TEXTURE_2D, texture.GetGLTexture());
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	// Draw
	glPushMatrix();

	// First translate [0, 0] to the center of the screen
	int windowWidth, windowHeight;
	SDL_GetWindowSize(m_pWindow, &windowWidth, &windowHeight);
	glTranslatef((float)windowWidth / 2.f, (float)windowHeight / 2.f, 0.0f);

	// Scale to camera size
	const auto camSize = m_pCamera->GetSize();
	glScalef(windowWidth / camSize.x, windowHeight / camSize.y, 1.0f);

	// Translate to camera position
	const auto camPos = m_pCamera->GetGameObject()->GetTransform()->GetWorldPosition();
	glTranslatef(-camPos.x, -camPos.y, 0.0f);

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
