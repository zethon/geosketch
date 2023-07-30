#pragma once
#include <boost/filesystem.hpp>

#include <SFML/Graphics.hpp>

namespace gs
{

namespace fs = boost::filesystem;

class ResourceManager
{
  
public:
    explicit ResourceManager(const fs::path& path);
    
    sf::Texture& cacheTexture(const std::string& name);
    sf::Texture& getTexture(const std::string& name);
    
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
    
private:
    fs::path    _respath;
    
};

} // namespace gs
