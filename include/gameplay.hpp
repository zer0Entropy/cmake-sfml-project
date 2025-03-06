#pragma once

#include "state.hpp"

struct Gameplay: public GameState {
    void DoAction(ActionID action, EntityID ownerID = MaxNumEntities);
};