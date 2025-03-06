#pragma once

#include <SFML/Graphics/Font.hpp>
#include "resource.hpp"

struct Font: public Resource {
    sf::Font        sfmlFont;
};