/*********************************************************************/
/*                                                                   */
/* Copyright (C) 2005, Alexander Marinov, Nadezhda Zlateva           */
/*                                                                   */
/* Project:       ia                                                 */
/* Filename:      ia_fft.h                                           */
/* Description:   Fast Fourrier Transform                            */
/*                                                                   */
/*********************************************************************/

#ifndef __IA_FFT_H
#define __IA_FFT_H

#include <ia/ia.h>

/*********************************************************************/
/*                         FFT API interface                         */
/*********************************************************************/

ia_complex_t CONJG( ia_complex_t a );
ia_complex_t CMULT( ia_complex_t a, ia_complex_t b );
ia_complex_t CSMULT( ia_complex_t a, ia_double_t b );

IA_API void ia_fft(
	ia_int32_t /* array size */, 
	ia_complex_p /* input array */, 
	ia_int8_t /* direction = +/-1 */, 
	ia_double_t /* scale factor */, 
	ia_complex_p /* output array */
);

#endif /* __IA_FFT_H */
