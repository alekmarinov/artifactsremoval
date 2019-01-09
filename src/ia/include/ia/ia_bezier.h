/*********************************************************************/
/*                                                                   */
/* Copyright (C) 2005, Alexander Marinov, Nadezhda Zlateva           */
/*                                                                   */
/* Project:       ia                                                 */
/* Filename:      ia_bezier.h                                        */
/* Description:   Bezier curve module interface                      */
/* Source:                                                           */
/* Paul Bourke, http://astronomy.swin.edu.au/~pbourke/curves/bezier/ */
/*                                                                   */
/*********************************************************************/

#ifndef __IA_BEZIER_H
#define __IA_BEZIER_H

#include <ia/ia.h>

/*********************************************************************/
/*                         Bezier API interface                      */
/*********************************************************************/

/** Computes a dot from bezier curve */
IA_API ia_pos_t ia_bezier_draw(
	ia_pos_p,                     /* control curve points */
	ia_int32_t,                   /* points count */
	ia_double_t                   /* a double in range [0, 1] traverses different curve dots */
);

#endif /* __IA_BEZIER_H */
