#include "GameEngine.h"
#include "SplashScreen.h"
#include "MainMenuScreen.h"
#include "AudioService.h"

namespace gs
{

GameEngine::GameEngine(sf::RenderTarget& target, const boost::filesystem::path& respath)
    : _target{ target },
      _resources{ respath }
{
    initAudioService();
    _currentScreen = std::make_shared<SplashScreen>(_target, _resources);
}

void GameEngine::drawScreen()
{
    assert(_currentScreen);
    _currentScreen->draw();
}

void GameEngine::update()
{
    assert(_currentScreen);
    if (const auto res = _currentScreen->update();
        res.type == gs::ActionType::CHANGE_SCREEN)
    {
        const auto screenId = boost::any_cast<std::uint16_t>(res.data);
        this->changeScreen(screenId);
    }
}

PollResult GameEngine::poll(const sf::Event& e)
{
    const auto result = _currentScreen->poll(e);
    if (result.type == ActionType::CHANGE_SCREEN)
    {
        changeScreen(boost::any_cast<std::uint16_t>(result.data));
    }
    else if (result.type == ActionType::EXIT_GAME)
    {
        return result;
    }

    return {};
}

void GameEngine::changeScreen(std::uint16_t screenId)
{
    switch (screenId)
    {
        default:
            throw std::runtime_error("invalid screenId");
        break;
            
        case SCREEN_SPLASH:
            _currentScreen->close();
            _currentScreen.reset();
            _currentScreen = std::make_shared<SplashScreen>(_target, _resources);
        break;
            
        case SCREEN_MAINMENU:
            _currentScreen->close();
            _currentScreen.reset();
            _currentScreen = std::make_shared<MainMenuScreen>(_target, _resources);
        break;
    }
}

void GameEngine::initAudioService()
{
    auto musicptr = std::make_shared<gs::MusicAudio>(_resources);
    musicptr->setAllVolume(100);
    
    auto soundptr = std::make_shared<gs::SfxAudio>(_resources);
    soundptr->setAllVolume(100);
    
    gs::AudioLocator::setMusic(musicptr);
    gs::AudioLocator::setSound(soundptr);
}

} // namespace gs
