/* luaia.c
 * 
 * author      : Alexander Marinov (alek@crazyland.com)
 * project     : luaia
 * description : Binds libia to Lua
 * copyright   : Copyright (c) Alexander Marinov
 *
 *******************************************************************************************/

#include <lua.h>
#include <lauxlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lua.h>
#include <lauxlib.h>
#include <ia/ia.h>
#include <ia/ia_line.h>
#include <ia/ia_bezier.h>
#include <ia/ia_image.h>
#include <ia/algo/ia_distance_transform.h>
#include <ia/algo/ia_binarize.h>
#include <ia/algo/ia_contours.h>
#include <ia/algo/ia_morphology.h>
#include <ia/algo/ia_fft.h>
#include <ia/algo/ia_correlation.h>

#ifndef LUAIA_API
#define LUAIA_API  extern
#endif

#define LUAIA_LIBNAME "ia"

/* wrap values of arbitrary type */
union luaValueT
{
	int nval;
	double dval;
	const char* sval;
	void* ptr;
};

static void pushLuaValueT(lua_State* L, int t, union luaValueT v)
{
	switch (t)
	{
		case LUA_TNIL: lua_pushnil(L); break;
		case LUA_TBOOLEAN: lua_pushboolean(L, v.nval); break;
		case LUA_TFUNCTION: 
		case LUA_TTABLE: 
		case LUA_TUSERDATA: lua_rawgeti(L, LUA_REGISTRYINDEX, v.nval); break;
		case LUA_TLIGHTUSERDATA: lua_pushlightuserdata(L, v.ptr); break;
		case LUA_TNUMBER: lua_pushnumber(L, v.nval); break;
		case LUA_TSTRING: lua_pushstring(L, v.sval); break;
		default: luaL_error(L, "invalid type %s\n", lua_typename(L, t));
	}
}

int lia_color(lua_State* L)
{
	lua_pushnumber(L, IA_RGB(
			(ia_uint32_t)luaL_checknumber(L, 1),
			(ia_uint32_t)luaL_checknumber(L, 2),
			(ia_uint32_t)luaL_checknumber(L, 3)
			)
	);
	return 1;
}

int lia_red_color(lua_State* L)
{
	lua_pushnumber(L, IA_RED((ia_uint32_t)luaL_checknumber(L, 1)));
	return 1;
}

int lia_green_color(lua_State* L)
{
	lua_pushnumber(L, IA_GREEN((ia_uint32_t)luaL_checknumber(L, 1)));
	return 1;
}

int lia_blue_color(lua_State* L)
{
	lua_pushnumber(L, IA_BLUE((ia_uint32_t)luaL_checknumber(L, 1)));
	return 1;
}

/* Line API */
typedef struct 
{
	lua_State *L;
	int line_callback_ref;
	int param_type;
	union luaValueT param;
} line_callback_param_t, *line_callback_param_p;

static void lia_line_callback(void* p, ia_int32_t x, ia_int32_t y)
{
	line_callback_param_p param=(line_callback_param_p)p;

	lua_rawgeti(param->L, LUA_REGISTRYINDEX, param->line_callback_ref);
	//pushLuaValueT(param->L, param->param_type, param->param);
	lua_pushnil(param->L);
	lua_pushnumber(param->L, x);
	lua_pushnumber(param->L, y);
	lua_call(param->L, 3, 0);
}

static int lia_line_draw(lua_State* L)
{
	line_callback_param_t param;
	ia_rect_t clip_rect;
	ia_rect_p clip_p=NULL;
	int x1, y1, x2, y2;

	x1=(ia_int32_t)luaL_checknumber(L, 1); /* x1 */
	y1=(ia_int32_t)luaL_checknumber(L, 2); /* y1 */
	x2=(ia_int32_t)luaL_checknumber(L, 3); /* x2 */
	y2=(ia_int32_t)luaL_checknumber(L, 4); /* y2 */
	luaL_checktype(L, 5, LUA_TFUNCTION);   /* lua callback function invoked for each line point */

	lua_pushvalue(L, 5);
	param.line_callback_ref=luaL_ref(L, LUA_REGISTRYINDEX);

	/* set clip area if given */
	//clip_rect.l = (ia_int32_t)lua_tonumber(L, 6);
	//clip_rect.t = (ia_int32_t)lua_tonumber(L, 7);
	//clip_rect.r = (ia_int32_t)lua_tonumber(L, 8);
	//clip_rect.b = (ia_int32_t)lua_tonumber(L, 9);

	//clip_p=(clip_rect.l == clip_rect.r && clip_rect.t == clip_rect.b)?NULL:&clip_rect;

	//param.param_type=lua_type(L, -1);
	//switch (lua_type(L, -1))
	//{
	//	case LUA_TFUNCTION:
	//	case LUA_TTABLE:
	//	case LUA_TUSERDATA:
	//		param.param.nval=luaL_ref(L, LUA_REGISTRYINDEX);
	//		break;
	//	case LUA_TSTRING:
	//		param.param.sval=lua_tostring(L, -1);
	//		break;
	//	case LUA_TNUMBER:
	//		param.param.dval=lua_tonumber(L, -1);
	//		break;
	//	case LUA_TLIGHTUSERDATA:
	//		param.param.ptr=lua_touserdata(L, -1);
	//		break;
	//	case LUA_TBOOLEAN:
	//		param.param.nval=lua_toboolean(L, -1);
	//		break;
	//	case LUA_TNIL:
	//		param.param.nval=0;
	//		break;
	//	default:
	//		luaL_argerror(L, 2, "Invalid callback param type");
	//}
	param.param.nval=0;
	param.param_type=LUA_TNIL;
	param.L=L;

	/* draws a line calling the given callback for each point */
	ia_line_draw(
		(ia_int32_t)luaL_checknumber(L, 1), 
		(ia_int32_t)luaL_checknumber(L, 2), 
		(ia_int32_t)luaL_checknumber(L, 3), 
		(ia_int32_t)luaL_checknumber(L, 4), 
		clip_p,
		lia_line_callback,
		&param
	);
	
	luaL_unref(L, LUA_REGISTRYINDEX, param.line_callback_ref);

	switch (param.param_type)
	{
		case LUA_TFUNCTION:
		case LUA_TTABLE:
		case LUA_TUSERDATA:
			luaL_unref(L, LUA_REGISTRYINDEX, param.param.nval);
			break;
	}
	return 0;
}

