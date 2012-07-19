#ifndef DUNE_GUI2_WIDGET
#define DUNE_GUI2_WIDGET

#include "Gfx.h"
#include "houses.h"

/*!
	Base class for all widgets
*/
class Widget : public Rect
{
    public:
		//! @name Constructors & Destructor
        //@{
        Widget();
	virtual ~Widget() {};
		//@}

        virtual void draw(Image * dest, SPoint off) {}

		//! @name Event handling functions
		//@{

		/*!
			called when the cursor is moved over the widget

			@param p cursor location relative to the widget
			@return true if the event was handled
		*/
        virtual bool handleMotion(SPoint p);
		/*!
			called when a mouse button is pressed 
			@param button button number. You can use SDL button constants.
			@param p cursor location relative to the widget
			@return true if the event was handled
		*/
        virtual bool handleButtonDown(uint8_t button, SPoint p);
		/*! 
			called when a mouse button is released
			@param button button number. You can use SDL button constants.
			@param p cursor location relative to the widget
			@return true if the event was handled
		*/
        virtual bool handleButtonUp(uint8_t button, SPoint p);

		/*!
			called when a key is pressed
			@param key SDL_keysym object. 
			@return true if the event was handled 
		*/
        virtual bool handleKeyDown(SDL_keysym* key) { return false; }
		/*!
			called when a key is released
			@param key SDL_keysym object
			@return true if the event was handled 
		*/
        virtual bool handleKeyUp(SDL_keysym* key) { return false; }
		//@}
        
		//! @name Widget properties
		//@{
		/*! 
			enable or disable the widget. 
			@param enabled true to enable the widget
		*/
        virtual void setEnabled(bool enabled) { m_enabled = enabled; }
		/*!
			@return true if the widget is enabled 
		*/
        inline bool enabled() { return m_enabled; }

		/*!
			set the focus to the widget
		*/
        virtual void setFocus();
		/*!
			@return true if the widget has focus
		*/
        inline bool focused() { return (m_focusedWidget == this); }
		/*!
			static function to return the widget with focus 
			@return the widget who has focus
		*/
        static Widget* getFocused() { return m_focusedWidget; }

		/*!
			make the widget visible
			@param visible true to make the widget visible
		*/
        virtual void setVisible(bool visible) { m_visible = visible; }
		/*!
			@return true if the widget is visible
		*/
        inline bool visible() { return m_visible; }

		inline bool parent() { return m_parent; }

		bool fadeIn(const int fadeAmt = 4);
		bool fadeOut(const int fadeAmt = 4);

		inline void recolorByHouse(HOUSETYPE house) { if(m_surface) m_surface->recolorByHouse(house); } 
		//@}

    protected:
        virtual void gotFocus() {};
        virtual void lostFocus() {};
        
		Rect m_rect;
		ImagePtr m_surface;

        bool m_enabled,
			 m_visible,
			 m_parent;

        static Widget* m_focusedWidget;
};

/*
inline bool Widget::hitTest(uint16_t x, uint16_t y)
{
    return ((x > m_x) && (x < m_x + m_width) &&
            (y > m_y) && (y < m_y + m_height));
}
*/

#endif // DUNE_GUI2_WIDGET

