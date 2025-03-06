#include "../../include/component/boundingBox.hpp"

void UpdateBoundingBox(BoundingBoxComponent& boundingBox, SpriteComponent& sprite) {
    if(!sprite.sfmlSprite) {
        boundingBox.sfmlBoundingBox = sf::FloatRect{{0.f, 0.f}, {0.f, 0.f}};
    } else {
        boundingBox.sfmlBoundingBox = sprite.sfmlSprite->getGlobalBounds();
    }
}

void UpdateBoundingBox(BoundingBoxComponent& boundingBox, TextComponent& text) {
    if(!text.sfmlText) {
        boundingBox.sfmlBoundingBox = sf::FloatRect{{0.f, 0.f}, {0.f, 0.f}};
    } else {
        boundingBox.sfmlBoundingBox = text.sfmlText->getGlobalBounds();
    }
}

bool IsInBounds(const sf::FloatRect& bounds, const sf::Vector2i& point) {
    bool inBounds{false};
    if(     point.x >= bounds.position.x
        &&  point.x < bounds.position.x + bounds.size.x
        &&  point.y >= bounds.position.y
        &&  point.y < bounds.position.y + bounds.size.y) {
        inBounds = true;
    }
    return inBounds;
}