static int lia_format_size(lua_State* L)
{
	ia_format_t format=(ia_format_t)luaL_checknumber(L, 1);
	lua_pushnumber(L, ia_format_size(format));
	return 1;
}

static int lia_format_name(lua_State* L)
{
	ia_format_t format=(ia_format_t)luaL_checknumber(L, 1);
	lua_pushstring(L, ia_format_name(format));
	return 1;
}

static int lia_format_signed(lua_State* L)
{
	ia_format_t format=(ia_format_t)luaL_checknumber(L, 1);
	lua_pushboolean(L, ia_format_signed(format));
	return 1;
}

static int lia_format_min_max(lua_State* L)
{
	ia_int32_t min;
	ia_uint32_t max;
	ia_format_t format=(ia_format_t)luaL_checknumber(L, 1);
	ia_format_min_max(format, &min, &max);
	lua_pushnumber(L, (double)min);
	lua_pushnumber(L, (double)max);
	return 2;
}

/* Image API */
#define IA_IMAGE_HANDLE "ia_image_p"


static int lia_image_init(lua_State* L, ia_image_p image)
{
	if (image)
	{
		ia_image_p* pi = (ia_image_p*)lua_newuserdata(L, sizeof(ia_image_p));
		*pi=image;
		luaL_getmetatable(L, IA_IMAGE_HANDLE);
		lua_setmetatable(L, -2);
		return 1;
	}
	return 0;
}

static int lia_image_new(lua_State* L)
{
	ia_uint16_t w=(ia_uint16_t)luaL_checknumber(L, 1);
	ia_uint16_t h=(ia_uint16_t)luaL_checknumber(L, 2);
	ia_format_t format=(ia_format_t)luaL_checknumber(L, 3);
	ia_bool_t is_gray=(ia_bool_t)lua_toboolean(L, 4);

	return lia_image_init(L, ia_image_new(w, h, format, is_gray));
}

static int lia_image_load(lua_State* L)
{
	return lia_image_init(L, ia_image_load((const ia_string_t)luaL_checkstring(L, 1)));
}

static ia_image_p toimage(lua_State* L, int cindex)
{
	ia_image_p* d=(ia_image_p*)luaL_checkudata(L, cindex, IA_IMAGE_HANDLE);
	if (!d) luaL_argerror(L, cindex, "invalid image object");
	if (!(*d)) luaL_error(L, "attempt to use closed image object");
	return *d;
}

static int lia_image_get_metrics(lua_State* L)
{
	ia_image_p self=toimage(L, 1);
	lua_pushnumber(L, (double)self->width);
	lua_pushnumber(L, (double)self->height);
	lua_pushnumber(L, (double)self->format);
	lua_pushboolean(L, self->is_gray);
	return 4;
}

static int lia_image_save(lua_State* L)
{
	ia_image_p self=toimage(L, 1);
	self->save(self, (const ia_string_t)luaL_checkstring(L, 2));
	return 0;
}

static int lia_image_set_pixel(lua_State* L)
{
	ia_image_p self=toimage(L, 1);
	self->set_pixel(self, (ia_uint16_t)luaL_checknumber(L, 2), (ia_uint16_t)luaL_checknumber(L, 3), (ia_uint32_t)luaL_checknumber(L, 4));
	return 0;
}

static int lia_image_get_pixel(lua_State* L)
{
	ia_image_p self=toimage(L, 1);
	lua_pushnumber(L, (ia_uint32_t)self->get_pixel(self, (ia_uint16_t)luaL_checknumber(L, 2), (ia_uint16_t)luaL_checknumber(L, 3)));
	return 1;
}

static int lia_image_fill(lua_State* L)
{
	ia_image_p self=toimage(L, 1);
	self->fill(self, (ia_uint32_t)luaL_checknumber(L, 2));
	return 0;
}

