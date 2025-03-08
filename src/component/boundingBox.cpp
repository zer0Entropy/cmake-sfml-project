#include "../../include/component/boundingBox.hpp"

void UpdateBoundingBox(BoundingBoxComponent& boundingBox, TransformComponent& transform, SpriteComponent& sprite) {
    sf::Sprite sfmlSprite{sprite.texture->sfmlTexture};
    sfmlSprite.setOrigin(transform.sfmlTransformable.getOrigin());
    sfmlSprite.setScale(transform.sfmlTransformable.getScale());
    sfmlSprite.setPosition(transform.sfmlTransformable.getPosition());

    boundingBox.sfmlBoundingBox = {
        sfmlSprite.getGlobalBounds().position,
        sfmlSprite.getGlobalBounds().size
    };
}

void UpdateBoundingBox(BoundingBoxComponent& boundingBox, TransformComponent& transform, TextComponent& text) {
    sf::Text sfmlText{text.font->sfmlFont};
    sfmlText.setCharacterSize(text.characterSize);
    sfmlText.setOutlineThickness(text.outlineThickness);
    sfmlText.setString(text.contents);
    sfmlText.setOrigin(transform.sfmlTransformable.getOrigin());
    sfmlText.setPosition(transform.sfmlTransformable.getPosition());

    boundingBox.sfmlBoundingBox = {
        sfmlText.getGlobalBounds().position,
        sfmlText.getGlobalBounds().size
    };
}

bool IsInBounds(const sf::FloatRect& bounds, const sf::Vector2i& point) {
    bool inBounds{false};
    if(     point.x >= bounds.position.x
        &&  point.x < bounds.position.x + bounds.size.x
        &&  point.y >= bounds.position.y
        &&  point.y < bounds.position.y + bounds.size.y) {
        inBounds = true;
    }
    return inBounds;
}