#pragma once

struct SDL_Texture;

namespace Mage
{
    // Simple RAII wrapper for an SDL_Texture
    class Texture2D
    {
    public:
        explicit Texture2D(SDL_Texture* texture, float pixelsPerUnit, float pivotX = 0.5, float pivotY = 0.5);
        ~Texture2D();

        Texture2D(const Texture2D&) = delete;
        Texture2D(Texture2D&&) = delete;
        Texture2D& operator= (const Texture2D&) = delete;
        Texture2D& operator= (const Texture2D&&) = delete;

        SDL_Texture* GetSDLTexture() const;
        const glm::vec2& GetPivot() const;
        float GetPixelsPerUnit() const;

    private:
        SDL_Texture* m_Texture;
        glm::vec2 m_Pivot{};
        float m_PixelsPerUnit{};
    };
}
