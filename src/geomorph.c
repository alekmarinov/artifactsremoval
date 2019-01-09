/*
	geomorph.c

	Alexander Marinov's Diploma Work - 2005, 
	Sofia University, Artificial Intelligence in Mathematics & Informatics department, 

	Artifacts removal algorithm from company logo images using Morphologic-Geometric approach
	D. T. Dimov
*/

#define CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>
#include <direct.h>
#include <io.h>
#include <math.h>
#include <time.h>
#include <windows.h>
#include <ia/ia_image.h>
#include <ia/algo/ia_binarize.h>
#include <ia/algo/ia_contours.h>
#include <ia/algo/ia_convolution.h>
#include <ia/algo/ia_distance_transform.h>

#define EXIT_SUCCESS 0
#define EXIT_FAIL 1

#define VERSION "3.0"

#define DEFAULT_SMOOTH_FACTOR 15
#define DEFAULT_DILATION_FACTOR 5
#define DEFAULT_FIRST_TIME_BINARIZE 192

#define DEFFAULT_PRIORITY 50

#define BK_QUALITY_BLACK_WHITE         0
#define BK_QUALITY_NEAREST_COLOR       1
#define BK_QUALITY_NEAREST_BACKGROUND  2

static const char* program_name="geomorph";
static const char* program_usage = "Usage: %s [OPTIONS] ... <source directory> <output directory>%s";

typedef struct _contour_tree
{
	ia_contour_p contour;
	struct _contour_tree* parent;
} contour_tree_t, *contour_tree_p;

/* 
	binarize image at specified level
*/
void binarize_image(ia_image_p image, int level)
{
	ia_int32_t min;
	ia_uint32_t max;
	image->get_min_max(image, &min, &max);
	ia_binarize_level( image, level?level:DEFAULT_FIRST_TIME_BINARIZE );
}

/* 
	binary search algorithm for extracting image contours at each binarization level 
	over distrance transformed input image
	returns array of all contours allocated during tree generation
*/
void find_tree_function(ia_image_p img_gray, ia_int32_t left, ia_int32_t right, ia_contours_p *contours_level_map, ia_bool_t first_time_call, ia_contours_p* contours_collection, ia_int32_t *count, ia_int32_t priority)
{
	ia_int32_t level = left + ((right - left) >> 1);
	if (level > left && level < right)
	{
		ia_int32_t i;
		ia_image_p img;
		ia_contours_p contours;

		img = img_gray->copy(img_gray);
		if (first_time_call)
		{
			ia_contours_p min_contours;
			ia_contours_p max_contours;

			*count=2;

			/* 
				on first time call determine minimum and maximum contours
				at the most low and the most high binarization levels
		    */
			ia_binarize_level( img, left );
			img->inverse(img);
			max_contours = ia_contours_find(img);
			img->destroy(img);
			img = img_gray->copy(img_gray);
			ia_binarize_level( img, right );
			img->inverse(img);
			min_contours = ia_contours_find(img);
			img->destroy(img);

			/* 
				allocates binarization level to contours count hashmap
			*/
			contours_level_map[left]=max_contours;
			contours_collection[0]=max_contours;
			contours_level_map[right]=min_contours;
			contours_collection[1]=min_contours;

			img = img_gray->copy(img_gray);
		}

		ia_binarize_level( img, level );

		/*
			find contours around non-zero pixels in the image
		*/
		img->inverse(img);
		contours = ia_contours_find(img);
		contours_collection[(*count)++] = contours;
		img->destroy(img);

		/*
			map level number to contour count at this level
		*/
		contours_level_map[level]=contours;

		/*
			make the CPU happy
		*/
		Sleep( 500*(100-priority)/100 );

		/*
			if contour count at left level match contour count at this level
		*/
		if (contours->ncontours == contours_level_map[left]->ncontours)
		{
			/*
				make the levels in all interval equals
			*/
			for (i=left+1; i<level; i++)
			{
				contours_level_map[i]=contours;
			}
		}
		else
		{
			if (left<level)
			{
				/*
					recurse in left interval
				*/
				find_tree_function(img_gray, left, level, contours_level_map, 0, contours_collection, count, priority);
			}
		}

		/*
			if contour count at right level match contour count at this level
		*/
		if (contours->ncontours == contours_level_map[right]->ncontours)
		{
			/*
				make the levels in all interval equals
			*/
			for (i=level+1; i<right; i++)
			{
				contours_level_map[i]=contours;
			}
		}
		else
		{
			if (level<right)
			{
				/*
					recurse in right interval
				*/
				find_tree_function(img_gray, level, right, contours_level_map, 0, contours_collection, count, priority);
			}
		}
	}
}

