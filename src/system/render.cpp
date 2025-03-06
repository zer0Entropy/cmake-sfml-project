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
            mainWindow.draw(*spriteCmp.sfmlSprite);
        }
        else if(textCmp.enabled && transformCmp.enabled) {
            mainWindow.draw(*textCmp.sfmlText);
        }
    }

}