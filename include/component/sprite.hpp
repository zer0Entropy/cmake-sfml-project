#pragma once

#include <memory>
#include <SFML/Graphics/Sprite.hpp>
#include "component.hpp"
#include "../resource/texture.hpp"

struct SpriteComponent: public Component {
    std::unique_ptr<sf::Sprite>     sfmlSprite;
    Texture*                        texture;
};