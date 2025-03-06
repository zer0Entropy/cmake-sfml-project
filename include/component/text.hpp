#pragma once

#include <memory>
#include <SFML/Graphics/Text.hpp>
#include "component.hpp"
#include "../resource/font.hpp"

struct TextComponent: public Component {
    std::unique_ptr<sf::Text>       sfmlText;
    Font*                           font;
};

void InitText(  TextComponent& text,
                Font& font,
                unsigned int charSize,
                float outlineThickness,
                sf::Color outlineColor,
                sf::Color fillColor,
                std::string_view contents);
