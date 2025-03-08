#pragma once

#include "state.hpp"

struct GameOver: public GameState {
    void DoAction(ActionID action, std::optional<EntityID> ownerID = std::nullopt);
};