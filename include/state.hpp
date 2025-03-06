#pragma once

#include <array>

#include "component/boundingBox.hpp"
#include "component/sprite.hpp"
#include "component/text.hpp"
#include "component/transform.hpp"
#include "component/widget.hpp"
#include "component/mouseOver.hpp"
#include "component/leftClick.hpp"
#include "ui/action.hpp"

struct GameState {

    enum class ID {
        MainMenu = 0,
        Gameplay,
        LoadGame,
        Settings,
        GameOver
    };

    enum class TransitionID {
        Null = -1,
        ReturnToMainMenu,
        InitGameplay,
        DisplayLoadGame,
        DisplaySettings,
        DisplayGameOver,
        Shutdown
    };

    ID                                                  id;
    TransitionID                                        transitionFlag;
    unsigned int                                        numEntities;
    std::array<SpriteComponent, MaxNumEntities>         spriteCmps;
    std::array<TextComponent, MaxNumEntities>           textCmps;
    std::array<TransformComponent, MaxNumEntities>      transformCmps;
    std::array<BoundingBoxComponent, MaxNumEntities>    boundingBoxCmps;
    std::array<WidgetComponent, MaxNumEntities>         widgetCmps;
    std::array<MouseOverComponent, MaxNumEntities>      mouseOverCmps;
    std::array<LeftClickComponent, MaxNumEntities>      leftClickCmps;

    virtual void DoAction(ActionID action, std::optional<EntityID> ownerID = std::nullopt) = 0;
};