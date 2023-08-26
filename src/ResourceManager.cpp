#include "ResourceManager.h"

namespace gs
{

ResourceManager::ResourceManager(const fs::path& path)
    : _respath{path}
{
}

std::string ResourceManager::getFilename(const std::string& name)
{
    auto filepath = _respath / name;
    return filepath.string();
}


// sf::Texture& ResourceManager::cacheTexture(const std::string& name)
// {
//     return {};
// }

// sf::Texture& ResourceManager::getTexture(const std::string& name)
// {
//     return {};
// }

sf::SoundBuffer* ResourceManager::cacheSound(const std::string & name)
{
    if (auto temp = getSound(name);
        temp != nullptr)
    {
        return temp;
    }

    const auto filename = getFilename(name);
    if (sf::SoundBuffer sound; sound.loadFromFile(filename))
    {
        auto retval = _soundcache.emplace(name, std::move(sound));
        return &(retval.first->second);
    }

    return nullptr;
}

sf::SoundBuffer* ResourceManager::getSound(const std::string & name)
{
    if (_soundcache.find(name) != _soundcache.end())
    {
        return &(_soundcache[name]);
    }

    return nullptr;
}

} // namespace
