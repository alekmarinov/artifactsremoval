/*********************************************************************/
/*                                                                   */
/* Copyright (C) 2005, Alexander Marinov, Nadezhda Zlateva           */
/*                                                                   */
/* Project:       ia                                                 */
/* Filename:      ia_contours.c                                      */
/* Description:   Contours module                                    */
/*                                                                   */
/*********************************************************************/
#define CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include <malloc.h>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <assert.h>
#include <ia/algo/ia_contours.h>

ia_contours_p ia_contours_find(ia_image_p);

static void          ia_contour_add        (ia_contour_p, ia_int32_t, ia_int32_t);
static ia_int32_t    ia_contour_count_at   (ia_contour_p, ia_int32_t, ia_int32_t);
static contour_point_location_t ia_contour_point_in   (ia_contour_p, ia_int32_t, ia_int32_t);
static ia_bool_t     ia_contour_in_contour (ia_contour_p, ia_contour_p);
static ia_int32_t    ia_contour_fill       (ia_contour_p, ia_image_p, ia_uint32_t, ia_image_p);
static struct _ia_contour*  ia_find_encapsulating_contour(ia_contour_p);
static void          ia_contour_add_ref    (ia_contour_p);
static void          ia_contour_destroy    (ia_contour_p);
static ia_bool_t     ia_contour_find_any   (ia_image_p, ia_image_p, ia_int32_t, ia_int32_t, ia_int32_t*, ia_int32_t*);
static ia_bool_t     ia_contour_find_next  (ia_image_p, ia_image_p, ia_int32_t, ia_int32_t, ia_int32_t*, ia_int32_t*, ia_int32_t *);
static void          ia_contours_add       (ia_contours_p, ia_contour_p);
static void          ia_contours_add_ref   (ia_contours_p);
static void          ia_contours_destroy   (ia_contours_p);

ia_contour_p         ia_contour_new        ( void );
ia_contours_p        ia_contours_new       ( void );
ia_contours_p        ia_contours_find      (ia_image_p);

ia_contour_p ia_contour_new( void )
{
	ia_contour_p contour = (ia_contour_p)malloc(sizeof(ia_contour_t));

	contour->points      = 0;
	contour->npoints     = 0;
	contour->nrefs       = 1;

	contour->add         = ia_contour_add;
	contour->count_at    = ia_contour_count_at;
	contour->point_in    = ia_contour_point_in;
	contour->in_contour  = ia_contour_in_contour;	
	contour->fill        = ia_contour_fill;
	contour->find_encapsulating_contour = ia_find_encapsulating_contour;
	contour->add_ref     = ia_contour_add_ref;
	contour->destroy     = ia_contour_destroy;

	return contour;
}

static void ia_contour_add(ia_contour_p self, ia_int32_t x, ia_int32_t y)
{
	self->points                   = (ia_pos_p*)realloc(self->points, sizeof(ia_pos_t)*(self->npoints+1));
	self->points[self->npoints]    = (ia_pos_p)malloc(sizeof(ia_pos_t));
	self->points[self->npoints]->x = x;
	self->points[self->npoints]->y = y;
	self->npoints++;
}

static void ia_contour_add_ref (ia_contour_p self)
{
	self->nrefs++;
}

static void ia_contour_destroy (ia_contour_p self)
{
	ia_int32_t i;
	assert(self->nrefs>0);
	self->nrefs--;
	if (self->nrefs == 0)
	{
		for (i=0; i<self->npoints; i++)
			free(self->points[i]);
		free(self->points);
		free(self);
	}
}

static ia_bool_t ia_contour_find_any(ia_image_p img, ia_image_p history, ia_int32_t start_x, ia_int32_t start_y, ia_int32_t* x, ia_int32_t* y)
{
	for (*y=start_y; (*y)<img->height; (*y)++)
		if ((*y)==start_y)
		{
			for (*x=start_x; (*x)<img->width;  (*x)++)
				if (!history->get_pixel(history, *x, *y) && img->get_pixel(img, *x, *y))
				{
						return 1;
				}
		}
		else
		{
			for (*x=0; (*x)<img->width;  (*x)++)
				if (!history->get_pixel(history, *x, *y) && img->get_pixel(img, *x, *y))
				{
						return 1;
				}
		}
	return 0;
}

