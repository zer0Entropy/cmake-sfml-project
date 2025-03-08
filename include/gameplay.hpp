#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "state.hpp"
#include "player.hpp"
#include "level.hpp"

struct GameplayState: public GameState {

    Player          player;
    Level           currentLevel;

    

    static constexpr std::string_view wallTextureID{"wallTexture"};
    static constexpr std::string_view wallTexturePath{
        "resource/16-Bit Fantasy Sprite Set/Sliced/world_24x24/oryx_16bit_fantasy_world_59.png"
    };

    static constexpr std::string_view floorTextureID{"floorTexture"};
    static constexpr std::string_view floorTexturePath{
        "resource/16-Bit Fantasy Sprite Set/Sliced/world_24x24/oryx_16bit_fantasy_world_62.png"
    };

    static constexpr std::string_view playerTextureID{"playerTexture"};
    static constexpr std::string_view playerTexturePath{"resource/16-Bit Fantasy Sprite Set/Sliced/creatures_24x24/oryx_16bit_fantasy_creatures_01.png"};

    void DoAction(ActionID action, std::optional<EntityID> ownerID = std::nullopt);
    void RenderLevel(Level& level, const Player& player, sf::RenderWindow& window);
};