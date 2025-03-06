#pragma once

#include <SFML/Graphics/Transform.hpp>
#include "component.hpp"

struct TransformComponent: public Component {
    sf::Transformable*      sfmlTransformable;
};