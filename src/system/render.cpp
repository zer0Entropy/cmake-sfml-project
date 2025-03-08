#include "../../include/system/render.hpp"
#include "../../include/state.hpp"

RenderSystem::RenderSystem(sf::RenderWindow& window):
    mainWindow{window} {

}

RenderSystem::~RenderSystem() {

}

void RenderSystem::Update(GameState& gameState) {

    for(unsigned int n = 0; n < gameState.numEntities; ++n) {
        const auto& spriteCmp{gameState.spriteCmps[n]};
        const auto& textCmp{gameState.textCmps[n]};
        const auto& transformCmp{gameState.transformCmps[n]};
        if(spriteCmp.enabled && transformCmp.enabled) {
            sf::Sprite sprite{spriteCmp.texture->sfmlTexture};
            sprite.setOrigin(transformCmp.sfmlTransformable.getOrigin());
            sprite.setScale(transformCmp.sfmlTransformable.getScale());
            sprite.setPosition(transformCmp.sfmlTransformable.getPosition());
            mainWindow.draw(sprite);
        }
        else if(textCmp.enabled && transformCmp.enabled) {
            sf::Text text{textCmp.font->sfmlFont};
            text.setCharacterSize(textCmp.characterSize);
            text.setOutlineThickness(textCmp.outlineThickness);
            text.setOutlineColor(textCmp.outlineColor);
            text.setFillColor(textCmp.fillColor);
            text.setString(textCmp.contents);
            text.setOrigin(transformCmp.sfmlTransformable.getOrigin());
            text.setPosition(transformCmp.sfmlTransformable.getPosition());
            mainWindow.draw(text);
        }
    }

}