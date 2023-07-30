#include <cassert>
#include <iostream>

#include <boost/program_options.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/filesystem.hpp>

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
#include "GSUtils.h"

namespace po = boost::program_options;
namespace fs = boost::filesystem;

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
        ("window-size,w", po::value<std::string>(), "window size")
        ("version,v", "print version string")
        ("logfile,l", po::value<std::string>(), "path of logfile")
        ("loglevel", po::value<std::string>(), "trace,debug,info,warning,error,critical,off")
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
    logger->info("built on {} for {}", BUILDTIMESTAMP, gs::getOsString());

    std::string resourceFolder = gs::defaultResourceFolder();
    if (vm.count("resources") > 0)
    {
        resourceFolder = vm["resources"].as<std::string>();
        
        // leading spaces can cause problems on macOS
        boost::algorithm::trim(resourceFolder);
    }
    logger->debug("resource folder: {}", resourceFolder);

    if (!gs::validateResourceFolder(resourceFolder))
    {
        logger->critical("invalid resource folder: {}", resourceFolder);
        return 1;
    }

    sf::RenderWindow win{sf::VideoMode(2560, 1440), APP_TITLE, sf::Style::Titlebar | sf::Style::Close};
    win.setFramerateLimit(60);

    gs::GameEngine engine{ win, fs::path{resourceFolder} };

    while (win.isOpen())
    {
        sf::Event e;
        while (win.pollEvent(e))
        {
            switch (e.type)
            {
                default:
                {
                    const auto result = engine.poll(e);
                    if (result.type == gs::ActionType::EXIT_GAME)
                    {
                        win.close();
                    }
                }
                break;

                case sf::Event::EventType::Closed:
                {
                    win.close();
                }
                break;
            }
        }

        engine.update();

        win.clear();
        engine.drawScreen();
        win.display();
    }
 
    return 0;
}
