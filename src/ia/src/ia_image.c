/*********************************************************************/
/*                                                                   */
/* Copyright (C) 2005, Alexander Marinov, Nadezhda Zlateva           */
/*                                                                   */
/* Project:       ia                                                 */
/* Filename:      ia_iamge.c                                         */
/* Description:   Image management module                            */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <ia/ia_image.h>

/*********************************************************************/
/*                        Local prototypes                           */
/*********************************************************************/

static void                ia_image_destroy         (struct _ia_image_t*);
static void                ia_image_set_pixel_2     (struct _ia_image_t*, ia_uint16_t, ia_uint16_t, ia_bool_t);
static ia_bool_t           ia_image_get_pixel_2     (struct _ia_image_t*, ia_uint16_t, ia_uint16_t);
static void                ia_image_set_pixel_8     (struct _ia_image_t*, ia_uint16_t, ia_uint16_t, ia_uint8_t);
static ia_uint8_t          ia_image_get_pixel_8     (struct _ia_image_t*, ia_uint16_t, ia_uint16_t);
static void                ia_image_set_pixel_16    (struct _ia_image_t*, ia_uint16_t, ia_uint16_t, ia_uint16_t);
static ia_uint16_t         ia_image_get_pixel_16    (struct _ia_image_t*, ia_uint16_t, ia_uint16_t);
static void                ia_image_set_pixel_24    (struct _ia_image_t*, ia_uint16_t, ia_uint16_t, ia_uint32_t);
static ia_uint32_t         ia_image_get_pixel_24    (struct _ia_image_t*, ia_uint16_t, ia_uint16_t);
static void                ia_image_set_pixel_32    (struct _ia_image_t*, ia_uint16_t, ia_uint16_t, ia_uint32_t);
static ia_uint32_t         ia_image_get_pixel_32    (struct _ia_image_t*, ia_uint16_t, ia_uint16_t);
static void                ia_image_set_pixel       (struct _ia_image_t*, ia_uint16_t, ia_uint16_t, ia_uint32_t);
static ia_uint32_t         ia_image_get_pixel       (struct _ia_image_t*, ia_uint16_t, ia_uint16_t);
static void                ia_image_fill            (struct _ia_image_t*, ia_uint32_t);
static struct _ia_image_t* ia_image_convert_rgb     (struct _ia_image_t*);
static struct _ia_image_t* ia_image_convert_gray    (struct _ia_image_t*, ia_format_t);
static void                ia_image_normalize_colors(struct _ia_image_t*, ia_int32_t, ia_uint32_t, ia_int32_t, ia_uint32_t);
static void                ia_image_inverse         (struct _ia_image_t*);
static void                ia_image_get_min_max     (struct _ia_image_t*, ia_int32_t*, ia_uint32_t*);
static void                ia_image_print           (struct _ia_image_t*);
static struct _ia_image_t* ia_image_copy            (struct _ia_image_t*);
static void                ia_image_save            (struct _ia_image_t*, const ia_string_t);
static void                ia_image_save_img        (ia_image_p, const ia_string_t);
static ia_image_p          ia_image_load_img        (const ia_string_t image_name);

#ifdef HAVE_JPEGLIB
ia_image_p ia_image_load_jpeg(const ia_string_t src);
void ia_image_save_jpeg(ia_image_p, const ia_string_t src);
#endif

static ia_image_p ia_image_load_img(const ia_string_t image_name)
{
	ia_image_p  img;
	ia_uint16_t w,h;
	ia_format_t format;
	ia_bool_t   is_gray;
	FILE* fp=fopen(image_name, "rb");
	if (!fp)
	{
		return 0;
	}
	fread(&w, 1, sizeof(ia_uint16_t), fp);
	fread(&h, 1, sizeof(ia_uint16_t), fp);
	fread(&format, 1, sizeof(ia_format_t), fp);
	fread(&is_gray, 1, sizeof(ia_bool_t), fp);
	img=ia_image_new(w, h, format, is_gray);
	fread(img->pixels.data, 1, img->pixels.size, fp);
	fclose(fp);
	return img;
}

static void ia_image_save_img(ia_image_p img, const ia_string_t image_name)
{
	FILE* fp=fopen(image_name, "wb");
	if (!fp)
	{
		return ;
	}
	fwrite(&img->width,   1, sizeof(ia_uint16_t), fp);
	fwrite(&img->height,  1, sizeof(ia_uint16_t), fp);
	fwrite(&img->format,  1, sizeof(ia_format_t), fp);
	fwrite(&img->is_gray, 1, sizeof(ia_bool_t),   fp);
	fwrite(img->pixels.data, 1, img->pixels.size, fp);
	fclose(fp);
}

