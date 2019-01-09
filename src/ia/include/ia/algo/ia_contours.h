/*********************************************************************/
/*                                                                   */
/* Copyright (C) 2005, Alexander Marinov, Nadezhda Zlateva           */
/*                                                                   */
/* Project:       ia                                                 */
/* Filename:      ia_contours.h                                      */
/* Description:   Contours module interface                          */
/*                                                                   */
/*********************************************************************/

#ifndef __IA_CONTOURS_H
#define __IA_CONTOURS_H

#include <ia/ia_image.h>

/*********************************************************************/
/*                     Contours API interface                        */
/*********************************************************************/
typedef enum
{
	CONTOUR_POINT_INSIDE=-1,
	CONTOUR_POINT_OVER=0,
	CONTOUR_POINT_OUTSIDE=+1
} contour_point_location_t;

typedef struct _ia_contour
{
	ia_pos_p*    points;
	ia_int32_t   npoints;
	ia_int32_t   nrefs;

	void (*add)            (
		struct _ia_contour*,
		ia_int32_t,
		ia_int32_t
	);

	ia_int32_t (*count_at) (
		struct _ia_contour*,
		ia_int32_t, 
		ia_int32_t
	);

	contour_point_location_t (*point_in) (
		struct _ia_contour*,
		ia_int32_t, 
		ia_int32_t
	);

	ia_bool_t (*in_contour) (
		struct _ia_contour*,
		struct _ia_contour*
	);

	ia_int32_t (*fill)     (
		struct _ia_contour*,
		ia_image_p,              /* image to fill to */
		ia_uint32_t,             /* fill color */
		ia_image_p               /* history map image */
	);

	struct _ia_contour* (*find_encapsulating_contour) (
		struct _ia_contour*
	);

	void (*add_ref)     (
		struct _ia_contour*
	);

	void (*destroy)        (
		struct _ia_contour*
	);

} ia_contour_t, *ia_contour_p;

typedef struct _ia_contours
{
	ia_contour_p*  contours;
	ia_int32_t     ncontours;
	ia_int32_t     nrefs;

	void (*add)     (
		struct _ia_contours*,
		ia_contour_p
	);

	void (*add_ref)     (
		struct _ia_contours*
	);

	void (*destroy) (
		struct _ia_contours*
	);

} ia_contours_t, *ia_contours_p;

IA_API ia_contour_p  ia_contour_new      (void);
IA_API ia_contours_p ia_contours_new     (void);
IA_API ia_contours_p ia_contours_find    (ia_image_p);

#define foreach_point_in_contour(p, contour)             \
	ia_pos_p p;                                          \
	ia_int32_t _##p;                                     \
	if (contour)                                         \
	for (_##p=0, p=contour->points[0]; _##p<contour->npoints; p=contour->points[++_##p])

#define foreach_contour_in_contours(c, cs)               \
	ia_contour_p c;                                      \
	ia_contour_p* _##c;                                  \
	if (cs)                                              \
	for (_##c=(cs)->contours, c=*_##c; *_##c; c=*++_##c)

#endif /* __IA_CONTOURS_H */
