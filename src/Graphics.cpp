#include "Graphics.h"
#include <iostream>

#define RENDER_SCALE 20.0f

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define WORLD_WIDTH = SCREEN_WIDTH / RENDER_SCALE
#define WORLD_HEIGHT = - (SCREEN_HEIGHT / RENDER_SCALE)

#define TRANSPOSE(val) ((int)(RENDER_SCALE * val))
#define TRANSPOSE_X(x) ((int)(RENDER_SCALE * x))
#define TRANSPOSE_Y(y) (windowHeight - (int)(RENDER_SCALE * y))

SDL_Window* Graphics::window = NULL;
SDL_Renderer* Graphics::renderer = NULL;
int Graphics::windowWidth = 0;
int Graphics::windowHeight = 0;

int Graphics::Width() {
    return windowWidth;
}

int Graphics::Height() {
    return windowHeight;
}

bool Graphics::OpenWindow(bool fullscreen) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Error initializing SDL" << std::endl;
        return false;
    }
    // SDL_DisplayMode display_mode;
    // SDL_GetCurrentDisplayMode(0, &display_mode);
    windowWidth = SCREEN_WIDTH;
    windowHeight = SCREEN_HEIGHT;

    Uint32 flags = SDL_WINDOW_SHOWN;
	if (fullscreen) {
		flags |= SDL_WINDOW_FULLSCREEN;
	}

    window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, flags);
    if (!window) {
        std::cerr << "Error creating SDL window" << std::endl;
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Error creating SDL renderer" << std::endl;
        return false;
    }

    return true;
}

void Graphics::ClearScreen(Uint32 color) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, color >> 16, color >> 8, color, 255);
    SDL_RenderClear(renderer);
}

void Graphics::RenderFrame() {
    SDL_RenderPresent(renderer);
}

void Graphics::DrawLine(float x0, float y0, float x1, float y1, Uint32 color) {
    lineColor(renderer, TRANSPOSE_X(x0), TRANSPOSE_Y(y0), TRANSPOSE_X(x1), TRANSPOSE_Y(y1), color);
}

void Graphics::DrawCircle(float x, float y, float radius, float angle, Uint32 color) {
    circleColor(renderer, TRANSPOSE_X(x), TRANSPOSE_Y(y), TRANSPOSE(radius), color);
    lineColor(renderer, TRANSPOSE_X(x), TRANSPOSE_Y(y), TRANSPOSE_X(x) + TRANSPOSE(cos(angle) * radius), TRANSPOSE_Y(y) + TRANSPOSE(sin(angle) * radius), color);
}

void Graphics::DrawFillCircle(float x, float y, float radius, Uint32 color) {
    filledCircleColor(renderer, TRANSPOSE_X(x), TRANSPOSE_Y(y), TRANSPOSE(radius), color);
}

void Graphics::DrawRect(float x, float y, float width, float height, Uint32 color) {
    DrawLine(x - width / 2, y - height / 2, x + width / 2, y - height / 2, color);
    DrawLine(x + width / 2, y - height / 2, x + width / 2, y + height / 2, color);
    DrawLine(x + width / 2, y + height / 2, x - width / 2, y + height / 2, color);
    DrawLine(x - width / 2, y + height / 2, x - width / 2, y - height / 2, color);
}

void Graphics::DrawFillRect(float x, float y, float width, float height, Uint32 color) {
    boxColor(renderer, TRANSPOSE_X(x - TRANSPOSE(width) / 2), TRANSPOSE_Y(y - TRANSPOSE(height) / 2), TRANSPOSE_X(x + TRANSPOSE(width) / 2), TRANSPOSE_Y(y + TRANSPOSE(height) / 2), color);
}

void Graphics::DrawPolygon(float x, float y, const std::vector<Vector2D>& vertices, Uint32 color) {
    for (int i = 0; i < vertices.size(); i++) {
        int currIndex = i;
        int nextIndex = (i + 1) % vertices.size();
        DrawLine(x + vertices[currIndex].x, y + vertices[currIndex].y, x + vertices[nextIndex].x, y + vertices[nextIndex].y, color);
    }
}

void Graphics::DrawFillPolygon(float x, float y, const std::vector<Vector2D>& vertices, Uint32 color) {
    std::vector<short> vx;
    std::vector<short> vy;
    for (int i = 0; i < vertices.size(); i++) {
        vx.push_back((int)vertices[i].x);
    }
    for (int i = 0; i < vertices.size(); i++) {
        vy.push_back((int)vertices[i].y);
    }
    filledPolygonColor(renderer, &vx[0], &vy[0], (int)vertices.size(), (int)color);
}

void Graphics::DrawText(char const* const text, float x, float y, float size) {
    DynamicText dynamicText("./fonts/roboto.ttf", (int)size);
    dynamicText.DrawText(renderer, text, (int)x, (int)y);
}

void Graphics::CloseWindow(void) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


// Initialize Static variables
// Should be initialized to false by default
bool DynamicText::s_ttfInitialized = false;

// Constructor
// example: "./assets/fonts/8bitOperatorPlus8-Regular.ttf"
DynamicText::DynamicText(char const* const fontfilepath, int fontSize) {
    // Just like when we initialize our graphics subsystem,
    // we need to do the same for our font system.         	
    if (!s_ttfInitialized && TTF_Init() == -1) {
        std::cout << "Could not initialize SDL2_ttf, error: " << TTF_GetError() << std::endl;
        Assert(false);
    } else {
        // Initialize TTF system only once, even if
        // we have multipl Text objects.
        s_ttfInitialized = true;
    }  
    // Load our font file and set the font size
    m_ourFont = TTF_OpenFont(fontfilepath, fontSize);
    // Confirm that it was loaded
    if (m_ourFont == nullptr) {
        std::cout << "Could not load font '" << fontfilepath << "'" << std::endl;
        exit(1);
    }
}

DynamicText::~DynamicText() {
    // Destroy our textured text
    SDL_DestroyTexture(m_texture);

    // Close our font subsystem
    TTF_CloseFont(m_ourFont);	
}

void DynamicText::DrawText(SDL_Renderer* renderer, char const* const text, int x, int y) {
    // Pixels from our text
    m_surface = TTF_RenderText_Solid(m_ourFont, text, {255,255,255});

    // Setup the texture
    m_texture = SDL_CreateTextureFromSurface(renderer, m_surface);
    // Free the surface
    // We are done with it after we have uploaded to
    // the texture
    SDL_FreeSurface(m_surface);

    // Create a rectangle to draw on
    m_rectangle.x = x;
    m_rectangle.y = y;
    m_rectangle.w = m_surface->w;
    m_rectangle.h = m_surface->h;
    // Render our text on a rectangle
    SDL_RenderCopy(renderer, m_texture, NULL, &m_rectangle);
    if (m_texture != nullptr){
        SDL_DestroyTexture(m_texture);
    }
}