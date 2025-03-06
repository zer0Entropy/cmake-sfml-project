#include "../../include/component/text.hpp"

void InitText(  TextComponent& text,
                Font& font,
                unsigned int charSize,
                float outlineThickness,
                sf::Color outlineColor,
                sf::Color fillColor,
                std::string_view contents) {
    text.font = &font;
    if(!text.sfmlText) {
        text.sfmlText = std::make_unique<sf::Text>(font.sfmlFont);
    } else {
        text.sfmlText->setFont(font.sfmlFont);
    }
    text.sfmlText->setCharacterSize(charSize);
    text.sfmlText->setOutlineThickness(outlineThickness);
    text.sfmlText->setOutlineColor(outlineColor);
    text.sfmlText->setFillColor(fillColor);
    text.sfmlText->setString((std::string)contents);
}
