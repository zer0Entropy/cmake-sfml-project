#include "../include/mainMenu.hpp"
#include "../include/resource/resourceMgr.hpp"

const sf::Color MainMenuState::fontOutlineColorNormal{sf::Color::White};
const sf::Color MainMenuState::fontOutlineColorHightlight{sf::Color::Red};
const sf::Color MainMenuState::fontFillColor{sf::Color::Black};

const sf::Vector2u MainMenuState::newGameRelPosition{50, 40};
const sf::Vector2u MainMenuState::loadGameRelPosition{50, 50};
const sf::Vector2u MainMenuState::settingsRelPosition{50, 60};
const sf::Vector2u MainMenuState::quitGameRelPosition{50, 70};

ResourceMgr::ErrorCode MainMenuState::LoadMainMenuFont(ResourceMgr& resourceMgr) {
    ResourceMgr::ErrorCode result{ResourceMgr::ErrorCode::Success};
    if(!resourceMgr.DoesResourceExist((ResourceID)fontID, Resource::Type::Font)) {
        result = resourceMgr.LoadResource((ResourceID)fontID, Resource::Type::Font, (std::string)fontPath);
    }
    return result;
}

ResourceMgr::ErrorCode MainMenuState::LoadMainMenuBackground(ResourceMgr& resourceMgr) {
    ResourceMgr::ErrorCode result{ResourceMgr::ErrorCode::Success};
    if(!resourceMgr.DoesResourceExist((ResourceID)backgroundID, Resource::Type::Texture)) {
        result = resourceMgr.LoadResource((ResourceID)backgroundID, Resource::Type::Texture, (std::string)backgroundPath);
    }
    return result;
}

Entity MainMenuState::CreateMainMenu(ResourceMgr& resourceMgr) {
    
    ResourceMgr::ErrorCode  loadBackgroundResult{LoadMainMenuBackground(resourceMgr)};
    ResourceMgr::ErrorCode  loadFontResult{LoadMainMenuFont(resourceMgr)};

    if(     loadBackgroundResult == ResourceMgr::ErrorCode::Success
        &&  loadFontResult == ResourceMgr::ErrorCode::Success) {
        /* TODO: HANDLE ERRORS FROM RESOURCEMGR */
    }

    EntityID menuID{numEntities++};

    /* WIDGET COMPONENT*/
    InitComponent(widgetCmps[menuID], menuID, Component::Type::Widget);
    widgetCmps[menuID].node.type = WidgetNode::Type::Menu;
    widgetCmps[menuID].node.parent = nullptr;
    /* SPRITE COMPONENT */
    InitComponent(spriteCmps[menuID], menuID, Component::Type::Sprite);
    spriteCmps[menuID].texture = resourceMgr.AcquireTexturePtr((ResourceID)backgroundID);
    /* TRANSFORM COMPONENT */
    InitComponent(transformCmps[menuID], menuID, Component::Type::Transform);
    sf::Vector2f size{spriteCmps[menuID].texture->sfmlTexture.getSize()};
    sf::Vector2f scale{
        windowSize.x / size.x,
        windowSize.y / size.y
    };
    transformCmps[menuID].sfmlTransformable.setScale(scale);
    sf::Vector2f position{0.f, 0.f};
    transformCmps[menuID].sfmlTransformable.setPosition(position);

    return Entity{
        menuID,
        ComponentList{
            &widgetCmps[menuID],
            &spriteCmps[menuID],
            &transformCmps[menuID]
        }
    };
}

