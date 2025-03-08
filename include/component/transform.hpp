#pragma once

#include <SFML/Graphics/Transformable.hpp>
#include "component.hpp"

struct TransformComponent: public Component {
    sf::Transformable       sfmlTransformable;
};