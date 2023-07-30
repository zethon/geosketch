#include "ResourceManager.h"

namespace gs
{

ResourceManager::ResourceManager(const fs::path& path)
    : _respath{path}
{
}

sf::Texture& ResourceManager::cacheTexture(const std::string& name)
{
}

sf::Texture& ResourceManager::getTexture(const std::string& name)
{
}

} // namespace
