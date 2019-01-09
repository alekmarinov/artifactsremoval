/*********************************************************************/
/*                                                                   */
/* Copyright (C) 2005, Alexander Marinov, Nadezhda Zlateva           */
/*                                                                   */
/* Project:       ia                                                 */
/* Filename:      ia_vector.c                                        */
/* Description:   Vector arithmetics                                 */
/*                                                                   */
/*********************************************************************/

#include <math.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <ia/ia_vector.h>


static ia_value_t ia_vector_get(ia_vector_p self, ia_uint32_t index)
{
	if (index<self->count)
		return self->coords[index];
	return ia_0;
}

static void ia_vector_set(ia_vector_p self, ia_uint32_t index, ia_value_t value)
{
	if (index<self->count)
		self->coords[index]=value;
}

static ia_uint32_t ia_vector_get_count(ia_vector_p self)
{
	return self->count;
}

static ia_double_t ia_vector_length(ia_vector_p self)
{
	ia_double_t length=0;
	ia_uint32_t i;
	for (i=0; i<self->count; i++)
	{
		ia_double_t value=ia_value_todouble(self->format, self->get(self, i));
		length+=value*value;
	}
	return sqrt(length);
}

static void ia_vector_scale(ia_vector_p self, ia_double_t multiplier)
{
	ia_uint32_t i;
	for (i=0; i<self->count; i++)
	{
		ia_double_t value=ia_value_todouble(self->format, self->get(self, i));
		self->set(self, i, ia_double_tovalue(self->format, value*multiplier));
	}
}

static void ia_vector_normalize(ia_vector_p self)
{
	self->scale(self, 1/self->length(self));
}

static void ia_vector_negate(ia_vector_p self)
{
	self->scale(self, -1);
}

static void ia_vector_add(ia_vector_p self, ia_vector_p vector)
{
	ia_uint32_t i;
	ASSERT(self->count == vector->count), "Vectors sizes don't match %d != %d", self->count, vector->count);
	for (i=0; i<self->count; i++)
	{
		ia_double_t value1=ia_value_todouble(self->format, self->get(self, i));
		ia_double_t value2=ia_value_todouble(vector->format, vector->get(vector, i));
		self->set(self, i, ia_double_tovalue(self->format, value1+value2));
	}
}

static void ia_vector_sub(ia_vector_p self, ia_vector_p vector)
{
	ia_uint32_t i;
	ASSERT(self->count == vector->count), "Vectors sizes don't match %d != %d", self->count, vector->count);
	for (i=0; i<self->count; i++)
	{
		ia_double_t value1=ia_value_todouble(self->format, self->get(self, i));
		ia_double_t value2=ia_value_todouble(vector->format, vector->get(vector, i));
		self->set(self, i, ia_double_tovalue(self->format, value1-value2));
	}
}

static ia_double_t ia_vector_scalar_mul(ia_vector_p self, ia_vector_p vector)
{
	ia_uint32_t i;
	ia_double_t result=0;
	ASSERT(self->count == vector->count), "Vectors sizes don't match %d != %d", self->count, vector->count);
	for (i=0; i<self->count; i++)
	{
		ia_double_t value1=ia_value_todouble(self->format, self->get(self, i));
		ia_double_t value2=ia_value_todouble(vector->format, vector->get(vector, i));
		result+=value1*value2;
	}
	return result;
}

static void ia_vector_cross_mul(ia_vector_p self, ia_vector_p vector)
{
	ia_double_t a[3], b[3];
	ia_uint32_t i;
	ASSERT(self->count == 3 && vector->count==3), "Cross multiply is only valid in 3D 1:%d or 2:%d != 3", self->count, vector->count);
	for (i=0; i<3; i++)
	{
		a[i]=ia_value_todouble(self->format, self->get(self, i));
		b[i]=ia_value_todouble(vector->format, vector->get(vector, i));
	}
	a[0]=a[1]*b[2]-a[2]*b[1];
	a[1]=a[2]*b[0]-a[0]*b[2];
	a[2]=a[0]*b[1]-a[1]*b[0];
	for (i=0; i<3; i++)
	{
		self->set(self, i, ia_double_tovalue(self->format, a[i]));
	}
}

static void ia_vector_destroy(ia_vector_p self)
{
	free(self->coords);
	free(self);
}

ia_vector_p ia_vector_new(ia_int32_t count, ia_format_t format)
{
	ia_vector_p vector;
	ia_uint32_t coord_alloc=ia_format_size(format);
	coord_alloc=(coord_alloc+7)>>3;
	vector=(ia_vector_p)malloc(sizeof(ia_vector_t));
	vector->coord_alloc=coord_alloc;
	vector->count=count;
	vector->format=format;
	vector->coords=(ia_value_t*)malloc(coord_alloc * count);
	memset(vector->coords, 0, coord_alloc * count);

	vector->get=ia_vector_get;
	vector->set=ia_vector_set;
	vector->get_count=ia_vector_get_count;
	vector->length=ia_vector_length;
	vector->scale=ia_vector_scale;
	vector->normalize=ia_vector_normalize;
	vector->negate=ia_vector_negate;
	vector->add=ia_vector_add;
	vector->sub=ia_vector_sub;
	vector->scalar_mul=ia_vector_scalar_mul;
	vector->cross_mul=ia_vector_cross_mul;
	vector->destroy=ia_vector_destroy;
	return vector;
}
