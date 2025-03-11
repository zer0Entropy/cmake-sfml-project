#pragma once

#include <array>

#include "component/boundingBox.hpp"
#include "component/sprite.hpp"
#include "component/text.hpp"
#include "component/transform.hpp"
#include "component/widget.hpp"
#include "component/mouseOver.hpp"
#include "component/leftClick.hpp"
#include "component/keyPress.hpp"
#include "component/creature.hpp"
#include "ui/action.hpp"

class LogMgr;

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

    bool                                                errorFlag;
    LogMgr*                                             logMgr;

    ID                                                  id;
    TransitionID                                        transitionFlag;
    unsigned int                                        numEntities;
    sf::Vector2u                                        windowSize;

    std::array<SpriteComponent, MaxNumEntities>         spriteCmps;
    std::array<TextComponent, MaxNumEntities>           textCmps;
    std::array<TransformComponent, MaxNumEntities>      transformCmps;
    std::array<BoundingBoxComponent, MaxNumEntities>    boundingBoxCmps;
    std::array<WidgetComponent, MaxNumEntities>         widgetCmps;
    std::array<MouseOverComponent, MaxNumEntities>      mouseOverCmps;
    std::array<LeftClickComponent, MaxNumEntities>      leftClickCmps;
    std::array<KeyPressComponent, MaxNumEntities>       keyPressCmps;
    std::array<CreatureComponent, MaxNumEntities>       creatureCmps;

    virtual void DoAction(ActionID action, std::optional<EntityID> ownerID = std::nullopt) = 0;

    void CenterEntity(EntityID ownerID);
};