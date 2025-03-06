#pragma once

#include <memory>
#include <vector>

#include <SFML/Graphics/RenderWindow.hpp>

#include "state.hpp"
#include "resource/resourceMgr.hpp"
#include "system/input.hpp"
#include "system/render.hpp"

class MainMenuState;
class Gameplay;
class GameOver;

class Game {
public:
    Game();
    Game(Game& copy) = delete;
    Game(const Game& copy) = delete;
    Game(Game&& move) = delete;
    ~Game();

    void                                    Update();

    GameState*                              GetCurrentState() const;
    GameState*                              FindInactiveState(GameState::ID stateID) const;
    void                                    CreateGameState(GameState::ID stateID);
    void                                    DestroyCurrentGameState();
private:
    void                                    CreateMainMenuState();
    void                                    CreateGameplayState();
    void                                    CreateGameOverState();

    ResourceMgr::ErrorCode                  LoadMainMenuFont(MainMenuState& gameState);
    ResourceMgr::ErrorCode                  LoadMainMenuBackground(MainMenuState& gameState);

    std::pair<EntityID, ComponentList>      CreateMainMenu(MainMenuState& gameState);
    std::pair<EntityID, ComponentList>      CreateNewGameOption(MainMenuState& gameState);
    std::pair<EntityID, ComponentList>      CreateLoadGameOption(MainMenuState& gameState);
    std::pair<EntityID, ComponentList>      CreateSettingsOption(MainMenuState& gameState);
    std::pair<EntityID, ComponentList>      CreateQuitGameOption(MainMenuState& gameState);

    sf::Vector2u                            windowSize;
    std::string                             windowTitle;

    ResourceMgr                             resourceMgr;
    std::unique_ptr<GameState>              currentState;
    std::vector<std::unique_ptr<GameState>> inactiveStates;
    sf::RenderWindow                        mainWindow;

    RenderSystem                            renderSystem;
    InputSystem                             inputSystem;
};

void        Center(BoundingBoxComponent& boundingBox, TransformComponent& transform);