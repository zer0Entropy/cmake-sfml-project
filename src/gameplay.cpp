#include "../include/gameplay.hpp"
#include "../include/log.hpp"

void GameplayState::CreateLevel(ResourceMgr& resourceMgr, RandomNumberGenerator& rng) {
    currentLevel.logMgr = logMgr;

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

    InitLevel(currentLevel, 0, rng);
    currentLevel.terrainTextures[(int)Terrain::Type::Ground] = resourceMgr.AcquireTexturePtr((ResourceID)floorTextureID);
    currentLevel.terrainTextures[(int)Terrain::Type::Wall] = resourceMgr.AcquireTexturePtr((ResourceID)wallTextureID);

    InitRooms(currentLevel);

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
    /* KEYPRESS COMPONENT */
    InitComponent(keyPressCmps[playerCharacterID], playerCharacterID, Component::Type::KeyPress);

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

bool GameplayState::MovePlayer(sf::Vector2u location) {
    bool success{false};
    if(location.x < Map::width && location.y < Map::height) {
        CreatureComponent& playerCharacter{static_cast<CreatureComponent&>(*GetComponent(player.character, Component::Type::Creature))};
        // First remove them from previous location
        currentLevel.map.tiles[playerCharacter.location.y * Map::width + playerCharacter.location.x].occupant = nullptr;
        // Put player at new location
        playerCharacter.location = location;
        currentLevel.map.tiles[location.y * Map::width + location.x].occupant = &playerCharacter;
        success = true;
    }
    return success;
}

void GameplayState::DoAction(ActionID action, std::optional<EntityID> ownerID) {

    const CreatureComponent& playerCharacter{static_cast<CreatureComponent&>(*GetComponent(player.character, Component::Type::Creature))};
    const auto currentLocation{playerCharacter.location};
    auto moveLocation{currentLocation};
    if(action == ActionID::PressDirectionLeft) {
        moveLocation.x--;
    }
    else if(action == ActionID::PressDirectionRight) {
        moveLocation.x++;
    }
    else if(action == ActionID::PressDirectionUp) {
        moveLocation.y--;
    }
    else if(action == ActionID::PressDirectionDown) {
        moveLocation.y++;
    }
    bool attemptMove{MovePlayer(moveLocation)};

}

void GameplayState::RenderLevel(Level& level, const Player& player, sf::RenderWindow& window) {
    CreatureComponent& playerCharacter{creatureCmps[player.character.first]};
    UpdateView(level, playerCharacter.location);

    assert(level.mapView.position.y + level.mapView.size.y <= Map::height && "MapView height > Map height");
    assert(level.mapView.position.x + level.mapView.size.x <= Map::width && "MapView width > Map width");

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