Entity MainMenuState::CreateNewGameOption(ResourceMgr& resourceMgr) {
    EntityID newGameID{numEntities++};
    EntityID menuID{mainMenu.first};
    /* WIDGET COMPONENT*/
    InitComponent(widgetCmps[newGameID], newGameID, Component::Type::Widget);
    widgetCmps[newGameID].node.type = WidgetNode::Type::MenuOption;
    widgetCmps[newGameID].node.parent = &widgetCmps[menuID].node;
    widgetCmps[menuID].node.children.push_back(&widgetCmps[newGameID].node);
    /* TEXT COMPONENT */
    InitComponent(textCmps[newGameID], newGameID, Component::Type::Text);
    InitText(
                textCmps[newGameID],
                *resourceMgr.AcquireFontPtr((ResourceID)fontID),
                fontSize,
                fontOutlineThicknessNormal,
                fontOutlineColorNormal,
                fontFillColor,
                newGameContents);
    /* TRANSFORM COMPONENT */
    InitComponent(transformCmps[newGameID], newGameID, Component::Type::Transform);
    sf::Vector2f position{
        windowSize.x * (newGameRelPosition.x / 100.f),
        windowSize.y * (newGameRelPosition.y / 100.f)
    };
    transformCmps[newGameID].sfmlTransformable.setPosition(position);
    /* BOUNDING BOX COMPONENT */
    InitComponent(boundingBoxCmps[newGameID], newGameID, Component::Type::BoundingBox);
    UpdateBoundingBox(boundingBoxCmps[newGameID], transformCmps[newGameID], textCmps[newGameID]);
    /* MOUSEOVER COMPONENT */
    InitComponent(mouseOverCmps[newGameID], newGameID, Component::Type::MouseOver);
    mouseOverCmps[newGameID].triggeredAction = ActionID::SetMenuSelection;
    /* LEFT CLICK COMPONENT */
    InitComponent(leftClickCmps[newGameID], newGameID, Component::Type::LeftClick);
    leftClickCmps[newGameID].triggeredAction = ActionID::PressMenuSelection;

    // Center text, then update BoundingBox
    CenterEntity(newGameID);
    UpdateBoundingBox(boundingBoxCmps[newGameID], transformCmps[newGameID], textCmps[newGameID]);

    return Entity{
        newGameID,
        ComponentList{
            &widgetCmps[newGameID],
            &textCmps[newGameID],
            &transformCmps[newGameID],
            &boundingBoxCmps[newGameID],
            &mouseOverCmps[newGameID],
            &leftClickCmps[newGameID]
        }  
    };
}

Entity MainMenuState::CreateLoadGameOption(ResourceMgr& resourceMgr) {
    EntityID loadGameID{numEntities++};
    EntityID menuID{mainMenu.first};
    /* WIDGET COMPONENT*/
    InitComponent(widgetCmps[loadGameID], loadGameID, Component::Type::Widget);
    widgetCmps[loadGameID].node.type = WidgetNode::Type::MenuOption;
    widgetCmps[loadGameID].node.parent = &widgetCmps[menuID].node;
    widgetCmps[menuID].node.children.push_back(&widgetCmps[loadGameID].node);
    /* TEXT COMPONENT */
    InitComponent(textCmps[loadGameID], loadGameID, Component::Type::Text);
    InitText(
                textCmps[loadGameID],
                *resourceMgr.AcquireFontPtr((ResourceID)fontID),
                fontSize,
                fontOutlineThicknessNormal,
                fontOutlineColorNormal,
                fontFillColor,
                loadGameContents);
    /* TRANSFORM COMPONENT */
    InitComponent(transformCmps[loadGameID], loadGameID, Component::Type::Transform);
    sf::Vector2f position{
        windowSize.x * (loadGameRelPosition.x / 100.f),
        windowSize.y * (loadGameRelPosition.y / 100.f)
    };
    transformCmps[loadGameID].sfmlTransformable.setPosition(position);
    /* BOUNDING BOX COMPONENT */
    InitComponent(boundingBoxCmps[loadGameID], loadGameID, Component::Type::BoundingBox);
    UpdateBoundingBox(boundingBoxCmps[loadGameID], transformCmps[loadGameID], textCmps[loadGameID]);
    /* MOUSEOVER COMPONENT */
    InitComponent(mouseOverCmps[loadGameID], loadGameID, Component::Type::MouseOver);
    mouseOverCmps[loadGameID].triggeredAction = ActionID::SetMenuSelection;
    /* LEFT CLICK COMPONENT */
    InitComponent(leftClickCmps[loadGameID], loadGameID, Component::Type::LeftClick);
    leftClickCmps[loadGameID].triggeredAction = ActionID::PressMenuSelection;

    // Center text, then update BoundingBox
    CenterEntity(loadGameID);
    UpdateBoundingBox(boundingBoxCmps[loadGameID], transformCmps[loadGameID], textCmps[loadGameID]);

    return Entity{
        loadGameID,
        ComponentList{
            &widgetCmps[loadGameID],
            &textCmps[loadGameID],
            &transformCmps[loadGameID],
            &boundingBoxCmps[loadGameID],
            &mouseOverCmps[loadGameID],
            &leftClickCmps[loadGameID]
        }  
    };
}

