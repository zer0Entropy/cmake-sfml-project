#include "../include/game.hpp"
#include "../include/gameOver.hpp"
#include "../include/gameplay.hpp"
#include "../include/mainMenu.hpp"

Game::Game():
    windowSize{1920u, 1080u},
    windowTitle{"Test Project"},
    mainWindow{
        sf::RenderWindow(
            sf::VideoMode{{windowSize.x, windowSize.y}},
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
    /*  MainMenu will be the "base" state, so before we create it, we must ensure
        that no other states exist.
    */
    while(currentState) {
        DestroyCurrentGameState();
    }

    currentState = std::make_unique<MainMenuState>();
    MainMenuState& gameState{static_cast<MainMenuState&>(*currentState)};
    gameState.id = GameState::ID::MainMenu;
    gameState.transitionFlag = GameState::TransitionID::Null;
    gameState.numEntities = 0;

    ResourceMgr::ErrorCode loadFontOutcome{
        resourceMgr.LoadResource((ResourceID)gameState.fontID, Resource::Type::Font, (std::string)gameState.fontPath)
    };
    if(loadFontOutcome != ResourceMgr::ErrorCode::Success) {
        // TO DO: HANDLE ERROR CODES RETURNED BY LOADRESOURCE!
    }

    gameState.mainMenu = CreateMainMenu(gameState);
    gameState.newGameOption = CreateNewGameOption(gameState);
    gameState.loadGameOption = CreateLoadGameOption(gameState);
    gameState.settingsOption = CreateSettingsOption(gameState);
    gameState.quitGameOption = CreateQuitGameOption(gameState);
}

void Game::CreateGameplayState() {

    currentState = std::make_unique<GameplayState>();
    GameplayState& gameState{static_cast<GameplayState&>(*currentState)};
    gameState.id = GameState::ID::Gameplay;
    gameState.transitionFlag = GameState::TransitionID::Null;
    gameState.numEntities = 0;

    ResourceMgr::ErrorCode loadWallOutcome{
        resourceMgr.LoadResource((ResourceID)gameState.wallTextureID, Resource::Type::Texture, (std::string)gameState.wallTexturePath)
    };

    ResourceMgr::ErrorCode loadFloorOutcome{
        resourceMgr.LoadResource((ResourceID)gameState.floorTextureID, Resource::Type::Texture, (std::string)gameState.floorTexturePath)
    };

    ResourceMgr::ErrorCode loadPlayerOutcome{
        resourceMgr.LoadResource((ResourceID)gameState.playerTextureID, Resource::Type::Texture, (std::string)gameState.playerTexturePath)
    };

    if(loadWallOutcome != ResourceMgr::ErrorCode::Success) {
         // TO DO: HANDLE ERROR CODES RETURNED BY LOADRESOURCE!
    }
    if(loadFloorOutcome != ResourceMgr::ErrorCode::Success) {
        // TO DO: HANDLE ERROR CODES RETURNED BY LOADRESOURCE!
    }
    if(loadPlayerOutcome != ResourceMgr::ErrorCode::Success) {
        // TO DO: HANDLE ERROR CODES RETURNED BY LOADRESOURCE!
    }

    InitLevel(gameState.currentLevel, 0);

    gameState.currentLevel.terrainTextures[(int)Terrain::Type::Ground] = resourceMgr.AcquireTexturePtr((ResourceID)gameState.floorTextureID);
    gameState.currentLevel.terrainTextures[(int)Terrain::Type::Wall] = resourceMgr.AcquireTexturePtr((ResourceID)gameState.wallTextureID);

    sf::Vector2u playerLocation{
        gameState.currentLevel.map.width / 2,
        gameState.currentLevel.map.height / 2
    };

    EntityID playerCharacterID{gameState.numEntities++};
    /* SPRITE COMPONENT */
    InitComponent(gameState.spriteCmps[playerCharacterID], playerCharacterID, Component::Type::Sprite);
    gameState.spriteCmps[playerCharacterID].texture = resourceMgr.AcquireTexturePtr((ResourceID)gameState.playerTextureID);
    /* TRANSFORM COMPONENT */
    InitComponent(gameState.transformCmps[playerCharacterID], playerCharacterID, Component::Type::Transform);
    gameState.transformCmps[playerCharacterID].sfmlTransformable.setScale(sf::Vector2f{3.f, 3.f});
    gameState.transformCmps[playerCharacterID].sfmlTransformable.setPosition(sf::Vector2f{0.f, 0.f});
    /* BOUNDINGBOX COMPONENT */
    InitComponent(gameState.boundingBoxCmps[playerCharacterID], playerCharacterID, Component::Type::BoundingBox);
    UpdateBoundingBox(gameState.boundingBoxCmps[playerCharacterID], gameState.transformCmps[playerCharacterID], gameState.spriteCmps[playerCharacterID]);
    /* CREATURE COMPONENT */
    InitComponent(gameState.creatureCmps[playerCharacterID], playerCharacterID, Component::Type::Creature);

    CreatureComponent& playerCharacter{gameState.creatureCmps[playerCharacterID]};
    InitCreature(playerCharacter, "PLAYER", playerLocation);
    PlaceCreature(gameState.currentLevel, playerCharacter);

    InitView(gameState.currentLevel.mapView, sf::Vector2f{0.f, 0.f}, windowSize);
    
}

void Game::CreateGameOverState() {
    
}

ResourceMgr::ErrorCode Game::LoadMainMenuFont(MainMenuState& gameState) {
    ResourceMgr::ErrorCode result{ResourceMgr::ErrorCode::Success};
    if(!resourceMgr.DoesResourceExist((ResourceID)gameState.fontID, Resource::Type::Font)) {
        result = resourceMgr.LoadResource((ResourceID)gameState.fontID, Resource::Type::Font, (std::string)gameState.fontPath);
    }
    return result;
}

ResourceMgr::ErrorCode Game::LoadMainMenuBackground(MainMenuState& gameState) {
    ResourceMgr::ErrorCode result{ResourceMgr::ErrorCode::Success};
    if(!resourceMgr.DoesResourceExist((ResourceID)gameState.backgroundID, Resource::Type::Texture)) {
        result = resourceMgr.LoadResource((ResourceID)gameState.backgroundID, Resource::Type::Texture, (std::string)gameState.backgroundPath);
    }
    return result;
}

std::pair<EntityID, ComponentList> Game::CreateMainMenu(MainMenuState& gameState) {
    ResourceMgr::ErrorCode  loadBackgroundResult{LoadMainMenuBackground(gameState)};
    ResourceMgr::ErrorCode  loadFontResult{LoadMainMenuFont(gameState)};
    EntityID menuID{gameState.numEntities++};
    if(     loadBackgroundResult == ResourceMgr::ErrorCode::Success
        &&  loadFontResult == ResourceMgr::ErrorCode::Success) {
        /* WIDGET COMPONENT*/
        InitComponent(gameState.widgetCmps[menuID], menuID, Component::Type::Widget);
        gameState.widgetCmps[menuID].node.type = WidgetNode::Type::Menu;
        gameState.widgetCmps[menuID].node.parent = nullptr;
        /* SPRITE COMPONENT */
        InitComponent(gameState.spriteCmps[menuID], menuID, Component::Type::Sprite);
        gameState.spriteCmps[menuID].texture = resourceMgr.AcquireTexturePtr((ResourceID)gameState.backgroundID);
        /* TRANSFORM COMPONENT */
        InitComponent(gameState.transformCmps[menuID], menuID, Component::Type::Transform);
        sf::Vector2f size{gameState.spriteCmps[menuID].texture->sfmlTexture.getSize()};
        sf::Vector2f scale{
            windowSize.x / size.x,
            windowSize.y / size.y
        };
        gameState.transformCmps[menuID].sfmlTransformable.setScale(scale);
        sf::Vector2f position{0.f, 0.f};
        gameState.transformCmps[menuID].sfmlTransformable.setPosition(position);
    }
    return std::pair<EntityID, ComponentList>{
        menuID,
        ComponentList{
            &gameState.widgetCmps[menuID],
            &gameState.spriteCmps[menuID],
            &gameState.transformCmps[menuID]
        }
    };
}

std::pair<EntityID, ComponentList> Game::CreateNewGameOption(MainMenuState& gameState) {
    EntityID newGameID{gameState.numEntities++};
    EntityID menuID{gameState.mainMenu.first};
    /* WIDGET COMPONENT*/
    InitComponent(gameState.widgetCmps[newGameID], newGameID, Component::Type::Widget);
    gameState.widgetCmps[newGameID].node.type = WidgetNode::Type::MenuOption;
    gameState.widgetCmps[newGameID].node.parent = &gameState.widgetCmps[menuID].node;
    gameState.widgetCmps[menuID].node.children.push_back(&gameState.widgetCmps[newGameID].node);
    /* TEXT COMPONENT */
    InitComponent(gameState.textCmps[newGameID], newGameID, Component::Type::Text);
    InitText(   gameState.textCmps[newGameID],
                *resourceMgr.AcquireFontPtr((ResourceID)gameState.fontID),
                gameState.fontSize,
                gameState.fontOutlineThicknessNormal,
                gameState.fontOutlineColorNormal,
                gameState.fontFillColor,
                gameState.newGameContents);
    /* TRANSFORM COMPONENT */
    InitComponent(gameState.transformCmps[newGameID], newGameID, Component::Type::Transform);
    sf::Vector2f position{
        windowSize.x * (gameState.newGameRelPosition.x / 100.f),
        windowSize.y * (gameState.newGameRelPosition.y / 100.f)
    };
    gameState.transformCmps[newGameID].sfmlTransformable.setPosition(position);
    /* BOUNDING BOX COMPONENT */
    InitComponent(gameState.boundingBoxCmps[newGameID], newGameID, Component::Type::BoundingBox);
    UpdateBoundingBox(gameState.boundingBoxCmps[newGameID], gameState.transformCmps[newGameID], gameState.textCmps[newGameID]);
    /* MOUSEOVER COMPONENT */
    InitComponent(gameState.mouseOverCmps[newGameID], newGameID, Component::Type::MouseOver);
    gameState.mouseOverCmps[newGameID].triggeredAction = ActionID::SetMenuSelection;
    /* LEFT CLICK COMPONENT */
    InitComponent(gameState.leftClickCmps[newGameID], newGameID, Component::Type::LeftClick);
    gameState.leftClickCmps[newGameID].triggeredAction = ActionID::PressMenuSelection;

    // Center text, then update BoundingBox
    Center(gameState.boundingBoxCmps[newGameID], gameState.transformCmps[newGameID]);
    UpdateBoundingBox(gameState.boundingBoxCmps[newGameID], gameState.transformCmps[newGameID], gameState.textCmps[newGameID]);

    return std::pair<EntityID, ComponentList>{
        newGameID,
        ComponentList{
            &gameState.widgetCmps[newGameID],
            &gameState.textCmps[newGameID],
            &gameState.transformCmps[newGameID],
            &gameState.boundingBoxCmps[newGameID],
            &gameState.mouseOverCmps[newGameID],
            &gameState.leftClickCmps[newGameID]
        }  
    };
}

std::pair<EntityID, ComponentList> Game::CreateLoadGameOption(MainMenuState& gameState) {
    EntityID loadGameID{gameState.numEntities++};
    EntityID menuID{gameState.mainMenu.first};
    /* WIDGET COMPONENT*/
    InitComponent(gameState.widgetCmps[loadGameID], loadGameID, Component::Type::Widget);
    gameState.widgetCmps[loadGameID].node.type = WidgetNode::Type::MenuOption;
    gameState.widgetCmps[loadGameID].node.parent = &gameState.widgetCmps[menuID].node;
    gameState.widgetCmps[menuID].node.children.push_back(&gameState.widgetCmps[loadGameID].node);
    /* TEXT COMPONENT */
    InitComponent(gameState.textCmps[loadGameID], loadGameID, Component::Type::Text);
    InitText(   gameState.textCmps[loadGameID],
                *resourceMgr.AcquireFontPtr((ResourceID)gameState.fontID),
                gameState.fontSize,
                gameState.fontOutlineThicknessNormal,
                gameState.fontOutlineColorNormal,
                gameState.fontFillColor,
                gameState.loadGameContents);
    /* TRANSFORM COMPONENT */
    InitComponent(gameState.transformCmps[loadGameID], loadGameID, Component::Type::Transform);
    sf::Vector2f position{
        windowSize.x * (gameState.loadGameRelPosition.x / 100.f),
        windowSize.y * (gameState.loadGameRelPosition.y / 100.f)
    };
    gameState.transformCmps[loadGameID].sfmlTransformable.setPosition(position);
    /* BOUNDING BOX COMPONENT */
    InitComponent(gameState.boundingBoxCmps[loadGameID], loadGameID, Component::Type::BoundingBox);
    UpdateBoundingBox(gameState.boundingBoxCmps[loadGameID], gameState.transformCmps[loadGameID], gameState.textCmps[loadGameID]);
    /* MOUSEOVER COMPONENT */
    InitComponent(gameState.mouseOverCmps[loadGameID], loadGameID, Component::Type::MouseOver);
    gameState.mouseOverCmps[loadGameID].triggeredAction = ActionID::SetMenuSelection;
    /* LEFT CLICK COMPONENT */
    InitComponent(gameState.leftClickCmps[loadGameID], loadGameID, Component::Type::LeftClick);
    gameState.leftClickCmps[loadGameID].triggeredAction = ActionID::PressMenuSelection;

    // Center text, then update BoundingBox
    Center(gameState.boundingBoxCmps[loadGameID], gameState.transformCmps[loadGameID]);
    UpdateBoundingBox(gameState.boundingBoxCmps[loadGameID], gameState.transformCmps[loadGameID], gameState.textCmps[loadGameID]);

    return std::pair<EntityID, ComponentList>{
        loadGameID,
        ComponentList{
            &gameState.widgetCmps[loadGameID],
            &gameState.textCmps[loadGameID],
            &gameState.transformCmps[loadGameID],
            &gameState.boundingBoxCmps[loadGameID],
            &gameState.mouseOverCmps[loadGameID],
            &gameState.leftClickCmps[loadGameID]
        }  
    };
}

std::pair<EntityID, ComponentList> Game::CreateSettingsOption(MainMenuState& gameState) {
    EntityID settingsID{gameState.numEntities++};
    EntityID menuID{gameState.mainMenu.first};
    /* WIDGET COMPONENT*/
    InitComponent(gameState.widgetCmps[settingsID], settingsID, Component::Type::Widget);
    gameState.widgetCmps[settingsID].node.type = WidgetNode::Type::MenuOption;
    gameState.widgetCmps[settingsID].node.parent = &gameState.widgetCmps[menuID].node;
    gameState.widgetCmps[menuID].node.children.push_back(&gameState.widgetCmps[settingsID].node);
    /* TEXT COMPONENT */
    InitComponent(gameState.textCmps[settingsID], settingsID, Component::Type::Text);
    InitText(   gameState.textCmps[settingsID],
                *resourceMgr.AcquireFontPtr((ResourceID)gameState.fontID),
                gameState.fontSize,
                gameState.fontOutlineThicknessNormal,
                gameState.fontOutlineColorNormal,
                gameState.fontFillColor,
                gameState.settingsContents);
    /* TRANSFORM COMPONENT */
    InitComponent(gameState.transformCmps[settingsID], settingsID, Component::Type::Transform);
    sf::Vector2f position{
        windowSize.x * (gameState.settingsRelPosition.x / 100.f),
        windowSize.y * (gameState.settingsRelPosition.y / 100.f)
    };
    gameState.transformCmps[settingsID].sfmlTransformable.setPosition(position);
    /* BOUNDING BOX COMPONENT */
    InitComponent(gameState.boundingBoxCmps[settingsID], settingsID, Component::Type::BoundingBox);
    UpdateBoundingBox(gameState.boundingBoxCmps[settingsID], gameState.transformCmps[settingsID], gameState.textCmps[settingsID]);
    /* MOUSEOVER COMPONENT */
    InitComponent(gameState.mouseOverCmps[settingsID], settingsID, Component::Type::MouseOver);
    gameState.mouseOverCmps[settingsID].triggeredAction = ActionID::SetMenuSelection;
    /* LEFT CLICK COMPONENT */
    InitComponent(gameState.leftClickCmps[settingsID], settingsID, Component::Type::LeftClick);
    gameState.leftClickCmps[settingsID].triggeredAction = ActionID::PressMenuSelection;

    // Center text, then update BoundingBox
    Center(gameState.boundingBoxCmps[settingsID], gameState.transformCmps[settingsID]);
    UpdateBoundingBox(gameState.boundingBoxCmps[settingsID], gameState.transformCmps[settingsID], gameState.textCmps[settingsID]);

    return std::pair<EntityID, ComponentList>{
        settingsID,
        ComponentList{
            &gameState.widgetCmps[settingsID],
            &gameState.textCmps[settingsID],
            &gameState.transformCmps[settingsID],
            &gameState.boundingBoxCmps[settingsID],
            &gameState.mouseOverCmps[settingsID],
            &gameState.leftClickCmps[settingsID]
        }  
    };
}

std::pair<EntityID, ComponentList> Game::CreateQuitGameOption(MainMenuState& gameState) {
    EntityID quitGameID{gameState.numEntities++};
    EntityID menuID{gameState.mainMenu.first};
    /* WIDGET COMPONENT*/
    InitComponent(gameState.widgetCmps[quitGameID], quitGameID, Component::Type::Widget);
    gameState.widgetCmps[quitGameID].node.type = WidgetNode::Type::MenuOption;
    gameState.widgetCmps[quitGameID].node.parent = &gameState.widgetCmps[menuID].node;
    gameState.widgetCmps[menuID].node.children.push_back(&gameState.widgetCmps[quitGameID].node);
    /* TEXT COMPONENT */
    InitComponent(gameState.textCmps[quitGameID], quitGameID, Component::Type::Text);
    InitText(   gameState.textCmps[quitGameID],
                *resourceMgr.AcquireFontPtr((ResourceID)gameState.fontID),
                gameState.fontSize,
                gameState.fontOutlineThicknessNormal,
                gameState.fontOutlineColorNormal,
                gameState.fontFillColor,
                gameState.quitGameContents);
    /* TRANSFORM COMPONENT */
    InitComponent(gameState.transformCmps[quitGameID], quitGameID, Component::Type::Transform);
    sf::Vector2f position{
        windowSize.x * (gameState.quitGameRelPosition.x / 100.f),
        windowSize.y * (gameState.quitGameRelPosition.y / 100.f)
    };
    gameState.transformCmps[quitGameID].sfmlTransformable.setPosition(position);
    /* BOUNDING BOX COMPONENT */
    InitComponent(gameState.boundingBoxCmps[quitGameID], quitGameID, Component::Type::BoundingBox);
    UpdateBoundingBox(gameState.boundingBoxCmps[quitGameID], gameState.transformCmps[quitGameID], gameState.textCmps[quitGameID]);
    /* MOUSEOVER COMPONENT */
    InitComponent(gameState.mouseOverCmps[quitGameID], quitGameID, Component::Type::MouseOver);
    gameState.mouseOverCmps[quitGameID].triggeredAction = ActionID::SetMenuSelection;
    /* LEFT CLICK COMPONENT */
    InitComponent(gameState.leftClickCmps[quitGameID], quitGameID, Component::Type::LeftClick);
    gameState.leftClickCmps[quitGameID].triggeredAction = ActionID::PressMenuSelection;

    // Center text, then update BoundingBox
    Center(gameState.boundingBoxCmps[quitGameID], gameState.transformCmps[quitGameID]);
    UpdateBoundingBox(gameState.boundingBoxCmps[quitGameID], gameState.transformCmps[quitGameID], gameState.textCmps[quitGameID]);

    return std::pair<EntityID, ComponentList>{
        quitGameID,
        ComponentList{
            &gameState.widgetCmps[quitGameID],
            &gameState.textCmps[quitGameID],
            &gameState.transformCmps[quitGameID],
            &gameState.boundingBoxCmps[quitGameID],
            &gameState.mouseOverCmps[quitGameID],
            &gameState.leftClickCmps[quitGameID]
        }  
    };
}

void Center(BoundingBoxComponent& boundingBox, TransformComponent& transform) {
    transform.sfmlTransformable.setOrigin({
        boundingBox.sfmlBoundingBox.size.x / 2.f,
        boundingBox.sfmlBoundingBox.size.y / 2.f
    });
}