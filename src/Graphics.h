#pragma once

#include "SDL.h"
#include "SDL2_gfxPrimitives.h"
#include "SDL_ttf.h"
#include "common/Vector2D.h"
#include "common/utils.h"
#include <vector>

struct Graphics {
    static int windowWidth;
    static int windowHeight;
    static SDL_Window* window;
    static SDL_Renderer* renderer;

    static int Width();
    static int Height();
    static bool OpenWindow(bool fullscreen);
    static void CloseWindow();
    static void ClearScreen(Uint32 color);
    static void RenderFrame();
    static void DrawLine(float x0, float y0, float x1, float y1, Uint32 color);
    static void DrawCircle(float x, float y, float radius, float angle, Uint32 color);
    static void DrawFillCircle(float x, float y, float radius, Uint32 color);
    static void DrawRect(float x, float y, float width, float height, Uint32 color);
    static void DrawFillRect(float x, float y, float width, float height, Uint32 color);
    static void DrawPolygon(float x, float y, const std::vector<Vector2D>& vertices, Uint32 color);
    static void DrawFillPolygon(float x, float y, const std::vector<Vector2D>& vertices, Uint32 color);
    static void DrawText(char const* const text, float x, float y, float = 14.0f);
};

class DynamicText{
    public:

		// Constructor
        // example: "./assets/fonts/8bitOperatorPlus8-Regular.ttf"
        DynamicText(char const* const fontfilepath, int fontSize);
        // Destructor
        ~DynamicText();
        // Function for drawing text
		void DrawText(SDL_Renderer* renderer, char const* const text, int x, int y);

	private:
        // Rect,texture, and surface that we draw our font to.
		SDL_Rect m_rectangle;
		SDL_Texture* m_texture;
		SDL_Surface* m_surface;
        // The actual font
		TTF_Font* m_ourFont;
        // Purpose is to ensure that TTF library is only
        // initialized one time
		static bool s_ttfInitialized;
};