void evaluate_contour_tree(ia_int32_t min, ia_int32_t max, ia_double_t *evaluation, ia_contours_p *contours_level_map)
{
	ia_int32_t current_level=min;
	ia_int32_t **contour_indeces;
	ia_int32_t **new_contour_indeces;
	ia_int32_t i, j;
	ia_int32_t max_size;

	max_size=contours_level_map[min]->ncontours;
	contour_indeces=(ia_int32_t**)malloc(sizeof(ia_int32_t*) * max_size);
	for (i=0; i<max_size; i++)
	{
		contour_indeces[i]=(ia_int32_t*)malloc(sizeof(ia_int32_t) * (max_size+1));
		contour_indeces[i][0]=1;
		contour_indeces[i][1]=i;
	}

	while (current_level<max)
	{
		ia_int32_t contours_count;
		ia_int32_t big_contours_count;
		ia_int32_t small_contours_count;

		/* 
			find next level with different contours count
		*/
		while ((current_level+1)<max && contours_level_map[current_level]->ncontours == contours_level_map[current_level+1]->ncontours)
			current_level++;
		current_level++;

		new_contour_indeces=(ia_int32_t**)malloc(sizeof(ia_int32_t*) * max_size);
		for (i=0; i<max_size; i++)
		{
			new_contour_indeces[i]=(ia_int32_t*)malloc(sizeof(ia_int32_t) * (max_size+1));
			new_contour_indeces[i][0]=0;
		}

		big_contours_count=contours_level_map[current_level]->ncontours;
		for (i=0; i<big_contours_count; i++)
		{
			ia_int32_t k;
			ia_contour_p big_contour=contours_level_map[current_level]->contours[i];
			small_contours_count=0;
			contours_count=0;
			for (j=0; j<contours_level_map[current_level-1]->ncontours; j++)
			{
				ia_contour_p small_contour=contours_level_map[current_level-1]->contours[j];
				if (big_contour->in_contour(big_contour, small_contour))
				{
					/*
						Combines all sets { new_contour_indeces[i] } <= { contour_indeces[j] } 
					*/
					for (k=0; k<contour_indeces[j][0]; k++)
					{
						new_contour_indeces[i][1+small_contours_count+k]=contour_indeces[j][1+k];
					}
					small_contours_count+=contour_indeces[j][0];
					contours_count++;
				}
			}
			assert(small_contours_count>0);
			new_contour_indeces[i][0]=small_contours_count;
			for (k=0; k<small_contours_count; k++)
				evaluation[ new_contour_indeces[i][1+k] ]*= (ia_double_t)1.0f/contours_count;

		}
		for (i=0; i<max_size; i++) free(contour_indeces[i]);
		free(contour_indeces);
		contour_indeces=new_contour_indeces;
	}
	for (i=0; i<max_size; i++) free(contour_indeces[i]);
	free(contour_indeces);
}

