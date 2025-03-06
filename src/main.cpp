#include "../include/game.hpp"

int main()
{
    Game    gameInstance;
    GameState* state{nullptr};

    do {
        gameInstance.Update();
        state = gameInstance.GetCurrentState();
    } while(state && state->transitionFlag != GameState::TransitionID::Shutdown);

    return 0;
}
