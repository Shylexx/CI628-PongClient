#ifndef __ASSET_MANAGER_H__
#define __ASSET_MANAGER_H__
#include <string>
#include <unordered_map>
#include <memory>
#include <SDL.h>
#include <SDL_ttf.h>
#include "../Engine/Graphics.h"

class AssetManager {
private:
	std::shared_ptr<Graphics> m_Gfx;
public:
	std::unordered_map<std::string, SDL_Texture*> m_Textures;
	std::unordered_map<std::string, TTF_Font*> m_Fonts;
	void Init(std::shared_ptr<Graphics> gfx);
	void Cleanup();
	// Should return a map object
	void loadMap(const std::string& filepath);
	void loadTexture(std::string tag, const std::string& filepath);
	void LoadFont(std::string tag, const std::string& filepath, const int& size);
};

#endif