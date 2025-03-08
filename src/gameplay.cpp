#include "../include/gameplay.hpp"

void GameplayState::DoAction(ActionID action, std::optional<EntityID> ownerID) {

}

void GameplayState::RenderLevel(Level& level, const Player& player, sf::RenderWindow& window) {
    CreatureComponent& playerCharacter{creatureCmps[player.character.first]};
    UpdateView(level, playerCharacter.location);
    for(unsigned int y = level.mapView.topLeft.y; y < level.mapView.topLeft.y + level.mapView.sizeInTiles.y; ++y) {
        if(y > Map::height) {
            break;
        }
        for(unsigned int x = level.mapView.topLeft.x; x < level.mapView.topLeft.x + level.mapView.sizeInTiles.x; ++x) {
            if(x > Map::width) {
                break;
            }
            Terrain::Type terrainType{level.map.tiles[y * Map::width + x].terrain.type};
            sf::Sprite tileSprite{level.terrainTextures[(int)terrainType]->sfmlTexture};
            tileSprite.setScale(sf::Vector2f{3.0f, 3.0f});
            tileSprite.setPosition(sf::Vector2f{
                (float)(x * Tile::widthInPixels) - (float)(level.mapView.topLeft.x * Tile::widthInPixels),
                (float)(y * Tile::heightInPixels) - (float)(level.mapView.topLeft.y * Tile::heightInPixels)});
            window.draw(tileSprite);
        }
    }
}