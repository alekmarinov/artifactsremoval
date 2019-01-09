/*********************************************************************/
/*                                                                   */
/* Copyright (C) 2005, Alexander Marinov, Nadezhda Zlateva           */
/*                                                                   */
/* Project:       ia                                                 */
/* Filename:      ia_binarize.h                                      */
/* Description:   Binarization module interface                      */
/*                                                                   */
/*********************************************************************/

#ifndef __IA_BINARIZE_H
#define __IA_BINARIZE_H

#include <ia/ia_image.h>

/*********************************************************************/
/*                 Binarization API interface                        */
/*********************************************************************/
void IA_API ia_binarize_level(ia_image_p, ia_uint32_t);
int IA_API ia_binarize_otsu(ia_image_p);
void IA_API ia_binarize_line_otsu(ia_int32_t*, ia_int32_t);

#endif