static int lia_image_convert_rgb(lua_State* L)
{
	ia_image_p self=toimage(L, 1);
	return lia_image_init(L, self->convert_rgb(self));
}

static int lia_image_convert_gray(lua_State* L)
{
	ia_image_p self=toimage(L, 1);
	return lia_image_init(L, self->convert_gray(self, (ia_format_t)luaL_checknumber(L, 2)));
}

static int lia_image_normalize_colors(lua_State* L)
{
	ia_image_p self=toimage(L, 1);
	self->normalize_colors(self, (ia_int32_t)luaL_checknumber(L, 2), (ia_uint32_t)luaL_checknumber(L, 3), (ia_int32_t)luaL_checknumber(L, 4), (ia_uint32_t)luaL_checknumber(L, 5));
	return 0;
}

static int lia_image_inverse(lua_State* L)
{
	ia_image_p self=toimage(L, 1);
	self->inverse(self);
	return 0;
}

static int lia_image_get_min_max(lua_State* L)
{
	ia_int32_t min;
	ia_uint32_t max;
	ia_image_p self=toimage(L, 1);

	self->get_min_max(self, &min, &max);

	lua_pushnumber(L, (double)min);
	lua_pushnumber(L, (double)max);
	return 2;
}

static int lia_image_copy(lua_State* L)
{
	ia_image_p self=toimage(L, 1);
	return lia_image_init(L, self->copy(self));
}

static int lia_image_destroy(lua_State* L)
{
	ia_image_p* d=(ia_image_p*)luaL_checkudata(L, 1, IA_IMAGE_HANDLE);
	if (d && *d) 
	{
		(*d)->destroy(*d);
		*d=0;
	}
	return 0;
}

				/* Algorithms */

/* Contours API */

typedef struct
{
	ia_contours_p contours;
	ia_int32_t* refs;
	ia_int32_t nrefs;
} lia_contours_t, *lia_contours_p;

#define IA_CONTOUR_HANDLE "ia_contour_p"
#define IA_CONTOURS_HANDLE "lia_contours_p"
#define COUNTOURS_GROW_STEP 10

static int lia_contour_init(lua_State* L, ia_contour_p contour)
{
	ia_contour_p* pc = (ia_contour_p*)lua_newuserdata(L, sizeof(ia_contour_p));
	*pc=contour;
	luaL_getmetatable(L, IA_CONTOUR_HANDLE);
	lua_setmetatable(L, -2);

	return 1;
}

static ia_contour_p tocontour(lua_State* L, int cindex)
{
	ia_contour_p* d=(ia_contour_p*)luaL_checkudata(L, cindex, IA_CONTOUR_HANDLE);
	if (!d) luaL_argerror(L, cindex, "invalid contour object");
	if (!(*d)) luaL_error(L, "attempt to use closed contour object");
	return *d;
}

static int lia_contour_new(lua_State* L)
{
	return lia_contour_init(L, ia_contour_new());
}

static int lia_contour_add(lua_State* L)
{
	ia_contour_p self=tocontour(L, 1);
	self->add(self, (ia_int32_t)luaL_checknumber(L, 2), (ia_int32_t)luaL_checknumber(L, 3));
	return 0;
}

static int lia_contour_fill(lua_State* L)
{
	ia_contour_p self=tocontour(L, 1);
	ia_image_p image=toimage(L, 2);
	ia_uint32_t color=(ia_uint32_t)luaL_checknumber(L, 3);
	ia_image_p* history=(ia_image_p*)luaL_checkudata(L, 4, IA_IMAGE_HANDLE);
	ia_int32_t npoints_inside;
	if (history)
	{
		npoints_inside=self->fill(self, image, color, *history);
	} else
	{
		npoints_inside=self->fill(self, image, color, 0);
	}
	lua_pushnumber(L, (double)npoints_inside);
	return 1;
}

static int lia_find_encapsulating_contour(lua_State* L)
{
	ia_contour_p self=tocontour(L, 1);
	return lia_contour_init(L, self->find_encapsulating_contour(self));
}

static int lia_contour_count_at(lua_State* L)
{
	ia_contour_p self=tocontour(L, 1);
	lua_pushnumber(L, (double)self->count_at(self, (ia_int32_t)luaL_checknumber(L, 2), (ia_int32_t)luaL_checknumber(L, 3)));
	return 1;
}

static int lia_contour_point_in(lua_State* L)
{
	ia_contour_p self=tocontour(L, 1);
	lua_pushnumber(L, (double)self->point_in(self, (ia_int32_t)luaL_checknumber(L, 2), (ia_int32_t)luaL_checknumber(L, 3)));
	return 1;
}

static int lia_contour_in_contour(lua_State* L)
{
	ia_contour_p self=tocontour(L, 1);
	ia_contour_p contour=tocontour(L, 2);
	lua_pushboolean(L, self->in_contour(self, contour));
	return 1;
}

static int lia_contour_count(lua_State* L)
{
	ia_contour_p self=tocontour(L, 1);
	lua_pushnumber(L, (double)self->npoints);
	return 1;
}

