#include "../include/game.hpp"
#include "../include/gameOver.hpp"
#include "../include/gameplay.hpp"
#include "../include/mainMenu.hpp"

Game::Game():
    targetWindowSize{1920u, 1080u},
    windowTitle{"Test Project"},
    mainWindow{
        sf::RenderWindow(
            sf::VideoMode{{targetWindowSize.x, targetWindowSize.y}},
            windowTitle,
            sf::Style::Titlebar | sf::Style::Close,
            sf::State::Fullscreen)
    },
    inputSystem{mainWindow},
    renderSystem{mainWindow} {
    
    mainWindow.setFramerateLimit(144);
    CreateGameState(GameState::ID::MainMenu);

}

Game::~Game() {

}

void Game::Update() {
    if(mainWindow.isOpen() && currentState) {
        inputSystem.Update(*currentState);
    }
    if(mainWindow.isOpen()) {    
        mainWindow.clear();
        if(currentState) {
            if(currentState->id == GameState::ID::Gameplay) {
                GameplayState& gameState{static_cast<GameplayState&>(*currentState)};
                gameState.RenderLevel(gameState.currentLevel, gameState.player, mainWindow);
                gameState.UpdatePlayer();
            }
            renderSystem.Update(*currentState);
        }
        mainWindow.display();
    }
    else if(currentState) {
        currentState->transitionFlag = GameState::TransitionID::Shutdown;
    }
        
    if(currentState) {
        switch(currentState->transitionFlag) {
            /* MAIN MENU */
            case GameState::TransitionID::ReturnToMainMenu: {
                CreateGameState(GameState::ID::MainMenu);
            } break;
            /* GAMEPLAY */
            case GameState::TransitionID::InitGameplay: {
                CreateGameState(GameState::ID::Gameplay);
            } break;
            /* LOAD GAME*/
            case GameState::TransitionID::DisplayLoadGame: {

            } break;
            /* SETTINGS */
            case GameState::TransitionID::DisplaySettings: {

            } break;
            case GameState::TransitionID::Shutdown: {
                currentState.release();
                for(auto stateIter = inactiveStates.begin(); stateIter != inactiveStates.end(); ++stateIter) {
                    (*stateIter).release();
                }
                inactiveStates.clear();
            }
            default:
            case GameState::TransitionID::Null: {

            } break;
        }
        if(currentState) {
            currentState->transitionFlag = GameState::TransitionID::Null;
        }
    }
}

GameState* Game::GetCurrentState() const {
    return currentState.get();
}

GameState* Game::FindInactiveState(GameState::ID stateID) const {
    GameState* state{nullptr};
    for(auto stateIter = inactiveStates.begin(); !state && stateIter != inactiveStates.end(); ++stateIter) {
        if((*stateIter)->id == stateID) {
            state = (*stateIter).get();
        }
    }
    return state;
}

void Game::CreateGameState(GameState::ID stateID) {
    if(currentState) {
        inactiveStates.push_back(std::move(currentState));
    }
    switch(stateID) {
        case GameState::ID::MainMenu: {
            CreateMainMenuState();
        } break;
        case GameState::ID::Gameplay: {
            CreateGameplayState();
        } break;
        case GameState::ID::GameOver: {
            CreateGameOverState();
        } break;
    }
}

void Game::DestroyCurrentGameState() {
    currentState.release();
    if(!inactiveStates.empty()) {
        auto mostRecentIter{inactiveStates.rbegin()};
        currentState = std::move(*mostRecentIter);
        inactiveStates.pop_back();
    }
}

void Game::CreateMainMenuState() {

    currentState = std::make_unique<MainMenuState>();
    MainMenuState& gameState{static_cast<MainMenuState&>(*currentState)};
    gameState.id = GameState::ID::MainMenu;
    gameState.transitionFlag = GameState::TransitionID::Null;
    gameState.numEntities = 0;
    gameState.windowSize = mainWindow.getSize();

    ResourceMgr::ErrorCode loadFontOutcome{
        resourceMgr.LoadResource((ResourceID)gameState.fontID, Resource::Type::Font, (std::string)gameState.fontPath)
    };
    if(loadFontOutcome != ResourceMgr::ErrorCode::Success) {
        // TO DO: HANDLE ERROR CODES RETURNED BY LOADRESOURCE!
    }

    gameState.mainMenu = gameState.CreateMainMenu(resourceMgr);
    gameState.newGameOption = gameState.CreateNewGameOption(resourceMgr);
    gameState.loadGameOption = gameState.CreateLoadGameOption(resourceMgr);
    gameState.settingsOption = gameState.CreateSettingsOption(resourceMgr);
    gameState.quitGameOption = gameState.CreateQuitGameOption(resourceMgr);
}

void Game::CreateGameplayState() {

    currentState = std::make_unique<GameplayState>();
    GameplayState& gameState{static_cast<GameplayState&>(*currentState)};
    gameState.id = GameState::ID::Gameplay;
    gameState.transitionFlag = GameState::TransitionID::Null;
    gameState.numEntities = 0;
    gameState.windowSize = mainWindow.getSize();

    gameState.CreateLevel(resourceMgr);
    gameState.player.character = gameState.CreatePlayer(resourceMgr);
}

void Game::CreateGameOverState() {
    
}

