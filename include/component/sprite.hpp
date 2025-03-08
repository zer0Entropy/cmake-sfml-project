#pragma once

#include <memory>
#include <SFML/Graphics/Sprite.hpp>
#include "component.hpp"
#include "../resource/texture.hpp"

struct SpriteComponent: public Component {
    Texture*        texture;
};