#include "../../include/component/leftClick.hpp"
#include "../../include/state.hpp"

void LeftClickComponent::Notify(GameState& gameState, const std::optional<sf::Event> event) const {
    if(const auto mouseClick = event->getIf<sf::Event::MouseButtonPressed>()) {
        if(mouseClick->button == sf::Mouse::Button::Left) {
            const auto& boundingBox{gameState.boundingBoxCmps[ownerID]};
            if(IsInBounds(boundingBox.sfmlBoundingBox, mouseClick->position)) {
                gameState.DoAction(triggeredAction, ownerID);
            }
        }
    }
}