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

        SDL_SetRenderDrawColor(m_Renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(m_Renderer, &scene->m_Transforms[e].m_Rect);
    }

    // Render all sprites
    for (Entity e = 0; e < MAX_ENTITIES; e++) {
        if (!scene->HasComponents(e, CompTags::Transform | CompTags::Sprite)) { continue; }

        // drawsprites
        if (&scene->m_SpriteRenders[e].m_Visible) {
            DrawSprite(&scene->m_SpriteRenders[e], &scene->m_Transforms[e]);
        }
    }
    // Draw all text components
    for (Entity e = 0; e < MAX_ENTITIES; e++) {
        if (!scene->HasComponents(e, CompTags::Text | CompTags::Transform)) { continue; }

        DrawText(&scene->m_Texts[e], &scene->m_Transforms[e]);
    }
}

void Graphics::DrawTexture(SDL_Texture* texture, SDL_Rect* dest) {
    SDL_RenderCopyEx(m_Renderer, texture, 0, dest, 0.0, 0, SDL_FLIP_NONE);
}

void Graphics::DrawText(ECS::Text* text, ECS::Transform* transform) {
    SDL_Texture* texture = TextureFromString(text->m_Text, text->m_Font, text->m_Color);
    int width, height;
    SDL_QueryTexture(texture, 0, 0, &width, &height);
    SDL_Rect dest = { transform->m_Rect.x, transform->m_Rect.y, width, height };
    DrawTexture(texture, &dest );
    SDL_DestroyTexture(texture);
}

void Graphics::DrawSprite(ECS::SpriteRender* sprite, ECS::Transform* transform) {
    DrawTexture(sprite->m_Sprite, &transform->m_Rect);
}

SDL_Texture* Graphics::TextureFromSurface(SDL_Surface* surface) {
    return SDL_CreateTextureFromSurface(m_Renderer, surface);
}

SDL_Texture* Graphics::TextureFromString(const std::string& string, TTF_Font* font, SDL_Color color) {
    SDL_Texture * textTexture = nullptr;
	SDL_Surface * textSurface = TTF_RenderText_Blended(font, string.c_str(), color);
	if (textSurface != nullptr) {
		textTexture = SDL_CreateTextureFromSurface(m_Renderer, textSurface);
		SDL_FreeSurface(textSurface);
	}
	else {
		std::cout << "Failed to create texture from string: " << string << std::endl;
		std::cout << TTF_GetError() << std::endl;
	}

    if (textTexture == nullptr)
        std::cerr << "Text Texture was not made!" << std::endl;
	return textTexture;
}