ia_image_p ia_image_load(const ia_string_t image_name)
{
	ia_string_t ext=strrchr(image_name, '.');
	if (ext)
	{
		if (!stricmp(ext, ".img"))
			return ia_image_load_img(image_name);
#ifdef HAVE_JPEGLIB
		if (!stricmp(ext, ".jpg") || !stricmp(ext, ".jpeg"))
			return ia_image_load_jpeg(image_name);
#endif
	}
	return 0;
}

void ia_image_save(struct _ia_image_t* self, const ia_string_t image_name)
{
	ia_string_t ext=strrchr(image_name, '.');
	if (ext)
	{
		if (!stricmp(ext, ".img"))
		{
			ia_image_save_img(self, image_name);
			return ;
		}
#ifdef HAVE_JPEGLIB
		if (!stricmp(ext, ".jpg") || !stricmp(ext, ".jpeg"))
		{
			ia_image_save_jpeg(self, image_name);
			return ;
		}
#endif
	}
}

ia_image_p ia_image_new(ia_uint16_t width, ia_uint16_t height, ia_format_t format, ia_bool_t is_gray)
{
	ia_uint32_t img_size  = height * ((width * ia_format_size(format) + 7) >> 3);
	ia_image_p  img       = (ia_image_p)malloc(sizeof(ia_image_t));

	img->width            = width;
	img->height           = height;
	img->format           = format;
	img->is_gray          = is_gray;
	img->marker_x         = 0;
	img->marker_y         = 0;
	img->pixels.data      = (ia_data_p)calloc(1, img_size);
	img->pixels.size      = img_size;

	img->destroy          = ia_image_destroy;
	img->set_pixel        = ia_image_set_pixel;
	img->get_pixel        = ia_image_get_pixel;
	img->fill             = ia_image_fill;
	img->convert_rgb      = ia_image_convert_rgb;
	img->convert_gray     = ia_image_convert_gray;
	img->normalize_colors = ia_image_normalize_colors;
	img->inverse          = ia_image_inverse;
	img->get_min_max      = ia_image_get_min_max;
	img->copy             = ia_image_copy;
	img->save             = ia_image_save;
	img->print            = ia_image_print;

	return img;
}

static void ia_image_destroy(struct _ia_image_t* self)
{
	if (self->pixels.data)
	{
		free(self->pixels.data);
	}
	free(self);
}

static void ia_image_set_pixel_2(struct _ia_image_t* self, ia_uint16_t x, ia_uint16_t y, ia_bool_t value)
{
	ia_uint32_t ofs=y * ((self->width+7) >> 3) + (x >> 3);
	ia_uint32_t bit=1<<(x & 7);
	if (value) 
	{
		((ia_uint8_t*)self->pixels.data)[ofs]|=bit;
	}
	else
	{
		((ia_uint8_t*)self->pixels.data)[ofs]&=~bit;
	}
}

static ia_bool_t ia_image_get_pixel_2(struct _ia_image_t* self, ia_uint16_t x, ia_uint16_t y)
{
	ia_uint32_t ofs=y * ((self->width+7) >> 3) + (x >> 3);
	ia_uint32_t bit=1<<(x & 7);
	return ((((ia_uint8_t*)self->pixels.data)[ofs]&bit)?1:0);
}

static void ia_image_set_pixel_8(struct _ia_image_t* self, ia_uint16_t x, ia_uint16_t y, ia_uint8_t value)
{
	((ia_uint8_t*)self->pixels.data)[y*self->width + x]=value;
}

static ia_uint8_t ia_image_get_pixel_8(struct _ia_image_t* self, ia_uint16_t x, ia_uint16_t y)
{
	return ((ia_uint8_t*)self->pixels.data)[y*self->width + x];
}

static void ia_image_set_pixel_16(struct _ia_image_t* self, ia_uint16_t x, ia_uint16_t y, ia_uint16_t value)
{
	((ia_uint16_t*)self->pixels.data)[y*self->width + x]=value;
}

static ia_uint16_t ia_image_get_pixel_16(struct _ia_image_t* self, ia_uint16_t x, ia_uint16_t y)
{
	return ((ia_uint16_t*)self->pixels.data)[y*self->width + x];
}

static void ia_image_set_pixel_24(struct _ia_image_t* self, ia_uint16_t x, ia_uint16_t y, ia_uint32_t value)
{
	((ia_uint8_t*)self->pixels.data)[y*self->width + x + 0] = (ia_uint8_t)((value >>  0) & 0xFF);
	((ia_uint8_t*)self->pixels.data)[y*self->width + x + 1] = (ia_uint8_t)((value >>  8) & 0xFF);
	((ia_uint8_t*)self->pixels.data)[y*self->width + x + 2] = (ia_uint8_t)((value >> 16) & 0xFF);
}