static int lia_contour_get(lua_State* L)
{
	ia_contour_p self=tocontour(L, 1);
	ia_int32_t n=(ia_int32_t)luaL_checknumber(L, 2);

	if (n>=0 && n<self->npoints && self->points[n])
	{
		lua_pushnumber(L, (double)self->points[n]->x);
		lua_pushnumber(L, (double)self->points[n]->y);
		return 2;
	}
	return 0;
}

static int lia_contour_destroy(lua_State* L)
{
	ia_contour_p* d=(ia_contour_p*)luaL_checkudata(L, 1, IA_CONTOUR_HANDLE);
	if (d && *d) 
	{
		(*d)->destroy(*d);
		*d=0;
	}
	return 0;
}

static int lia_contours_init(lua_State* L, ia_contours_p contours)
{
	/* allocate space for contours pointer and integer pointer for contour references array */
	lia_contours_p* pc = (lia_contours_p*)lua_newuserdata(L, sizeof(lia_contours_p*));
	*pc=(lia_contours_p)malloc(sizeof(lia_contours_t));
	(*pc)->contours=contours;
	(*pc)->refs=0;
	(*pc)->nrefs=0;
	luaL_getmetatable(L, IA_CONTOURS_HANDLE);
	lua_setmetatable(L, -2);

	return 1;
}

static ia_contours_p tocontours(lua_State* L, int cindex)
{
	lia_contours_p* d=(lia_contours_p*)luaL_checkudata(L, cindex, IA_CONTOURS_HANDLE);
	if (!d) luaL_argerror(L, cindex, "invalid contours object");
	if (!(*d)) luaL_error(L, "attempt to use closed contours object");
	return (*d)->contours;
}

static int lia_contours_new(lua_State* L)
{
	return lia_contours_init(L, ia_contours_new());
}

static int lia_contours_add(lua_State* L)
{
	lia_contours_p* lcontours=(lia_contours_p*)luaL_checkudata(L, 1, IA_CONTOURS_HANDLE);
	if (lcontours && *lcontours)
	{
		ia_contour_p contour=tocontour(L, 2);
		if (((*lcontours)->nrefs & COUNTOURS_GROW_STEP) == 0)
		{
			(*lcontours)->refs=(ia_int32_t*)realloc((*lcontours)->refs, ((*lcontours)->nrefs + COUNTOURS_GROW_STEP)*sizeof(ia_int32_t));
		}
		lua_pushvalue(L, 2);
		(*lcontours)->refs[(*lcontours)->nrefs++]=luaL_ref(L, LUA_REGISTRYINDEX);
		(*lcontours)->contours->add((*lcontours)->contours, contour);
	}
	return 0;
}

static int lia_contours_count(lua_State* L)
{
	ia_contours_p self=tocontours(L, 1);
	lua_pushnumber(L, (double)self->ncontours);
	return 1;
}

static int lia_contours_get(lua_State* L)
{
	lia_contours_p* lcontours=(lia_contours_p*)luaL_checkudata(L, 1, IA_CONTOURS_HANDLE);
	ia_int32_t n=(ia_int32_t)luaL_checknumber(L, 2);
	if (lcontours && *lcontours)
	{
		if (n>=0 && n<(*lcontours)->nrefs && (*lcontours)->contours->contours[n])
		{
			lua_rawgeti(L, LUA_REGISTRYINDEX, (*lcontours)->refs[n]);
			return 1;
		}
	}
	return 0;
}

static int lia_contours_destroy(lua_State* L)
{
	int i;
	lia_contours_p* lcontours=(lia_contours_p*)luaL_checkudata(L, 1, IA_CONTOURS_HANDLE);
	if (lcontours && *lcontours)
	{
		if ((*lcontours)->refs)
		{
			for (i=0; i<(*lcontours)->nrefs; i++)
			{
				luaL_unref(L, LUA_REGISTRYINDEX, (*lcontours)->refs[i]);
				(*lcontours)->refs[i]=0;
			}
			free((*lcontours)->refs);
			(*lcontours)->refs=0;
		}
		(*lcontours)->contours->contours[0]=0; /* attached contours must be automatically destroyed */
		(*lcontours)->contours->destroy((*lcontours)->contours);
		*lcontours=0;
	}
	return 0;
}

static int lia_contours_find(lua_State* L)
{
	ia_image_p image=toimage(L, 1);
	ia_contours_p contours=ia_contours_find(image);
	if (contours)
	{
		int i;
		lia_contours_p* lcontours;
		lia_contours_init(L, contours);
		lcontours=(lia_contours_p*)luaL_checkudata(L, -1, IA_CONTOURS_HANDLE);
		(*lcontours)->refs=(ia_int32_t*)malloc(contours->ncontours * sizeof(ia_int32_t));
		(*lcontours)->nrefs=contours->ncontours;
		for (i=0; i<(*lcontours)->nrefs; i++)
		{
			lia_contour_init(L, contours->contours[i]);
			(*lcontours)->refs[i]=luaL_ref(L, LUA_REGISTRYINDEX);
		}
		return 1;
	}
	return 0;
}

