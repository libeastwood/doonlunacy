#include "State.h"

#define STATE_DEBUG

State::State()
{
    mp_parent = NULL;
};

State::~State()
{
};


void State::JustMadeActive()
{
#ifdef STATE_DEBUG
    printf("--> %s just made active\n", GetName());
#endif 
};

void State::JustMadeInactive()
{
#ifdef STATE_DEBUG
    printf("<-- %s just made inactive\n", GetName());
#endif 
};

void State::PopState()
{ 
    mp_parent->PopState(); 
};



//-------------------------------------------------------------------

StateMachine::StateMachine()
{
};

StateMachine::~StateMachine()
{
};

void StateMachine::PushState(State* state)
{
    if (!m_stateStack.empty())
    {
        GetCurrentState()->JustMadeInactive();
    };
    
    m_stateStack.push_front(state);

    state->SetParent(this);
    state->JustMadeActive();
};

void StateMachine::PopState()
{
    State* state = GetCurrentState();
    assert(state != NULL);

    state->JustMadeInactive();
    
    m_stateStack.pop_front();    

    delete state;
    
    if (m_stateStack.empty())
    {
        if (mp_parent != NULL)
        {
            mp_parent->PopState();
        };
    }
    else
    {
        GetCurrentState()->JustMadeActive();
    };
};

int StateMachine::Execute(float dt)
{
    // can probably do without this line as pop takes care of it 
    if (m_stateStack.empty()) return -1;

    if ( GetCurrentState()->Execute(dt) == -1)
    {
        PopState();
    };

    return 0;
};













