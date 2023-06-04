#include <cassert>

#include <boost/program_options.hpp>

#include <SFML/Graphics.hpp>

namespace po = boost::program_options;

int main() 
{
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,?", "print help message")
        ("resources,r", po::value<std::string>(), "path of resource folder")
        ("window-size,w", po::value<std::string>(), "window size")
        ("version,v", "print version string")
        ;


    std::shared_ptr<sf::RenderWindow> win;
    win = std::make_shared<sf::RenderWindow>(sf::VideoMode(1280, 720),
        "Hello World SFML Window", sf::Style::Titlebar | sf::Style::Close);
    win->setFramerateLimit(60);
    sf::Font font;
    if (!font.loadFromFile("/Users/addy/src/ttvg/resources/fonts/mono.ttf")) 
    {
        return -1;
    }
    sf::Text message("Hello, World !", font);

    while (win->isOpen())
    {
        sf::Event e;
        while (win->pollEvent(e))
        {

            switch (e.type) 
            {
                case sf::Event::EventType::Closed:
                    win->close();
                break;
                default:
                break;
            }
        }

        win->clear();
        win->draw(message);
        win->display();
    }
    return 0;
}
