#pragma once

enum class ActionID {
    Null,
    TransitionMainMenu,
    TransitionLoadGame,
    TransitionGameplay,
    TransitionSettings,
    TransitionGameOver,
    Shutdown,
    SetMenuSelection,
    ResetMenuSelection,
    PressMenuSelection,
    PressDirectionLeft,
    PressDirectionRight,
    PressDirectionUp,
    PressDirectionDown
};