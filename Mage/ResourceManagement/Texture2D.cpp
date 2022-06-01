#include "Mage/MagePCH.h"
#include "Texture2D.h"

Mage::Texture2D::Texture2D(GLuint texture, int width, int height, float pixelsPerUnit, const glm::vec2& pivot)
	: m_Texture{ texture}
	, m_Width{ width }
	, m_Height{ height }
	, m_Pivot{ pivot }
	, m_PixelsPerUnit{ pixelsPerUnit }
{}

Mage::Texture2D::Texture2D(SDL_Surface* pSurface, int width, int height, float pixelsPerUnit, const glm::vec2& pivot)
	: m_Width{ width }
	, m_Height{ height }
	, m_Pivot{ pivot }
	, m_PixelsPerUnit{ pixelsPerUnit }
{
	// Code from Prog2 framework

	// Get pixel format information and translate to OpenGl format
	GLenum pixelFormat{ GL_RGB };
	switch (pSurface->format->BytesPerPixel)
	{
		case 3:
		{
			if (pSurface->format->Rmask == 0x000000ff)
			{
				pixelFormat = GL_RGB;
			}
			else
			{
				pixelFormat = GL_BGR;
			}
			break;
		}
		case 4:
		{
			if (pSurface->format->Rmask == 0x000000ff)
			{
				pixelFormat = GL_RGBA;
			}
			else
			{
				pixelFormat = GL_BGRA;
			}
			break;
		}
		default:
		{
			std::cout << "Error creating texture: unknow pixel format, BytesPerPixel: " << (int)pSurface->format->BytesPerPixel << ". Use 32 bit or 24 bit images.\n";
			return;
		}
	}

	// Generate id
	glGenTextures(1, &m_Texture);

	// Set texture as the one we're working with
	glBindTexture(GL_TEXTURE_2D, m_Texture);

	// check for errors. Can happen if a texture is created while a static pointer is being initialized, even before the call to the main function.
	GLenum e = glGetError();
	if (e != GL_NO_ERROR)
	{
		std::cerr << "Texture::CreateFromSurface, error binding textures, Error id = " << e << '\n';
		std::cerr << "Can happen if a texture is created before performing the initialization code (e.g. a static Texture object).\n";
		std::cerr << "There might be a white rectangle instead of the image.\n";
	}

	// Copy data from SDL surface to OpenGL texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pSurface->w, pSurface->h, 0, pixelFormat, GL_UNSIGNED_BYTE, pSurface->pixels);

	// Texture does not repeat
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Use nearest neighbour interpolation
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

Mage::Texture2D::~Texture2D()
{
	glDeleteTextures(1, &m_Texture);
}