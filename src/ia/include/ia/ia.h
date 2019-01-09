/*********************************************************************/
/*                                                                   */
/* Copyright (C) 2005, Alexander Marinov, Nadezhda Zlateva           */
/*                                                                   */
/* Project:       ia                                                 */
/* Filename:      ia.h                                               */
/* Description:   Image Analyser Interface                           */
/*                                                                   */
/*********************************************************************/

#ifndef __IA_H
#define __IA_H

#ifndef IA_API
#  ifdef IA_DLL
#    define IA_API __declspec(dllexport)
#else
#    define IA_API extern
#  endif
#endif

#define IA_VERSION "1.0"

/*********************************************************************/
/*                      Image Analyser definitions                   */
/*********************************************************************/

/**
	Type ia_format_t 

	Defines enumeration of all IA variable types
*/
typedef enum
{
	IAT_UINT_8,	IAT_UINT_16, IAT_UINT_24, IAT_UINT_32, 
	IAT_INT_8,  IAT_INT_16,  IAT_INT_24,  IAT_INT_32,
	IAT_FLOAT,  IAT_DOUBLE,  IAT_BOOL
} ia_format_t;

/**
	Some IA variable types
*/
typedef unsigned char  ia_uint8_t;
typedef unsigned short ia_uint16_t;
typedef ia_uint8_t     ia_uint24_t[3];
typedef unsigned long  ia_uint32_t;
typedef signed char    ia_int8_t;
typedef signed short   ia_int16_t;
typedef ia_int8_t      ia_int24_t[3];
typedef signed int     ia_int32_t;
typedef float          ia_float_t;
typedef double         ia_double_t;
typedef ia_uint8_t     ia_bool_t;
typedef char*          ia_string_t;

typedef struct
{
	ia_double_t re, im;
} ia_complex_t, *ia_complex_p;

/**
	Common used macros
*/
#define IA_TRUE        1
#define IA_FALSE       0
#define IA_MAX_INT     ((ia_int32_t)0x7FFFFFFF)
#define IA_MIN_INT     ((ia_int32_t)0x80000000)
#define IA_MAX_UINT    ((ia_uint32_t)0xFFFFFFFF)
#define IA_MIN_UINT    ((ia_uint32_t)0x00000000)
#define MIN(a, b)      ((a)<(b)?a:b)
#define MAX(a, b)      ((a)>(b)?a:b)
#define ASSERT(condition)	\
	if (!(condition))  \
		fprintf(stderr, "%s <%d> assert:", __FILE__, __LINE__), \
		fprintf(stderr

/** 
	Type ia_rect_t 

	Defines rectangular area
*/
typedef struct
{
	ia_int32_t l, t, r, b;
} ia_rect_t, *ia_rect_p;

#define ia_set_rect   (rect, l, t, r, b) \
	rect.left   = l; \
	rect.top    = t; \
	rect.right  = r; \
	rect.bottom = b;

#define ia_set_rect_p(rect, l, t, r, b) \
	rect->left   = l; \
	rect->top    = t; \
	rect->right  = r; \
	rect->bottom = b;

/** 
	Type ia_pos_t, ia_size_t 

	Defines position
*/
typedef struct
{
	ia_int32_t x, y;
} ia_pos_t, *ia_pos_p, ia_size_t, *ia_size_p;

/**
	Type ia_data_t
	
	Describes raw data
*/
typedef struct 
{
	ia_uint32_t size;
	void* data;
} ia_data_t, *ia_data_p;

/** 
	Arbitraty type
*/
typedef union
{
	ia_int8_t bt;
	ia_int16_t s;
	ia_int32_t n;
	ia_float_t f;
	ia_double_t d;
	ia_bool_t bl;
} ia_value_t;

extern const ia_value_t ia_0;

IA_API void ia_format_min_max(ia_format_t format, ia_int32_t* min, ia_uint32_t* max);
IA_API ia_bool_t ia_format_signed(ia_format_t format);
IA_API const ia_string_t ia_format_name(ia_format_t format);
IA_API ia_uint16_t ia_format_size(ia_format_t format);
IA_API ia_double_t ia_value_todouble(ia_format_t format, ia_value_t value);
IA_API ia_value_t ia_double_tovalue(ia_format_t format, ia_double_t number);

#endif /* __IA_H */
