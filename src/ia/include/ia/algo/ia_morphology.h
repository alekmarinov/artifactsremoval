/*********************************************************************/
/*                                                                   */
/* Copyright (C) 2005, Alexander Marinov, Nadezhda Zlateva           */
/*                                                                   */
/* Project:       ia                                                 */
/* Filename:      ia_morphology.h                                    */
/* Description:   Morphology algorithms                              */
/*                                                                   */
/*********************************************************************/

#ifndef __IA_MORPHOLOGY_H
#define __IA_MORPHOLOGY_H

#include <ia/ia_image.h>

/*********************************************************************/
/*                     Morphology API interface                      */
/*********************************************************************/

IA_API ia_image_p ia_morphology_dilation(ia_image_p, ia_image_p);
IA_API ia_image_p ia_morphology_erosion (ia_image_p, ia_image_p);
IA_API ia_image_p ia_morphology_opening (ia_image_p, ia_image_p);
IA_API ia_image_p ia_morphology_closing (ia_image_p, ia_image_p);

#endif /* __IA_MORPHOLOGY_H */
