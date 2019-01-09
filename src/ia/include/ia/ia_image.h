/*********************************************************************/
/*                                                                   */
/* Copyright (C) 2005, Alexander Marinov, Nadezhda Zlateva           */
/*                                                                   */
/* Project:       ia                                                 */
/* Filename:      ia_image.h                                         */
/* Description:   Image management module                            */
/*                                                                   */
/*********************************************************************/

#ifndef __IA_IMAGE_H
#define __IA_IMAGE_H

#include <ia/ia.h>

#define IA_IMAGE_GRAY  1
#define IA_IMAGE_RGB   0
#define IA_RGB(r,g,b)  (((ia_uint32_t)((r) & 0xFF)) | ((ia_uint32_t)(((g) & 0xFF) << 8)) | ((ia_uint32_t)(((b) & 0xFF) << 16)))
#define IA_RED(c)      ((c)       & 0xFF)
#define IA_GREEN(c)    (((c)>>8)  & 0xFF)
#define IA_BLUE(c)     (((c)>>16) & 0xFF)
#define IA_GRAY(c)     ((ia_uint8_t)((IA_RED(c) + IA_GREEN(c) + IA_BLUE(c))/3))

/*********************************************************************/
/*                              Image API                            */
/*********************************************************************/

/**
	Type ia_image_t

	Defines set ot functions for primitive image manipulations
*/
typedef struct _ia_image_t
{
	/** image horizontal pixels count */
	ia_uint16_t                         width;

	/** image vertical pixels count */
	ia_uint16_t                         height;
	
	/** image pixel format */
	ia_format_t                         format;

	/** true if the image is gray */
	ia_bool_t                           is_gray;

	/** raw image data */
	ia_data_t                           pixels;

	/** marker x position */
	ia_uint32_t                         marker_x;

	/** marker y position */
	ia_uint32_t                         marker_y;

	/** set a pixel color at specified position */
	void (*set_pixel)                   (
		struct _ia_image_t*, /** self */
		ia_uint16_t,         /** x coordinate */
		ia_uint16_t,         /** y coordinate */
		ia_uint32_t          /** color value with bit size depending of the image pixel format */
	);

	/** get a pixel color from specified position */
	ia_uint32_t (*get_pixel)            (
		struct _ia_image_t*, /** self */
		ia_uint16_t,         /** x coordinate */
		ia_uint16_t          /** y coordinate */
	);

	/** fill the image with specified color */
	void (*fill)                        (
		struct _ia_image_t*, /** self */
		ia_uint32_t          /** color value with bit size depending of the image pixel format */
	);

	/** save image to file */
	void (*save)                        (
		struct _ia_image_t*, /** self */
		const ia_string_t    /* file name */
	);

	/** convert an image into 32-bit ARGB pixel format, where the alpha mask is 0 */
	struct _ia_image_t* (*convert_rgb)  (
		struct _ia_image_t* /** self */
	);

	/** convert an image to gray */
	struct _ia_image_t* (*convert_gray) (
		struct _ia_image_t*, /** self */
		ia_format_t          /** pixel format */
	);
	
	/** normalize pixel colors to fit in 8-bit format */
	void (*normalize_colors)            (
		struct _ia_image_t*, /** self */
		ia_int32_t,           /** from min color or 0 if unknown */
		ia_uint32_t,          /** from max color or 0 if unknown */
		ia_int32_t,           /** to min color */
		ia_uint32_t           /** to max color */
	);

	/** inverse */
	void (*inverse)                     (
		struct _ia_image_t* /** self */
	);

	/** determines min and max colors in the image */
	void (*get_min_max)                 (
		struct _ia_image_t*, /** self */
		ia_int32_t  *,       /* return min color */
		ia_uint32_t *        /* return max color */
	);
	
	/** allocate memory and copy this image */
	struct _ia_image_t* (*copy)         (
		struct _ia_image_t* /** self */
	);

	/** destroy image object */
	void (*destroy)                     (
		struct _ia_image_t* /** self */
	);

	/** print binarized image to the console */
	void (*print)                       (
		struct _ia_image_t* /** self */
	);

} ia_image_t, *ia_image_p;


/** load an image from file                */
IA_API ia_image_p ia_image_load   (
	const ia_string_t /** file name        */
);

/** creates new image */
IA_API ia_image_p ia_image_new    (
	ia_uint16_t, /** image width            */
	ia_uint16_t, /** image height           */
	ia_format_t, /** image format           */
	ia_bool_t    /** 1 if the image is gray */
);

#endif /* __IA_IMAGE_H */
