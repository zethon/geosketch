#pragma once
#include <boost/filesystem.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace gs
{

namespace fs = boost::filesystem;

class ResourceManager
{
  
public:
    explicit ResourceManager(const fs::path& path);
    
    std::string getFilename(const std::string& name);
    
    sf::Texture& cacheTexture(const std::string& name);
    sf::Texture& getTexture(const std::string& name);
    
    sf::SoundBuffer* cacheSound(const std::string& name);
    sf::SoundBuffer* getSound(const std::string& name);
    void clearSoundCache() { _soundcache.clear();  }
    
    template<typename T>
    std::optional<T> load(const std::string& name)
    {
        auto filepath = _respath / name;
        if (T item; item.loadFromFile(filepath.string()))
        {
            return item;
        }

        return {};
    }
    
    template<typename T>
    std::unique_ptr<T> openUniquePtr(const std::string& name)
    {
        auto filepath = _respath / name;
        std::unique_ptr<T> item = std::make_unique<T>();
        if (item->openFromFile(filepath.string()))
        {
            return item;
        }

        return {};
    }
    
private:
    fs::path    _respath;
    
    using SoundCache = std::map<std::string, sf::SoundBuffer>;
    SoundCache  _soundcache;
};

} // namespace gs
