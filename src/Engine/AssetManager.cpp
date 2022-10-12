#include "AssetManager.h"
#include "Graphics.h"
#include "SDL_image.h"
#include <iostream>


void AssetManager::Init(std::shared_ptr<Graphics> gfx) {
    m_Gfx = gfx;
}

void AssetManager::Cleanup() {
    // Cleanup all assets loaded into the manager
    for (auto& pair : m_Textures) {
        // Destroy allocated texture
        SDL_DestroyTexture(pair.second);
    }
    // Remove entries from the map
    m_Textures.clear();

    for (auto& pair : m_Fonts) {
        // Destroy allocated font
        TTF_CloseFont(pair.second);
    }
    m_Fonts.clear();

    m_Gfx.reset();
}

void AssetManager::loadTexture(std::string tag, const std::string& path) {
	    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        newTexture = m_Gfx->TextureFromSurface(loadedSurface);
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    m_Textures.insert(std::pair<std::string, SDL_Texture*>(tag,newTexture));
}

void AssetManager::LoadFont(std::string tag, const std::string& filepath, const int& size) {
    TTF_Font * font = TTF_OpenFont(filepath.c_str(), size);
    if (nullptr == font)
        std::cerr << "Could not open font at path: " << filepath << std::endl;
	m_Fonts[tag] = font;
    SDL_Log("Loaded font %s", tag.c_str());
}