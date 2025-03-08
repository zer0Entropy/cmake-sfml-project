#include "../../include/component/text.hpp"

void InitText(  TextComponent& text,
                Font& font,
                unsigned int charSize,
                float outlineThickness,
                sf::Color outlineColor,
                sf::Color fillColor,
                std::string_view contents) {
    text.font = &font;
    text.characterSize = charSize;
    text.outlineThickness = outlineThickness;
    text.outlineColor = outlineColor;
    text.fillColor = fillColor;
    text.contents = contents;
}
