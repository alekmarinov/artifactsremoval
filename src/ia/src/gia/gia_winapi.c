#ifdef WIN32

#include <windows.h>
#include <windowsx.h>
#include <winuser.h>
#include <shellapi.h>
#include <commdlg.h>
#include <commctrl.h>
#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <ia/gia/gia.h>

#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL                                0x020A
#define WHEEL_DELTA                                  120
#endif

#define DEFAULT_BUTTON_W                             70
#define DEFAULT_BUTTON_H                             30

const char* cls_slidebar                             = "msctls_trackbar32";
const char* cls_progressbar                          = "msctls_progress32";
const char* cls_button                               = "Button";

gia_app_p         gia_winapi_app_new                 (const ia_string_t);
gia_menu_popup_p  gia_menu_popup_new                 (const ia_string_t);
gia_control_template_p gia_control_template_new      (gia_control_t, const ia_string_t, ia_uint32_t, ia_int32_t, ia_int32_t, ia_int32_t, ia_int32_t, ia_int32_t id);
static void       gia_control_template_destroy       (struct _gia_control_template*);
gia_dialog_template_p gia_dialog_template_new        (const ia_string_t, ia_uint32_t, ia_int32_t, ia_int32_t, ia_int32_t, ia_int32_t, ia_int32_t);
static void       gia_dialog_template_add            (struct _gia_dialog_template*, gia_control_template_p);
static void       gia_dialog_template_destroy        (struct _gia_dialog_template*);
static void       gia_menu_popup_add                 (struct _gia_menu_popup*, const ia_string_t, command_handler_t);
static void       gia_menu_popup_destroy             (struct _gia_menu_popup*);
static void       gia_draw_context_add               (struct _gia_draw_context*, gia_draw_primitive_p);
static void       gia_draw_context_add_pixel         (struct _gia_draw_context*, ia_int32_t,  ia_int32_t, ia_uint32_t);
static void       gia_draw_context_add_line          (struct _gia_draw_context*, ia_int32_t,  ia_int32_t, ia_int32_t, ia_int32_t, ia_uint32_t);
static void       gia_draw_context_add_rect          (struct _gia_draw_context*, ia_int32_t,  ia_int32_t, ia_int32_t, ia_int32_t, ia_uint32_t);
static void       gia_draw_context_add_image         (struct _gia_draw_context*, ia_image_p,  ia_int32_t, ia_int32_t, ia_float_t, ia_float_t);
static void       gia_draw_context_add_text          (struct _gia_draw_context*, ia_string_t, ia_int32_t, ia_int32_t, ia_uint32_t, ia_int32_t, ia_align_t);
static void       gia_draw_context_draw              (struct _gia_draw_context*, struct _gia_win*);
static void       gia_draw_context_destroy           (struct _gia_draw_context*);
static void       gia_winapi_run                     ( void );
static void       gia_winapi_idle                    ( void );
static void       gia_winapi_destroy                 (gia_app_p);
static void       gia_winapi_quit                    ( void );
static void       gia_winapi_set_main_window         ( gia_app_p, gia_win_p );
static gia_win_p  gia_winapi_get_main_window         ( gia_app_p );
static gia_win_p  gia_winapi_window_new              (gia_app_p, ia_int32_t, ia_int32_t, ia_int32_t, ia_int32_t, gia_win_p, gia_menu_popup_p* menu, ia_uint32_t, const ia_string_t);
static gia_win_p  gia_winapi_window_child            (gia_win_p, ia_int32_t, ia_int32_t, ia_int32_t, ia_int32_t, gia_menu_popup_p* menu, ia_uint32_t);
static void       gia_winapi_window_message          (gia_win_p, ia_string_t);
static gia_win_p  gia_winapi_window_dialog_new       (gia_win_p, const ia_string_t);
static gia_win_p  gia_winapi_window_dialog_new_template(gia_win_p, gia_dialog_template_p);
static gia_win_p  gia_winapi_window_get_dialog_item  (gia_win_p, ia_uint32_t);
static ia_int32_t gia_winapi_window_get_control_id   (gia_win_p);
static void       gia_winapi_window_destroy          (gia_win_p);

static void       gia_winapi_window_set_title        (gia_win_p, const ia_string_t);
static const ia_string_t gia_winapi_window_get_title (gia_win_p);
static void       gia_winapi_window_set_visible      (gia_win_p, ia_bool_t);
static ia_bool_t  gia_winapi_window_is_visible       (gia_win_p);
static void       gia_winapi_window_set_enable       (gia_win_p, ia_bool_t);
static ia_bool_t  gia_winapi_window_is_enabled       (gia_win_p);
static void       gia_winapi_window_update           (gia_win_p);
static void       gia_winapi_window_redraw           (gia_win_p);
static ia_bool_t  gia_winapi_window_dialog_file      (gia_win_p self, const ia_string_t filter, ia_string_t file_name, ia_uint32_t file_name_size, ia_bool_t is_save);
static void       gia_winapi_window_draw_bitmap      (gia_win_p, void*, ia_int32_t, ia_int32_t, ia_int32_t, ia_int32_t, ia_float_t, ia_float_t);
static void       gia_winapi_window_draw_image       (gia_win_p, ia_image_p, ia_int32_t, ia_int32_t, ia_float_t, ia_float_t);
static void       gia_winapi_window_draw_pixel       (gia_win_p, ia_int32_t, ia_int32_t, ia_uint32_t);
static void       gia_winapi_window_line_move_to     (gia_win_p, ia_int32_t, ia_int32_t);
static void       gia_winapi_window_line_draw_to     (gia_win_p, ia_int32_t, ia_int32_t, ia_uint32_t);
static void       gia_winapi_window_draw_rect        (gia_win_p, ia_int32_t, ia_int32_t, ia_int32_t, ia_int32_t, ia_uint32_t);
static void       gia_winapi_window_draw_text        (gia_win_p, ia_int32_t, ia_int32_t, ia_string_t, ia_int32_t);
static void*      gia_winapi_window_bitmap_create    (gia_win_p, ia_image_p);
static void       gia_winapi_window_bitmap_destroy   (gia_win_p, void*);
static void       gia_winapi_window_set_rect         (gia_win_p, ia_rect_p);
static void       gia_winapi_window_get_rect         (gia_win_p, ia_rect_p);
static void       gia_winapi_window_get_window_rect  (gia_win_p, ia_rect_p);
static void       gia_winapi_window_add_button       (gia_win_p, ia_rect_p, ia_string_t, command_handler_t);
static void       gia_winapi_window_add_edit         (gia_win_p, ia_rect_p, ia_string_t, command_handler_t);
static void       gia_winapi_window_add_label        (gia_win_p, ia_rect_p, ia_string_t, command_handler_t);

static ia_bool_t  gia_winapi_button_is_checked       (gia_button_p);
static void       gia_winapi_button_check            (gia_button_p, ia_bool_t);

static void       gia_winapi_slidebar_set_minmax     (gia_slidebar_p, ia_pos_p);
static void       gia_winapi_slidebar_get_minmax     (gia_slidebar_p, ia_pos_p);
static void       gia_winapi_slidebar_set_pos        (gia_slidebar_p, ia_int32_t);
static ia_int32_t gia_winapi_slidebar_get_pos        (gia_slidebar_p);