static ia_uint32_t ia_image_get_pixel_24(struct _ia_image_t* self, ia_uint16_t x, ia_uint16_t y)
{
	return 
	(((ia_uint8_t*)self->pixels.data)[y*self->width + x + 0] << 0) |
	(((ia_uint8_t*)self->pixels.data)[y*self->width + x + 1] << 8) |
	(((ia_uint8_t*)self->pixels.data)[y*self->width + x + 2] << 16);
}

static void ia_image_set_pixel_32(struct _ia_image_t* self, ia_uint16_t x, ia_uint16_t y, ia_uint32_t value)
{
	((ia_uint32_t*)self->pixels.data)[y*self->width + x]=value;
}

static ia_uint32_t ia_image_get_pixel_32(struct _ia_image_t* self, ia_uint16_t x, ia_uint16_t y)
{
	return ((ia_uint32_t*)self->pixels.data)[y*self->width + x];
}

static void ia_image_set_pixel(struct _ia_image_t* self, ia_uint16_t x, ia_uint16_t y, ia_uint32_t value)
{
	if (x<self->width && y<self->height)
	{
		switch (self->format)
		{
		case IAT_BOOL:
				ia_image_set_pixel_2(self, x, y, (ia_bool_t)value);
				break;
		case IAT_UINT_8: case IAT_INT_8:
				ia_image_set_pixel_8(self, x, y, (ia_uint8_t)value);
				break;
		case IAT_UINT_16: case IAT_INT_16:
				ia_image_set_pixel_16(self, x, y, (ia_uint16_t)value);
				break;
		case IAT_UINT_24: case IAT_INT_24:
				ia_image_set_pixel_24(self, x, y, (ia_uint32_t)value);
				break;
		case IAT_UINT_32: case IAT_INT_32:
				ia_image_set_pixel_32(self, x, y, (ia_uint32_t)value);
				break;
			default:
				ASSERT(0), "set_pixel(%d, %d, %X) -> Not supported format %d!\n", x, y, value, self->format);
		}
	}
}

static ia_uint32_t ia_image_get_pixel(struct _ia_image_t* self, ia_uint16_t x, ia_uint16_t y)
{
	if (x<self->width && y<self->height)
	{
		switch (self->format)
		{
			case IAT_BOOL:
				return ia_image_get_pixel_2(self, x, y);
			case IAT_UINT_8: case IAT_INT_8: 
				return ia_image_get_pixel_8(self, x, y);
			case IAT_UINT_16: case IAT_INT_16:
				return ia_image_get_pixel_16(self, x, y);
			case IAT_UINT_24: case IAT_INT_24:
				return ia_image_get_pixel_24(self, x, y);
			case IAT_UINT_32: case IAT_INT_32:
				return ia_image_get_pixel_32(self, x, y);
			default:
				ASSERT(0), "get_pixel(%d, %d) -> Not supported format %d!\n", x, y, self->format);
		}
	}
	return 0;
}

static void ia_image_fill(struct _ia_image_t* self, ia_uint32_t value)
{
	int i, j;
	for (i=0; i<self->height; i++)
		for (j=0; j<self->width; j++)
			self->set_pixel(self, j, i, value);
}

static struct _ia_image_t* ia_image_convert_rgb(struct _ia_image_t* self)
{
	int i, j;
	ia_image_p img_new;
	if (self->is_gray)
	{
		ia_image_p img_temp = self->copy(self);
		img_temp->normalize_colors(img_temp, 0, 0, 0, 255);
		img_new = ia_image_new(self->width, self->height, IAT_UINT_32, IA_IMAGE_RGB);
		for (i=0; i<img_temp->height; i++)
		for (j=0; j<img_temp->width; j++)
		{
			ia_uint32_t c = (ia_uint32_t)img_temp->get_pixel(img_temp, j, i);
			img_new->set_pixel(img_new, j, i, IA_RGB(c, c, c));
		}
		img_temp->destroy(img_temp);
	}
	else if (ia_format_size(self->format) != ia_format_size(IAT_UINT_32))
	{
		ASSERT(ia_format_size(self->format) >= 24), "FIXME: Converting to 32 bit RGB format from %d bit is not supported!\n", ia_format_size(self->format));
		img_new = ia_image_new(self->width, self->height, IAT_UINT_32, IA_IMAGE_RGB);
		for (i=0; i<img_new->height; i++)
		for (j=0; j<img_new->width; j++)
			img_new->set_pixel(img_new, j, i, self->get_pixel(self, j, i));
	} else
	{
		img_new=self->copy(self);
	}
	return img_new;
}

