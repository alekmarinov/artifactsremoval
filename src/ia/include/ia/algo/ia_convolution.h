/*********************************************************************/
/*                                                                   */
/* Copyright (C) 2005, Alexander Marinov, Nadezhda Zlateva           */
/*                                                                   */
/* Project:       ia                                                 */
/* Filename:      ia_convolution.h                                   */
/* Description:   Convolution algorithms                             */
/*                                                                   */
/*********************************************************************/

#ifndef __IA_CONVOLUTION_H
#define __IA_CONVOLUTION_H

#include <ia/ia.h>

/*********************************************************************/
/*                    Convolution API interface                     */
/*********************************************************************/

IA_API void ia_convolution(ia_int32_t, ia_complex_p, ia_int32_t, ia_complex_p, ia_bool_t, ia_int8_t, ia_complex_p);
IA_API void ia_convolution_by_sums(ia_int32_t, ia_complex_p, ia_int32_t, ia_complex_p, ia_complex_p);

#endif /* __IA_CONVOLUTION_H */