static void       gia_winapi_progressbar_set_minmax  (gia_progressbar_p, ia_pos_p);
static void       gia_winapi_progressbar_get_minmax  (gia_progressbar_p, ia_pos_p);
static void       gia_winapi_progressbar_set_pos     (gia_progressbar_p, ia_int32_t);
static ia_int32_t gia_winapi_progressbar_get_pos     (gia_progressbar_p);

static void       gia_winapi_register_class          (gia_app_p, HINSTANCE);
static LRESULT    gia_winapi_wnd_proc                (HWND, UINT, WPARAM, LPARAM);
static BOOL       gia_winapi_dlg_proc                (HWND, UINT, WPARAM, LPARAM);
static void       ErrorExit                          (LPTSTR lpszFunction);

gia_draw_context_p gia_draw_context_new( void )
{
	gia_draw_context_p draw_context = (gia_draw_context_p)malloc(sizeof(gia_draw_context_t));

	draw_context->primitives        = 0;
	draw_context->nprimitives       = 0;

	draw_context->add               = gia_draw_context_add;
	draw_context->add_pixel         = gia_draw_context_add_pixel;
	draw_context->add_line          = gia_draw_context_add_line;
	draw_context->add_rect          = gia_draw_context_add_rect;
	draw_context->add_image         = gia_draw_context_add_image;
	draw_context->add_text          = gia_draw_context_add_text;
	draw_context->draw              = gia_draw_context_draw;
	draw_context->destroy           = gia_draw_context_destroy;

	return draw_context;
}

static void gia_draw_context_add(struct _gia_draw_context* self, gia_draw_primitive_p primitive)
{
	self->primitives=(gia_draw_primitive_p*)realloc(self->primitives, (self->nprimitives+1) * sizeof(gia_draw_primitive_p));
	self->primitives[self->nprimitives++]=primitive;
}

static void gia_draw_context_add_pixel(struct _gia_draw_context* self, ia_int32_t x, ia_int32_t y, ia_uint32_t color)
{
	gia_draw_primitive_p primitive=(gia_draw_primitive_p)malloc(sizeof(gia_draw_primitive_t));
	primitive->draw_operator=DRAW_PIXEL;
	primitive->x=x;
	primitive->y=y;
	primitive->color=color;
	self->add(self, primitive);
}

static void gia_draw_context_add_line(struct _gia_draw_context* self, ia_int32_t x1, ia_int32_t y1, ia_int32_t x2, ia_int32_t y2, ia_uint32_t color)
{
	gia_draw_primitive_p primitive=(gia_draw_primitive_p)malloc(sizeof(gia_draw_primitive_t));
	primitive->draw_operator=DRAW_LINE;
	primitive->x=x1;
	primitive->y=y1;
	primitive->w.w=x2;
	primitive->h.h=y2;
	primitive->color=color;
	self->add(self, primitive);
}

static void gia_draw_context_add_rect(struct _gia_draw_context* self, ia_int32_t x, ia_int32_t y, ia_int32_t w, ia_int32_t h, ia_uint32_t color)
{
	gia_draw_primitive_p primitive=(gia_draw_primitive_p)malloc(sizeof(gia_draw_primitive_t));
	primitive->draw_operator=DRAW_RECT;
	primitive->x=x;
	primitive->y=y;
	primitive->w.w=w;
	primitive->h.h=h;
	primitive->color=color;
	self->add(self, primitive);
}

static void gia_draw_context_add_image(struct _gia_draw_context* self, ia_image_p image, ia_int32_t x, ia_int32_t y, ia_float_t scale_x, ia_float_t scale_y)
{
	gia_draw_primitive_p primitive=(gia_draw_primitive_p)malloc(sizeof(gia_draw_primitive_t));
	primitive->draw_operator=DRAW_IMAGE;
	primitive->x=x;
	primitive->y=y;
	primitive->w.scale_x=scale_x;
	primitive->h.scale_y=scale_y;
	primitive->d.image=image;
	self->add(self, primitive);
}

static void gia_draw_context_add_text(struct _gia_draw_context* self, ia_string_t text, ia_int32_t x, ia_int32_t y, ia_uint32_t color, ia_int32_t size, ia_align_t align)
{
	gia_draw_primitive_p primitive=(gia_draw_primitive_p)malloc(sizeof(gia_draw_primitive_t));
	primitive->draw_operator=DRAW_TEXT;
	primitive->x=x;
	primitive->y=y;
	primitive->color=color;
	primitive->d.t.text=strdup(text);
	primitive->d.t.size=size;
	primitive->d.t.align=align;
	self->add(self, primitive);
}

static void gia_draw_context_draw(struct _gia_draw_context* self, struct _gia_win* win)
{
	RECT   rect;
	HWND   wnd_h  = (HWND)win->impl;
	HDC    dc_h   = GetDC(wnd_h);
	HPEN   pen    = 0;
	HBRUSH brush  = 0;
	HFONT  font   = 0;
	ia_uint32_t last_pen_color   = -1;
	ia_uint32_t last_brush_color = -1;
	ia_uint32_t last_font_size   = -1;
	ia_int32_t  i;
	POINT  point;
	LOGBRUSH logbrush;

	for (i=0; i<self->nprimitives; i++)
	{
		gia_draw_primitive_p primitive=self->primitives[i];
		switch (primitive->draw_operator)
		{
		case DRAW_PIXEL:
				SetPixel(dc_h, primitive->x, primitive->y, primitive->color);
			break;
		case DRAW_LINE:
			if (!pen || primitive->color!=last_pen_color)
			{
				if (pen) DeleteObject(pen);
				pen=CreatePen(PS_SOLID, 1, primitive->color);
				SelectObject(dc_h, pen);
				last_pen_color=primitive->color;
			}
			MoveToEx(dc_h, primitive->x, primitive->y, &point);
			LineTo(dc_h, primitive->w.w, primitive->h.h);
			break;
		case DRAW_RECT:
			if (!brush || primitive->color!=last_brush_color)
			{
				if (brush) DeleteObject(brush);
				logbrush.lbStyle=BS_SOLID;
				logbrush.lbColor=primitive->color;
				logbrush.lbHatch=0;
				brush=CreateBrushIndirect(&logbrush);
				last_brush_color=primitive->color;
			}
			rect.left=primitive->x;
			rect.top=primitive->y;
			rect.right=primitive->x+primitive->w.w;
			rect.bottom=primitive->y+primitive->h.h;
			FillRect(dc_h, &rect, brush);
			ValidateRect(wnd_h, &rect);
			break;
		case DRAW_IMAGE:
			win->draw_image(win, primitive->d.image, primitive->x, primitive->y, primitive->w.scale_x, primitive->h.scale_y);
			break;
		case DRAW_TEXT:
			if (!font || primitive->d.t.size!=last_font_size)
			{
				if (font) DeleteObject(font);
				font=CreateFont(
						primitive->d.t.size, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
						CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, "Arial");

				last_font_size=primitive->d.t.size;
			}
			SetBkMode(dc_h, TRANSPARENT);
			SelectObject(dc_h, font);
			SetTextColor(dc_h, primitive->color);
			switch (primitive->d.t.align)
			{
			case IA_ALIGN_LEFT: SetTextAlign(dc_h, TA_LEFT); break;
			case IA_ALIGN_RIGHT: SetTextAlign(dc_h, TA_RIGHT); break;
			case IA_ALIGN_CENTER: SetTextAlign(dc_h, TA_CENTER); break;
			}
			
			TextOut(dc_h, primitive->x, primitive->y, primitive->d.t.text, (int)strlen(primitive->d.t.text));
			break;
		}
	}

	if (brush) DeleteObject(brush);
	if (pen) DeleteObject(pen);
	if (font) DeleteObject(font);
	ReleaseDC(wnd_h, dc_h);
}

