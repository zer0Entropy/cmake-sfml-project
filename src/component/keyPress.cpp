#include "../../include/component/keyPress.hpp"
#include "../../include/state.hpp"

void KeyPressComponent::Notify(GameState& gameState, const std::optional<sf::Event> event) const {
    if(const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
        if(keyPressed->scancode == sf::Keyboard::Scancode::Left) {
            gameState.DoAction(ActionID::PressDirectionLeft, ownerID);
        }
        else if(keyPressed->scancode == sf::Keyboard::Scancode::Right) {
            gameState.DoAction(ActionID::PressDirectionRight, ownerID);
        }
        else if(keyPressed->scancode == sf::Keyboard::Scancode::Up) {
            gameState.DoAction(ActionID::PressDirectionUp, ownerID);
        }
        else if(keyPressed->scancode == sf::Keyboard::Scancode::Down) {
            gameState.DoAction(ActionID::PressDirectionDown, ownerID);
        }
    }
}