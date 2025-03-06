#include "../include/mainMenu.hpp"

const sf::Color MainMenuState::fontOutlineColor{sf::Color::White};
const sf::Color MainMenuState::fontFillColor{sf::Color::Black};

const sf::Color MainMenuState::fontHighlightColor{sf::Color::Red};

const sf::Vector2u MainMenuState::newGameRelPosition{50, 40};
const sf::Vector2u MainMenuState::loadGameRelPosition{50, 50};
const sf::Vector2u MainMenuState::settingsRelPosition{50, 60};
const sf::Vector2u MainMenuState::quitGameRelPosition{50, 70};

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
        textCmp.sfmlText->setOutlineThickness(fontHighlightThickness);
        textCmp.sfmlText->setOutlineColor(fontHighlightColor);
    }
}

void MainMenuState::ResetHighlight() {
    for(int n = 0; n < numEntities; ++n) {
        auto& textCmp{textCmps[n]};
        if(textCmp.enabled) {
            textCmp.sfmlText->setOutlineThickness(fontOutlineThickness);
            textCmp.sfmlText->setOutlineColor(fontOutlineColor);
        }
    }
}