#include "Graphics.h"
#include "../ECS/ECS.h"
#include <iostream>

void Graphics::Init() {
    // Window
    m_Window = SDL_CreateWindow(
        "Multiplayer Pong Client",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_SHOWN
    );

    if (!m_Window) {
        std::cerr << "Failed to create window" << SDL_GetError() << std::endl;
    }

    // Renderer
    m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);

    if (!m_Renderer) {
        std::cerr << "Failed to create renderer" << SDL_GetError() << std::endl;
    }
}

void Graphics::Cleanup() {
    SDL_DestroyWindow(m_Window);
}


void Graphics::ClearScreen() {
    SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_Renderer);
}

void Graphics::Present() {
    SDL_RenderPresent(m_Renderer);
}

void Graphics::DrawScene(ECS::Scene* scene) {
    for (Entity e = 0; e < MAX_ENTITIES; e++) {
        if (!scene->HasComponents(e, CompTags::Transform)) { continue; }

        // TODO: DRAW RECT FOR EVERY TRANSFORM IN SCENE
        SDL_SetRenderDrawColor(m_Renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(m_Renderer, &scene->m_Transforms[e].m_Rect);
    }
}