static ia_int32_t ia_contour_count_at(ia_contour_p self, ia_int32_t x, ia_int32_t y)
{
	ia_int32_t j=0, count=0;
	while (j<self->npoints)
	{
		if (self->points[j]->y == y && self->points[j]->x == x) count++;
		j++;
	}
	return count;
}

static contour_point_location_t ia_contour_point_in(ia_contour_p self, ia_int32_t x, ia_int32_t y)
{
	ia_int32_t count=self->count_at(self, x, y);
	if (count) 
		return CONTOUR_POINT_OVER;
	else
	{
		x--;
		for(; x>=0; x--)
		{
			count+=self->count_at(self, x, y);
		}
	}
	return (count&1)?CONTOUR_POINT_INSIDE:CONTOUR_POINT_OUTSIDE;
}

/*
 * checks if specified contour lies inside this contour
 */
static ia_bool_t ia_contour_in_contour(ia_contour_p self, ia_contour_p contour)
{
	contour_point_location_t location=self->point_in(self, contour->points[0]->x, contour->points[0]->y);
	return location == CONTOUR_POINT_INSIDE || location == CONTOUR_POINT_OVER;
}

/*
 * fill contour with specified color and return points number inside of contour
 */
static ia_int32_t ia_contour_fill(ia_contour_p self, ia_image_p img, ia_uint32_t color, ia_image_p history_map)
{
	ia_int32_t points_inside=0;
	ia_int32_t pen, min_y=IA_MAX_INT, max_y=-IA_MAX_INT, min_x=IA_MAX_INT, max_x=-IA_MAX_INT, x, y;
	foreach_point_in_contour(point, self)
	{
		if (min_y>point->y) min_y = point->y;
		if (max_y<point->y) max_y = point->y;
		if (min_x>point->x) min_x = point->x;
		if (max_x<point->x) max_x = point->x;
	}

	for (y=min_y; y<=max_y; y++)
	{
		ia_int32_t in=0;
		ia_int32_t state_changed=0;
		ia_int32_t pts_cnt=0;
		pen=0; 
		for (x=min_x; x<=max_x; x++)
		{
			ia_int32_t pts=history_map?history_map->get_pixel(history_map, x, y):self->count_at(self, x, y);

			state_changed=0;
			if (pts > 0)
			{
				if (!in)
				{
					in=1;
					pts_cnt=pts;
				} else
				{
					pts_cnt+=pts;
				}
				
				points_inside++;
			} else
			{
				if (in)
				{
					in=0;
					state_changed=1;
				}
			}

			if (state_changed && (pts_cnt & 1) == 1) pen=!pen;
			if (!pts) pts_cnt=0;

			if (pen && !in)
			{
				points_inside++;
				img->set_pixel(img, x, y, color);
			}
		}
	}
	return points_inside;
}

/*
 * find encapsulating contour (with correct Jordan rule) of points from the current contour
 */
static int vector_angle(ia_pos_t* r1, ia_pos_t* center, ia_pos_t* r2)
{
	double l1, l2, a;
	double v1x, v1y, v2x, v2y;
	double dot_product, direction;
	v1x=center->x-r1->x;
	v1y=center->y-r1->y;
	v2x=center->x-r2->x;
	v2y=center->y-r2->y;

	l1=sqrt(v1x * v1x + v1y * v1y);
	l2=sqrt(v2x * v2x + v2y * v2y);

	/* compute dot product */
	dot_product=(v1x*v2x+v1y*v2y)/(l1*l2);

	/* compute vector product to determine orientation */
	direction=v2x*v1y-v2y*v1x;

	a=acos(dot_product);
	if (direction>0)
	{
		a=2*M_PI-a;
	}

	return (int)(a*180/M_PI);
}

static struct _ia_contour* ia_find_encapsulating_contour(ia_contour_p self)
{
	int i;
	int a1, a2, ad;
	double len;
	ia_contour_p contour=ia_contour_new();
	ia_pos_t current_point, next_point, prev_point;

	/* find the most top-left point */
	current_point.x=IA_MAX_INT;
	current_point.y=IA_MAX_INT;
	for (i=0; i<self->npoints; i++)
	{
		if (self->points[i]->y < current_point.y || (self->points[i]->y == current_point.y && self->points[i]->x < current_point.x))
		{
			current_point=*self->points[i];
		}
	}
	prev_point.x=current_point.x-1;
	prev_point.y=current_point.y;
	next_point.x=current_point.x+1;
	next_point.y=current_point.y;

