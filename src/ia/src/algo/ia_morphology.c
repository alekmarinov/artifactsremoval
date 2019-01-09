/*********************************************************************/
/*                                                                   */
/* Copyright (C) 2005, Alexander Marinov, Nadezhda Zlateva           */
/*                                                                   */
/* Project:       ia                                                 */
/* Filename:      ia_morphology.h                                    */
/* Description:   Morphology algorithms                              */
/*                                                                   */
/*********************************************************************/
#define CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include <malloc.h>
#include <stdio.h>
#include <math.h>
#include <ia/algo/ia_morphology.h>

ia_image_p ia_morphology_dilation(ia_image_p image, ia_image_p structure)
{
	ia_int32_t i, j, k, l, w2 = (structure->width >> 1), h2 = (structure->height >> 1);
	ia_image_p output = ia_image_new(image->width, image->height, image->format, IA_IMAGE_GRAY);
	output->fill(output, 0);

	for (i=-h2; i<((structure->height+1) >> 1); i++)
		for (j=-w2; j<((structure->width+1) >> 1); j++)
			if (structure->get_pixel(structure, w2+j, h2+i))
				for (k=0; k<image->height; k++)
					for (l=0; l<image->width; l++)
					{
						ia_uint32_t color=image->get_pixel(image, l, k);
						if (color)
							output->set_pixel(output, l+j, k+i, color);
					}
	return output;
}

ia_image_p ia_morphology_erosion (ia_image_p image, ia_image_p structure)
{
	ia_int32_t i, j, k, l, w2 = (structure->width >> 1), h2 = (structure->height >> 1);
	ia_image_p output = ia_image_new(image->width, image->height, image->format, IA_IMAGE_GRAY);
	output->fill(output, 0);

	for (k=0; k<image->height; k++)
		for (l=0; l<image->width; l++)
		{
			for (i=-h2; i<((structure->height+1) >> 1); i++)
				for (j=-w2; j<((structure->width+1) >> 1); j++)
					if (structure->get_pixel(structure, w2+j, h2+i))
							if (!image->get_pixel(image, l-j, k-i)) goto failed;

			output->set_pixel(output, l, k, image->get_pixel(image, l, k));
failed:;
		}
	return output;
}

ia_image_p ia_morphology_opening(ia_image_p image, ia_image_p structure)
{
	ia_image_p erosion=ia_morphology_erosion(image, structure);
	ia_image_p dilation=ia_morphology_dilation(erosion, structure);
	erosion->destroy(erosion);
	return dilation;
}

ia_image_p ia_morphology_closing(ia_image_p image, ia_image_p structure)
{
	ia_image_p dilation=ia_morphology_dilation(image, structure);
	ia_image_p erosion=ia_morphology_erosion(dilation, structure);
	dilation->destroy(dilation);
	return erosion;
}
