#ifndef DUNE_APPLICATION_H
#define DUNE_APPLICATION_H

#include "singleton.h"
#include "State.h"
#include "SDL.h"
#include "gui2/Container.h"

#include "State.h"
#include "TopLevelState.h"
typedef enum
{
	CURSOR_NORMAL,
	CURSOR_RIGHT,
	CURSOR_UP,
	CURSOR_LEFT,
	CURSOR_DOWN,
	CURSOR_TARGET,
	CURSOR_SELECTION,
	NUM_CURSORS
} Cursor;

class Application : public Singleton<Application>
{
    friend class Singleton<Application>;

    protected:
        Application();
        ~Application();

    public:
        void Init();
        void Run();

        void Die();

        inline Image * Screen() { return m_screen; }

        inline Uint16 CursorX() { return m_cursorX; }
        inline Uint16 CursorY() { return m_cursorY; }
        Cursor CurrentCursor() { return m_cursorFrame; }
        void SetCursor(Cursor cursor) { m_cursorFrame = cursor; }

        StateMachine* RootState() { return m_rootState; }

        Container* RootWidget() { return m_rootWidget; }

        void SetPalette();
        SDL_Palette* GetCurrentPalette() { return m_currentPalette; }

        void SetClearColor(Uint32 palIndex) { m_clearColor = palIndex; }

        void UpdateVideoMode(bool fullscreen);
        void UpdateVideoMode(Uint16 w, Uint16 h);
        void UpdateVideoMode(Uint16 w, Uint16 h, bool fullscreen);
        
    private:
        void InitSettings();
        void InitAudio();
        void InitNet();
        void InitFont();
        void InitVideo();
        void LoadData();


        void HandleEvents();
        void BlitCursor();

        Image * m_screen;
        SDL_Palette* m_currentPalette;
        
        StateMachine* m_rootState;

        Container* m_rootWidget;
        
        bool m_running;

        Uint16 m_cursorX, m_cursorY;
        Cursor m_cursorFrame;
        ImagePtr m_cursor;

        Uint32 m_clearColor;
};


#endif // DUNE_APPLICATION_H