ia_image_p make_background_image(ia_image_p image, ia_image_p image_dt, ia_int32_t quality, ia_uint32_t level, ia_double_t blur_factor, ia_bool_t inversed)
{
	ia_image_p image_bk=image->copy(image);
	ia_bool_t  bk_hit;
	ia_int32_t rx, ry, dx, dy, wx, wy;
	ia_uint16_t x, y, k;
	ia_complex_p color_lines[6];
	ia_complex_p filter_line;
	ia_int32_t filter_count, i;

	/*
		Draws background
	*/
	switch (quality)
	{
	case BK_QUALITY_BLACK_WHITE:
		{
			if (inversed)
			{
				image_bk->fill(image_bk, 0);
			} 
			else
			{
				image_bk->fill(image_bk, IA_RGB(255, 255, 255));
			}
			return image_bk;
		}
	case BK_QUALITY_NEAREST_COLOR:
		{
			struct histogram_t
			{
				ia_uint32_t color;
				ia_uint32_t count;
			} *histogram_p;
			ia_int32_t ncolors=0;
			ia_uint32_t max_ncolors;
			ia_uint32_t max_color;
			ia_bool_t found;
			ia_int32_t size=sizeof(struct histogram_t) * (image->width+image->height) * 2;
			histogram_p=(struct histogram_t*)malloc(size);
			memset(histogram_p, 0, size);

			/* 
				Traverse image boundary 
			*/
			for (k=0; k<4; k++)
			{
				for (x=0; x<(k<2?image->width:image->height); x++)
				{
					ia_uint32_t color;
					if (k<2)
						color=image_bk->get_pixel(image_bk, x, k*(image->height-1));
					else
						color=image_bk->get_pixel(image_bk, (k-2)*(image->width-1), x);

					found=0;
					for (i=0; i<ncolors; i++)
					{
						if (histogram_p[i].color == color)
						{
							histogram_p[i].count++;
							found=1;
							break;
						}
					}
					if (!found)
					{
						/* 
							Add new color 
						*/
						histogram_p[ncolors].color=color;
						histogram_p[ncolors].count=1;
						ncolors++;
					}
				}
			}
			max_ncolors=0;
			max_color=0;
			for (i=0; i<ncolors; i++)
			{
				if (histogram_p[i].count>max_ncolors)
				{
					max_ncolors=histogram_p[i].count;
					max_color=histogram_p[i].color;
				}
			}
			free(histogram_p);
			image_bk->fill(image_bk, max_color);
			return image_bk;
		}
	}

	/* 
		Caclulates nearest image background  
	*/
	image_dt=image_dt->copy(image_dt);
	ia_binarize_level(image_dt, level);

	rx=ry=wx=wy=dy=0;
	dx=1;

	bk_hit=0; /* Indicates missing background color at image bounding zone */
	do
	{
		ia_uint32_t color=image_dt->get_pixel(image_dt, (ia_uint16_t)rx, (ia_uint16_t)ry);
		if (rx + 1 == image->width)
		{
			rx=0; ry++;
		} 
		else
		{
			rx++;
		}

		if (color>level)
		{
			bk_hit=1;
			/* 
				The reader is over background color 
			*/
			if (image_dt->get_pixel(image_dt, (ia_uint16_t)wx, (ia_uint16_t)wy) < level)
			{
				/* 
					The writer is over object color 
				*/
				image_bk->set_pixel(image_bk, (ia_uint16_t)wx, (ia_uint16_t)wy, 
					image_bk->get_pixel(image_bk, (ia_uint16_t)rx, (ia_uint16_t)ry)
				);
			}
			if (wx + 1 == image->width)
			{
				wx=0; wy++;
			} 
			else
			{
				wx++;
			}
		}

		if (rx == image->width-1 && ry == image->height-1) 
		{
			if (!bk_hit)
			{
				/* 
					Unable to find background pixels on the image boundary 
				*/
				break;
			}
			rx=ry=0;
		}

	} while (wy < image->height);

	image_dt->destroy(image_dt);

	/*
		Smooth horizontally 
	*/
	filter_count=(ia_int32_t)floor((image->width * blur_factor)/100);
	filter_line=(ia_complex_p)malloc(sizeof(ia_complex_t) * filter_count);
	for (i=0; i<filter_count; i++)
	{
		filter_line[i].re=(ia_double_t)1.0f/filter_count;
		filter_line[i].im=0;
	}
	for (k=0; k<6; k++)
		color_lines[k]=(ia_complex_p)malloc(sizeof(ia_complex_t) * image->width);
	for (y=0; y<image->height; y++)
	{
		for (x=0; x<image->width; x++)
		{
			ia_uint32_t color=image_bk->get_pixel(image_bk, x, y);
			color_lines[0][x].re=IA_RED(color);
			color_lines[1][x].re=IA_GREEN(color);
			color_lines[2][x].re=IA_BLUE(color);
			for (k=0; k<3; k++) color_lines[k][x].im=(ia_double_t)0.0f;
		}
		for (k=0; k<3; k++)
			ia_convolution(image->width, color_lines[k], filter_count, filter_line, 0, 2, color_lines[3+k]);
		for (x=0; x<image->width; x++)
		{
			ia_uint32_t color=IA_RGB((ia_int32_t)color_lines[3][x].re, (ia_int32_t)color_lines[4][x].re, (ia_int32_t)color_lines[5][x].re);
			image_bk->set_pixel(image_bk, x, y, color);
		}
	}

	free(filter_line);
	for (k=0; k<6; k++)
		free(color_lines[k]);

	/* 
		Smooth vertically 
	*/
	filter_count=(ia_int32_t)floor((image->height * blur_factor)/100);
	filter_line=(ia_complex_p)malloc(sizeof(ia_complex_t) * filter_count);
	for (i=0; i<filter_count; i++)
	{
		filter_line[i].re=(ia_double_t)1.0f/filter_count;
		filter_line[i].im=0;
	}
	for (k=0; k<6; k++)
		color_lines[k]=(ia_complex_p)malloc(sizeof(ia_complex_t) * image->height);
	for (x=0; x<image->width; x++)
	{
		for (y=0; y<image->height; y++)
		{
			ia_uint32_t color=image_bk->get_pixel(image_bk, x, y);
			color_lines[0][y].re=IA_RED(color);
			color_lines[1][y].re=IA_GREEN(color);
			color_lines[2][y].re=IA_BLUE(color);
			for (k=0; k<3; k++) color_lines[k][y].im=(ia_double_t)0.0f;
		}
		for (k=0; k<3; k++)
			ia_convolution(image->height, color_lines[k], filter_count, filter_line, 0, 2, color_lines[3+k]);
		for (y=0; y<image->height; y++)
		{
			ia_uint32_t color=IA_RGB((ia_int32_t)color_lines[3][y].re, (ia_int32_t)color_lines[4][y].re, (ia_int32_t)color_lines[5][y].re);
			image_bk->set_pixel(image_bk, x, y, color);
		}
	}

	free(filter_line);
	for (k=0; k<6; k++)
		free(color_lines[k]);

	return image_bk;
}
int I=1;
/*
	Process one image
*/
ia_bool_t process(const ia_string_t source_file, const ia_string_t output_file, ia_int32_t bk_quality, ia_int32_t blur_factor, ia_int32_t dilation_factor, ia_int32_t priority)
{
	char temp[255];
	ia_image_p image;                    /* original loaded image */
	ia_image_p img_gray;                 /* the orinal image converted to gray levels */
	ia_image_p img_bk;                   /* image background generated according the specified background quality */
	ia_image_p mask;                     /* image mask used for distance transformation */
	ia_image_p image_eval;               /* evaluation image where the evaluated contours are filled with color 
										    corresponding to their evaluation value */
	ia_int32_t i,
		black_color_count,               /* number of the black pixels on the otsu binarized image boundary */
		min;                             /* minimum gray value getermined by DT algorithm */
	ia_uint32_t max,                     /* maximum gray value getermined by DT algorithm */
		bin_eval_level;                  /* binarization level for distance transformed evaluation image */
	ia_uint16_t w, h, x, y;
	ia_contours_p *contours_level_map;   /* maps binarization level over DT image to contours */
	ia_contours_p *contours_collection;  /* stores all contours objects created during finding contours tree */
	ia_int32_t contours_count;           /* contours count in the collection */
	ia_bool_t inversed=0;                /* true if the original image is negative */
	ia_double_t *evaluation;             /* evaluation value for each initial contours determining 
										    the weight of the given contour treated as artifact */

	/*
		Check if arguments are valid 
	*/
	assert(source_file);
	assert(output_file);
	assert(bk_quality>=BK_QUALITY_BLACK_WHITE && bk_quality<=BK_QUALITY_NEAREST_BACKGROUND);
	assert(blur_factor>=0 && blur_factor<=100);
	assert(dilation_factor>=0 && dilation_factor<=100);

	/*
		Loads input image and return 0 if error occurred
	*/
	image=ia_image_load(source_file);
	if (!image)
	{
		printf("Unable to load %s\n", source_file);
		return 0;
	}

	/*
		Converts the original image to gray levels in range 0 - 64K
	*/
	img_gray=image->convert_gray(image, IAT_UINT_16);

	/*
		Otsu binarization
	*/
	ia_binarize_otsu(img_gray);
	sprintf(temp, "stage%d.jpg", I++);
	img_gray->save(img_gray, temp);

	/*
		Checks if the binarized image is positive or negative traversing image boundary pixels
	*/
	w=img_gray->width;
	h=img_gray->height;
	black_color_count=0;
	for (y=0; y<h; y++)
	{
		if (!img_gray->get_pixel(img_gray, 0, y)) black_color_count++;
		if (!img_gray->get_pixel(img_gray, w-1, y)) black_color_count++;
	}

	for (x=0; x<w; x++)
	{
		if (!img_gray->get_pixel(img_gray, x, 0)) black_color_count++;
		if (!img_gray->get_pixel(img_gray, x, h-1)) black_color_count++;
	}

	if (black_color_count >= (w+h))
	{
		inversed=1;
		img_gray->inverse(img_gray);
	}

	/*
		Add small artifacts at each image corner to force higher evaluation on the central objects
		also avoiding the image to be empty 
	*/
	img_gray->set_pixel(img_gray, 0, 0, 0);
	img_gray->set_pixel(img_gray, 0, h-1, 0);
	img_gray->set_pixel(img_gray, w-1, 0, 0);
	img_gray->set_pixel(img_gray, w-1, h-1, 0);

	/*
		Performs sequential DT-5 7 11 distance tranformation algorithm over binarized image
	*/
	mask=ia_distance_transform_mask(5, 5, 7, 11, DT_INF, DT_INF);
	ia_distance_transform_sequential(img_gray, mask, &min, &max);

	/*
		Generate contours development tree initializing contours_level_map
		which maps a level to the corresponding contours object
	*/
	contours_level_map = (ia_contours_p *)malloc(sizeof(ia_contours_p) * (max-min+1));
	contours_count=0;
	contours_collection=(ia_contours_p*)malloc(sizeof(ia_contours_p) * max * 2);
	find_tree_function(img_gray, min+1, max, contours_level_map, 1, contours_collection, &contours_count, priority);

	/*
		Initializes evaluation factor for each initial object (from level - min+1) with 1.0f
	*/
	evaluation=(ia_double_t*)malloc(sizeof(ia_double_t) * contours_level_map[min+1]->ncontours);
	for (i=0; i<contours_level_map[min+1]->ncontours; i++) evaluation[i]=(ia_double_t)1.0f;

	/*
		Bottom to top recursive climbing evaluating each initial contour with coefficient determining
		its artifact weight. 
		As the evaluation is higher as more the change the contour to represents artifact
	*/
	evaluate_contour_tree(min+1, max, evaluation, contours_level_map);
	
	/*
		Creates evaluation image containing initial contours with gray levels corresponding
		to their evaluation factors
	*/
	image_eval=ia_image_new(w, h, IAT_UINT_8, IA_IMAGE_GRAY);
	image_eval->fill(image_eval, 255);
	{
		/*
			Determines maximum evaluation factor to normalize all eval factors between 0 and 1 
		*/
		ia_double_t eval_max=0;
		for (i=0; i<contours_level_map[min+1]->ncontours; i++)
		{
			if (evaluation[i]>eval_max)
				eval_max=evaluation[i];
		}

		/* 
			Compute color for each evaluated contour and fill it over image_eval 
		*/
		for (i=0; i<contours_level_map[min+1]->ncontours; i++)
		{
			ia_contour_p contour=contours_level_map[min+1]->contours[i];
			ia_int32_t color=(ia_int32_t)floor((evaluation[i]/eval_max) * 256);
			if (color>255) color = 255;
			contour->fill(contour, image_eval, color, 0);
		}
	}

	/* 
		Create image background according specified background generation method 
	*/
	img_bk=make_background_image(image, img_gray, bk_quality, 8, blur_factor, inversed);

	/* 
		Performs distance transformation over evaluation image 
	*/
	binarize_image(image_eval, 220);
	ia_distance_transform_sequential(image_eval, mask, &min, &max);

	/* 
		Binarize DT evaluation image according the given dilation factor which makes
		the contour objects bigger
	*/
	bin_eval_level=(ia_uint32_t)((max-min)*(ia_double_t)dilation_factor/100);
	if (bin_eval_level==0) bin_eval_level=1;
	binarize_image(image_eval, bin_eval_level);

	/* 
		Copy pixels from the original image laying under object pixels from the DT eval image
		over the generated background image
	*/
	for (y=0; y<img_bk->height-1; y++)
		for (x=0; x<img_bk->width-1; x++)
			if (image_eval->get_pixel(image_eval, x, y) == 0)
				img_bk->set_pixel(img_bk, x, y, image->get_pixel(image, x, y));


	/* 
		Save the result ot output_file
	*/
	img_bk->save(img_bk, output_file);

	/* 
		Free all occuppied memory
	*/
	free(evaluation);
	for (i=0; i<contours_count; i++)
	{
		contours_collection[i]->destroy(contours_collection[i]);
	}
	free(contours_collection);
	free(contours_level_map);
	mask->destroy(mask);
	img_bk->destroy(img_bk);
	img_gray->destroy(img_gray);
	image_eval->destroy(image_eval);
	image->destroy(image);
	return 1;
}

