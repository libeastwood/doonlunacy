#ifndef DUNE_TOPLEVELSTATE_H
#define DUNE_TOPLEVELSTATE_H

#include "State.h"

class TopLevelState : public StateMachine 
{
    public:
        TopLevelState();
        ~TopLevelState();

        void JustMadeActive();
        void JustMadeInactive();

        int Execute(float dt);

        const char* GetName() { return "TopLevelState"; }
};

#endif // DUNE_TOPLEVELSTATE_H
