#pragma once

#include "state.hpp"

struct GameOver: public GameState {
    void DoAction(ActionID action, EntityID ownerID = MaxNumEntities);
};