/* Program option definitions */

typedef struct 
{
	const char* name;        /* option name   */
	int         optch;       /* option letter */
	const char *description; /* option description */
} getopt_t;

static const getopt_t options_defs[] =
{
	{ "dilation", 'd', "Image dilation factor. A percentage 0-100% as bigger as more original pixels around non-artifact objects are copied from the original image [default=5%]. " },
	{ "quality", 'q', "Background quality. 0 - black or white (fastest speed), 1 - nearest color, 2 - populated original background over the artifact objects [default=1] (slowest speed)." },
	{ "smooth", 's', "Background smoothing level if the background quality = 2 [default=15%]" },
	{ "priority", 'p', "Process priority [0-100] determines how much the CPU to be used by the process [default=50%]" },
	{ "version", 'v', "Show program version" },
	{ "help", 'h', "Show this help screen" },
	{ "", 0, "" } /* last option entry */
};

void format_time(clock_t clocks, char* output)
{
	clocks/=1000; /* convert to seconds */
	if (clocks < 30)
		sprintf(output, "%ds", clocks);
	else
	if (clocks < 60)
		strcpy(output, "< 1m");
	else
	if (clocks < 3600)
	{
		sprintf(output, "%d:%dm", clocks / 60, clocks % 60);
	}
	else
	if (clocks < 12*3600)
	{
		sprintf(output, "%d:%dh", clocks / 3600, (clocks % 3600)/60);
	}
	else
	{
		sprintf(output, "> %dh", clocks / 3600);
	}
}

