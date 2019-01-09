/*********************************************************************/
/*                                                                   */
/* Copyright (C) 2005, Alexander Marinov, Nadezhda Zlateva           */
/*                                                                   */
/* Project:       ia                                                 */
/* Filename:      gia.h                                              */
/* Description:   Graphical User Interface                           */
/*                                                                   */
/*********************************************************************/

#ifndef __GIA_H
#define __GIA_H

#include <ia/ia_image.h>

/*********************************************************************/
/*                      Graphical User Interface                     */
/*********************************************************************/

/**
	Type gia_toolkit_t

	Defines enumeration of all supported gia toolkits
*/
typedef enum
{
	/** Win32 toolkit */
	GIA_WINAPI

} gia_toolkit_t;


/**
	Type gia_control_t

	Defines enumeration of all supported gia controls
*/
typedef enum
{
	/** Window control */
	GIA_CTL_WINDOW,
	/** Button control */
	GIA_CTL_BUTTON,
	/** Edit control */
	GIA_CTL_EDIT,
	/** Label control */
	GIA_CTL_LABEL,
	/** ListBox control */
	GIA_CTL_LISTBOX,
	/** ComboBox control */
	GIA_CTL_COMBOBOX,
	/** GropupBox control */
	GIA_CTL_GROUPBOX,
	/** ScrollBar control */
	GIA_CTL_SCROLLBAR,
	/** Slide Bar control */
	GIA_CTL_SLIDEBAR,
	/** Progress Bar control */
	GIA_CTL_PROGRESSBAR,
	
} gia_control_t;


/** 
	Type command_handler_t 

	Defines command handler function type
*/
struct _gia_win_t;
typedef void (*command_handler_t)(
	struct _gia_win_t*, /** caller window */
	ia_int32_t          /** command identifier */
);

/** 
	Type gia_menu_item_t 

	Defines menu element
*/
typedef struct _gia_menu_item
{
	/** the caption of the menu item */
	ia_string_t       caption;

	/** the command handler to be invoked on selecting the given menu item */
	command_handler_t command;

} gia_menu_item_t, *gia_menu_item_p;

/** 
	Type gia_menu_popup_t 

	Defines popup menu
*/
typedef struct _gia_menu_popup
{
	/** the caption of the popup menu */
	ia_string_t                         caption;

	/** popup menu items       */
	gia_menu_item_p                     items;

	/** popup menu items count */
	ia_uint32_t                         nitems;

	/** add new menu item      */
	void (*add)                         (
		struct _gia_menu_popup*, /** self */
		const ia_string_t,       /** the caption of the menu item */
		command_handler_t        /** the command handler to be invoked on selecting this menu item */
	);

	/** destroy the popup menu object */
	void (*destroy)                     (
		struct _gia_menu_popup* /** self */
	);
} gia_menu_popup_t, *gia_menu_popup_p;

/** Window style masks */
#define IAS_CAPTION       0x80000000
#define IAS_MINIMIZE      0x40000000
#define IAS_MAXIMIZE      0x20000000
#define IAS_SYSMENU       0x10000000
#define IAS_BORDER        0x08000000
#define IAS_DIALOG        0x04000000
#define IAS_MODAL         0x02000000
#define IAS_THICKFRAME    0x01000000
#define IAS_TOPMOST       0x00800000
#define IAS_CHILD         0x00400000
#define IAS_GROUP         0x00200000
#define IAS_TABSTOP       0x00100000
#define IAS_LEFT          0x00080000
#define IAS_RIGHT         0x00040000
#define IABS_CHECKBOX     0x00020000
#define IABS_GROUPBOX     0x00010000

/** Text alignments */
typedef enum
{
	IA_ALIGN_LEFT,
	IA_ALIGN_RIGHT,
	IA_ALIGN_CENTER
} ia_align_t;

/**
	Type gia_control_template_t

	Defines dialog control description object
*/
typedef struct _gia_control_template
{
	gia_control_t control_type;
	ia_string_t   title;
	ia_uint32_t   style;
	ia_rect_t     rect;
	ia_int32_t    id;

	void (*destroy)         (
		struct _gia_control_template* /** self           */
	);

} gia_control_template_t, *gia_control_template_p;