/* bezier algorithm over contour object */
static int lia_bezier_draw(lua_State* L)
{
	int i;
	ia_contour_p contour=tocontour(L, 1);
	ia_double_t mu=luaL_checknumber(L, 2);
	ia_pos_p points=(ia_pos_p)malloc(contour->npoints * sizeof(ia_pos_t));
	ia_pos_t pos;
	for (i=0; i<contour->npoints; i++)
	{
		points[i].x=contour->points[i]->x;
		points[i].y=contour->points[i]->y;
	}

	pos=ia_bezier_draw(points, contour->npoints-1, mu);
	lua_pushnumber(L, pos.x);
	lua_pushnumber(L, pos.y);

	free(points);
	return 2;
}

/* Distance Transform API */

static int lia_distance_transform_mask(lua_State* L)
{
	return lia_image_init(L, ia_distance_transform_mask( 
		(ia_int32_t)luaL_checknumber(L, 1), /* mask size */
		(ia_int32_t)luaL_checknumber(L, 2), /* a */
		(ia_int32_t)luaL_checknumber(L, 3), /* b */
		(ia_int32_t)luaL_checknumber(L, 4), /* c */
		(ia_int32_t)luaL_checknumber(L, 5), /* d */
		(ia_int32_t)luaL_checknumber(L, 6)) /* e */
	);
}

static int lia_distance_transform_parallel(lua_State* L)
{
	ia_int32_t min;
	ia_uint32_t max;
	ia_image_p self=toimage(L, 1);
	ia_image_p mask=toimage(L, 2);

	ia_distance_transform_parallel(self, mask, &min, &max);

	lua_pushnumber(L, (double)min);
	lua_pushnumber(L, (double)max);
	
	return 2;
}

static int lia_distance_transform_sequential(lua_State* L)
{
	ia_int32_t min;
	ia_uint32_t max;
	ia_image_p self=toimage(L, 1);
	ia_image_p mask=toimage(L, 2);

	ia_distance_transform_sequential(self, mask, &min, &max);

	lua_pushnumber(L, (double)min);
	lua_pushnumber(L, (double)max);
	
	return 2;
}

/* binarization */
static int lia_binarize_level(lua_State* L)
{
	ia_image_p self=toimage(L, 1);
	ia_binarize_level(self, (ia_int32_t)luaL_checknumber(L, 2));
	return 0;
}

static int lia_binarize_otsu(lua_State* L)
{
	ia_image_p self=toimage(L, 1);
	ia_binarize_otsu(self);
	return 0;
}

/* morphology */
static int lia_morphology_dilation(lua_State* L)
{
	return lia_image_init(L, ia_morphology_dilation(toimage(L, 1), toimage(L, 2)));
}

static int lia_morphology_erosion(lua_State* L)
{
	return lia_image_init(L, ia_morphology_erosion(toimage(L, 1), toimage(L, 2)));
}

static int lia_morphology_opening(lua_State* L)
{
	return lia_image_init(L, ia_morphology_opening(toimage(L, 1), toimage(L, 2)));
}

static int lia_morphology_closing(lua_State* L)
{
	return lia_image_init(L, ia_morphology_closing(toimage(L, 1), toimage(L, 2)));
}

/* FFT API */
static int lia_fft(lua_State* L)
{
	ia_int32_t i, pos, grow_step=20, tab_index;
	ia_complex_p in = (ia_complex_p)malloc(grow_step * sizeof(ia_complex_t));
	luaL_argcheck(L, lua_istable(L, 1), 1, "expecting a table");                 /* array of reals */
	luaL_argcheck(L, lua_istable(L, 2), 2, "expecting a table");                 /* array of imags */
	luaL_checktype(L, 3, LUA_TNUMBER);                                           /* direction        */
	luaL_checktype(L, 4, LUA_TNUMBER);                                           /* scale factor     */

	for (tab_index=1; tab_index<=2; tab_index++)
	{
		lua_pushnil(L);
		pos=0;
		while( lua_next(L, tab_index) != 0 )
		{
			if ( lua_type(L, -1) != LUA_TNUMBER) 
			{
				free(in);
				luaL_error(L, "this table must be a number list");
			}

			if ( lua_type(L, -2) != LUA_TNUMBER ) 
			{
				free(in);
				luaL_error(L, "this table is a list, keys must be unused");
			}

			if (tab_index == 1)
				in[pos].re = lua_tonumber(L, -1);
			else
				in[pos].im = lua_tonumber(L, -1);

			lua_pop(L, 1);
		
			pos++;
			if (tab_index == 1 && ( (pos % grow_step) == 0) )
			{
				in = (ia_complex_p)realloc(in, (pos+grow_step) * sizeof(ia_complex_t));
			}
		}
	}

	ia_fft(pos, in, lua_tonumber(L, 3), lua_tonumber(L, 4));

	for (tab_index=1; tab_index<=2; tab_index++)
	{
		lua_newtable(L);
		for(i=0; i<pos; i++)
		{
			lua_pushnumber(L, i+1);
			lua_pushnumber(L, (tab_index == 1) ? in[i].re : in[i].im);
			lua_settable(L, -3);
		}
	}
	free(in);

	return 2;
}