	a1=vector_angle(&prev_point, &current_point, &next_point);

	do 
	{
		double tlen;
		contour->add(contour, current_point.x, current_point.y);
		ad=360;
		len=0;
		for (i=0; i<self->npoints; i++)
		{
			if (self->points[i]->x != current_point.x || self->points[i]->y != current_point.y)
			{
				//a2=atan2(current_point.y-self->points[i]->y, current_point.x-self->points[i]->x);
				a2=vector_angle(&prev_point, &current_point, self->points[i]);
				tlen=sqrt( 
						(self->points[i]->x - current_point.x)*(self->points[i]->x - current_point.x) + 
						(self->points[i]->y - current_point.y)*(self->points[i]->y - current_point.y)
				);

				if (a1-a2<ad || ( (a1-a2==ad) && tlen > len) )
				{
					ad=a1-a2;
					len=tlen;
					next_point=*self->points[i];
				}
			}
		}
		prev_point=current_point;
		current_point=next_point;
		a1=a2;
	} while (current_point.x != contour->points[0]->x || current_point.y != contour->points[0]->y);

	return contour;
}

static ia_bool_t ia_contour_find_next(ia_image_p img, ia_image_p history, ia_int32_t start_j, ia_int32_t start_i, ia_int32_t* j, ia_int32_t* i, ia_int32_t *d)
{
	static ia_pos_t neighbours[8]= { {-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1} };
	ia_int32_t nd=*d;
	ia_int32_t next_i, next_j;
	do
	{
		nd=(nd+1)&7;
		next_i=*i+neighbours[nd].y;
		next_j=*j+neighbours[nd].x;
		if (next_j>=0 && next_j<img->width && next_i>=0 && next_i<img->height)
		{
			if ( (next_j==start_j) && (next_i==start_i))
			{
				*d=(nd+4)&7;
				*j=next_j;
				*i=next_i;
				/* contour done */
				return 2;
			}
			else
			if (img->get_pixel(img, next_j, next_i))
			{
				*d=(nd+4)&7;
				*j=next_j;
				*i=next_i;
				/* new contour pixel */
				return 1;
			}
		}
	} while (nd!=*d);
	/* 1 isolated pixel */
	return 0;
}