gia_control_template_p gia_control_template_new( 
	gia_control_t control_type, 
	const ia_string_t title,
	ia_uint32_t style, 
	ia_int32_t x,
	ia_int32_t y,
	ia_int32_t w,
	ia_int32_t h,
	ia_int32_t id);

/**
	Type gia_dialog_template_t

	Defines dialog description object
*/
typedef struct _gia_dialog_template
{
	ia_string_t       title;
	ia_uint32_t       style; 
	ia_rect_t         rect;
    ia_int32_t        id;
	gia_control_template_p* controls;
	ia_int32_t        ncontrols;

	void (*add)             (
		struct _gia_dialog_template*, /** self           */
		gia_control_template_p        /** dialog control */
	);

	void (*destroy)         (
		struct _gia_dialog_template*  /** self           */
	);
} gia_dialog_template_t, *gia_dialog_template_p;

gia_dialog_template_p gia_dialog_template_new(
	const ia_string_t, 
	ia_uint32_t, 
	ia_int32_t x,
	ia_int32_t y,
	ia_int32_t w,
	ia_int32_t h,
	ia_int32_t id);

/**
	Type gia_draw_operator_t
*/

typedef enum 
{
	DRAW_PIXEL,
	DRAW_LINE,
	DRAW_RECT,
	DRAW_IMAGE,
	DRAW_TEXT
} gia_draw_operator_t;

/**
	Type gia_draw_primitive_t

	Defines drawing context
*/
typedef struct _gia_draw_primitive
{
	/** drawing primitive type */
	gia_draw_operator_t draw_operator;

	/** drawing area */
	ia_int32_t x, y;

	union
	{
		ia_int32_t w;
		ia_float_t scale_x;
	} w;

	union
	{
		ia_int32_t h;
		ia_float_t scale_y;
	} h;

	/** color */ 
	ia_uint32_t color;

	union
	{
		/** text primitive data */
		struct 
		{
			ia_string_t text;
			ia_int32_t  size;
			ia_align_t  align;
		} t;

		/** image primitive data */
		ia_image_p  image;
	} d;
} gia_draw_primitive_t, *gia_draw_primitive_p;

/**
	Type gia_draw_context_t

	Defines drawing context
*/
struct _gia_win;
typedef struct _gia_draw_context
{
	/** list of drawing primitives                          */
	gia_draw_primitive_p* primitives;

	/** number of primitves in the list                     */
	ia_int32_t            nprimitives;

	/** add new draw primitive                              */
	void (*add)                         (
		struct _gia_draw_context*, /** self                 */ 
		gia_draw_primitive_p       /** primitve to add      */
	);

	/** add pixel primitive */
	void (*add_pixel)                   (
		struct _gia_draw_context*, /** self                 */
		ia_int32_t,                /** pixel x coord.       */
		ia_int32_t,                /** pixel y coord.       */
		ia_uint32_t                /** color                */
	);

	/** add line primitive */
	void (*add_line)                    (
		struct _gia_draw_context*, /** self                 */
		ia_int32_t,                /** line start x1 coord. */
		ia_int32_t,                /** line start y1 coord. */
		ia_int32_t,                /** line end x2 coord.   */
		ia_int32_t,                /** line end y2 coord.   */
		ia_uint32_t                /** color                */
	);

	/** add rectangle primitive */
	void (*add_rect)                    (
		struct _gia_draw_context*, /** self                 */
		ia_int32_t,                /** rect start x coord.  */
		ia_int32_t,                /** rect start y coord.  */
		ia_int32_t,                /** rect width           */
		ia_int32_t,                /** rect height          */
		ia_uint32_t                /** color                */
	);

	/** add image primitive */
	void (*add_image)                   (
		struct _gia_draw_context*, /** self                 */
		ia_image_p,                /** image to draw        */
		ia_int32_t,                /** image start x        */
		ia_int32_t,                /** image start y        */
		ia_float_t,                /** image scale x        */
		ia_float_t                 /** image scale y        */
	);

	/** add text primitive */
	void (*add_text)                   (
		struct _gia_draw_context*, /** self                 */
		const ia_string_t,         /** text to draw         */
		ia_int32_t,                /** text start x         */
		ia_int32_t,                /** text start y         */
		ia_uint32_t,               /** text color           */
		ia_int32_t,                /** text size            */
		ia_align_t                 /** text alignment       */
	);

	/** draw this context to a window */
	void (*draw)                     (
		struct _gia_draw_context*, /** self */
		struct _gia_win*
	);

	/** destroy the drawing context */
	void (*destroy)                     (
		struct _gia_draw_context* /** self */
	);

} gia_draw_context_t, *gia_draw_context_p;