Entity MainMenuState::CreateSettingsOption(ResourceMgr& resourceMgr) {
    EntityID settingsID{numEntities++};
    EntityID menuID{mainMenu.first};
    /* WIDGET COMPONENT*/
    InitComponent(widgetCmps[settingsID], settingsID, Component::Type::Widget);
    widgetCmps[settingsID].node.type = WidgetNode::Type::MenuOption;
    widgetCmps[settingsID].node.parent = &widgetCmps[menuID].node;
    widgetCmps[menuID].node.children.push_back(&widgetCmps[settingsID].node);
    /* TEXT COMPONENT */
    InitComponent(textCmps[settingsID], settingsID, Component::Type::Text);
    InitText(
                textCmps[settingsID],
                *resourceMgr.AcquireFontPtr((ResourceID)fontID),
                fontSize,
                fontOutlineThicknessNormal,
                fontOutlineColorNormal,
                fontFillColor,
                settingsContents);
    /* TRANSFORM COMPONENT */
    InitComponent(transformCmps[settingsID], settingsID, Component::Type::Transform);
    sf::Vector2f position{
        windowSize.x * (settingsRelPosition.x / 100.f),
        windowSize.y * (settingsRelPosition.y / 100.f)
    };
    transformCmps[settingsID].sfmlTransformable.setPosition(position);
    /* BOUNDING BOX COMPONENT */
    InitComponent(boundingBoxCmps[settingsID], settingsID, Component::Type::BoundingBox);
    UpdateBoundingBox(boundingBoxCmps[settingsID], transformCmps[settingsID], textCmps[settingsID]);
    /* MOUSEOVER COMPONENT */
    InitComponent(mouseOverCmps[settingsID], settingsID, Component::Type::MouseOver);
    mouseOverCmps[settingsID].triggeredAction = ActionID::SetMenuSelection;
    /* LEFT CLICK COMPONENT */
    InitComponent(leftClickCmps[settingsID], settingsID, Component::Type::LeftClick);
    leftClickCmps[settingsID].triggeredAction = ActionID::PressMenuSelection;

    // Center text, then update BoundingBox
    CenterEntity(settingsID);
    UpdateBoundingBox(boundingBoxCmps[settingsID], transformCmps[settingsID], textCmps[settingsID]);

    return Entity{
        settingsID,
        ComponentList{
            &widgetCmps[settingsID],
            &textCmps[settingsID],
            &transformCmps[settingsID],
            &boundingBoxCmps[settingsID],
            &mouseOverCmps[settingsID],
            &leftClickCmps[settingsID]
        }  
    };
}

