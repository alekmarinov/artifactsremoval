/*********************************************************************/
/*                                                                   */
/* Copyright (C) 2005, Alexander Marinov, Nadezhda Zlateva           */
/*                                                                   */
/* Project:       ia                                                 */
/* Filename:      ia_jpeg.c                                          */
/* Description:   Load/Save JPEG image format                        */
/*                                                                   */
/*********************************************************************/

#ifdef HAVE_JPEGLIB

#define MaxBufferExtent  8192

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <memory.h>
#include <setjmp.h>
#include <jpeglib.h>
#include <jerror.h>
#include <ia/ia_image.h>

#define DEFAULT_JPEG_QUALITY	100

struct my_error_mgr 
{
	struct jpeg_error_mgr pub;    /* "public" fields */
	jmp_buf setjmp_buffer;        /* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;
void my_error_exit(j_common_ptr cinfo);

/* Read JPEG file */
typedef struct _SourceManager
{
	struct jpeg_source_mgr manager;
	void* input;
	JOCTET *buffer;
	boolean start_of_blob;
} SourceManager;

static void InitializeSource(j_decompress_ptr cinfo)
{
	SourceManager *source;
	source=(SourceManager *) cinfo->src;
	source->start_of_blob=1;
}

static boolean FillInputBuffer(j_decompress_ptr cinfo)
{
	SourceManager *source;

	source=(SourceManager *) cinfo->src;
	source->manager.bytes_in_buffer=(size_t)fread(source->buffer, 1, MaxBufferExtent, (FILE*)source->input);
	if (source->manager.bytes_in_buffer == 0)
	{
		if (source->start_of_blob != 0)
			ERREXIT(cinfo, JERR_INPUT_EMPTY);
		WARNMS(cinfo,JWRN_JPEG_EOF);
		source->buffer[0]=(JOCTET) 0xff;
		source->buffer[1]=(JOCTET) JPEG_EOI;
		source->manager.bytes_in_buffer=2;
	}
	source->manager.next_input_byte=source->buffer;
	source->start_of_blob=0;
	return 1;
}

static void SkipInputData(j_decompress_ptr cinfo,long number_bytes)
{
  SourceManager *source;

  if (number_bytes <= 0) 
  {
	  return;
  }
  source=(SourceManager *) cinfo->src;
  while (number_bytes > (long) source->manager.bytes_in_buffer)
  {
    number_bytes-=(long) source->manager.bytes_in_buffer;
    (void) FillInputBuffer(cinfo);
  }
  source->manager.next_input_byte+=(size_t) number_bytes;
  source->manager.bytes_in_buffer-=(size_t) number_bytes;
}

static void TerminateSource(j_decompress_ptr cinfo)
{
  cinfo=cinfo;
}

static void JPEGSourceManager(j_decompress_ptr cinfo, FILE* imageFile)
{
  SourceManager *source;

  cinfo->src=(struct jpeg_source_mgr *) (*cinfo->mem->alloc_small)
	  ((j_common_ptr) cinfo,JPOOL_IMAGE,sizeof(SourceManager));
  source=(SourceManager *) cinfo->src;
  source->buffer=(JOCTET *) (*cinfo->mem->alloc_small)
    ((j_common_ptr) cinfo,JPOOL_IMAGE,MaxBufferExtent*sizeof(JOCTET));
  source=(SourceManager *) cinfo->src;
  source->manager.init_source=InitializeSource;
  source->manager.fill_input_buffer=FillInputBuffer;
  source->manager.skip_input_data=SkipInputData;
  source->manager.resync_to_restart=jpeg_resync_to_restart;
  source->manager.term_source=TerminateSource;
  source->manager.bytes_in_buffer=0;
  source->manager.next_input_byte=NULL;
  source->input=imageFile;
}

ia_image_p ia_image_load_jpeg(const ia_string_t src)
{
	FILE *fp;
	ia_image_p img;

	struct jpeg_decompress_struct cinfo;
	struct my_error_mgr jerr;
	JSAMPROW row_pointer[1];
	ia_uint8_t* line_buf=0;
	ia_uint16_t loop;

	fp=fopen(src, "rb");
	if (!fp)
	{
		return 0;
	}

    /* We set up the normal JPEG error routines */
	cinfo.err = jpeg_std_error ( &jerr.pub );

	/* Install the new error handler so that we don't get dumped out unceremoniously -- GLM 2000 */
	/* RETURNS IInvalidError on error! (Should really insert a new error enum for it though). */
	jerr.pub.error_exit = my_error_exit;
	if (setjmp (jerr.setjmp_buffer))  
	{
		/* If we get here, then the JPEG library has signalled an event */
		fprintf(stderr, "ia_jpeg: %s\n", jerr.pub.jpeg_message_table[jerr.pub.last_jpeg_message]);
		jpeg_destroy_decompress (&cinfo);
		fclose(fp);
		return 0;
	}

	/* Now we can initialize the JPEG decompression object. */
	jpeg_create_decompress ( &cinfo );

	/* Step 2: specify data source (eg, a file) */
	//jpeg_stdio_src ( &cinfo, fp );
	JPEGSourceManager( &cinfo, fp );

	/* Step 3: read file parameters with jpeg_read_header() */
	(void) jpeg_read_header ( &cinfo, TRUE );

	/* Step 5: Start decompressor */
	(void) jpeg_start_decompress ( &cinfo );

	/* allocate Ilib image as either grayscale or rgb */
	img = ia_image_new(cinfo.output_width, cinfo.output_height, (cinfo.output_components == 1 ? IAT_UINT_8 : IAT_UINT_32), (cinfo.output_components==1?IA_IMAGE_GRAY:IA_IMAGE_RGB));

	if (cinfo.output_components>1 && cinfo.output_components<4)
	{
		line_buf=(ia_uint8_t*)malloc(cinfo.output_width * cinfo.output_components);
	}

	/* Step 6: while (scan lines remain to be read) */
	for ( loop = 0; loop < cinfo.output_height; loop++ ) 
	{
		if (cinfo.output_components == 1)
		{
			row_pointer[0] = &((ia_uint8_t*)img->pixels.data)[loop * cinfo.output_width *
			cinfo.output_components];
		} else
		{
			row_pointer[0]=(JSAMPROW)line_buf;
		}

		(void) jpeg_read_scanlines ( &cinfo, row_pointer, 1 );

		if (line_buf)
		{
			ia_uint16_t i;
			ia_uint8_t* p=line_buf;
			for (i=0; i<cinfo.output_width; i++)
			{
				switch (cinfo.output_components)
				{
				case 2:
					{
					ia_uint32_t c=*p | ((*p+1) << 8);
					img->set_pixel(img, i, loop, IA_RGB((c >> 11) & 0x1F, (c >> 5) & 0x3F, c & 0x1F));
					p+=2;
					}
					break;
				case 3:
					img->set_pixel(img, i, loop, IA_RGB(*(p+0), *(p+1), *(p+2)));
					p+=3;
					break;
				}
			}
		}
	}

	if (line_buf) free(line_buf);

	/* Step 7: Finish decompression */
	(void) jpeg_finish_decompress ( &cinfo );

	/* Step 8: Release JPEG decompression object */
	jpeg_destroy_decompress ( &cinfo );

	fclose(fp);

	if (img->format == IAT_UINT_8)
	{
		ia_image_p img_rgb=img->convert_rgb(img);
		img->destroy(img);
		return img_rgb;
	}
	return img;
}

/* Write JPEG file */

typedef struct _DestinationManager
{
	struct jpeg_destination_mgr manager;
	void* output;
	JOCTET *buffer;
} DestinationManager;

static boolean EmptyOutputBuffer(j_compress_ptr cinfo)
{
	DestinationManager *destination;

	destination=(DestinationManager *) cinfo->dest;
	destination->manager.free_in_buffer=(size_t) fwrite(destination->buffer, 1, MaxBufferExtent, (FILE*)destination->output);
	if (destination->manager.free_in_buffer != MaxBufferExtent)
		ERREXIT(cinfo, JERR_FILE_WRITE);
	destination->manager.next_output_byte=destination->buffer;
	return 1;
}

static void InitializeDestination(j_compress_ptr cinfo)
{
	DestinationManager *destination;

	destination=(DestinationManager *) cinfo->dest;
	destination->buffer=(JOCTET *) (*cinfo->mem->alloc_small)
		((j_common_ptr) cinfo, JPOOL_IMAGE, MaxBufferExtent*sizeof(JOCTET));
	destination->manager.next_output_byte=destination->buffer;
	destination->manager.free_in_buffer=MaxBufferExtent;
}

static void TerminateDestination(j_compress_ptr cinfo)
{
	DestinationManager *destination;

	destination=(DestinationManager *) cinfo->dest;
	if ((MaxBufferExtent - (int) destination->manager.free_in_buffer) > 0)
	{
		size_t count;

		count=fwrite(destination->buffer, 1, MaxBufferExtent-destination->manager.free_in_buffer, (FILE*)destination->output);
		if (count != (size_t)(MaxBufferExtent-destination->manager.free_in_buffer))
			ERREXIT(cinfo,JERR_FILE_WRITE);
	}
	//fflush((FILE*)destination->output);
}

static void JPEGDestinationManager(j_compress_ptr cinfo, FILE* imageFile)
{
  DestinationManager *destination;

	cinfo->dest=(struct jpeg_destination_mgr *) (*cinfo->mem->alloc_small)
		((j_common_ptr) cinfo, JPOOL_IMAGE, sizeof(DestinationManager));
	destination=(DestinationManager *) cinfo->dest;
	destination->manager.init_destination=InitializeDestination;
	destination->manager.empty_output_buffer=EmptyOutputBuffer;
	destination->manager.term_destination=TerminateDestination;
	destination->output=imageFile;
}

void ia_image_save_jpeg(ia_image_p image, const ia_string_t dst)
{
	/* This struct contains the JPEG compression parameters and pointers to
	* working space (which is allocated as needed by the JPEG library).
	* It is possible to have several such structures, representing multiple
	* compression/decompression processes, in existence at once.  We refer
	* to any one struct (and its associated working data) as a "JPEG object".
	*/
	struct jpeg_compress_struct cinfo;
	/* This struct represents a JPEG error handler.  It is declared separately
	* because applications often want to supply a specialized error handler
	* (see the second half of this file for an example).  But here we just
	* take the easy way out and use the standard error handler, which will
	* print a message on stderr and call exit() if compression fails.
	* Note that this struct must live as long as the main JPEG parameter
	* struct, to avoid dangling-pointer problems.
	*/
	struct jpeg_error_mgr jerr;
	/* More stuff */
	FILE * outfile;		/* target file */
	JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
	int row_stride;		/* physical row width in image buffer */
	ia_uint8_t* line_buffer;
	ia_int32_t format_size;
	ia_image_p normalized=0;

	/* Step 1: allocate and initialize JPEG compression object */

	/* We have to set up the error handler first, in case the initialization
	* step fails.  (Unlikely, but it could happen if you are out of memory.)
	* This routine fills in the contents of struct jerr, and returns jerr's
	* address which we place into the link field in cinfo.
	*/
	cinfo.err = jpeg_std_error(&jerr);
	/* Now we can initialize the JPEG compression object. */
	jpeg_create_compress(&cinfo);

	/* Step 2: specify data destination (eg, a file) */
	/* Note: steps 2 and 3 can be done in either order. */

	/* Here we use the library-supplied code to send compressed data to a
	* stdio stream.  You can also write your own code to do something else.
	* VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
	* requires it in order to write binary files.
	*/
	if ((outfile = fopen(dst, "wb")) == NULL)
	{
		fprintf(stderr, "can't open %s\n", dst);
		exit(1);
	}

	/* jpeg_stdio_dest(&cinfo, outfile); */
	JPEGDestinationManager( &cinfo, outfile );

	/* Step 3: set parameters for compression */

	/* First we supply a description of the input image.
	* Four fields of the cinfo struct must be filled in:
	*/
	cinfo.image_width = image->width; 	/* image width and height, in pixels */
	cinfo.image_height = image->height;
	cinfo.input_components = image->is_gray?1:3;		/* # of color components per pixel */
	cinfo.in_color_space = image->is_gray?JCS_GRAYSCALE:JCS_RGB; 	/* colorspace of input image */
	/* Now use the library's routine to set default compression parameters.
	* (You must set at least cinfo.in_color_space before calling this,
	* since the defaults depend on the source color space.)
	*/
	jpeg_set_defaults(&cinfo);
	/* Now you can set any non-default parameters you wish to.
	* Here we just illustrate the use of quality (quantization table) scaling:
	*/
	jpeg_set_quality(&cinfo, DEFAULT_JPEG_QUALITY, TRUE /* limit to baseline-JPEG values */);

	/* Step 4: Start compressor */

	/* TRUE ensures that we will write a complete interchange-JPEG file.
	* Pass TRUE unless you are very sure of what you're doing.
	*/
	jpeg_start_compress(&cinfo, TRUE);

	/* Step 5: while (scan lines remain to be written) */
	/*           jpeg_write_scanlines(...); */

	/* Here we use the library's state variable cinfo.next_scanline as the
	* loop counter, so that we don't have to keep track ourselves.
	* To keep things simple, we pass one scanline per call; you can pass
	* more if you wish, though.
	*/
	row_stride = image->width * (image->is_gray?1:3);	/* JSAMPLEs per row in image_buffer */
	line_buffer=(ia_uint8_t*)malloc(row_stride);
	format_size=ia_format_size(image->format);
	while (cinfo.next_scanline < cinfo.image_height) 
	{
		/* jpeg_write_scanlines expects an array of pointers to scanlines.
		* Here the array is only one element long, but you could pass
		* more than one scanline at a time if that's more convenient.
		*/
		if (format_size == 8)
		{
			row_pointer[0]=(JSAMPROW)((char*)image->pixels.data + cinfo.next_scanline * row_stride);
		} 
		else
		if (format_size == 24 && !image->is_gray)
		{
			row_pointer[0]=(JSAMPROW)((char*)image->pixels.data + cinfo.next_scanline * row_stride);
		} 
		else
		{
			int i;
			if (image->is_gray)
			{
				if (!normalized) 
				{
					normalized=image->copy(image);
					normalized->normalize_colors(normalized, 0, 0, 0, 255);
				}
				for (i=0; i<image->width; i++)
				{
					line_buffer[i]=(ia_uint8_t)normalized->get_pixel(normalized, i, cinfo.next_scanline);
				}
			} 
			else
			{
				ia_uint32_t color;
				for (i=0; i<image->width; i++)
				{
					color=image->get_pixel(image, i, cinfo.next_scanline);
					line_buffer[i*3+0]=(ia_uint8_t)IA_RED(color);
					line_buffer[i*3+1]=(ia_uint8_t)IA_GREEN(color);
					line_buffer[i*3+2]=(ia_uint8_t)IA_BLUE(color);
				}
			}
			row_pointer[0] = line_buffer;
		}
		(void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

	/* Step 6: Finish compression */

	jpeg_finish_compress(&cinfo);
	/* After finish_compress, we can close the output file. */
	fclose(outfile);
	free(line_buffer);

	if (normalized)
	{
		normalized->destroy(normalized);
	}

	/* Step 7: release JPEG compression object */

	/* This is an important step since it will release a good deal of memory. */
	jpeg_destroy_compress(&cinfo);
}


/*
 * The routine that will replace the standard error_exit method:
 */
void my_error_exit(j_common_ptr cinfo)
{
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  /* (*cinfo->err->output_message) (cinfo); */

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}

#endif /* HAVE_JPEGLIB */