IA_API gia_draw_context_p gia_draw_context_new( void );

/**
	Type gia_win_t

	Defines window
*/
#define _GIA_WIN_CLASS                                                          \
	/** handle to the created window specific to the target toolkit */          \
	void*                                impl;                                  \
                                                                                \
	/** control type */                                                         \
	gia_control_t                        type;                                  \
                                                                                \
	/** window title */                                                         \
	ia_string_t                          title;                                 \
                                                                                \
	/** application reference */                                                \
	struct _gia_app*                     app;                                   \
                                                                                \
	/** command handlers attached to this window */                             \
	command_handler_t*                   cmd_handlers;                          \
                                                                                \
	/** command handlers count */                                               \
    ia_uint32_t                          nhandlers;                             \
                                                                                \
	/** set window window */                                                    \
	void      (*set_title)               (                                      \
		struct _gia_win*, /** self */                                           \
		ia_string_t       /** title */                                          \
	);                                                                          \
                                                                                \
	/** get window window */                                                    \
	const ia_string_t (*get_title)             (                                \
		struct _gia_win* /** self */                                            \
	);                                                                          \
                                                                                \
	/** show/hide window */                                                     \
	void      (*set_visible)             (                                      \
		struct _gia_win*, /** self */                                           \
		ia_bool_t        /** visible status */                                  \
	);                                                                          \
                                                                                \
	/** return true if the window is visible */                                 \
	ia_bool_t (*is_visible)              (                                      \
		struct _gia_win* /** self */                                            \
	);                                                                          \
                                                                                \
	/** enable/disable window */                                                \
	void      (*set_enable)              (                                      \
		struct _gia_win*, /** self */                                           \
		ia_bool_t        /** enable status */                                   \
	);                                                                          \
                                                                                \
	/** return true if the window is enabled */                                 \
	ia_bool_t (*is_enabled)              (                                      \
		struct _gia_win* /** self */                                            \
	);                                                                          \
                                                                                \
	/** update window */                                                        \
	void      (*update)                  (                                      \
		struct _gia_win* /** self */                                            \
	);                                                                          \
                                                                                \
	/** redraw window */                                                        \
	void      (*redraw)                  (                                      \
		struct _gia_win* /** self */                                            \
	);                                                                          \
																		        \
	/** event triggered on window repainting */                                 \
	void      (*on_paint)                (                                      \
		struct _gia_win* /** self */                                            \
	);                                                                          \
                                                                                \
	/** event triggered on mouse wheel activation */                            \
	void      (*on_mousewheel)           (                                      \
		struct _gia_win*, /** self */                                           \
		ia_bool_t         /** wheel direction, true - up, false - down */       \
	);                                                                          \
                                                                                \
	/** event triggered on mouse click */                                       \
	void      (*on_mouseclick)           (                                      \
		struct _gia_win*, /** self */                                           \
		ia_pos_p,        /** pos */                                             \
		ia_bool_t,        /** true - left button, false - right button */       \
		ia_bool_t         /** true - down, false - up */                        \
	);                                                                          \
                                                                                \
	/** event triggered on mouse move  */                                       \
	void      (*on_mousemove)            (                                      \
		struct _gia_win*, /** self */                                           \
		ia_pos_p         /** pos */                                             \
	);                                                                          \
                                                                                \
	/** event triggered on window resize  */                                    \
	void      (*on_size)                 (                                      \
		struct _gia_win*, /** self */                                           \
		ia_pos_p         /** size */                                            \
	);                                                                          \
	                                                                            \
	/** event triggered on window close */                                      \
	void      (*on_close)                (                                      \
		struct _gia_win* /** self */                                            \
	);                                                                          \
	                                                                            \
	/** event triggered on window destroy */                                    \
	void      (*on_destroy)              (                                      \
		struct _gia_win* /** self */                                            \
	);                                                                          \
	                                                                            \
	/** dialog event triggered on dialog command */                             \
    ia_bool_t (*on_dialog_command)       (                                      \
		struct _gia_win*, /** self */                                           \
		struct _gia_win*  /** widget window producing the event */              \
	);                                                                          \
	                                                                            \
	/** draw bitmap */                                                          \
	void      (*draw_bitmap)             (                                      \
		struct _gia_win*, /** self */                                           \
		void*,            /** bitmap created with create_bitmap */              \
		ia_int32_t,       /** bitmap width  */                                  \
		ia_int32_t,       /** bitmap height */                                  \
		ia_int32_t,       /** destination x */                                  \
		ia_int32_t,       /** destination y */                                  \
		ia_float_t,       /** scale factor x */                                 \
		ia_float_t        /** scale factor u */                                 \
	);                                                                          \
	                                                                            \
	/** draw image */                                                           \
	void      (*draw_image)              (                                      \
		struct _gia_win*, /** self */                                           \
		ia_image_p,       /** image to draw */                                  \
		ia_int32_t,       /** destination x */                                  \
		ia_int32_t,       /** destination y */                                  \
		ia_float_t,       /** scale factor x */                                 \
		ia_float_t        /** scale factor y */                                 \
	);                                                                          \
                                                                                \
	void      (*draw_pixel)              (                                      \
		struct _gia_win*, /** self           */                                 \
		ia_int32_t,       /** x              */                                 \
		ia_int32_t,       /** y              */                                 \
		ia_uint32_t       /** color          */                                 \
    );                                                                          \
                                                                                \
	void      (*line_move_to)            (                                      \
		struct _gia_win*, /** self           */                                 \
		ia_int32_t,       /** x              */                                 \
		ia_int32_t        /** y              */                                 \
    );                                                                          \
                                                                                \
	void      (*line_draw_to)            (                                      \
		struct _gia_win*, /** self           */                                 \
		ia_int32_t,       /** x              */                                 \
		ia_int32_t,       /** y              */                                 \
		ia_uint32_t       /** color          */                                 \
    );                                                                          \
                                                                                \
	void      (*draw_rect)               (                                      \
		struct _gia_win*, /** self           */                                 \
		ia_int32_t,       /** x              */                                 \
		ia_int32_t,       /** y              */                                 \
		ia_int32_t,       /** w              */                                 \
		ia_int32_t,       /** h              */                                 \
		ia_uint32_t       /** color          */                                 \
    );                                                                          \
                                                                                \
	void      (*draw_text)               (                                      \
		struct _gia_win*, /** self           */                                 \
		ia_int32_t,       /** x              */                                 \
		ia_int32_t,       /** y              */                                 \
		ia_string_t,      /** text           */                                 \
		ia_int32_t        /** size           */                                 \
    );                                                                          \
                                                                                \
	/** creates device dependent bitmap */                                      \
	void*     (*bitmap_create)           (                                      \
		struct _gia_win*, /** self */                                           \
		ia_image_p        /** image to convert to device dependent bitmap */    \
	);                                                                          \
                                                                                \
	/** destroys device dependent bitmap, created by bitmap_create */           \
	void      (*bitmap_destroy)          (                                      \
		struct _gia_win*, /** self */                                           \
		void*             /** bitmap object */                                  \
	);                                                                          \
	                                                                            \
	/** set window position and size */                                         \
	void      (*set_rect)                (                                      \
		struct _gia_win*, /** self */                                           \
		ia_rect_p         /** left or x1, top or y1, right or x2, bottom or y2 */\
	);                                                                          \
                                                                                \
	/** get window position and size */                                         \
	void      (*get_rect)                (                                      \
		struct _gia_win*, /** self */                                           \
		ia_rect_p         /** client area rectangle */                          \
	);                                                                          \
                                                                                \
	/** get window position and size */                                         \
	void      (*get_window_rect)         (                                      \
		struct _gia_win*, /** self */                                           \
		ia_rect_p         /** window rectangle */                               \
	);                                                                          \
	                                                                            \
	/** add button */                                                           \
	void      (*add_button)              (                                      \
		struct _gia_win*, /** self */                                           \
		ia_rect_p,       /** upper-left and lower-right corners of the button */\
		ia_string_t,      /** button title */                                   \
		command_handler_t /** command handler for the button activations */     \
	);                                                                          \
                                                                                \
	/** add edit field */                                                       \
	void      (*add_edit)                (                                      \
		struct _gia_win*, /** self */                                           \
		ia_rect_p,       /** upper-left and lower-right corners of the edit field */\
		ia_string_t,      /** edit field title */                               \
		command_handler_t /** command handler for the edit field activations */ \
	);                                                                          \
                                                                                \
	/** add label */                                                            \
	void      (*add_label)               (                                      \
		struct _gia_win*, /** self */                                           \
		ia_rect_p,       /** upper-left and lower-right corners of the label */\
		ia_string_t,      /** label title */                                    \
		command_handler_t /** command handler for the label activations */      \
	);                                                                          \
	                                                                            \
	/** show file chooser dialog */                                             \
	ia_bool_t (*dialog_file)             (                                      \
		struct _gia_win*, /** self */                                           \
		const ia_string_t,/** files filter */                                   \
		ia_string_t,      /** file name */                                      \
		ia_uint32_t,      /** file name size */                                 \
		ia_bool_t         /** save (true) or open file (false) */               \
	);                                                                          \
	                                                                            \
	/** creates a child dialog  */                                              \
	struct _gia_win* (*dialog_new)       (                                      \
		struct _gia_win*, /** self */                                           \
		const ia_string_t /** resource ID specific to the target toolkit */     \
	);                                                                          \
	                                                                            \
	/** creates a child dialogs from template */                                \
	struct _gia_win* (*dialog_new_template)(                                    \
		struct _gia_win*, /** self */                                           \
		gia_dialog_template_p /** dialog template */                            \
	);                                                                          \
                                                                                \
	/** return dialog item by item ID */                                        \
    struct _gia_win* (*get_dialog_item)  (                                      \
		struct _gia_win*, /** self */                                           \
		ia_uint32_t       /** item identificator */                             \
	);                                                                          \
                                                                                \
	/** gets current window item id */                                          \
	ia_int32_t (*get_control_id)         (                                      \
		struct _gia_win*  /** self */                                           \
	);                                                                          \
                                                                                \
	/** creates a child window */                                               \
	struct _gia_win* (*window_child)     (                                      \
		struct _gia_win*,  /** self */                                          \
		ia_int32_t,        /** left position */                                 \
		ia_int32_t,        /** top position */                                  \
		ia_int32_t,        /** window width */                                  \
		ia_int32_t,        /** window height */                                 \
		gia_menu_popup_p*, /** window menu */                                   \
		ia_uint32_t        /** window style */                                  \
	);                                                                          \
	                                                                            \
	void      (*message)                 (                                      \
		struct _gia_win*,  /** self */                                          \
		ia_string_t        /** message text */                                  \
	);                                                                          \
	                                                                            \
	/** quit the application */                                                 \
	void      (*quit)                    (                                      \
	);                                                                          \
	                                                                            \
	/** destroy this window */                                                  \
	void      (*destroy)                 (                                      \
		struct _gia_win* /** self */                                            \
	);

