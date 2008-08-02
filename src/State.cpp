#include "Log.h"
#include "State.h"
#include "Settings.h"
#include "DataCache.h"
#include "SoundPlayer.h"

#include <assert.h>

State::State()
{
    mp_parent = NULL;
    set = Settings::Instance();
    m_song = NULL;
}

State::~State()
{
    if(m_song != NULL)
        free(m_song);
}


void State::JustMadeActive()
{
    LOG_INFO("State", "--> %s just made active", GetName());
}

void State::JustMadeInactive()
{
    LOG_INFO("State", "<-- %s just made inactive", GetName());
}

void State::PopState()
{ 
    mp_parent->PopState(); 
}

void State::playMusic()
{
    if(m_song != NULL){
	    if(set->GetMusic() && (SoundPlayer::Instance()->getCurrentSong() == NULL ||
		    !(SoundPlayer::Instance()->getCurrentSong()->filename == m_song->filename
	 	     && SoundPlayer::Instance()->getCurrentSong()->track == m_song->track)))
		    SoundPlayer::Instance()->playMusic(m_song->filename, m_song->track);
	    }
}

//-------------------------------------------------------------------

StateMachine::StateMachine()
{
}

StateMachine::~StateMachine()
{
}

void StateMachine::PushState(State* state)
{
    if (!m_stateStack.empty())
    {
        GetCurrentState()->JustMadeInactive();
    };
    
    m_stateStack.push_front(state);

    state->SetParent(this);
    state->JustMadeActive();
}

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
}

int StateMachine::Execute(float dt)
{
    // can probably do without this line as pop takes care of it 
    if (m_stateStack.empty()) return -1;

    GetCurrentState()->playMusic();

    if ( GetCurrentState()->Execute(dt) == -1)
    {
        PopState();
    };


    return 0;
}
