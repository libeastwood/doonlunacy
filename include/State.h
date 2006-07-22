#ifndef DUNE_STATE_H
#define DUNE_STATE_H

#include <list>

class StateMachine;

class State
{
    public:
        State();
        virtual ~State();
        
        StateMachine* GetParent() { return mp_parent; }
        void SetParent(StateMachine* sm) { mp_parent = sm; }

        virtual int Execute(float dt) = 0;

        virtual void JustMadeActive();
        virtual void JustMadeInactive();

        virtual void PopState();

        virtual const char* GetName() { return "Unknown"; }
    
    protected:
        StateMachine* mp_parent;
};


class StateMachine : public State
{
    typedef std::list<State*> StateList;

    public:
        StateMachine();
        virtual ~StateMachine();

        virtual void PushState(State* state);
        virtual void PopState();
        //virtual void ReplaceState(State* state);

        State* GetCurrentState() { return *(m_stateStack.begin()); };

        int Execute(float dt);

    private:
        StateList m_stateStack;
};

#endif // DUNE_STATE_H
