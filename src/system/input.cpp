#include "../../include/system/input.hpp"
#include "../../include/state.hpp"

InputSystem::InputSystem(sf::RenderWindow& window):
    mainWindow{window} {

}

InputSystem::~InputSystem() {

}

void InputSystem::Update(GameState& gameState) {

    while(const std::optional event = mainWindow.pollEvent())
    {
        if(event->is<sf::Event::Closed>()) {
            mainWindow.close();
        }
        else if(const auto* mouseMove = event->getIf<sf::Event::MouseMoved>()) {
            gameState.DoAction(ActionID::ResetMenuSelection);
            for(unsigned int n = 0; n < gameState.numEntities; ++n) {
                const auto& boundingBoxCmp{gameState.boundingBoxCmps[n]};
                const auto& mouseOverCmp{gameState.mouseOverCmps[n]};
                if(boundingBoxCmp.enabled && mouseOverCmp.enabled) {
                    if(IsInBounds(boundingBoxCmp.sfmlBoundingBox, mouseMove->position)) {
                        mouseOverCmp.Notify(gameState, *mouseMove);
                        break;
                    }
                }
            }
        }
        else if(const auto* mouseClick = event->getIf<sf::Event::MouseButtonPressed>()) {
            if(mouseClick->button == sf::Mouse::Button::Left) {
                for(unsigned int n = 0; n < gameState.numEntities; ++n) {
                    const auto& boundingBoxCmp{gameState.boundingBoxCmps[n]};
                    const auto& leftClickCmp{gameState.leftClickCmps[n]};
                    if(boundingBoxCmp.enabled && leftClickCmp.enabled) {
                        if(IsInBounds(boundingBoxCmp.sfmlBoundingBox, mouseClick->position)) {
                            leftClickCmp.Notify(gameState, *mouseClick);
                            break;
                        }
                    }
                }
            }
        }
        else if(const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            for(unsigned int n = 0; n < gameState.numEntities; ++n) {
                const auto& keyPressCmp{gameState.keyPressCmps[n]};
                if(keyPressCmp.enabled) {
                    keyPressCmp.Notify(gameState, *keyPressed);
                    break;
                }
            }
        }
    }

}