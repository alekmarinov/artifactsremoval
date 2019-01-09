/*********************************************************************/
/*                                                                   */
/* Copyright (C) 2005, Alexander Marinov, Nadezhda Zlateva           */
/*                                                                   */
/* Project:       ia                                                 */
/* Filename:      ia_convolution.c                                   */
/* Description:   Convolution algorithms                             */
/*                                                                   */
/*********************************************************************/

#define CRTDBG_MAP_ALLOC
#include <crtdbg.h>

/* FIXME: arrange this file */

#include <stdio.h>
#include <malloc.h>

#include <math.h>
#include <ia/algo/ia_convolution.h>
#include <ia/algo/ia_fft.h>

/* border type
	0 - zero padding
	1 - periodic
	2 - mirrored
	3 - constant
*/
void ia_convolution(ia_int32_t m, ia_complex_p x, ia_int32_t n, ia_complex_p y, ia_bool_t full_output, ia_int8_t border_type, ia_complex_p w)
{
	ia_int32_t i, c=n/2,  N = m + n -1, size;
	ia_complex_p _x, _y, _w;
	size=(full_output ? N:m);

	_x = (ia_complex_p)malloc( N * sizeof(ia_complex_t) );
	_y = (ia_complex_p)malloc( N * sizeof(ia_complex_t) );
	_w = (ia_complex_p)malloc( N * sizeof(ia_complex_t) );
	//*w = (ia_complex_p)malloc( size * sizeof(ia_complex_t) );

	if (border_type<0) border_type=0;
	else if (border_type>3) border_type=3;

	for (i=0; i<m; i++)  
	{ 
		_x[i].re = x[i].re;  
		_x[i].im = x[i].im; 
	}

	for (i=m; i<N; i++)
	{ 
		if (border_type==0) // zero padding
		{
			_x[i].re = 0.0;         
			_x[i].im = 0.0; 
		}
		else if (border_type==1) // periodic
		{
			_x[i].re = _x[i-m].re;  
			_x[i].im = _x[i-m].im;
		}else
		{
			if ( (i-m)<(n-1)/2 ) 
			{
				if (border_type==2) //mirror last elements
				{
					_x[i].re = _x[2*m-i-1].re;
					_x[i].im = _x[2*m-i-1].im;
				}else if(border_type==3) //replication
				{
					_x[i].re = _x[m-1].re; 
					_x[i].im = _x[m-1].im;
				}
			}
			else
			{
				if (border_type==2) //mirror last elements
				{
					_x[i].re = _x[n-(i-m+2)].re;
					_x[i].im = _x[n-(i-m+2)].im;
				}else if(border_type==3) //replication
				{
					_x[i].re = _x[0].re;
					_x[i].im = _x[0].im;
				}
			}
		}
	}

	
	ia_fft(N, _x, 1, 1, _x);
	
	for (i=0; i<n; i++)  
	{ 
		_y[i].re = y[i].re;  
		_y[i].im = y[i].im; 
	}

	for (i=n; i<N; i++)  
	{ 
		_y[i].re = 0.0;
		_y[i].im = 0.0;
	}

	ia_fft(N, _y, 1, 1, _y);
	
	for (i=0; i<N; i++)  
	{
		_w[i] = CMULT( _x[i], _y[i] );
	}

	ia_fft( N, _w, -1, 1, _w);
	
	if (full_output)
	{
		for (i=0; i<N; i++) 
		{
			w[i] = CSMULT( _w[i], (ia_double_t)N );
		}
	}
	else
	{
		for (i=0; i<m; i++) 
		{
			w[i] = CSMULT( _w[i+c], (ia_double_t)N );
		}
	}

	free(_x);
	free(_y);
	free(_w);
}

/*
	uses replication (borders take first or last func value), 
	not periodic continuation or zero padding or mirroring.	
*/
void ia_convolution_by_sums(ia_int32_t m, ia_complex_p x, ia_int32_t n, ia_complex_p y, ia_complex_p w)
{
	ia_int32_t i,j,c1,c2,c;
	ia_double_t sum;
	
	c1=-n/2;
	c2=n/2-1;
	
	if (n&1) c2++;

	for (i=0; i<n/2; i++)
	{
		ia_complex_t tmp=y[i];	
		y[i]=y[n-1-i];
		y[n-1-i]=tmp;
	}

	for (i=0; i<m; i++)
	{
		sum=0;
		for (j=c1; j<=c2; j++)
		{
			c=i+j;
			if (c>=0 && c<m)
			{
				sum+=y[j-c1].re * x[c].re;
			}
			else if (c>=m)
					sum+=y[j-c1].re * x[m-1].re;
					//sum+=y[j-c1].re * x[i-j].re;
			else
					sum+=y[j-c1].re * x[0].re;

		}
		w[i].re=sum;
	}
}
