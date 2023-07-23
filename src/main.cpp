#include <cassert>
#include <iostream>

#include <boost/program_options.hpp>

#include <fmt/core.h>

#include <SFML/Graphics.hpp>

#include "core.h"
#include "GameEngine.h"
#include "PollResult.h"

namespace po = boost::program_options;

int main(int argc, char *argv[])
{
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,?", "print help message")
        ("resources,r", po::value<std::string>(), "path of resource folder")
        ("window-size,w", po::value<std::string>(), "window size")
        ("version,v", "print version string")
        ;

    po::variables_map vm;

    try
    {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
    }
    catch(const po::error& e)
    {
        std::cerr << e.what();
        return -1;
    }

    if (vm.count("help"))
    {
        std::cout << desc << "\n";
        return 0;
    }
    else if (vm.count("version"))
    {
        std::cout << fmt::format("{}\n", APP_TITLE);
        std::cout << fmt::format("build-date : {}\n", BUILDTIMESTAMP);
        std::cout << std::endl;
        return 0;
    }

    std::shared_ptr<sf::RenderWindow> win;
    win = std::make_shared<sf::RenderWindow>(sf::VideoMode(2560, 1440), APP_TITLE, sf::Style::Titlebar | sf::Style::Close);
    win->setFramerateLimit(60);

    gs::GameEngine engine{ win };

    while (win->isOpen())
    {
        sf::Event e;
        while (win->pollEvent(e))
        {
            switch (e.type)
            {
                default:
                {
                    const auto result = engine.poll(e);
                    if (result.type == gs::ActionType::EXIT_GAME)
                    {
                        win->close();
                    }
                }
                break;

                case sf::Event::EventType::Closed:
                {
                    win->close();
                }
                break;
            }
        }

        engine.update();

        win->clear();
        engine.drawScreen();
        win->display();
    }

    // sf::Font font;
    // if (!font.loadFromFile("/Users/addy/src/ttvg/resources/fonts/mono.ttf")) 
    // {
    //     return -1;
    // }
    // sf::Text message("Hello, World !", font);

    // while (win->isOpen())
    // {
    //     sf::Event e;
    //     while (win->pollEvent(e))
    //     {

    //         switch (e.type) 
    //         {
    //             case sf::Event::EventType::Closed:
    //                 win->close();
    //             break;
    //             default:
    //             break;
    //         }
    //     }

    //     win->clear();
    //     win->draw(message);
    //     win->display();
    // }
    return 0;
}