Entity MainMenuState::CreateQuitGameOption(ResourceMgr& resourceMgr) {
    EntityID quitGameID{numEntities++};
    EntityID menuID{mainMenu.first};
    /* WIDGET COMPONENT*/
    InitComponent(widgetCmps[quitGameID], quitGameID, Component::Type::Widget);
    widgetCmps[quitGameID].node.type = WidgetNode::Type::MenuOption;
    widgetCmps[quitGameID].node.parent = &widgetCmps[menuID].node;
    widgetCmps[menuID].node.children.push_back(&widgetCmps[quitGameID].node);
    /* TEXT COMPONENT */
    InitComponent(textCmps[quitGameID], quitGameID, Component::Type::Text);
    InitText(
                textCmps[quitGameID],
                *resourceMgr.AcquireFontPtr((ResourceID)fontID),
                fontSize,
                fontOutlineThicknessNormal,
                fontOutlineColorNormal,
                fontFillColor,
                quitGameContents);
    /* TRANSFORM COMPONENT */
    InitComponent(transformCmps[quitGameID], quitGameID, Component::Type::Transform);
    sf::Vector2f position{
        windowSize.x * (quitGameRelPosition.x / 100.f),
        windowSize.y * (quitGameRelPosition.y / 100.f)
    };
    transformCmps[quitGameID].sfmlTransformable.setPosition(position);
    /* BOUNDING BOX COMPONENT */
    InitComponent(boundingBoxCmps[quitGameID], quitGameID, Component::Type::BoundingBox);
    UpdateBoundingBox(boundingBoxCmps[quitGameID], transformCmps[quitGameID], textCmps[quitGameID]);
    /* MOUSEOVER COMPONENT */
    InitComponent(mouseOverCmps[quitGameID], quitGameID, Component::Type::MouseOver);
    mouseOverCmps[quitGameID].triggeredAction = ActionID::SetMenuSelection;
    /* LEFT CLICK COMPONENT */
    InitComponent(leftClickCmps[quitGameID], quitGameID, Component::Type::LeftClick);
    leftClickCmps[quitGameID].triggeredAction = ActionID::PressMenuSelection;

    // Center text, then update BoundingBox
    CenterEntity(quitGameID);
    UpdateBoundingBox(boundingBoxCmps[quitGameID], transformCmps[quitGameID], textCmps[quitGameID]);

    return Entity{
        quitGameID,
        ComponentList{
            &widgetCmps[quitGameID],
            &textCmps[quitGameID],
            &transformCmps[quitGameID],
            &boundingBoxCmps[quitGameID],
            &mouseOverCmps[quitGameID],
            &leftClickCmps[quitGameID]
        }  
    };
}

void MainMenuState::DoAction(ActionID action, std::optional<EntityID> ownerID) {
    if(action == ActionID::SetMenuSelection) {
        if(ownerID) {
            WidgetComponent& menuOption{widgetCmps[*ownerID]};
            const auto newIndex{menuOption.node.GetIndex()};
            if(newIndex) {
                selectedIndex = *newIndex;
                HighlightSelectedOption(*ownerID);
            }
        }
    }
    else if(action == ActionID::ResetMenuSelection) {
        selectedIndex.reset();
        ResetHighlight();
    }
    else if(action == ActionID::PressMenuSelection) {
        if(ownerID) {
            WidgetComponent& menuOption{widgetCmps[*ownerID]};
            const auto newIndex{menuOption.node.GetIndex()};
            if(newIndex) {
                pressedIndex = *newIndex;
            }
        }
    }
    if(pressedIndex && ownerID) {
        if(ownerID == newGameOption.first) {
            transitionFlag = GameState::TransitionID::InitGameplay;
        }
        else if(ownerID == loadGameOption.first) {
            transitionFlag = GameState::TransitionID::DisplayLoadGame;
        }
        else if(ownerID == settingsOption.first) {
            transitionFlag = GameState::TransitionID::DisplaySettings;
        }
        else if(ownerID == quitGameOption.first) {
            transitionFlag = GameState::TransitionID::Shutdown;
        }
    }
}

void MainMenuState::HighlightSelectedOption(EntityID menuOptionID) {
    auto& textCmp{textCmps[menuOptionID]};
    if(textCmp.enabled) {
        textCmp.outlineThickness = fontOutlineThicknessHighlight;
        textCmp.outlineColor = fontOutlineColorHightlight;
    }
}

void MainMenuState::ResetHighlight() {
    for(int n = 0; n < numEntities; ++n) {
        auto& textCmp{textCmps[n]};
        if(textCmp.enabled) {
            textCmp.outlineThickness = fontOutlineThicknessNormal;
            textCmp.outlineColor = fontOutlineColorNormal;
        }
    }
}