struct _gia_app;
typedef struct _gia_win
{
    _GIA_WIN_CLASS
} gia_win_t, *gia_win_p;

/**
	Type gia_button_t

	Defines Button control
*/
typedef struct _gia_button
{
	_GIA_WIN_CLASS

	ia_bool_t (*is_checked) (
		struct _gia_button* /** self */
	);

	void (*check)           (
		struct _gia_button*, /** self */
		ia_bool_t            /** check state */
	);

} gia_button_t, *gia_button_p;

/**
	Type gia_slidebar_t

	Defines Slide Bar control
*/
typedef struct _gia_slidebar
{
	_GIA_WIN_CLASS

	/** Set the minimum and maximum positions for the slider */
	void (*set_minmax)   (
		struct _gia_slidebar*, /** self */
		ia_pos_p               /** new maximum and minimum positions */
	);

	/** Get the minimum and maximum positions of the slider */
	void (*get_minmax)   (
		struct _gia_slidebar*, /** self */
		ia_pos_p               /** new maximum and minimum positions */
	);

	/** Set current position for the slider */
	void (*set_pos)      (
		struct _gia_slidebar*, /** self */
		ia_int32_t             /** new position */
	);

	/** Get current position of the slider */
	ia_int32_t (*get_pos)(
		struct _gia_slidebar*  /** self */
	);

} gia_slidebar_t, *gia_slidebar_p;
/**
	Type gia_progressbar_t

	Defines Progress Bar control
*/
typedef struct _gia_progressbar
{
	_GIA_WIN_CLASS

	/** Set the minimum and maximum positions for the progress bar */
	void (*set_minmax)   (
		struct _gia_progressbar*, /** self */
		ia_pos_p                  /** new maximum and minimum positions */
	);

	/** Get the minimum and maximum positions of the progress bar */
	void (*get_minmax)   (
		struct _gia_progressbar*, /** self */
		ia_pos_p                  /** new maximum and minimum positions */
	);

	/** Set current position for the slider */
	void (*set_pos)      (
		struct _gia_progressbar*, /** self */
		ia_int32_t                /** new position */
	);

	/** Get current position of the slider */
	ia_int32_t (*get_pos)(
		struct _gia_progressbar*  /** self */
	);

} gia_progressbar_t, *gia_progressbar_p;

