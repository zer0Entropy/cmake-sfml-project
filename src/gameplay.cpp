#include "../include/gameplay.hpp"
#include "../include/log.hpp"

void GameplayState::CreateLevel(ResourceMgr& resourceMgr) {
    ResourceMgr::ErrorCode loadWallOutcome{
        resourceMgr.LoadResource((ResourceID)wallTextureID, Resource::Type::Texture, (std::string)wallTexturePath)
    };

    ResourceMgr::ErrorCode loadFloorOutcome{
        resourceMgr.LoadResource((ResourceID)floorTextureID, Resource::Type::Texture, (std::string)floorTexturePath)
    };

    if(loadWallOutcome != ResourceMgr::ErrorCode::Success) {
        std::string errorHeader{"Attempt to load texture \"" + std::string(wallTextureID) + "\" failed"};
        std::string errorBody{"File not found: " + std::string(wallTexturePath)};
        logMgr->CreateMessage(errorHeader, errorBody, true);
        errorFlag = true;
        return;
    }
    else {
        std::string successHeader{"Texture \"" + std::string(wallTextureID) + "\" loaded successfully"};
        logMgr->CreateMessage(successHeader, "");
    }
    if(loadFloorOutcome != ResourceMgr::ErrorCode::Success) {
        std::string errorHeader{"Attempt to load texture \"" + std::string(floorTextureID) + "\" failed"};
        std::string errorBody{"File not found: " + std::string(floorTexturePath)};
        logMgr->CreateMessage(errorHeader, errorBody, true);
        errorFlag = true;
        return;
    }
    else {
        std::string successHeader{"Font \"" + std::string(floorTextureID) + "\" loaded successfully"};
        logMgr->CreateMessage(successHeader, "");
    }

    InitLevel(currentLevel, 0);
    currentLevel.terrainTextures[(int)Terrain::Type::Ground] = resourceMgr.AcquireTexturePtr((ResourceID)floorTextureID);
    currentLevel.terrainTextures[(int)Terrain::Type::Wall] = resourceMgr.AcquireTexturePtr((ResourceID)wallTextureID);

    InitView(currentLevel, sf::Vector2u{playerStartLocationX, playerStartLocationY}, windowSize);
}

Entity GameplayState::CreatePlayer(ResourceMgr& resourceMgr) {

    ResourceMgr::ErrorCode loadPlayerOutcome{
        resourceMgr.LoadResource((ResourceID)playerTextureID, Resource::Type::Texture, (std::string)playerTexturePath)
    };

    if(loadPlayerOutcome != ResourceMgr::ErrorCode::Success) {
        // TO DO: HANDLE ERROR CODES RETURNED BY LOADRESOURCE!
    }

    EntityID playerCharacterID{numEntities++};
    sf::Vector2u playerLocation{
        playerStartLocationX,
        playerStartLocationY
    };
    /* SPRITE COMPONENT */
    InitComponent(spriteCmps[playerCharacterID], playerCharacterID, Component::Type::Sprite);
    spriteCmps[playerCharacterID].texture = resourceMgr.AcquireTexturePtr((ResourceID)playerTextureID);
    /* TRANSFORM COMPONENT */
    InitComponent(transformCmps[playerCharacterID], playerCharacterID, Component::Type::Transform);
    transformCmps[playerCharacterID].sfmlTransformable.setScale(sf::Vector2f{3.f, 3.f});
    transformCmps[playerCharacterID].sfmlTransformable.setPosition(sf::Vector2f{0.f, 0.f});
    /* BOUNDINGBOX COMPONENT */
    InitComponent(boundingBoxCmps[playerCharacterID], playerCharacterID, Component::Type::BoundingBox);
    UpdateBoundingBox(boundingBoxCmps[playerCharacterID], transformCmps[playerCharacterID], spriteCmps[playerCharacterID]);
    /* CREATURE COMPONENT */
    InitComponent(creatureCmps[playerCharacterID], playerCharacterID, Component::Type::Creature);

    CreatureComponent& playerCharacter{creatureCmps[playerCharacterID]};
    InitCreature(playerCharacter, "PLAYER", playerLocation);
    PlaceCreature(currentLevel, playerCharacter);

    return Entity{
        playerCharacterID,
        ComponentList{
            &spriteCmps[playerCharacterID],
            &transformCmps[playerCharacterID],
            &boundingBoxCmps[playerCharacterID],
            &creatureCmps[playerCharacterID]
        }
    };
}

void GameplayState::DoAction(ActionID action, std::optional<EntityID> ownerID) {

}

void GameplayState::RenderLevel(Level& level, const Player& player, sf::RenderWindow& window) {
    CreatureComponent& playerCharacter{creatureCmps[player.character.first]};
    UpdateView(level, playerCharacter.location);

    for(unsigned int y = level.mapView.position.y; y < level.mapView.position.y + level.mapView.size.y; ++y) {
        for(unsigned int x = level.mapView.position.x; x < level.mapView.position.x + level.mapView.size.x; ++x) {
            Terrain::Type terrainType{level.map.tiles[y * Map::width + x].terrain.type};
            sf::Sprite tileSprite{level.terrainTextures[(int)terrainType]->sfmlTexture};
            tileSprite.setScale(sf::Vector2f{3.0f, 3.0f});
            tileSprite.setPosition(sf::Vector2f{
                (float)(x * Tile::widthInPixels) - (float)(level.mapView.position.x * Tile::widthInPixels),
                (float)(y * Tile::heightInPixels) - (float)(level.mapView.position.y * Tile::heightInPixels)});
            window.draw(tileSprite);
        }
    }
}

void GameplayState::UpdatePlayer() {
    const CreatureComponent& playerCharacter{creatureCmps[player.character.first]};
    unsigned int relativeX{playerCharacter.location.x - currentLevel.mapView.position.x};
    unsigned int relativeY{playerCharacter.location.y - currentLevel.mapView.position.y};
    TransformComponent& transform{transformCmps[player.character.first]};
    transform.sfmlTransformable.setPosition(sf::Vector2f{
        (float)(relativeX * Tile::widthInPixels),
        (float)(relativeY * Tile::heightInPixels)
    });
}