ia_contours_p ia_contours_find(ia_image_p img)
{
	/* container of contours */
	ia_contours_p contours=ia_contours_new();

	ia_int32_t 
		x, y,        /* contour pixel*/
		d,           /* 0 <= d <= 7, 
						In the matrix below
						7 6 5
						0 x 4
						1 2 3
						x is 8-connected pixel, with 8 neighbours
						variable d represents vector used for rotation in counter-clockwise direction 
						for finding the next contour pixel. On moving to the next pixel 
						this vector is inversed with the formule d = (d+4) mod 8;
                     */
		start_x, start_y, /* the position from where we start looking for the first contour pixel */
		first_d,     /* The direction from where we are moving to the 2nd pixel.
						We are using this variable when the contour achieves the start pixel,
						we are performing additional check looking the direction for the next pixel 
						and if they are the same then we stop here, otherwise the contours continues.
						Here is an example where we need this check:
						13
						2
						where 1 is our start pixel, 2 is the 2nd. We enter in '2' from direction 5.
						The next pixel will be '1' which is the first pixel which is our stop condition.
						But we should not stop here so we check the direction for the next pixel
						(= 0). 5!=0 so we continue with pixel '3'. After '3' is '1' and again we are doing the
						additional check above. The next pixel is '2' with direction 5. 5==5 so we stop here;
					 */
		y_dir,       /* -1 if the previous pixel is above the current or +1 otherwise */
		prev_y_dir,  /* y_dir for the previous pixel. If current y_dir != prev_y_dir then 
						we have up-down or down-up case, otherwise the case is up-up or down-down.
						To achieve Jordan rule in horizontal direction for determining that a pixel lies 
						inside of a closed contour, we want to have even count of contour pixels for each
						horizontal line. So we are fixing the number of the neighbour contour pixels with 
						the same x to be odd in case of up-up or down-down and even otherwise;
					 */
		prev_x, prev_y;/* The previous contour pixel. When we want to fix the pixel number in Jordan sense,
							we are on the next pixel occurring immediatly after the line we want to fix. 
							So in case of fix we are doubling the pixel - prev_x, prev_y;
					 */

	/* A temporary image, where we mark the area occuppied by a contour, 
	so to be skipped when looking for any start contour point */
	ia_image_p history=ia_image_new(img->width, img->height, IAT_UINT_8, IA_IMAGE_GRAY);

	/* stores the result of ia_contour_find_next which can be: 
		0 - found one isolated pixel
		1 - found a pixel
		2 - found the start pixel
	 */
	ia_bool_t find_result;

	/* looking for all contours starting from the upper-left corner */
	start_x=0; start_y=0;

	/* Searches for first contour pixel */
	while (ia_contour_find_any(img, history, 0, 0, &x, &y))
	{
		/* container of pixels */
		ia_contour_p contour=ia_contour_new();

		/* the number of contour neighbour pixels with same x position */
		ia_int32_t pts=1;

		/* rotate from direction 0 which we already know that is clear.*/
		d=0; 

		/* FIXME: to be clarified! */
		prev_y_dir=+1;

		start_x=x; 
		start_y=y;

		/* mark first direction as not determined */
		first_d=-1;

		do
		{
			history->set_pixel(history, x, y, 1+history->get_pixel(history, x, y));
			contour->add(contour, x, y);
			prev_x=x; prev_y=y;
			find_result=ia_contour_find_next(img, history, start_x, start_y, &x, &y, &d);
			if (prev_y!=y)
			{
				if (prev_y>y)
				{
					y_dir=-1;
				} else
				{
					y_dir=+1;
				}
				if (y_dir!=prev_y_dir)
				{
					if (pts&1)
					{
						contour->add(contour, prev_x, prev_y);
						history->set_pixel(history, prev_x, prev_y, 1+history->get_pixel(history, prev_x, prev_y));
					}
				} else
				{
					if ((pts&1) == 0)
					{
						contour->add(contour, prev_x, prev_y);
						history->set_pixel(history, prev_x, prev_y, 1+history->get_pixel(history, prev_x, prev_y));
					}
				}

				pts=1;
 				prev_y_dir = y_dir;
			}
			else
			{
				pts++;
			}

			if (first_d==-1) first_d=d;
			if (find_result==2)
			{
				prev_x=x; prev_y=y;
				ia_contour_find_next(img, history, start_x, start_y, &x, &y, &d);
				if (first_d != d)
				{
					find_result=1;
				} else
				{
					if (pts&1)
					{
						contour->add(contour, prev_x, prev_y);
						history->set_pixel(history, prev_x, prev_y, 1+history->get_pixel(history, prev_x, prev_y));
					}
				}
			}
		} while (find_result==1);

  		contour->fill(contour, history, 1, 0);
		contours->add(contours, contour);
	}
	return contours;
}

ia_contours_p ia_contours_new( void )
{
	ia_contours_p self=(ia_contours_p)malloc(sizeof(ia_contours_t));
	self->contours=(ia_contour_p*)malloc(sizeof(ia_contour_p));
	self->contours[0]=0;
	self->ncontours=0;
	self->nrefs=1;

	self->add     = ia_contours_add;
	self->add_ref = ia_contours_add_ref;
	self->destroy = ia_contours_destroy;
	return self;
}

static void ia_contours_add(struct _ia_contours* self, ia_contour_p contour)
{
	self->ncontours++;
	self->contours=(ia_contour_p*)realloc(self->contours, sizeof(ia_contour_p)*(1+self->ncontours));
	self->contours[self->ncontours-1]=contour;
	self->contours[self->ncontours]=0;
}

static void ia_contours_add_ref (ia_contours_p self)
{
	self->nrefs++;
}

static void ia_contours_destroy(struct _ia_contours* self)
{
	ia_contour_p* contours;
	assert(self->nrefs>0);
	self->nrefs--;
	if (self->nrefs == 0)
	{
		for (contours=self->contours; *contours; contours++)
		{
			(*contours)->destroy(*contours);
		}
		free(self->contours);
		free(self);
	}
}
