#include <cassert>
#include <iostream>
#include <optional>

#include <boost/program_options.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/filesystem.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/dll.hpp>

#include <fmt/core.h>

#include <spdlog/spdlog.h>
#include <spdlog/common.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>

#include <SFML/Graphics.hpp>

#include "core.h"
#include "GeoSketchLogger.h"
#include "GameEngine.h"
#include "PollResult.h"
#include "OSUtils.h"

namespace po = boost::program_options;
namespace fs = boost::filesystem;
namespace x3 = boost::spirit::x3;

bool validateResourceFolder(const fs::path& folder)
{
    namespace fs = boost::filesystem;
    
    if (!fs::exists(folder)) return false;
    const auto f = folder / "images" / "splash.jpg";
    return fs::exists(folder);
}

std::optional<std::string> defaultResourceFolder()
{
    const auto exepath = boost::dll::program_location().parent_path();
    auto resfolder = exepath / "resources";
    if (validateResourceFolder(resfolder.string())) return resfolder.string();

    resfolder = exepath.parent_path() / "Resources";
    if (validateResourceFolder(resfolder.string())) return resfolder.string();

    auto parent = exepath.parent_path();
    resfolder = parent / "Resources";
    if (validateResourceFolder(resfolder.string())) return resfolder.string();

    resfolder = parent / "resources";
    if (validateResourceFolder(resfolder.string())) return resfolder.string();
    
    return {};
}

bool validateDataFolder(const fs::path& folder)
{
    return true;
}

std::optional<std::string> defaultDataFolder()
{
    const auto exepath = boost::dll::program_location().parent_path();
    auto resfolder = exepath / "data";
    if (validateDataFolder(resfolder.string())) return resfolder.string();

    resfolder = exepath.parent_path() / "Data";
    if (validateDataFolder(resfolder.string())) return resfolder.string();

    auto parent = exepath.parent_path();
    resfolder = parent / "Data";
    if (validateDataFolder(resfolder.string())) return resfolder.string();

    resfolder = parent / "data";
    if (validateDataFolder(resfolder.string())) return resfolder.string();
    
    return {};
}

void initLogging(std::string_view logfile)
{
    // create the root logger
    spdlog::stdout_logger_mt(gs::log::GLOBAL_LOGGER);

#ifdef RELEASE
    spdlog::set_level(spdlog::level::info);
#else
    spdlog::set_level(spdlog::level::trace);
#endif

    if (logfile.size() > 0)
    {
        auto logger = gs::log::rootLogger();
        auto rotating = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
            logfile.data(), 1024 * 1024 * 5, 3);

        logger->sinks().push_back(rotating);
    }
}

int main(int argc, char *argv[])
{
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,?", "print help message")
        ("resources,r", po::value<std::string>(), "path of resource folder")
        ("data-dir,d", po::value<std::string>(), "path of data folder")
        ("window-size,w", po::value<std::string>(), "window size")
        ("version,v", "print version string")
        ("logfile,l", po::value<std::string>(), "path of logfile")
        ("loglevel", po::value<std::string>(), "trace,debug,info,warning,error,critical,off")
        ("mute", po::bool_switch()->default_value(false), "mute all sounds")
        ("screen,s", po::value<std::uint16_t>(), "start screen id")
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

    std::string logfile;
    if (vm.count("logfile"))
    {
        logfile = vm["logfile"].as<std::string>();

        // leading spaces can cause problems on macOS
        boost::algorithm::trim(logfile);
    }

    initLogging(logfile);
    auto logger = gs::log::rootLogger();
    if (vm.count("loglevel"))
    {
        const auto configLevel = spdlog::level::from_str(vm["loglevel"].as<std::string>());
        logger->set_level(configLevel);
    }
    
    logger->info("Starting {} v{}",APP_NAME_LONG, VERSION);
    logger->info("built on {} for {}", BUILDTIMESTAMP, lz::getOsString());
    
    gs::GameSettings settings;

    auto resourceFolder = defaultResourceFolder(); // does validation
    if (vm.count("resources") > 0)
    {
        resourceFolder = vm["resources"].as<std::string>();
        
        // leading spaces can cause problems on macOS
        boost::algorithm::trim(*resourceFolder);
        if (!validateResourceFolder(fs::path{*resourceFolder}))
        {
            resourceFolder.reset();
        }
    }
    
    if (!resourceFolder.has_value())
    {
        logger->critical("invalid resource folder: no resource folder specified");
        return 1;
    }
    logger->debug("resource folder: {}", *resourceFolder);

    //auto dataFolder = defaultDataFolder(); // does validation
    //if (vm.count("data-dir") > 0)
    //{
    //    dataFolder = vm["data-dir"].as<std::string>();
    //    
    //    // leading spaces can cause problems on macOS
    //    boost::algorithm::trim(*dataFolder);
    //    if (!validateDataFolder(fs::path{*dataFolder}))
    //    {
    //        dataFolder.reset();
    //    }
    //}

    //if (!dataFolder.has_value())
    //{
    //    logger->critical("invalid data folder: no data folder specified");
    //    return 1;
    //}

    if (vm.count("screen") > 0)
    {
        settings.startScreen = vm["screen"].as<std::uint16_t>();
    }
    
    settings.muteAllSounds = vm["mute"].as<bool>();
    
    std::optional<sf::Vector2u> winsize;
    if (vm.count("window-size") > 0)
    {
        winsize.emplace();
        std::string windowsize = vm["window-size"].as<std::string>();

        auto w = [&](auto& ctx) { winsize->x = _attr(ctx); };
        auto h = [&](auto& ctx) { winsize->y = _attr(ctx); };
        bool r = x3::phrase_parse(windowsize.begin(), windowsize.end(),
            (
                x3::int_[w] >> 'x' >> x3::int_[h]
            ),
            x3::space);

        if (!r || winsize->x < 480 || winsize->y < 320)
        {
            logger->critical("invalid screen size '{}'\n", windowsize);
            return 1;
        }
    }
    
    std::unique_ptr<sf::RenderWindow> win;
    if (winsize)
    {
        win = std::make_unique<sf::RenderWindow>(
            sf::VideoMode(static_cast<unsigned int>(winsize->x), static_cast<unsigned int>(winsize->y)), APP_NAME_LONG, sf::Style::Titlebar | sf::Style::Close);
    }
    else
    {
        win = std::make_unique<sf::RenderWindow>(
            sf::VideoMode::getDesktopMode(), APP_NAME_LONG, sf::Style::Fullscreen);
    }

    win->setFramerateLimit(60);

    gs::GameEngine engine{ *win, fs::path{*resourceFolder}, settings };

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
 
    win.release();
    return 0;
}