int main(int argc, char* argv[])
{
	ia_int32_t i;
	ia_int32_t optch=0;    /* the single character of each option */
	ia_int32_t dilation=DEFAULT_DILATION_FACTOR;
	ia_int32_t smooth=DEFAULT_SMOOTH_FACTOR;
	ia_int32_t quality=BK_QUALITY_NEAREST_COLOR;
	ia_int32_t priority=DEFFAULT_PRIORITY;
	ia_string_t source_directory;
	ia_string_t output_directory;
	ia_string_t source_directory_name;
	ia_string_t output_directory_name;
	ia_int8_t source_buffer[255];
	ia_int8_t output_buffer[255];
	struct _finddata_t c_file;
	intptr_t h_file;
	ia_string_t* file_names;
	ia_int32_t file_count=0;
	ia_int32_t file_step_grow=2;
	ia_int32_t error_count=0;
	ia_uint32_t elapsed_clocks=0;
	ia_bool_t show_humor=1;

	/* parse program options */
	for (i=1; i<argc; i++)
	{
		if (argv[i][0] != '-') break;
		if (argv[i][1] == '-')
		{
			const getopt_t* opt=options_defs;
			while (opt->name[0])
			{
				if (!strcmp(argv[i]+2, opt->name))
				{
					optch=opt->optch;
					break;
				}
				opt++;
			}
		} 
		else
		{
			optch=argv[i][1];
		}
		switch (optch)
		{
			/* dilation option */
			case 'd': 
			{
				char* s=argv[i+1];
				if (!s) goto invalid_opt_arg;
				dilation=atoi(s);
				if (dilation<0 || dilation>100)
				{
					printf("%s: Invalid --dilation parameter %d. Expected in interval [0-100]\n", program_name, dilation);
					goto invalid_opt_arg;
				}
				i++;
			}
			break;

			/* smooth option */
			case 's': 
			{
				char* s=argv[i+1];
				if (!s) goto invalid_opt_arg;
				smooth=atoi(s);
				if (smooth<0 || smooth>100)
				{
					printf("%s: Invalid --smooth parameter %d. Expected in interval [0-100]\n", program_name, smooth);
					goto invalid_opt_arg;
				}
				i++;
			}
			break;

			/* quality option */
			case 'q': 
			{
				char* s=argv[i+1];
				if (!s) goto invalid_opt_arg;
				quality=atoi(s);
				if (quality<0 || quality>2)
				{
					printf("%s: Invalid --quality parameter %d. Expected in interval [0-2]\n", program_name, quality);
					goto invalid_opt_arg;
				}
				i++;
			}
			break;

			/* priority option */
			case 'p': 
			{
				char* s=argv[i+1];
				if (!s) goto invalid_opt_arg;
				priority=atoi(s);
				if (priority<0 || priority>100)
				{
					printf("%s: Invalid --priority parameter %d. Expected in interval [0-100]\n", program_name, priority);
					goto invalid_opt_arg;
				}
				i++;
			}
			break;

			/* show program version and exit */
			case 'v': 
			{
				printf("%s\n", VERSION);
				return (EXIT_SUCCESS);
			}

			/* show program help and exit */
			case 'h': 
			{
				i=0;
				printf(program_usage, program_name, "\n\n");
				while (options_defs[i].optch)
				{
					char opt_name[255];
					size_t spcs;
					sprintf(opt_name, "  -%c, --%s", 
						options_defs[i].optch, options_defs[i].name);
					spcs=25-strlen(opt_name);
					printf("%s", opt_name);
					while (spcs--) printf(" ");
					printf("%s\n\n", options_defs[i].description);
					i++;
				}
				return (EXIT_FAIL);
			}

			/* illegal option */
			default: goto invalid_opt_name;
		}
	}

	source_directory=argv[i];
	if (!source_directory)
	{
		printf("%s: missing source directory argument. ", program_name);
		goto invalid_opt;
	}
	output_directory=argv[i+1];
	if (!output_directory)
	{
		printf("%s: missing output directory argument. ", program_name);
		goto invalid_opt;
	}
	
	{
		ia_string_t s=(ia_string_t)malloc(strlen(source_directory) + 5);
		strcpy(s, source_directory);
		strcat(s, "\\*.*");
		if ( (h_file = _findfirst (s, &c_file)) == -1L)
		{
			if ( (h_file = _findfirst (source_directory, &c_file)) == -1L)
			{
				printf("%s: invalid source directory pattern. ", program_name, source_directory);
				free(s);
				return (EXIT_FAIL);
			}
			else
			{
				ia_string_t p=(ia_string_t)malloc(255);
				strcpy(p, source_directory);
				source_directory=p;
				p=strrchr(source_directory, '\\');
				if (p) *p=0;
				source_directory_name=strdup(source_directory);
				strcat(source_directory, "\\%s");
			}
		}
		else
		{
			ia_string_t p=(ia_string_t)malloc(255);
			strcpy(p, source_directory);
			if (p[strlen(p)-1]!='\\')
				strcat(p, "\\");
			source_directory_name=strdup(p);
			strcat(p, "%s");
			source_directory=p;
		}
		free(s);
	}

	{
		ia_string_t p=(ia_string_t)malloc(255);
		strcpy(p, output_directory);
		if (p[strlen(p)-1]!='\\')
			strcat(p, "\\");
		output_directory_name=strdup(p);
		strcat(p, "%s");
		output_directory=p;
	}

	/* 
		Collect all files to be processed
	*/
	file_names=(ia_string_t*)malloc(sizeof(ia_string_t) * file_step_grow);
	do
	{
		if (c_file.attrib != _A_SUBDIR)
		{
			file_names[file_count++]=strdup(c_file.name);
			if (file_count % file_step_grow == 0)
			{
				file_step_grow*=file_step_grow;
				file_names=(ia_string_t*)realloc(file_names, sizeof(ia_string_t) * (file_count + file_step_grow));
			}
		}
	} while (_findnext (h_file, &c_file) != -1L);

    _findclose (h_file);

	printf("Processing %d images from %s to %s with dilation=%d%%, smooth=%d%%, quality=%d, CPU=%d%%\n", file_count, source_directory_name, output_directory_name, dilation, smooth, quality, priority);

	for (i=0; i<file_count; i++)
	{
		char elapsed_time[50];
		char estimated_time[50];
		ia_uint32_t estimated_clocks;
		clock_t current_clocks=clock();
		sprintf(source_buffer, source_directory, file_names[i]);
		sprintf(output_buffer, output_directory, file_names[i]);
		format_time(elapsed_clocks, elapsed_time);
		if (i>0)
		{
			estimated_clocks=elapsed_clocks * (file_count-i) / i;
			if (show_humor)
			{
				if ( (estimated_clocks / 1000) > 14*3600 )
					printf("Come tomorrow to see the results!\n");
				else
				if ( (estimated_clocks / 1000) > 20*60 && (estimated_clocks / 1000) < 60*60 )
					printf("You have time for one coffee!\n");
				else
				if ( (estimated_clocks / 1000) > 60*60 && (estimated_clocks / 1000) < 2*60*60 )
					printf("You have enough time for 2-3 coffees!\n");

				show_humor=0;
			}

			format_time(estimated_clocks, estimated_time);
		}
		else
			strcpy(estimated_time, "...");
		printf("%s->%s [%s elapsed, %s estimated]\n", file_names[i], output_directory_name, elapsed_time, estimated_time);
		if (!process(source_buffer, output_buffer, quality, smooth, dilation, priority))
		{
			error_count++;
		}
		current_clocks=clock()-current_clocks;
		elapsed_clocks+=current_clocks;
		free(file_names[i]);
	}

	free(file_names);
	free(source_directory);
	free(output_directory);
	free(source_directory_name);
	free(output_directory_name);
	printf("Done. %d images processed with %d errors\n", file_count, error_count);
	_CrtDumpMemoryLeaks();
	return EXIT_SUCCESS;

/* invalid option argument */
invalid_opt_arg:
	fprintf(stderr, "%s: option requires an argument - %c\n", program_name, optch);
	goto invalid_opt;

/* invalid option name */
invalid_opt_name:
	fprintf(stderr, "%s: illegal option %c\n", program_name, optch);
	goto invalid_opt;

/* fail exit with hint message */
invalid_opt:
	fprintf(stderr, "Try -h for help\n");
	return (EXIT_FAIL);
}