static struct _ia_image_t* ia_image_convert_gray(struct _ia_image_t* self, ia_format_t format)
{
	ia_uint16_t i, j;
	ia_image_p img_new;
	if (!self->is_gray)
	{
		/* convert RGB image */
		ia_image_p img_temp;
		ASSERT(ia_format_size(self->format) >= 24), "FIXME: Converting to gray from %d bit RGB format is not supported!\n", ia_format_size(self->format));
		img_temp = self->convert_rgb(self);
		img_new = ia_image_new(self->width, self->height, format, IA_IMAGE_GRAY);
		for (i=0; i<img_temp->height; i++)
		for (j=0; j<img_temp->width; j++)
		{
			ia_uint32_t c = img_temp->get_pixel(img_temp, j, i);
			ia_uint8_t  g = IA_GRAY(c);
			if (format == IAT_BOOL)
			{
				img_new->set_pixel(img_new, j, i, (g>=128?1:0));
			} else
			{
				img_new->set_pixel(img_new, j, i, g); 
			}
		}
		img_temp->destroy(img_temp);
	}
	else if (ia_format_size(self->format) != ia_format_size(format))
	{
		/* convert gray image */
		ia_int32_t min;
		ia_uint32_t max;
		if (ia_format_size(self->format) < ia_format_size(format))
		{
			img_new = ia_image_new(self->width, self->height, format, IA_IMAGE_GRAY);
			for (i=0; i<img_new->height; i++)
			for (j=0; j<img_new->width; j++)
				img_new->set_pixel(img_new, j, i, self->get_pixel(self, j, i));
		}
		else
		{
			img_new=self->copy(self);
		}
		ia_format_min_max(format, &min, &max);
		img_new->normalize_colors(img_new, 0, 0, min, max);
	}		
	ASSERT(img_new->is_gray), "Internal Error! The returned image is not marked as gray\n");
	return img_new;
}

static void ia_image_normalize_colors(struct _ia_image_t* self, ia_int32_t min, ia_uint32_t max, ia_int32_t new_min, ia_uint32_t new_max)
{
	ia_uint16_t i,j;
	ASSERT(self->is_gray), "FIXME: RGB format is not supported by ia_image_normalize_colors!\n");
	if (!min && !max)
	{
		self->get_min_max(self, &min, &max);
	}

	if (min != max)
	{
		for (i=0; i<self->height; i++)
		for (j=0; j<self->width; j++)
		{
			ia_uint32_t c = self->get_pixel(self, j, i);
			c=(ia_uint32_t)floor(new_min + (new_max-new_min)*(float)(c-min)/(float)(max-min));
			self->set_pixel(self, j, i, c);
		}
	}
}

static void ia_image_inverse(struct _ia_image_t* self)
{
	ia_int32_t  i,j;
	ia_int32_t  min;
	ia_uint32_t max;

	ASSERT(self->is_gray), "FIXME: RGB format is not supported by ia_image_inverse!\n");

	self->get_min_max(self, &min, &max);

	for (i=0; i<self->height; i++)
	for (j=0; j<self->width; j++)
		self->set_pixel(self, j, i, min+max-self->get_pixel(self, j, i));
}

static void ia_image_get_min_max(struct _ia_image_t* self, ia_int32_t* min, ia_uint32_t* max)
{
	ia_uint16_t i,j;
	ia_bool_t is_signed=ia_format_signed(self->format);
	ia_format_min_max(self->format, max, min);
	for (i=0; i<self->height; i++)
	for (j=0; j<self->width; j++)
	{
		if (is_signed)
		{
			ia_int32_t c = (ia_int32_t)self->get_pixel(self, j, i);
			if (c>(ia_int32_t)*max) *max=(ia_int32_t)c;
			if (c<*min) *min=c;
		}
		else
		{
			ia_uint32_t c = self->get_pixel(self, j, i);
			if (c>*max) *max=c;
			if (c<(ia_uint32_t)*min) *min=c;
		}
	}
}

static struct _ia_image_t* ia_image_copy(struct _ia_image_t* self)
{
	ia_image_p img_new = ia_image_new(self->width, self->height, self->format, self->is_gray);
	memcpy(img_new->pixels.data, self->pixels.data, self->pixels.size);
	return img_new;
}

static void ia_image_print(struct _ia_image_t* self)
{
	ia_int32_t i,j;
	for (i=0; i<self->height; i++)
	{
		for (j=0; j<self->width; j++)
		{
			ia_uint8_t c=(ia_uint8_t)self->get_pixel(self, j, i);
			if ((j == self->marker_x) && (i == self->marker_y))
			{
				printf("+");
			} else
			{
				if (c)
					printf("%d", c);
				else
					printf("-");
			}
		}
		printf("\n");
	}
	for (j=0; j<self->width; j++) printf("=");
	printf("\n");
}
