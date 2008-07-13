#ifndef DUNE_STATE_H
#define DUNE_STATE_H

#include <list>

// fwd dec
class StateMachine;
class Settings;
/*!
	Base State class used with StateMachine. 
*/
class State
{
    public:
		//! @name Constructors & Destructor
        //@{
        State();
        virtual ~State();
		//@}
        
		//! @name Parent methods
		//@{
        StateMachine* GetParent() { return mp_parent; }
        void SetParent(StateMachine* sm) { mp_parent = sm; }
		//@}

		/*!
		Called once per frame when the state is active.
		@param dt time in seconds since last frame
		*/
        virtual int Execute(float dt) = 0;

		/*!
		Called just before the state is made the current state
		*/
        virtual void JustMadeActive();
		/*! 
		Called just after the state is no-longer the current
		*/
        virtual void JustMadeInactive();

		/*!
		Pops this state. 
		*/
        virtual void PopState();

		/*! 
		Returns the state name. Used internally for debug information.
		@return the state name
		*/
        virtual const char* GetName() { return "UnknownState"; }
    
    protected:
        StateMachine* mp_parent;
	Settings* set;
};

/*
	StateMachine class. Derived from State so it can be used as a state 
	in another StateMachine
*/
class StateMachine : public State
{
    typedef std::list<State*> StateList;

    public:
		//! @name Constructors & Destructor
		//@{
        StateMachine();
        virtual ~StateMachine();
		//@}

		/*!
		Push a state onto the stack. The state will become the current state. 
		@param state the state to push onto the state stack
		*/
        virtual void PushState(State* state);
		/*!
		Pop the current child state. If there are no child states left, pop 
		this statemachine from its parent
		*/
        virtual void PopState();
        //virtual void ReplaceState(State* state);

		/*! 
		Return the current child state
		*/
        State* GetCurrentState() { return *(m_stateStack.begin()); };

        int Execute(float dt);

    private:
        StateList m_stateStack;
};

#endif // DUNE_STATE_H
