#ifndef __ASSET_MANAGER_H__
#define __ASSET_MANAGER_H__
#include <string>
#include <unordered_map>
#include <memory>
#include <SDL.h>
#include "SDL_mixer.h"
#include <SDL_ttf.h>
#include "../Engine/Graphics.h"

class AssetManager {
public:
	static std::unordered_map<std::string, SDL_Texture*> Textures;
	static std::unordered_map<std::string, TTF_Font*> Fonts;
	static std::unordered_map<std::string, Mix_Chunk*> SFX;
	static void Cleanup();
	// Should return a map object
	static void loadMap(const std::string& filepath);
	static void loadSFX(std::string tag, const std::string& filepath);
	static void loadTexture(std::string tag, const std::string& filepath, Graphics* graphics);
	static void LoadFont(std::string tag, const std::string& filepath, const int& size);
};

#endif