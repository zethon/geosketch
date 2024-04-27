#include "GameEngine.h"
#include "SplashScreen.h"
#include "MainMenuScreen.h"
#include "AudioService.h"
#include "GameSelectScreen.h"
#include "SettingsScreen.h"
#include "GameScreen.h"

std::ostream& operator<<(std::ostream &os, const gs::NewGameSettings::Difficulty &diff)
{
    switch (diff)
    {
        case gs::NewGameSettings::Difficulty::EASY:
            os << "Easy";
        break;
        
        case gs::NewGameSettings::Difficulty::MEDIUM:
            os << "Medium";
        break;
        
        case gs::NewGameSettings::Difficulty::HARD:
            os << "Hard";
        break;
        
        case gs::NewGameSettings::Difficulty::EXPERT:
            os << "Expert";
        break;
    }
    
    return os;
}

std::ostream& operator<<(std::ostream &os, const gs::NewGameSettings::GameType &type)
{
    switch (type)
    {
        case gs::NewGameSettings::GameType::FREE:
            os << "Free";
        break;
        
        case gs::NewGameSettings::GameType::TIMED:
            os << "Timed";
        break;

        case gs::NewGameSettings::GameType::COUNTDOWN:
            os << "Countdown";
        break;
    }
    
    return os;
}

std::ostream& operator<<(std::ostream &os, const gs::NewGameSettings &settings)
{
    os << "Game Settings { Difficulty: " << settings.difficulty << ", GameType: " << settings.gameType << " }";
    return os;
}

namespace gs
{

GameEngine::GameEngine(sf::RenderTarget& target, const boost::filesystem::path& respath, const GameSettings& settings)
    : _target{ target },
      _resources{ respath },
      _settings{ settings }
{
    initAudioService();
    changeScreen(settings.startScreen);
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
    else if (result.type == ActionType::NEW_GAME)
    {
        const auto newGameSettings = boost::any_cast<NewGameSettings>(result.data);
        changeScreen(SCREEN_GAME, newGameSettings);
    }
    else if (result.type == ActionType::EXIT_GAME)
    {
        return result;
    }

    return {};
}

void GameEngine::changeScreen(std::uint16_t screenId, const NewGameSettings& settings)
{
    if (_currentScreen)
    {
        _currentScreen->close();
        _currentScreen.reset();
    }
    
    switch (screenId)
    {
        default:
            throw std::runtime_error("invalid screenId");
        break;
            
        case SCREEN_SPLASH:
            _currentScreen = std::make_shared<SplashScreen>(_target, _resources);
        break;
            
        case SCREEN_MAINMENU:
            _currentScreen = std::make_shared<MainMenuScreen>(_target, _resources);
        break;
            
        case SCREEN_GAMESELECT:
            _currentScreen = std::make_shared<GameSelectScreen>(_target, _resources);
        break;

        case SCREEN_SETTINGS:
            _currentScreen = std::make_shared<SettingsScreen>(_target, _resources);
        break;

        case SCREEN_GAME:
            _currentScreen = std::make_shared<GameScreen>(_target, _resources, settings);
        break;
    }
}

void GameEngine::initAudioService()
{
    IAudioPtr musicptr;
    IAudioPtr soundptr;
    if (_settings.muteAllSounds)
    {
        musicptr = std::make_shared<gs::NullAudio>();
        soundptr = std::make_shared<gs::NullAudio>();
    }
    else
    {
        musicptr = std::make_shared<gs::MusicAudio>(_resources);
        musicptr->setAllVolume(100);
        
        soundptr = std::make_shared<gs::SfxAudio>(_resources);
        soundptr->setAllVolume(100);
    }
    
    musicptr = std::make_shared<gs::LoggedAudio>(musicptr);
    soundptr = std::make_shared<gs::LoggedAudio>(soundptr);
    
    gs::AudioLocator::setMusic(musicptr);
    gs::AudioLocator::setSound(soundptr);
}

} // namespace gs