static void gia_draw_context_destroy(struct _gia_draw_context* self)
{
	ia_int32_t i;
	for (i=0; i<self->nprimitives; i++)
	{
		gia_draw_primitive_p primitive=self->primitives[i];
		if (primitive->draw_operator == DRAW_TEXT)
		{
			free(primitive->d.t.text);
		}
		free(primitive);
	}
	free(self);
}

static gia_win_p         g_gia_win_caster;
static gia_button_p      g_gia_button_caster;
static gia_slidebar_p    g_gia_slider_caster;
static gia_progressbar_p g_gia_progress_caster;


gia_win_p gia_win_cast(void* impl)
{
	g_gia_win_caster->impl = impl;
	return g_gia_win_caster;
}

gia_button_p gia_button_cast(void* impl)
{
	g_gia_button_caster->impl = impl;
	return g_gia_button_caster;
}

gia_slidebar_p gia_slider_cast(void* impl)
{
	g_gia_slider_caster->impl = impl;
	return g_gia_slider_caster;
}

gia_progressbar_p gia_progress_cast(void* impl)
{
	g_gia_progress_caster->impl = impl;
	return g_gia_progress_caster;
}

gia_win_p gia_auto_cast(void* impl)
{
	char cls_name[100];
	HWND wnd_h=(HWND)impl;
	GetClassName(wnd_h, cls_name, 100);
	printf("autocast %s\n", cls_name);
	if (!strcmp(cls_slidebar, cls_name))
	{
		return (gia_win_p)gia_slider_cast(impl);
	}
	if (!strcmp(cls_progressbar, cls_name))
	{
		return (gia_win_p)gia_progress_cast(impl);
	}
	else if (!strcmp(cls_button, cls_name))
	{
		return (gia_win_p)gia_button_cast(impl);
	}
	return gia_win_cast(impl);
}

void gia_win_init( gia_win_p win )
{
	win->impl              = 0;
	win->type              = GIA_CTL_WINDOW;
	win->app               = 0;
	win->title             = 0;
	win->cmd_handlers      = 0;
	win->nhandlers         = 0;

	win->quit              = gia_winapi_quit;
	win->window_child      = gia_winapi_window_child;
	win->message           = gia_winapi_window_message;	
	win->dialog_new        = gia_winapi_window_dialog_new;
	win->dialog_new_template= gia_winapi_window_dialog_new_template;
	win->destroy           = gia_winapi_window_destroy;
	win->set_title         = gia_winapi_window_set_title;
	win->get_title         = gia_winapi_window_get_title;
	win->set_visible       = gia_winapi_window_set_visible;
	win->is_visible        = gia_winapi_window_is_visible;
	win->set_enable        = gia_winapi_window_set_enable;
	win->is_enabled        = gia_winapi_window_is_enabled;
	win->is_visible        = gia_winapi_window_is_visible;
	win->update            = gia_winapi_window_update;
	win->redraw            = gia_winapi_window_redraw;
	win->get_dialog_item   = gia_winapi_window_get_dialog_item;
	win->get_control_id    = gia_winapi_window_get_control_id;
	win->dialog_file       = gia_winapi_window_dialog_file;
	win->draw_bitmap       = gia_winapi_window_draw_bitmap;
	win->draw_image        = gia_winapi_window_draw_image;
	win->draw_pixel        = gia_winapi_window_draw_pixel;
	win->line_move_to      = gia_winapi_window_line_move_to;
	win->line_draw_to      = gia_winapi_window_line_draw_to;
	win->draw_rect         = gia_winapi_window_draw_rect;
	win->draw_text         = gia_winapi_window_draw_text;
	win->bitmap_create     = gia_winapi_window_bitmap_create;
	win->bitmap_destroy    = gia_winapi_window_bitmap_destroy;
	win->set_rect          = gia_winapi_window_set_rect;
	win->get_rect          = gia_winapi_window_get_rect;
	win->get_window_rect   = gia_winapi_window_get_window_rect;
	win->add_button        = gia_winapi_window_add_button;
	win->add_edit          = gia_winapi_window_add_edit;
	win->add_label         = gia_winapi_window_add_label;

	win->on_paint          = 0;
	win->on_mousewheel     = 0;
	win->on_mouseclick     = 0;
	win->on_mousemove      = 0;
	win->on_close          = 0;
	win->on_destroy        = 0;
	win->on_dialog_command = 0;
}

gia_win_p gia_win_alloc( void )
{
	gia_win_p win          = (gia_win_p)malloc(sizeof(gia_win_t));
    gia_win_init(win);
    return win;
}

gia_button_p gia_button_alloc( void )
{
	gia_button_p button     = (gia_button_p)malloc(sizeof(gia_button_t));
	gia_win_init((gia_win_p)button);

	button->type              = GIA_CTL_BUTTON;
	button->is_checked        = gia_winapi_button_is_checked;
	button->check             = gia_winapi_button_check;

	return button;
}

gia_slidebar_p gia_slidebar_alloc( void )
{
	gia_slidebar_p slider     = (gia_slidebar_p)malloc(sizeof(gia_slidebar_t));
	gia_win_init((gia_win_p)slider);

	slider->type              = GIA_CTL_SLIDEBAR;
	slider->set_minmax        = gia_winapi_slidebar_set_minmax;
	slider->get_minmax        = gia_winapi_slidebar_get_minmax;
	slider->set_pos           = gia_winapi_slidebar_set_pos;
	slider->get_pos           = gia_winapi_slidebar_get_pos;
	return slider;
}

gia_progressbar_p gia_progressbar_alloc( void )
{
	gia_progressbar_p progress     = (gia_progressbar_p)malloc(sizeof(gia_progressbar_t));
	gia_win_init((gia_win_p)progress);

	progress->type              = GIA_CTL_PROGRESSBAR;
	progress->set_minmax        = gia_winapi_progressbar_set_minmax;
	progress->get_minmax        = gia_winapi_progressbar_get_minmax;
	progress->set_pos           = gia_winapi_progressbar_set_pos;
	progress->get_pos           = gia_winapi_progressbar_get_pos;
	return progress;
}

gia_app_p gia_winapi_app_new(const ia_string_t app_name)
{
	gia_app_p app=(gia_app_p)malloc(sizeof(gia_app_t));

	app->app_name        = app_name;
	app->main_window     = 0;

	app->set_main_window = gia_winapi_set_main_window;
	app->get_main_window = gia_winapi_get_main_window;
	app->run             = gia_winapi_run;
	app->idle            = gia_winapi_idle;
	app->destroy         = gia_winapi_destroy;
	app->window_new      = gia_winapi_window_new;

	g_gia_win_caster     = gia_win_alloc();
	g_gia_slider_caster  = gia_slidebar_alloc();
	g_gia_progress_caster= gia_progressbar_alloc();
	g_gia_button_caster  = gia_button_alloc();

	gia_winapi_register_class(app, GetModuleHandle(NULL));

	return app;
}