/* Correlation */
static int lia_cross_correlation(lua_State* L)
{
	ia_int32_t i, size[2], grow_step=20, tab_index;
	ia_complex_p x = (ia_complex_p)malloc(grow_step * sizeof(ia_complex_t));
	ia_complex_p y = (ia_complex_p)malloc(grow_step * sizeof(ia_complex_t));
	ia_complex_p w=0;
	luaL_argcheck(L, lua_istable(L, 1), 1, "expecting a table");                 /* vector x */
	luaL_argcheck(L, lua_istable(L, 2), 2, "expecting a table");                 /* vector y */
	luaL_checktype(L, 3, LUA_TNUMBER);											 /* is_cyclic */

	for (tab_index=0; tab_index<=1; tab_index++)
	{
		size[tab_index]=0;
		lua_pushnil(L);
		while( lua_next(L, tab_index+1) != 0 )
		{
			if ( lua_type(L, -1) != LUA_TNUMBER) 
			{
				free(x);
				luaL_error(L, "this table must be a number list");
			}

			if ( lua_type(L, -2) != LUA_TNUMBER ) 
			{
				free(x);
				luaL_error(L, "this table is a list, keys must be unused");
			}

			if (tab_index == 0)
			{
				x[size[tab_index]].re = lua_tonumber(L, -1);
				x[size[tab_index]].im = 0;
			} 
			else
			{
				y[size[tab_index]].re = lua_tonumber(L, -1);
				y[size[tab_index]].im = 0;
			}

			lua_pop(L, 1);

			size[tab_index]++;
			if ((size[tab_index] % grow_step) == 0)
			{
				if (tab_index == 0)
				{
					x = (ia_complex_p)realloc(x, (size[tab_index]+grow_step) * sizeof(ia_complex_t));
				}
				else
				{
					y = (ia_complex_p)realloc(y, (size[tab_index]+grow_step) * sizeof(ia_complex_t));
				}
			}
		}
	}

	ia_cross_correlation(size[0], x, size[1], y,(ia_bool_t)lua_tonumber(L, 3), &w);

	lua_newtable(L);
	for(i=0; i<size[0]; i++)
	{
		lua_pushnumber(L, i+1);
		lua_pushnumber(L, w[i].re);
		lua_settable(L, -3);
	}
	free(x);
	free(y);

	if (w) 
	{
		free(w);
	}

	return 1;
}

static int lia_auto_correlation(lua_State* L)
{
	ia_int32_t i, pos=0, grow_step=20;
	ia_complex_p x = (ia_complex_p)malloc(grow_step * sizeof(ia_complex_t));
	ia_complex_p w=0;
	luaL_argcheck(L, lua_istable(L, 1), 1, "expecting a table");                 /* vector */
	luaL_checktype(L, 2, LUA_TNUMBER);											 /* is_cyclic */

	lua_pushnil(L);
	while( lua_next(L, 1) != 0 )
	{
		if ( lua_type(L, -1) != LUA_TNUMBER) 
		{
			free(x);
			luaL_error(L, "this table must be a number list");
		}

		if ( lua_type(L, -2) != LUA_TNUMBER ) 
		{
			free(x);
			luaL_error(L, "this table is a list, keys must be unused");
		}

		x[pos].re = lua_tonumber(L, -1);
		x[pos].im = 0;

		lua_pop(L, 1);

		pos++;
		if ((pos % grow_step) == 0)
		{
			x = (ia_complex_p)realloc(x, (pos+grow_step) * sizeof(ia_complex_t));
		}
	}

	if (pos>0)
	{
		ia_auto_correlation(pos, x,(ia_bool_t)lua_tonumber(L, 2), &w);
	}
	lua_newtable(L);
	for(i=0; i<pos; i++)
	{
		lua_pushnumber(L, i+1);
		lua_pushnumber(L, w[i].re);
		lua_settable(L, -3);
	}
	free(x);

	if (w) 
	{
		free(w);
	}

	return 1;
}

static int lia_auto_correlation_by_sums(lua_State* L)
{
	ia_int32_t i, pos=0, grow_step=20;
	ia_complex_p x = (ia_complex_p)malloc(grow_step * sizeof(ia_complex_t));
	ia_complex_p w=0;
	luaL_argcheck(L, lua_istable(L, 1), 1, "expecting a table");                 /* vector */
	luaL_checktype(L, 2, LUA_TNUMBER);											 /* is_cyclic */

	lua_pushnil(L);
	while( lua_next(L, 1) != 0 )
	{
		if ( lua_type(L, -1) != LUA_TNUMBER) 
		{
			free(x);
			luaL_error(L, "this table must be a number list");
		}

		if ( lua_type(L, -2) != LUA_TNUMBER ) 
		{
			free(x);
			luaL_error(L, "this table is a list, keys must be unused");
		}

		x[pos].re = lua_tonumber(L, -1);
		x[pos].im = 0;

		lua_pop(L, 1);

		pos++;
		if ((pos % grow_step) == 0)
		{
			x = (ia_complex_p)realloc(x, (pos+grow_step) * sizeof(ia_complex_t));
		}
	}

	ia_auto_correlation_by_sums(pos, x, (ia_bool_t)lua_tonumber(L, 2), &w);

	lua_newtable(L);
	for(i=0; i<pos; i++)
	{
		lua_pushnumber(L, i+1);
		lua_pushnumber(L, w[i].re);
		lua_settable(L, -3);
	}
	free(x);

	if (w) 
	{
		free(w);
	}

	return 1;
}

