/*********************************************************************/
/*                                                                   */
/* Copyright (C) 2005, Alexander Marinov, Nadezhda Zlateva           */
/*                                                                   */
/* Project:       ia                                                 */
/* Filename:      ia_distance_trasnform.h                            */
/* Description:   Algorithms for Distance Trasnform                  */
/*                                                                   */
/*********************************************************************/

#ifndef __IA_DISTANCE_TRANSFORM_H
#define __IA_DISTANCE_TRANSFORM_H

#include <ia/ia_image.h>

/*********************************************************************/
/*                Distance Trasnform API interface                   */
/*********************************************************************/

#define DT_INF 0x7FFFFFFF

IA_API ia_image_p ia_distance_transform_mask           
	(ia_int32_t, 
	ia_int32_t, 
	ia_int32_t, 
	ia_int32_t, 
	ia_int32_t, 
	ia_int32_t);

IA_API void ia_distance_transform_parallel  
	(ia_image_p, 
	ia_image_p, 
	ia_int32_t *, 
	ia_uint32_t *);

IA_API void ia_distance_transform_sequential
	(ia_image_p, 
	ia_image_p, 
	ia_int32_t *,
	ia_uint32_t *);

#endif /* __IA_DISTANCE_TRANSFORM_H */