typedef struct _gia_app
{
	/** application name */
	ia_string_t             app_name;
	
	/** main application window */
	gia_win_p               main_window;

	/** set main window */
	void (*set_main_window) (
		struct _gia_app*, /** self */
		gia_win_p         /** window */
	);

	/** get main window */
	gia_win_p (*get_main_window) (
		struct _gia_app*  /** self */
	);

	/** main event loop */
	void (*run)             (
	);

	/** call when the app is idle */
	void (*idle)            (
	);
	
	/** destroy application object */
	void (*destroy)         (
		struct _gia_app*  /** self */
	);

	/** create new window */
	gia_win_p (*window_new) (
		struct _gia_app*, /** self */
		ia_int32_t,       /** left position */
		ia_int32_t,       /** top position */
		ia_int32_t,       /** window width */
		ia_int32_t,       /** window height */
		gia_win_p,        /** parent window */
		gia_menu_popup_p*,/** window menu */
		ia_uint32_t,      /** window style */
		const ia_string_t /** resource ID specific to the target toolkit */
	);

} gia_app_t, *gia_app_p;

/** returns the bits number of the given variable type */
IA_API ia_uint16_t ia_format_size (
	ia_format_t /** format type             */
);

/** returns format name as string */
IA_API const ia_string_t ia_format_name (
	ia_format_t /** format type             */
);

/** returns true if the the given format is signed */
IA_API ia_bool_t ia_format_signed (
	ia_format_t /** format type             */
);

/** returns the min and max possible values for the specified format */
IA_API void ia_format_min_max (
	ia_format_t,  /** format type           */
	ia_int32_t*,  /** returned min value    */
	ia_uint32_t*  /** returned max value    */
);

/** creates a new popup menu                */
IA_API gia_menu_popup_p gia_menu_popup_new(
	const ia_string_t /** menu title         */
);

#ifdef WIN32

	/** creates application for Win32 toolkit */
	IA_API gia_app_p gia_winapi_app_new(
			const ia_string_t /** application name */
	);

	/** Application generator function */
	#define gia_app_new gia_winapi_app_new
#endif

/** Converts toolkit specific object to type gia_win_p */
IA_API gia_win_p         gia_win_cast      (void*);
IA_API gia_slidebar_p    gia_slider_cast   (void*);
IA_API gia_progressbar_p gia_progress_cast (void*);
IA_API gia_win_p         gia_auto_cast     (void*);

#endif /* __GIA_H */
