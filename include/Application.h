#ifndef DUNE_APPLICATION_H
#define DUNE_APPLICATION_H

#include "singleton.h"
#include "State.h"
#include "SDL.h"
#include "gui2/Container.h"

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

        inline SDL_Surface* Screen() { return m_screen; }

        inline Uint16 CursorX() { return m_cursorX; }
        inline Uint16 CursorY() { return m_cursorY; }
        Cursor CurrentCursor() { return m_cursorFrame; }
        void SetCursor(Cursor cursor) { m_cursorFrame = cursor; }

        StateMachine* RootState() { return m_rootState; }

        Container* RootWidget() { return m_rootWidget; }

        void Blit(SDL_Surface* surface, SDL_Rect* src, SDL_Rect* dest);
        void BlitCentered(SDL_Surface* surface, SDL_Rect* src=NULL);

        void SetPalette(SDL_Palette* pal);

    private:
        void InitSettings();
        void InitAudio();
        void InitNet();
        void InitFont();
        void InitVideo();
        void LoadData();

        void HandleEvents();
        void BlitCursor();

        SDL_Surface* m_screen;
        
        StateMachine* m_rootState;

        Container* m_rootWidget;
        
        bool m_running;

        Uint16 m_cursorX, m_cursorY;
        Cursor m_cursorFrame;
};


#endif // DUNE_APPLICATION_H






