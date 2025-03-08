#pragma once

#include <SFML/Graphics/Rect.hpp>

#include "component.hpp"
#include "transform.hpp"
#include "sprite.hpp"
#include "text.hpp"

struct BoundingBoxComponent: public Component {
    sf::FloatRect       sfmlBoundingBox;
};

void UpdateBoundingBox(BoundingBoxComponent& boundingBox, TransformComponent& transform, SpriteComponent& sprite);
void UpdateBoundingBox(BoundingBoxComponent& boundingBox, TransformComponent& transform, TextComponent& text);
bool IsInBounds(const sf::FloatRect& bounds, const sf::Vector2i& point);