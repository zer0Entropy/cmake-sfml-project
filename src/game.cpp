#include "../include/game.hpp"
#include "../include/gameOver.hpp"
#include "../include/gameplay.hpp"
#include "../include/mainMenu.hpp"

Game::Game():
    errorFlag{false},
    targetWindowSize{1920u, 1080u},
    windowTitle{"Test Project"},
    mainWindow{
        sf::RenderWindow(
            sf::VideoMode{{targetWindowSize.x, targetWindowSize.y}},
            windowTitle,
            sf::Style::Titlebar | sf::Style::Close,
            sf::State::Fullscreen)
    },
    logMgr{(std::string)Game::logPath},
    inputSystem{mainWindow},
    renderSystem{mainWindow} {
    
    std::string windowCreatedHeader{"MainWindow successfully created"};
    std::string targetSizeString{"targetSize=(" + std::to_string(targetWindowSize.x) + ", " + std::to_string(targetWindowSize.y) + ")"};
    std::string actualSizeString{"actualSize=(" + std::to_string(mainWindow.getSize().x) + ", " + std::to_string(mainWindow.getSize().y) + ")"};
    logMgr.CreateMessage(windowCreatedHeader, targetSizeString + " " + actualSizeString);

    unsigned int frameLimit{144};
    mainWindow.setFramerateLimit(frameLimit);
    logMgr.CreateMessage("Frame limit set to " + std::to_string(frameLimit), "");
    CreateGameState(GameState::ID::MainMenu);

}

Game::~Game() {

}

void Game::Update() {
    if(errorFlag || (currentState && currentState->errorFlag)) {
        mainWindow.close();
    }
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
                logMgr.CreateMessage("Shutting down...", "");
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
            logMgr.CreateMessage("MainMenuState successfully created", "");
        } break;
        case GameState::ID::Gameplay: {
            CreateGameplayState();
            logMgr.CreateMessage("GameplayState successfully created", "");
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
    currentState->errorFlag = false;
    currentState->logMgr = &logMgr;
    MainMenuState& gameState{static_cast<MainMenuState&>(*currentState)};
    gameState.id = GameState::ID::MainMenu;
    gameState.transitionFlag = GameState::TransitionID::Null;
    gameState.numEntities = 0;
    gameState.windowSize = mainWindow.getSize();

    ResourceMgr::ErrorCode loadFontOutcome{
        resourceMgr.LoadResource((ResourceID)gameState.fontID, Resource::Type::Font, (std::string)gameState.fontPath)
    };
    if(loadFontOutcome != ResourceMgr::ErrorCode::Success) {
        std::string errorHeader{"Attempt to load font \"" + std::string(gameState.fontID) + "\" failed"};
        std::string errorBody{"File not found: " + std::string(gameState.fontPath)};
        logMgr.CreateMessage(errorHeader, errorBody, true);
        errorFlag = true;
        return;
    }
    std::string successHeader{"Font \"" + std::string(gameState.fontID) + "\" loaded successfully"};
    logMgr.CreateMessage(successHeader, "");

    gameState.mainMenu = gameState.CreateMainMenu(resourceMgr);
    gameState.newGameOption = gameState.CreateNewGameOption(resourceMgr);
    gameState.loadGameOption = gameState.CreateLoadGameOption(resourceMgr);
    gameState.settingsOption = gameState.CreateSettingsOption(resourceMgr);
    gameState.quitGameOption = gameState.CreateQuitGameOption(resourceMgr);
}

void Game::CreateGameplayState() {

    currentState = std::make_unique<GameplayState>();
    currentState->errorFlag = false;
    currentState->logMgr = &logMgr;
    GameplayState& gameState{static_cast<GameplayState&>(*currentState)};
    gameState.id = GameState::ID::Gameplay;
    gameState.transitionFlag = GameState::TransitionID::Null;
    gameState.numEntities = 0;
    gameState.windowSize = mainWindow.getSize();

    gameState.CreateLevel(resourceMgr);
    logMgr.CreateMessage("Level " + std::to_string(gameState.currentLevel.index) + " created", "");

    gameState.player.character = gameState.CreatePlayer(resourceMgr);
    CreatureComponent& playerCharacter = static_cast<CreatureComponent&>(*GetComponent(gameState.player.character, Component::Type::Creature));
    logMgr.CreateMessage(   "Player successfully created", "name=" + playerCharacter.name + ", location=(" + 
                            std::to_string(playerCharacter.location.x) + ", " + std::to_string(playerCharacter.location.y) + ")");
}

void Game::CreateGameOverState() {
    
}

