#pragma once

#include <SFML/Audio/Music.hpp>
#include "resource.hpp"

struct Music: public Resource {
    sf::Music       sfmlMusic;
};