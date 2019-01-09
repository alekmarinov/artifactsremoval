/*********************************************************************/
/*                                                                   */
/* Copyright (C) 2005, Alexander Marinov, Nadezhda Zlateva           */
/*                                                                   */
/* Project:       ia                                                 */
/* Filename:      ia_distance_trasnform.c                            */
/* Description:   Algorithms for Distance Trasnform                  */
/*                                                                   */
/*********************************************************************/
#define CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include <stdio.h>
#include <stdlib.h>
#include <ia/algo/ia_distance_transform.h>

ia_image_p ia_distance_transform_mask(ia_int32_t mask_size, ia_int32_t a, ia_int32_t b, ia_int32_t c, ia_int32_t d, ia_int32_t e)
{
	ia_int32_t center;
	ia_image_p mask=ia_image_new(mask_size, mask_size, IAT_INT_32, IA_IMAGE_GRAY);
	mask->fill(mask, DT_INF);

	center=(mask_size+1)/2-1;
	switch(mask_size)
	{
		case 7:
			mask->set_pixel(mask, center-2, center-3, e);
			mask->set_pixel(mask, center-1, center-3, d);
			mask->set_pixel(mask, center+1, center-3, d);
			mask->set_pixel(mask, center+2, center-3, e);
			mask->set_pixel(mask, center-3, center-2, e);
			mask->set_pixel(mask, center+3, center-2, e);
			mask->set_pixel(mask, center-3, center-1, d);
			mask->set_pixel(mask, center+3, center-1, d);
			mask->set_pixel(mask, center-2, center+3, e);
			mask->set_pixel(mask, center-1, center+3, d);
			mask->set_pixel(mask, center+1, center+3, d);
			mask->set_pixel(mask, center+2, center+3, e);
			mask->set_pixel(mask, center-3, center+2, e);
			mask->set_pixel(mask, center+3, center+2, e);
			mask->set_pixel(mask, center-3, center+1, d);
			mask->set_pixel(mask, center+3, center+1, d);

		case 5:
			mask->set_pixel(mask, center-1, center-2, c);
			mask->set_pixel(mask, center+1, center-2, c);
			mask->set_pixel(mask, center-2, center-1, c);
			mask->set_pixel(mask, center+2, center-1, c);
			mask->set_pixel(mask, center-2, center+1, c);
			mask->set_pixel(mask, center+2, center+1, c);
			mask->set_pixel(mask, center-1, center+2, c);
			mask->set_pixel(mask, center+1, center+2, c);

		case 3:
			mask->set_pixel(mask, center-1, center-1, b);
			mask->set_pixel(mask, center+1, center-1, b);
			mask->set_pixel(mask, center-1, center+1, b);
			mask->set_pixel(mask, center+1, center+1, b);
			mask->set_pixel(mask, center, center-1, a);
			mask->set_pixel(mask, center-1, center, a);
			mask->set_pixel(mask, center+1, center, a);
			mask->set_pixel(mask, center, center+1, a);
			mask->set_pixel(mask, center, center, 0);

		break;
		default:
			ASSERT(0), "ia_distance_transform_mask -> mask_size=%d is not supported. It must be one of 3,5 or 7\n", mask_size);
	}

	return mask;
}

void ia_distance_transform_parallel(ia_image_p in, ia_image_p mask, ia_int32_t *min, ia_uint32_t *max)
{
	int mask_center=(mask->height+1)/2-1;
	ia_bool_t change=1;
	ia_uint32_t value, old_value, neigh_value, new_max;
	int cycles=0;
	*min=0; *max=0xFFFFFF;
	
	while(change)
	{
		int i,j,n,m,neighbour_x,neighbour_y;
		cycles++;
		change=0;
		new_max=0;

		for(j=0; j<in->height; j++)
			for(i=0; i<in->width; i++)
			{
				value=DT_INF;
				old_value=in->get_pixel(in, i, j);

				for(n=0; n<mask->height; n++)
					for(m=0; m<mask->width; m++)
					{
						neighbour_x=i-mask_center+m;
						neighbour_y=j-mask_center+n;

						if (neighbour_x<0 || neighbour_y<0 || neighbour_x>=in->width || neighbour_y>=in->height)
							neigh_value=DT_INF;
						else
							neigh_value=in->get_pixel(in, neighbour_x, neighbour_y);

						value=MIN(value, mask->get_pixel(mask, m, n)+neigh_value); 
					}
				if (value!=old_value) 
				{
					in->set_pixel(in, i, j, value);
					change=1;
				}
				if (value > new_max) new_max=value;
			}
			if (new_max < *max) *max=new_max;
	}
}

void ia_distance_transform_sequential(ia_image_p in, ia_image_p mask, ia_int32_t *min, ia_uint32_t *max)
{
	ia_int32_t mask_center=(mask->height+1)/2-1;
	ia_int32_t i,j,n,m,neighbour_x,neighbour_y;
	ia_uint32_t value, old_value, neigh_value;
	*min=0; *max=0;
	/* forward pass */
	for(j=0; j<in->height; j++)
	for(i=0; i<in->width; i++)
	{
		value=DT_INF;
		old_value=in->get_pixel(in, i, j);

		for(n=0; n<=mask_center; n++)
		for(m=0; m<mask->width && (n!=mask_center || m<=mask_center); m++)
		{
			neighbour_x=i-mask_center+m;
			neighbour_y=j-mask_center+n;

			if (neighbour_x<0 || neighbour_y<0 || neighbour_x>=in->width || neighbour_y>=in->height)
			{
				neigh_value=DT_INF;
			}
			else
			{
				neigh_value=in->get_pixel(in, neighbour_x, neighbour_y);
			}

			value= MIN(value, mask->get_pixel(mask, m, n) + neigh_value);
		}

		if (value != old_value) 
		{
			in->set_pixel(in, i, j, value);
		}
	}

	/* backward pass */
	for(j=in->height-1; j>=0; j--)
	for(i=in->width-1; i>=0; i--)
	{
		value=DT_INF;
		old_value=in->get_pixel(in, i, j);

		for(n=mask->height-1; n>=mask_center; n--)
		for(m=mask->width-1; m>=0 && (n!=mask_center || m>=mask_center); m--)
		{

			neighbour_x=i-mask_center+m;
			neighbour_y=j-mask_center+n;

			if (neighbour_x<0 || neighbour_y<0 || neighbour_x>=(*in).width || neighbour_y>=(*in).height) 
			{
				neigh_value=DT_INF;
			}
			else
			{
				neigh_value=in->get_pixel(in, neighbour_x, neighbour_y);
			}

			value= MIN(value, mask->get_pixel(mask, m, n) + neigh_value);
		}

		if (value != old_value) 
		{
			in->set_pixel(in, i, j, value);
		}

		if (value>*max) *max=value;
	}
}