static int lia_convolution(lua_State* L)
{
	ia_int32_t i, size[2], grow_step=20, tab_index;
	ia_complex_p x = (ia_complex_p)malloc(grow_step * sizeof(ia_complex_t));
	ia_complex_p y = (ia_complex_p)malloc(grow_step * sizeof(ia_complex_t));
	ia_complex_p w=0;
	luaL_argcheck(L, lua_istable(L, 1), 1, "expecting a table");                 /* vector x */
	luaL_argcheck(L, lua_istable(L, 2), 2, "expecting a table");                 /* vector y */
	luaL_checktype(L, 3, LUA_TNUMBER);											 /* is_cyclic 0/1 */

	for (tab_index=0; tab_index<=1; tab_index++)
	{
		size[tab_index]=0;
		lua_pushnil(L);
		while( lua_next(L, tab_index+1) != 0 )
		{
			if ( lua_type(L, -1) != LUA_TNUMBER) 
			{
				free(x);
				luaL_error(L, "this table must be a number list");
			}

			if ( lua_type(L, -2) != LUA_TNUMBER ) 
			{
				free(x);
				luaL_error(L, "this table is a list, keys must be unused");
			}

			if (tab_index == 0)
			{
				x[size[tab_index]].re = lua_tonumber(L, -1);
				x[size[tab_index]].im = 0;
			} 
			else
			{
				y[size[tab_index]].re = lua_tonumber(L, -1);
				y[size[tab_index]].im = 0;
			}

			lua_pop(L, 1);

			size[tab_index]++;
			if ((size[tab_index] % grow_step) == 0)
			{
				if (tab_index == 0)
				{
					x = (ia_complex_p)realloc(x, (size[tab_index]+grow_step) * sizeof(ia_complex_t));
				}
				else
				{
					y = (ia_complex_p)realloc(y, (size[tab_index]+grow_step) * sizeof(ia_complex_t));
				}
			}
		}
	}

	w=(ia_complex_p)malloc(size[0] * sizeof(ia_complex_t));
	ia_convolution(size[0], x, size[1], y,(ia_bool_t)lua_tonumber(L, 3), &w);

	lua_newtable(L);
	for(i=0; i<size[0]; i++)
	{
		lua_pushnumber(L, i+1);
		lua_pushnumber(L, w[i].re);
		lua_settable(L, -3);
	}
	free(x);
	free(y);
	free(w);

	return 1;
}


static const struct luaL_reg luaia_funcs[] =
{
	{"color",                         lia_color},
	{"red_color",                     lia_red_color},
	{"green_color",                   lia_green_color},
	{"blue_color",                    lia_blue_color},

/* Line API */
	{"line_draw",                     lia_line_draw},

/* Image API */
	{"format_size",                   lia_format_size},
	{"format_name",                   lia_format_name},
	{"format_signed",                 lia_format_signed},
	{"format_min_max",                lia_format_min_max},
	{"image_new",                     lia_image_new},
	{"image_load",                    lia_image_load},

/* Contours API */
	{"contour_new",                   lia_contour_new},
	{"contours_new",                  lia_contours_new},

/* Distance Transform API */
	{"distance_transform_mask",       lia_distance_transform_mask},

/* FFT API */
	{"fft",                           lia_fft},

/* Correlations API */
	{"cross_correlation",             lia_cross_correlation},
	{"auto_correlation",              lia_auto_correlation},
	{"auto_correlation_by_sums",      lia_auto_correlation_by_sums},
	{"convolution",		              lia_convolution},
};

/* Image methods */
static const struct luaL_reg luaia_image_meths[] =
{
	{"get_metrics",             lia_image_get_metrics},
	{"save",                    lia_image_save},
	{"set_pixel",               lia_image_set_pixel},
	{"get_pixel",               lia_image_get_pixel},
	{"fill",                    lia_image_fill},
	{"convert_rgb",             lia_image_convert_rgb},
	{"convert_gray",            lia_image_convert_gray},
	{"normalize_colors",        lia_image_normalize_colors},
	{"inverse",                 lia_image_inverse},
	{"get_min_max",             lia_image_get_min_max},
	{"copy",                    lia_image_copy},
	{"__gc",                    lia_image_destroy},

/* contours */
	{"contours_find",                 lia_contours_find},

/* binarization */
	{"binarize_level",                lia_binarize_level},
	{"binarize_otsu",                 lia_binarize_otsu},

/* distance transform */
	{"distance_transform_parallel",   lia_distance_transform_parallel},
	{"distance_transform_sequential", lia_distance_transform_sequential},

/* morphology */
	{"morphology_dilation",           lia_morphology_dilation},
	{"morphology_erosion",            lia_morphology_erosion},
	{"morphology_opening",            lia_morphology_opening},
	{"morphology_closing",            lia_morphology_closing},
	{NULL, NULL}
};

