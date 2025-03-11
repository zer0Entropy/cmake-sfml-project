#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "state.hpp"
#include "player.hpp"
#include "level.hpp"
#include "resource/resourceMgr.hpp"

struct GameplayState: public GameState {

    Player          player;
    Level           currentLevel;

    static constexpr std::string_view   wallTextureID{"wallTexture"};
    static constexpr std::string_view   wallTexturePath{
        "resource/16-Bit Fantasy Sprite Set/Sliced/world_24x24/oryx_16bit_fantasy_world_59.png"
    };

    static constexpr std::string_view   floorTextureID{"floorTexture"};
    static constexpr std::string_view   floorTexturePath{
        "resource/16-Bit Fantasy Sprite Set/Sliced/world_24x24/oryx_16bit_fantasy_world_62.png"
    };

    static constexpr std::string_view   playerTextureID{"playerTexture"};
    static constexpr std::string_view   playerTexturePath{"resource/16-Bit Fantasy Sprite Set/Sliced/creatures_24x24/oryx_16bit_fantasy_creatures_01.png"};

    static constexpr unsigned int       playerStartLocationX{Map::width / 2};
    static constexpr unsigned int       playerStartLocationY{Map::height / 2};

    void            CreateLevel(ResourceMgr& resourceMgr, RandomNumberGenerator& rng);
    Entity          CreatePlayer(ResourceMgr& resourceMgr);

    bool            MovePlayer(sf::Vector2u location);

    void DoAction(ActionID action, std::optional<EntityID> ownerID = std::nullopt);
    void RenderLevel(Level& level, const Player& player, sf::RenderWindow& window);
    void UpdatePlayer();

};