static void gia_winapi_run( void )
{
	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

static void gia_winapi_idle( void )
{	
	MSG msg;
	GetMessage(&msg, NULL, 0, 0);
	TranslateMessage(&msg);
	DispatchMessage(&msg);
}

static void gia_winapi_destroy(gia_app_p self)
{
	if (self->main_window)
	{
		self->main_window->destroy(self->main_window);
	}
	free(self);
}

static void gia_winapi_make_windows_styles(gia_control_t control_type, ia_uint32_t options, ia_uint32_t* style, ia_uint32_t* style_ex)
{
	*style=*style_ex=0;

	if (!options && control_type == GIA_CTL_WINDOW)
	{
		options=IAS_SYSMENU | IAS_THICKFRAME | IAS_MAXIMIZE | IAS_MINIMIZE;
		*style = WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	}

	if (options & IAS_CAPTION)    *style    |= WS_CAPTION;
	if (options & IAS_MINIMIZE)   *style    |= (WS_MINIMIZE | WS_MINIMIZEBOX);
	if (options & IAS_MAXIMIZE)   *style    |= (WS_MAXIMIZE | WS_MAXIMIZEBOX);
	if (options & IAS_SYSMENU)    *style    |= WS_SYSMENU;
	if (options & IAS_BORDER)     *style    |= WS_BORDER;
	if (options & IAS_MODAL)      *style_ex |= (WS_EX_DLGMODALFRAME | WS_EX_CONTROLPARENT);
	if (options & IAS_THICKFRAME) *style    |= WS_THICKFRAME;
	if (options & IAS_TOPMOST)    *style_ex |= WS_EX_TOPMOST;
	if (options & IAS_CHILD)      *style    |= WS_CHILD;
	if (options & IAS_GROUP)      *style    |= WS_GROUP;
	if (options & IAS_TABSTOP)    *style    |= WS_TABSTOP;
	if (options & IABS_CHECKBOX)  *style    |= BS_AUTOCHECKBOX;

	switch (control_type)
	{
		case GIA_CTL_BUTTON:
				*style |= WS_VISIBLE;
				if (!(options & IABS_CHECKBOX)) *style |= BS_PUSHBUTTON; 
				if (options & IAS_LEFT)         *style |= BS_LEFT; 
				if (options & IAS_RIGHT)        *style |= BS_RIGHT; 
				if (options & IABS_GROUPBOX)    *style |= WS_TABSTOP | BS_GROUPBOX; 
		break;
		case GIA_CTL_EDIT:        *style |= WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_MULTILINE | ES_LEFT; break;
		case GIA_CTL_LABEL:       *style |= WS_CHILD | WS_VISIBLE | SS_LEFT; break;
		case GIA_CTL_SLIDEBAR:    *style |= WS_CHILD | WS_VISIBLE | TBS_BOTH | TBS_NOTICKS | TBS_ENABLESELRANGE; break;
		case GIA_CTL_PROGRESSBAR: *style |= WS_CHILD | WS_VISIBLE; break;
		case GIA_CTL_LISTBOX:     *style |= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON; break;
		case GIA_CTL_SCROLLBAR:   *style |= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON; break;
		case GIA_CTL_COMBOBOX:    *style |= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON; break;
	}
}

static gia_win_p gia_winapi_window_new(
				gia_app_p self, 
				ia_int32_t x, 
				ia_int32_t y, 
				ia_int32_t w, 
				ia_int32_t h, 
				gia_win_p parent, 
				gia_menu_popup_p* menu, 
				ia_uint32_t options, 
				const ia_string_t resource_ID)
{
	HWND wnd_h;
	HMENU menu_h=0;
	ia_uint32_t style=0, style_ex=0;
	command_handler_t* command_handlers=0;
	ia_string_t class_name=self->app_name;
	ia_uint32_t nhandlers=0;

	if (menu)
	{
		HMENU sub_menu_h;
		menu_h=CreateMenu();
		while (*menu)
		{
			gia_menu_popup_p pmenu=*menu;
			ia_uint32_t i;
			sub_menu_h=CreatePopupMenu();
			command_handlers=(command_handler_t*)realloc(command_handlers, (nhandlers+pmenu->nitems)*sizeof(command_handler_t));
			for (i=0; i<pmenu->nitems; i++)
			{
				AppendMenu(sub_menu_h, MF_STRING, nhandlers, pmenu->items[i].caption);
				command_handlers[nhandlers++]=pmenu->items[i].command;
			}
			AppendMenu(menu_h, MF_POPUP, (UINT_PTR)sub_menu_h, pmenu->caption);
			pmenu->destroy(pmenu);
			menu++;
		}
	}

	if (options & IAS_DIALOG) 
	{
		wnd_h = CreateDialog(GetModuleHandle(NULL), 
                         MAKEINTRESOURCE(resource_ID), 
                         parent?(HWND)parent->impl:0, (DLGPROC) gia_winapi_dlg_proc); 
	}
	else
	{
		gia_winapi_make_windows_styles(GIA_CTL_WINDOW, options, &style, &style_ex);

		wnd_h = CreateWindowEx(style_ex, class_name, "", style,
			x?x:CW_USEDEFAULT, y?y:CW_USEDEFAULT, w?w:CW_USEDEFAULT, h?h:CW_USEDEFAULT, parent?(HWND)parent->impl:0, menu_h, GetModuleHandle(NULL), NULL);
	}

	if (wnd_h)
	{
		gia_win_p win     = gia_win_alloc();
		win->impl         = wnd_h;
		win->app          = self;
		win->cmd_handlers = command_handlers;
		win->nhandlers    = nhandlers;

		SetWindowLong(wnd_h, GWL_USERDATA, (LONG)win);

		return win;
	}
	return 0;
}

static void gia_winapi_quit( void )
{
	PostQuitMessage(0);
}

static void gia_winapi_set_main_window( gia_app_p self, gia_win_p win)
{
	self->main_window=win;
}

static gia_win_p gia_winapi_get_main_window( gia_app_p self )
{
	return self->main_window;
}

static gia_win_p gia_winapi_window_child(gia_win_p self, ia_int32_t x, ia_int32_t y, ia_int32_t w, ia_int32_t h, gia_menu_popup_p* menu, ia_uint32_t options)
{
	return self->app->window_new(self->app, x, y, w, h, self, menu, options, NULL);
}

static void gia_winapi_window_message(gia_win_p self, ia_string_t message)
{
	self->set_visible(self, 0);
	MessageBox((HWND)self->impl, message, "Message...", MB_OK | MB_ICONINFORMATION | MB_SYSTEMMODAL);
	self->set_visible(self, 1);
}

static gia_win_p gia_winapi_window_dialog_new(gia_win_p self, const ia_string_t resource_id)
{
	return self->app->window_new(self->app, 0, 0, 0, 0, self, 0, IAS_DIALOG, resource_id);
}

static gia_win_p gia_winapi_window_dialog_new_template(gia_win_p self, gia_dialog_template_p dialog_template)
{
	HWND wnd_h;
	HLOCAL hGlobal;
	DLGTEMPLATE dlg;
	DLGITEMTEMPLATE dlg_item;
	WCHAR*  pchCaption;
	ia_uint8_t* pdest, *pBuffer;
	ia_uint32_t style, style_ex;
	ia_int32_t title_size;
	ia_int32_t n_item;
	ia_int32_t nBufferSize, nItemLength;
	ia_int32_t nChars, nActualChars;
	WCHAR szFontName[] = L"ARIAL";
	WCHAR szGroupBox[] = L"GROUPBOX";
	WCHAR szSlideBar[] = L"msctls_trackbar32";
	WCHAR szProgressBar[] = L"msctls_progress32";

	gia_winapi_make_windows_styles(GIA_CTL_WINDOW, dialog_template->style, &style, &style_ex);

	dlg.style           = DS_SETFONT | DS_FIXEDSYS | DS_CENTER | WS_VISIBLE | style;
	dlg.dwExtendedStyle = WS_EX_CONTROLPARENT;
	dlg.cdit            = dialog_template->ncontrols;
	dlg.x               = dialog_template->rect.l;
	dlg.y               = dialog_template->rect.t;
	dlg.cx              = dialog_template->rect.r;
	dlg.cy              = dialog_template->rect.b;

	nChars = (ia_int32_t)strlen(dialog_template->title) + 1;
	title_size=nChars * sizeof(WCHAR);
	pchCaption = (WCHAR*)malloc( title_size );
	nActualChars = MultiByteToWideChar(CP_ACP, 0, dialog_template->title, -1, pchCaption, nChars);
	if (nActualChars <= 0)
	{
		ErrorExit("gia_winapi_window_dialog_new_template");
	}
	nBufferSize = sizeof(DLGTEMPLATE) + (2 * sizeof(WORD))/*menu and class*/ + nActualChars * sizeof(WORD);
	nBufferSize += sizeof(WORD) + sizeof(szFontName); /* font information*/
	nBufferSize = (nBufferSize + 3) & ~3;  // adjust size to make first control DWORD aligned

	for (n_item=0; n_item<dialog_template->ncontrols; n_item++)
	{
		gia_control_template_p control=dialog_template->controls[n_item];
			
		nItemLength = sizeof(DLGITEMTEMPLATE) + sizeof(WORD);

		switch (control->control_type)
		{
		// atom value for the control
		case GIA_CTL_BUTTON:    
		case GIA_CTL_EDIT:      
		case GIA_CTL_LABEL:     
		case GIA_CTL_LISTBOX:   
		case GIA_CTL_SCROLLBAR: 
		case GIA_CTL_COMBOBOX:  
			nItemLength+=2*sizeof(WORD); break;
		case GIA_CTL_GROUPBOX:  nItemLength+=sizeof(szGroupBox); break;
		case GIA_CTL_SLIDEBAR:  nItemLength+=sizeof(szSlideBar); break;
		case GIA_CTL_PROGRESSBAR: nItemLength+=sizeof(szProgressBar); break;
		}

		nItemLength += (ia_int32_t)(strlen(control->title)+1)*sizeof(WORD);
		if (n_item < dialog_template->ncontrols-1 )   // the last control does not need extra bytes
			nItemLength = (nItemLength + 3) & ~3;    // take into account gap so next control is DWORD aligned
		nBufferSize += nItemLength;
	}

	hGlobal  = GlobalAlloc(LHND, nBufferSize);
	pBuffer = (ia_uint8_t*)GlobalLock(hGlobal);
	pdest=pBuffer;

	memcpy(pdest, &dlg, sizeof(DLGTEMPLATE));
	pdest += sizeof(DLGTEMPLATE);
	*(WORD*)pdest = 0; // no menu
	*(WORD*)(pdest + sizeof(WORD)) = 0;  // use default window class
	pdest += 2 * sizeof(WORD);
	memcpy(pdest, pchCaption, nActualChars * sizeof(WCHAR));
	free(pchCaption);
	pdest += nActualChars * sizeof(WCHAR);
	*(WORD*)pdest = 11;  // font size
	pdest += sizeof(WORD);
	memcpy(pdest, szFontName, sizeof(szFontName));
	pdest += sizeof(szFontName);

	for (n_item=0; n_item<dialog_template->ncontrols; n_item++)
	{
		gia_control_template_p control=dialog_template->controls[n_item];
		gia_winapi_make_windows_styles(control->control_type, control->style, &style, &style_ex);

		pdest = (BYTE*)(((DWORD_PTR)pdest + 3) & ~3);  // make the pointer DWORD aligned

		dlg_item.dwExtendedStyle = style_ex;
		dlg_item.style           = style;
		dlg_item.x               = control->rect.l;
		dlg_item.y               = control->rect.t;
		dlg_item.cx              = control->rect.r;
		dlg_item.cy              = control->rect.b;
		dlg_item.id              = control->id;

		memcpy(pdest, (void *)&dlg_item, sizeof(DLGITEMTEMPLATE));
		pdest += sizeof(DLGITEMTEMPLATE);

		switch (control->control_type)
		{
		// atom value for the control
		case GIA_CTL_BUTTON:    *(DWORD*)pdest = 0x0080FFFF; ; pdest+=sizeof(DWORD); break;
		case GIA_CTL_EDIT:      *(DWORD*)pdest = 0x0081FFFF; ; pdest+=sizeof(DWORD); break;
		case GIA_CTL_LABEL:     *(DWORD*)pdest = 0x0082FFFF; ; pdest+=sizeof(DWORD); break;
		case GIA_CTL_LISTBOX:   *(DWORD*)pdest = 0x0083FFFF; ; pdest+=sizeof(DWORD); break;
		case GIA_CTL_SCROLLBAR: *(DWORD*)pdest = 0x0084FFFF; ; pdest+=sizeof(DWORD); break;
		case GIA_CTL_COMBOBOX:  *(DWORD*)pdest = 0x0085FFFF; ; pdest+=sizeof(DWORD); break;
		case GIA_CTL_GROUPBOX:     memcpy(pdest, szGroupBox, sizeof(szGroupBox)); pdest+=sizeof(szGroupBox); break;
		case GIA_CTL_SLIDEBAR:     memcpy(pdest, szSlideBar, sizeof(szSlideBar)); pdest+=sizeof(szSlideBar); break;
		case GIA_CTL_PROGRESSBAR:  memcpy(pdest, szProgressBar, sizeof(szProgressBar)); pdest+=sizeof(szProgressBar); break;
		}

		nChars = (ia_int32_t)strlen(control->title) + 1;
		title_size=nChars * sizeof(WCHAR);
		pchCaption = (WCHAR*)malloc( title_size );
		nActualChars = MultiByteToWideChar(CP_ACP, 0, control->title, -1, pchCaption, nChars);
		if (nActualChars <= 0)
		{
			ErrorExit("gia_winapi_window_dialog_new_template");
		}
		memcpy(pdest, pchCaption, nActualChars * sizeof(WCHAR));
		pdest += nActualChars * sizeof(WCHAR);
		free(pchCaption);

		*(WORD*)pdest = 0;  // How many bytes in data for control
		pdest += sizeof(WORD);
	}

	if (pdest - pBuffer != nBufferSize) // just make sure we did not overrun the heap
	{
			ErrorExit("gia_winapi_window_dialog_new_template");
	}

	wnd_h = CreateDialogIndirect(GetModuleHandle(NULL), 
						(DLGTEMPLATE*)pBuffer, 
						(HWND)(self?self->impl:0), (DLGPROC)gia_winapi_dlg_proc);

	GlobalUnlock(hGlobal);
	GlobalFree(hGlobal);

	if (wnd_h)
	{
		gia_win_p win     = gia_win_alloc();
		win->impl         = wnd_h;
		win->app          = self->app;
		win->cmd_handlers = 0;
		win->nhandlers    = 0;

		SetWindowLong(wnd_h, GWL_USERDATA, (LONG)win);

		return win;
	} else
	{
		ErrorExit("gia_winapi_window_dialog_new_template");
	}
	return 0;
}

static void gia_winapi_window_destroy(gia_win_p self)
{
	if (self->cmd_handlers) free(self->cmd_handlers);
	DestroyWindow((HWND)self->impl);
	if (self->title) free(self->title);
	free(self);
}

static void gia_winapi_window_set_title(gia_win_p self, const ia_string_t title)
{
	if (self->title) free(self->title);
	self->title = strdup(title);
	SetWindowText((HWND)self->impl, self->title);
}

static const ia_string_t gia_winapi_window_get_title(gia_win_p self)
{
	if (!self->title)
	{
		self->title=(ia_string_t)malloc(255);
		GetWindowText((HWND)self->impl, self->title, 255);
	}

	return self->title;
}

static void gia_winapi_window_set_visible(gia_win_p self, ia_bool_t is_visible)
{
	ShowWindow((HWND)self->impl, is_visible?SW_NORMAL:0);
	UpdateWindow((HWND)self->impl);
}

static ia_bool_t gia_winapi_window_is_visible(gia_win_p self)
{
	return IsWindowVisible((HWND)self->impl);
}

static void gia_winapi_window_set_enable(gia_win_p self, ia_bool_t is_enabled)
{
	EnableWindow((HWND)self->impl, is_enabled);
}

static ia_bool_t gia_winapi_window_is_enabled(gia_win_p self)
{
	return IsWindowEnabled((HWND)self->impl);
}

static void gia_winapi_window_update(gia_win_p self)
{
	UpdateWindow((HWND)self->impl);
}

static void gia_winapi_window_redraw(gia_win_p self)
{
	HWND wnd_h=(HWND)self->impl;
	//HDC dc_h=GetDC(wnd_h);
	RECT rect;
	GetClientRect(wnd_h, &rect);
	//FillRect(dc_h, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
	//ReleaseDC(wnd_h, dc_h);
	InvalidateRect(wnd_h, &rect, 0);
	RedrawWindow(wnd_h, &rect, 0, RDW_INTERNALPAINT);
}

static gia_win_p gia_winapi_window_get_dialog_item(gia_win_p self, ia_uint32_t item_id)
{
	return gia_auto_cast(GetDlgItem((HWND)self->impl, item_id));
}

static ia_int32_t gia_winapi_window_get_control_id(gia_win_p self)
{
	return GetWindowLong((HWND)self->impl, GWL_ID);
}



static ia_bool_t gia_winapi_window_dialog_file(gia_win_p self, const ia_string_t filter, ia_string_t file_name, ia_uint32_t file_name_size, ia_bool_t is_save)
{
	OPENFILENAME ofn;
	memset(&ofn, 0, sizeof(OPENFILENAME));

	ofn.lStructSize  = sizeof(OPENFILENAME);
	ofn.hwndOwner    = (HWND)self->impl;
	ofn.hInstance    = GetModuleHandle(0);
	ofn.lpstrFilter  = filter;
	file_name[0]     = 0;
	ofn.lpstrFile    = file_name;
	ofn.nMaxFile     = file_name_size; 
	ofn.Flags        = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

	return (is_save?GetSaveFileName(&ofn):GetOpenFileName(&ofn));
}

static void* gia_winapi_window_bitmap_create(gia_win_p self, ia_image_p img)
{
	HWND       wnd_h = (HWND)self->impl;
	HDC        dc_h  = GetDC(wnd_h), bmp_dc;
	BITMAPINFO bi;
	HBITMAP    bmp;

	bi.bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biClrUsed       = 0;
	bi.bmiHeader.biBitCount      = ia_format_size(img->format);
	bi.bmiHeader.biClrImportant  = 0;
	bi.bmiHeader.biClrUsed       = 0;
	bi.bmiHeader.biCompression   = BI_RGB;
	bi.bmiHeader.biHeight        = -img->height;
	bi.bmiHeader.biWidth         = img->width;
	bi.bmiHeader.biPlanes        = 1;
	bi.bmiHeader.biSizeImage     = 0;
	bi.bmiHeader.biXPelsPerMeter = 0;
	bi.bmiHeader.biYPelsPerMeter = 0;
	bi.bmiColors[0].rgbRed       = 0;
	bi.bmiColors[0].rgbGreen     = 0;
	bi.bmiColors[0].rgbBlue      = 0;
	bi.bmiColors[0].rgbReserved  = 0;

	bmp=CreateDIBitmap(dc_h, &bi.bmiHeader, CBM_INIT, img->pixels.data, &bi, 0);
	if (!bmp) ErrorExit("gia_winapi_window_bitmap_create");

	bmp_dc=CreateCompatibleDC(dc_h);
	if (!SelectObject(bmp_dc, bmp)) ErrorExit("gia_winapi_window_bitmap_create");

	ReleaseDC(wnd_h, dc_h);
	DeleteObject(bmp);

	return bmp_dc;
}

static void gia_winapi_window_bitmap_destroy(gia_win_p self, void* bitmap_dc)
{
	DeleteDC((HDC)bitmap_dc);
}

static void gia_winapi_window_set_rect(gia_win_p self, ia_rect_p rect)
{
	ia_rect_t rect_w;
	rect_w.l = MIN(rect->l, rect->r);
	rect_w.t = MIN(rect->t, rect->b);
	rect_w.r = MAX(rect->l, rect->r) - rect_w.l + 1;
	rect_w.b = MAX(rect->t, rect->b) - rect_w.t + 1;

	SetWindowPos((HWND)self->impl, HWND_NOTOPMOST, rect_w.l, rect_w.t, rect_w.r, rect_w.b, SWP_NOZORDER);
}

static void gia_winapi_window_get_rect(gia_win_p self, ia_rect_p rect)
{
	GetClientRect((HWND)self->impl, (PRECT)rect);
}

static void gia_winapi_window_get_window_rect(gia_win_p self, ia_rect_p rect)
{
	GetWindowRect((HWND)self->impl, (PRECT)rect);
}

static void gia_winapi_window_add_button(gia_win_p self, ia_rect_p rect, ia_string_t caption, command_handler_t cmd_handler)
{
	HWND wnd_h;
	ia_rect_t rect_w;

	if (rect->r == 0) rect->r=rect->l+DEFAULT_BUTTON_W;
	if (rect->b == 0) rect->b=rect->t+DEFAULT_BUTTON_H;

	rect_w.l = MIN(rect->l, rect->r);
	rect_w.t = MIN(rect->t, rect->b);
	rect_w.r = MAX(rect->l, rect->r) - rect_w.l;
	rect_w.b = MAX(rect->t, rect->b) - rect_w.t;

	wnd_h = CreateWindowEx(0, "BUTTON", caption, WS_TABSTOP | WS_GROUP | WS_CHILD,
		rect_w.l, rect_w.t, rect_w.r, rect_w.b, (HWND)self->impl, NULL, GetModuleHandle(NULL), NULL);

	SetWindowLong(wnd_h, self->nhandlers++, GWL_ID);
	self->cmd_handlers=(command_handler_t*)realloc(self->cmd_handlers, self->nhandlers*sizeof(command_handler_t));
	self->cmd_handlers[self->nhandlers-1]=cmd_handler;

	ShowWindow(wnd_h, SW_NORMAL);
	UpdateWindow(wnd_h);
	AnimateWindow(wnd_h, 300, AW_BLEND);
}

static void gia_winapi_window_add_edit(gia_win_p self, ia_rect_p rect, ia_string_t caption, command_handler_t handler)
{

}

static void gia_winapi_window_add_label(gia_win_p self, ia_rect_p rect, ia_string_t caption, command_handler_t handler)
{
}

static void gia_winapi_window_draw_bitmap(gia_win_p self, void* bitmap_dc, ia_int32_t w, ia_int32_t h, ia_int32_t x, ia_int32_t y, ia_float_t scale_x, ia_float_t scale_y)
{
	RECT rect;
	HWND wnd_h=(HWND)self->impl;
	HDC dc_h=GetDC(wnd_h);
	StretchBlt(dc_h, x, y, (ia_uint32_t)floor(scale_x*w), (ia_uint32_t)floor(scale_y*h), (HDC)bitmap_dc, 0, 0, w, h, SRCCOPY);
	ReleaseDC(wnd_h, dc_h);

	rect.left=x;
	rect.top=y;
	rect.right=x+w;
	rect.bottom=y+h;

	ValidateRect((HWND)self->impl, &rect);
}

static void gia_winapi_window_draw_image(gia_win_p self, ia_image_p image, ia_int32_t x, ia_int32_t y, ia_float_t scale_x, ia_float_t scale_y)
{
	void* bitmap=self->bitmap_create(self, image);
	self->draw_bitmap(self, bitmap, image->width, image->height, x, y, scale_x, scale_y);
	self->bitmap_destroy(self, bitmap);
}

static void gia_winapi_window_draw_pixel(gia_win_p self, ia_int32_t x, ia_int32_t y, ia_uint32_t color)
{
	HWND wnd_h=(HWND)self->impl;
	HDC dc_h=GetDC(wnd_h);
	SetPixel(dc_h, x, y, RGB(IA_RED(color), IA_GREEN(color), IA_BLUE(color)));
	ReleaseDC(wnd_h, dc_h);
}

static void gia_winapi_window_line_move_to(gia_win_p self, ia_int32_t x, ia_int32_t y)
{
	HWND wnd_h=(HWND)self->impl;
	HDC dc_h=GetDC(wnd_h);
	POINT point;
	MoveToEx(dc_h, x, y, &point);
	ReleaseDC(wnd_h, dc_h);
}

static void gia_winapi_window_line_draw_to(gia_win_p self, ia_int32_t x, ia_int32_t y, ia_uint32_t color)
{
	HWND wnd_h=(HWND)self->impl;
	HDC dc_h=GetDC(wnd_h);
	HPEN pen=CreatePen(PS_SOLID, 1, color);
	LineTo(dc_h, x, y);
	DeleteObject(pen);
	ReleaseDC(wnd_h, dc_h);
}

static void gia_winapi_window_draw_rect(gia_win_p self, ia_int32_t x, ia_int32_t y, ia_int32_t w, ia_int32_t h, ia_uint32_t color)
{
	HWND wnd_h=(HWND)self->impl;
	HDC dc_h=GetDC(wnd_h);
	LOGBRUSH logbrush;
	HBRUSH brush_h;
	RECT rect;
    rect.left=x;
    rect.top=y;
    rect.right=x+w;
    rect.bottom=y+h;
	logbrush.lbStyle=BS_SOLID;
	logbrush.lbColor=color;
	logbrush.lbHatch=0;
	brush_h=CreateBrushIndirect(&logbrush);
	FillRect(dc_h, &rect, brush_h);
	DeleteObject(brush_h);
	ReleaseDC(wnd_h, dc_h);
}

static void gia_winapi_window_draw_text(gia_win_p self, ia_int32_t x, ia_int32_t y, ia_string_t str, ia_int32_t size)
{
	HWND wnd_h=(HWND)self->impl;
	HDC dc_h=GetDC(wnd_h);
	HFONT font=CreateFont(
		-size, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, NULL);

	SetBkMode(dc_h, TRANSPARENT); 
	SelectObject(dc_h, font);
	TextOut(dc_h, x, y, str, (int)strlen(str));
	DeleteObject(font);
	ReleaseDC(wnd_h, dc_h);
	ValidateRect(wnd_h, 0);
}

/** Button implementation */
static ia_bool_t gia_winapi_button_is_checked(gia_button_p self)
{
	return (ia_bool_t)SendMessage((HWND)self->impl, BM_GETCHECK, 0, 0);
}

static void gia_winapi_button_check(gia_button_p self, ia_bool_t is_check)
{
	SendMessage(self->impl, BM_SETCHECK, is_check?BST_CHECKED:BST_UNCHECKED, 0);
}

/** Slide Bar implementation */
static void gia_winapi_slidebar_set_minmax(gia_slidebar_p self, ia_pos_p pos)
{
	SendMessage((HWND)self->impl, TBM_SETRANGEMIN, (WPARAM)0, (LPARAM)pos->x);
	SendMessage((HWND)self->impl, TBM_SETRANGEMAX, (WPARAM)1, (LPARAM)pos->y);
}

static void gia_winapi_slidebar_get_minmax(gia_slidebar_p self, ia_pos_p pos)
{ 
	pos->x=(ia_int32_t)SendMessage((HWND)self->impl, TBM_GETRANGEMIN, (WPARAM)0, (LPARAM)0);
	pos->y=(ia_int32_t)SendMessage((HWND)self->impl, TBM_GETRANGEMAX, (WPARAM)0, (LPARAM)0);
}

static void gia_winapi_slidebar_set_pos(gia_slidebar_p self, ia_int32_t nval)
{
	SendMessage((HWND)self->impl, TBM_SETPOS, (WPARAM)1, (LPARAM)nval);
}

static ia_int32_t gia_winapi_slidebar_get_pos(gia_slidebar_p self)
{
	return (ia_int32_t)SendMessage((HWND)self->impl, TBM_GETPOS, (WPARAM)0, (LPARAM)0);
}

/** Progress Bar implementation */
static void gia_winapi_progressbar_set_minmax(gia_progressbar_p self, ia_pos_p pos)
{
	SendMessage((HWND)self->impl, TBM_SETRANGEMIN, (WPARAM)0, (LPARAM)pos->x);
	SendMessage((HWND)self->impl, TBM_SETRANGEMAX, (WPARAM)1, (LPARAM)pos->y);
}

static void gia_winapi_progressbar_get_minmax(gia_progressbar_p self, ia_pos_p pos)
{ 
	pos->x=(ia_int32_t)SendMessage((HWND)self->impl, TBM_GETRANGEMIN, (WPARAM)0, (LPARAM)0);
	pos->y=(ia_int32_t)SendMessage((HWND)self->impl, TBM_GETRANGEMAX, (WPARAM)0, (LPARAM)0);
}

static void gia_winapi_progressbar_set_pos(gia_progressbar_p self, ia_int32_t nval)
{
	SendMessage((HWND)self->impl, TBM_SETPOS, (WPARAM)1, (LPARAM)nval);
}

static ia_int32_t gia_winapi_progressbar_get_pos(gia_progressbar_p self)
{
	return (ia_int32_t)SendMessage((HWND)self->impl, TBM_GETPOS, (WPARAM)0, (LPARAM)0);
}

static void gia_winapi_register_class(gia_app_p self, HINSTANCE instance_h)
{
	WNDCLASSEX wcex;

	memset(&wcex,  0, sizeof(WNDCLASSEX));

	wcex.cbSize         = sizeof(WNDCLASSEX); 
	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = (WNDPROC)gia_winapi_wnd_proc;
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 4;
	wcex.hInstance      = instance_h;
	wcex.hIcon          = LoadIcon(instance_h, MAKEINTRESOURCE(101));
	wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName   = NULL;
	wcex.lpszClassName  = self->app_name;

	RegisterClassEx(&wcex);
}

static LRESULT gia_winapi_wnd_proc(HWND wnd_h, UINT msg, WPARAM param_w, LPARAM param_l)
{
	gia_win_p win = (gia_win_p)GetWindowLong(wnd_h, GWL_USERDATA);
	switch (msg)
	{
		case WM_COMMAND:
			{
				if (win && win->cmd_handlers)
				{
					((command_handler_t*)win->cmd_handlers)[LOWORD(param_w)](win, LOWORD(param_w));
				}
			}
			break;

		case WM_PAINT:
			if (win && win->on_paint)
				win->on_paint(win);
			break;

		case WM_MOUSEWHEEL:
			if (win && win->on_mousewheel)
			{
				win->on_mousewheel(win, !(HIWORD(param_w) - WHEEL_DELTA));
			}
			break;

		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
			if (win && win->on_mouseclick)
			{
				ia_pos_t pos;
				pos.x=GET_X_LPARAM(param_l); 
				pos.y=GET_Y_LPARAM(param_l); 
				win->on_mouseclick(win, &pos, msg==WM_LBUTTONDOWN || msg==WM_LBUTTONUP, msg==WM_LBUTTONDOWN || msg==WM_RBUTTONDOWN);
			}
			break;

		case WM_MOUSEMOVE:
			if (win && win->on_mousemove)
			{
				ia_pos_t pos;
				pos.x=GET_X_LPARAM(param_l); 
				pos.y=GET_Y_LPARAM(param_l); 
				win->on_mousemove(win, &pos);
			}
			break;

		case WM_SIZE:
			if (win && win->on_size)
			{
				ia_pos_t size;
				size.x=GET_X_LPARAM(param_l); 
				size.y=GET_Y_LPARAM(param_l); 
				win->on_size(win, &size);
			}
			break;

		case WM_CLOSE:
			if (win && win->on_close)
			{
				win->on_close(win);
			} else
			{
				//return (LRESULT)DefWindowProc(wnd_h, msg, param_w, param_l);
			}
			break;

		case WM_DESTROY:
			if (win && win->on_destroy)
			{
				win->on_destroy(win);
			}
			break;

		default:
			return (LRESULT)DefWindowProc(wnd_h, msg, param_w, param_l);
   }
   return 0;
}

static BOOL gia_winapi_dlg_proc(HWND dlg_h, UINT msg, WPARAM param_w, LPARAM param_l)
{
	gia_win_p dlg = (gia_win_p)GetWindowLong(dlg_h, GWL_USERDATA);

	switch (msg) 
	{ 
		case WM_HSCROLL: 
		case WM_VSCROLL: 
			if (dlg && dlg->on_dialog_command)
			{
				return dlg->on_dialog_command(dlg, gia_auto_cast((void*)param_l));
			}
			break;
		case WM_COMMAND: 
			if (dlg)
			{
				if (LOWORD(param_w) == IDCANCEL)
				{
					dlg->destroy(dlg);
					return 1;
				}
				if (dlg->on_dialog_command)
				{
					return dlg->on_dialog_command(dlg, gia_auto_cast(GetDlgItem(dlg_h, LOWORD(param_w))));
				}
			}
		break;

		case WM_DESTROY:
			if (dlg && dlg->on_destroy)
			{
				dlg->on_destroy(dlg);
			}
			break;
	} 
	return 0; 
}

gia_control_template_p gia_control_template_new(gia_control_t control_type, const ia_string_t title, ia_uint32_t style, 
	ia_int32_t x, ia_int32_t y, ia_int32_t w, ia_int32_t h, ia_int32_t id)
{
	gia_control_template_p control_template = (gia_control_template_p)malloc(sizeof(gia_control_template_t));
	control_template->control_type          = control_type;
	control_template->title                 = title;
	control_template->style                 = style;
	control_template->rect.l                = x;
	control_template->rect.t                = y;
	control_template->rect.r                = w;
	control_template->rect.b                = h;
	control_template->id                    = id;

	control_template->destroy               = gia_control_template_destroy;

	return control_template;
}

static void gia_control_template_destroy(struct _gia_control_template* self)
{
	free(self);
}

gia_dialog_template_p gia_dialog_template_new(const ia_string_t title,  ia_uint32_t style, 
	ia_int32_t x, ia_int32_t y, ia_int32_t w, ia_int32_t h, ia_int32_t id)
{
	gia_dialog_template_p dialog_template = (gia_dialog_template_p)malloc(sizeof(gia_dialog_template_t));
	dialog_template->title                = title;
	dialog_template->style                = style;
	dialog_template->rect.l               = x;
	dialog_template->rect.t               = y;
	dialog_template->rect.r               = w;
	dialog_template->rect.b               = h;
	dialog_template->id                   = id;
	dialog_template->controls             = 0;
	dialog_template->ncontrols            = 0;

	dialog_template->add                  = gia_dialog_template_add;
	dialog_template->destroy              = gia_dialog_template_destroy;

	return dialog_template;
}
static void gia_dialog_template_add(struct _gia_dialog_template* self, gia_control_template_p control_template)
{
	self->controls=(gia_control_template_p*)realloc(self->controls, 
			(1+self->ncontrols) * sizeof(gia_control_template_p));
	self->controls[self->ncontrols++] = control_template;
}

static void gia_dialog_template_destroy(struct _gia_dialog_template* self)
{
	ia_int32_t i;
	for (i=0; i<self->ncontrols; i++)
	{
		self->controls[i]->destroy(self->controls[i]);
	}
	free(self->controls);
	free(self);
}

gia_menu_popup_p gia_menu_popup_new(const ia_string_t caption)
{
	gia_menu_popup_p self=(gia_menu_popup_p)malloc(sizeof(gia_menu_popup_t));

	self->caption = caption;
	self->items   = 0;
	self->nitems  = 0;

	self->add     = gia_menu_popup_add;
	self->destroy = gia_menu_popup_destroy;

	return self;
}

void static gia_menu_popup_add(struct _gia_menu_popup* self, const ia_string_t caption, command_handler_t command)
{
	self->items=(gia_menu_item_p)realloc(self->items, (1+self->nitems)*sizeof(gia_menu_item_t));
	self->items[self->nitems].caption = caption;
	self->items[self->nitems].command = command;
	self->nitems++;
}

static void gia_menu_popup_destroy(struct _gia_menu_popup* self)
{
	free(self->items);
	free(self);
}

static void ErrorExit(LPTSTR lpszFunction) 
{ 
    TCHAR szBuf[80]; 
    LPVOID lpMsgBuf;
    DWORD dw = GetLastError(); 

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    wsprintf(szBuf, "%s failed with error %d: %s", lpszFunction, dw, lpMsgBuf); 
    MessageBox(NULL, szBuf, "Error", MB_OK); 

    LocalFree(lpMsgBuf);
    ExitProcess(dw); 
}

#endif
