#pragma once

#include <memory>
#include <SFML/Graphics/Text.hpp>
#include "component.hpp"
#include "../resource/font.hpp"

struct TextComponent: public Component {
    Font*           font;
    unsigned int    characterSize;
    float           outlineThickness;
    sf::Color       outlineColor;
    sf::Color       fillColor;
    std::string     contents;
};

void InitText(  TextComponent& text,
                Font& font,
                unsigned int charSize,
                float outlineThickness,
                sf::Color outlineColor,
                sf::Color fillColor,
                std::string_view contents);
