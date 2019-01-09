/*********************************************************************/
/*                                                                   */
/* Copyright (C) 2005, Alexander Marinov, Nadezhda Zlateva           */
/*                                                                   */
/* Project:       ia                                                 */
/* Filename:      ia_vector.h                                        */
/* Description:   Vector arithmetics                                 */
/*                                                                   */
/*********************************************************************/

#ifndef __IA_VECTOR_H
#define __IA_VECTOR_H

#include <ia/ia.h>

typedef struct _ia_vector_t
{
	ia_format_t format;
	ia_value_t *coords;
	ia_uint32_t count;
	ia_uint32_t coord_alloc;

	ia_value_t (*get)( struct _ia_vector_t*, ia_uint32_t );
	void (*set)( struct _ia_vector_t*, ia_uint32_t, ia_value_t );
	ia_uint32_t (*get_count)( struct _ia_vector_t* );
	ia_double_t (*length)( struct _ia_vector_t* );
	void (*scale)( struct _ia_vector_t*, ia_double_t );
	void (*normalize)( struct _ia_vector_t* );
	void (*negate)( struct _ia_vector_t*);
	void (*add)( struct _ia_vector_t*, struct _ia_vector_t* );
	void (*sub)( struct _ia_vector_t*, struct _ia_vector_t* );
	void (*scalar_mul)( struct _ia_vector_t*, struct _ia_vector_t* );
	void (*cross_mul)( struct _ia_vector_t*, struct _ia_vector_t* );
	void (*destroy)( struct _ia_vector_t* );
} ia_vector_t, *ia_vector_p;

ia_vector_p ia_vector_new(ia_int32_t count, ia_format_t type);

#endif /* __IA_VECTOR_H */
