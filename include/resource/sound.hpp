#pragma once

#include <SFML/Audio/SoundBuffer.hpp>
#include "resource.hpp"

struct Sound: public Resource {
    sf::SoundBuffer     sfmlSoundBuffer;
};