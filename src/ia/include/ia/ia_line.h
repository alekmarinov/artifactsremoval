/*********************************************************************/
/*                                                                   */
/* Copyright (C) 2005, Alexander Marinov, Nadezhda Zlateva           */
/*                                                                   */
/* Project:       ia                                                 */
/* Filename:      ia_line.h                                          */
/* Description:   Line module interface                              */
/*                                                                   */
/*********************************************************************/

#ifndef __IA_LINE_H
#define __IA_LINE_H

#include <ia/ia.h>

/*********************************************************************/
/*                         Line API interface                        */
/*********************************************************************/

typedef void (*ia_line_call_t)(void* /* user param */, ia_int32_t /* x */, ia_int32_t /* y */);

/** Draw a line */
IA_API void ia_line_draw(
	ia_int32_t,                   /* start point x */
	ia_int32_t,                   /* start point y */
	ia_int32_t,                   /* end point x */
	ia_int32_t,                   /* end point y */
	ia_rect_p,                    /* clipping region */
	ia_line_call_t,               /* callback for each line point */
	void*                         /* callback parameter */
);

#endif /* __IA_LINE_H */
