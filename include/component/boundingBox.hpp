#pragma once

#include <SFML/Graphics/Rect.hpp>

#include "component.hpp"
#include "sprite.hpp"
#include "text.hpp"

struct BoundingBoxComponent: public Component {
    sf::FloatRect       sfmlBoundingBox;
};

void UpdateBoundingBox(BoundingBoxComponent& boundingBox, SpriteComponent& sprite);
void UpdateBoundingBox(BoundingBoxComponent& boundingBox, TextComponent& text);
bool IsInBounds(const sf::FloatRect& bounds, const sf::Vector2i& point);