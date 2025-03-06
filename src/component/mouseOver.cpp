#include "../../include/component/mouseOver.hpp"
#include "../../include/state.hpp"

void MouseOverComponent::Notify(GameState& gameState, const std::optional<sf::Event> event) const {
    if(const auto mouseMove = event->getIf<sf::Event::MouseMoved>()) {
        const auto& boundingBox{gameState.boundingBoxCmps[ownerID]};
        if(IsInBounds(boundingBox.sfmlBoundingBox, mouseMove->position)) {
            gameState.DoAction(triggeredAction, ownerID);
        }
    }
}