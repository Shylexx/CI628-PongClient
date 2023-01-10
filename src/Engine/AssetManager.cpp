#include "AssetManager.h"
#include "Graphics.h"
#include "SDL_image.h"
#include <iostream>

std::unordered_map<std::string, SDL_Texture*> AssetManager::Textures;
std::unordered_map<std::string, TTF_Font*> AssetManager::Fonts;
std::unordered_map<std::string, Mix_Chunk*> AssetManager::SFX;

void AssetManager::Cleanup() {
    // Cleanup all assets loaded into the manager
    for (auto& pair : Textures) {
        // Destroy allocated texture
        SDL_DestroyTexture(pair.second);
    }
    // Remove entries from the map
    Textures.clear();

    for (auto& pair : Fonts) {
        // Destroy allocated font
        TTF_CloseFont(pair.second);
    }
    Fonts.clear();

    for (auto& pair : SFX) {
      Mix_FreeChunk(pair.second);
    }
    SFX.clear();

}

void AssetManager::loadTexture(std::string tag, const std::string& path, Graphics* graphics) {
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
        newTexture = graphics->TextureFromSurface(loadedSurface);
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    Textures.insert(std::pair<std::string, SDL_Texture*>(tag,newTexture));
    SDL_Log("Loaded Texture %s", tag.c_str());
}

void AssetManager::LoadFont(std::string tag, const std::string& filepath, const int& size) {
    TTF_Font * font = TTF_OpenFont(filepath.c_str(), size);
    if (nullptr == font)
        std::cerr << "Could not open font at path: " << filepath << std::endl;
	  Fonts[tag] = font;
    SDL_Log("Loaded font %s", tag.c_str());
}


void AssetManager::loadSFX(std::string tag, const std::string& filepath) {
  Mix_Chunk* sfx = Mix_LoadWAV(filepath.c_str());
  if (sfx) {
    std::cout << "Sfx Loaded" << std::endl;
  }
  else {
    printf("Mix_LoadWAV: %s\n", Mix_GetError());
  }
  SFX.insert(std::pair<std::string, Mix_Chunk*>(tag, sfx));
}