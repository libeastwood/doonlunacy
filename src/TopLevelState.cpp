#include "TopLevelState.h"

TopLevelState::TopLevelState()
{
}

TopLevelState::~TopLevelState()
{
}

void TopLevelState::JustMadeActive()
{
    StateMachine::JustMadeActive();
}

void TopLevelState::JustMadeInactive()
{
    StateMachine::JustMadeInactive();
}

int TopLevelState::Execute(float dt)
{
    return StateMachine::Execute(dt);

}