/* Contours API */
static const struct luaL_reg luaia_contour_meths[] =
{
	{"add",                   lia_contour_add},
	{"count_at",              lia_contour_count_at},
	{"count",                 lia_contour_count},
	{"get",                   lia_contour_get},
	{"fill",                  lia_contour_fill},
	{"find_encapsulating_contour", lia_find_encapsulating_contour},
	{"point_in",              lia_contour_point_in},	
	{"in_contour",            lia_contour_in_contour},
	{"bezier",                lia_bezier_draw},
	{"__gc",                  lia_contour_destroy},
	{NULL, NULL}
};

/* Contours API */
static const struct luaL_reg luaia_contours_meths[] =
{
/* contours */
	{"add",                  lia_contours_add},
	{"count",                lia_contours_count},
	{"get",                  lia_contours_get},
	{"__gc",                 lia_contours_destroy},
	{NULL, NULL}
};

static void createmeta (lua_State *L, const char* handle_name) 
{
	luaL_newmetatable(L, handle_name);
	lua_pushliteral(L, "__index");
	lua_pushvalue(L, -2);
	lua_rawset(L, -3);
}

/* Fast set table macro */
#define LUA_SET_TABLE(context, key_type, key, value_type, value) \
	lua_push##key_type(context, key); \
	lua_push##value_type(context, value); \
	lua_settable(context, -3);

static void set_info(lua_State *L) 
{
	LUA_SET_TABLE(L, literal, "_COPYRIGHT", literal, "(C) 2003-2005 Alexander Marinov, Nadezhda Zlateva");
	LUA_SET_TABLE(L, literal, "_DESCRIPTION", literal, "LuaIA binds IA library to Lua");
	LUA_SET_TABLE(L, literal, "_NAME", literal, "luaia");
	LUA_SET_TABLE(L, literal, "_VERSION", literal, "1.0.0");
	LUA_SET_TABLE(L, literal, "_IAVERSION", literal, IA_VERSION);
}

static void set_iaconsts(lua_State *L)
{
	LUA_SET_TABLE(L, literal, "UINT_8", number, IAT_UINT_8);
	LUA_SET_TABLE(L, literal, "UINT_16", number, IAT_UINT_16);
	LUA_SET_TABLE(L, literal, "UINT_24", number, IAT_UINT_24);
	LUA_SET_TABLE(L, literal, "UINT_32", number, IAT_UINT_32);
	LUA_SET_TABLE(L, literal, "INT_8", number, IAT_INT_8);
	LUA_SET_TABLE(L, literal, "INT_16", number, IAT_INT_16);
	LUA_SET_TABLE(L, literal, "INT_24", number, IAT_INT_24);
	LUA_SET_TABLE(L, literal, "INT_32", number, IAT_INT_32);
	LUA_SET_TABLE(L, literal, "FLOAT", number, IAT_FLOAT);
	LUA_SET_TABLE(L, literal, "DOUBLE", number, IAT_DOUBLE);
	LUA_SET_TABLE(L, literal, "BOOL", number, IAT_BOOL);

	LUA_SET_TABLE(L, literal, "MAX_INT", number, IA_MAX_INT);
	LUA_SET_TABLE(L, literal, "MIN_INT", number, IA_MIN_INT);
	LUA_SET_TABLE(L, literal, "MAX_UINT", number, IA_MAX_UINT);
	LUA_SET_TABLE(L, literal, "MIN_UINT", number, IA_MIN_UINT);

	LUA_SET_TABLE(L, literal, "IMAGE_GRAY", number, IA_IMAGE_GRAY);
	LUA_SET_TABLE(L, literal, "IMAGE_RGB", number, IA_IMAGE_RGB);

	LUA_SET_TABLE(L, literal, "POINT_INSIDE", number, CONTOUR_POINT_INSIDE);
	LUA_SET_TABLE(L, literal, "POINT_OVER", number, CONTOUR_POINT_OVER);
	LUA_SET_TABLE(L, literal, "POINT_OUTSIDE", number, CONTOUR_POINT_OUTSIDE);

	LUA_SET_TABLE(L, literal, "DT_INF", number, DT_INF);
}

LUAIA_API int luaopen_luaia(lua_State *L) 
{
	createmeta(L, IA_IMAGE_HANDLE);
	luaL_openlib (L, 0, luaia_image_meths, 0);

	createmeta(L, IA_CONTOUR_HANDLE);
	luaL_openlib (L, 0, luaia_contour_meths, 0);

	createmeta(L, IA_CONTOURS_HANDLE);
	luaL_openlib (L, 0, luaia_contours_meths, 0);

	luaL_openlib (L, LUAIA_LIBNAME, luaia_funcs, 0);
	set_info(L);
	set_iaconsts(L);
	return 1;
}
