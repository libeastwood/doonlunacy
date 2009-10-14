#ifndef DUNE_APPLICATION_H
#define DUNE_APPLICATION_H

#include <boost/python.hpp>

#include "singleton.h"
#include "states/State.h"
#include "states/TopLevelState.h"

#include "gui2/Container.h"
#include "gui2/Label.h"

namespace python = boost::python;

enum Cursor {
	CURSOR_NORMAL,
	CURSOR_RIGHT,
	CURSOR_UP,
	CURSOR_LEFT,
	CURSOR_DOWN,
	CURSOR_TARGET,
	CURSOR_SELECTION,
	NUM_CURSORS
};

class Container;
class Label;
class Application : public Singleton<Application>
{
    friend class Singleton<Application>;
    friend class Settings;

    protected:
        Application();
        ~Application();

    public:
        void Init();
        void Run();

        void Die();

        inline Image  *Screen() { return m_screen; }

        inline uint16_t CursorX() { return m_cursorX; }
        inline uint16_t CursorY() { return m_cursorY; }
        Cursor CurrentCursor() { return m_cursorFrame; }
        void SetCursor(Cursor cursor) { m_cursorFrame = cursor; }

        StateMachine* RootState() { return m_rootState; }

        Container* RootWidget() { return m_rootWidget; }

        void SetPalette();
	eastwood::Palette GetCurrentPalette() { return m_currentPalette; }

        void SetClearColor(uint32_t palIndex) { m_clearColor = palIndex; }

    private:
        void InitSettings();
        void InitAudio();
        void InitNet();
        void InitFont();
        void InitVideo();
        void LoadData();


        void HandleEvents();
        void BlitCursor();

        Image *m_screen;
	eastwood::Palette m_currentPalette;
        
        StateMachine *m_rootState;

        Container *m_rootWidget;
                
        bool m_running;

        uint16_t m_cursorX, m_cursorY;
        Cursor m_cursorFrame;
        ConstImagePtr m_cursor;

        uint32_t m_clearColor;
};


#endif // DUNE_APPLICATION_H

