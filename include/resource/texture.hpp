#pragma once

#include <SFML/Graphics/Texture.hpp>
#include "resource.hpp"

struct Texture: public Resource {
    sf::Texture         sfmlTexture;
};