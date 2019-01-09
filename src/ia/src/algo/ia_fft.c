/*********************************************************************/
/*                                                                   */
/* Copyright (C) 2005, Alexander Marinov, Nadezhda Zlateva           */
/*                                                                   */
/* Project:       ia                                                 */
/* Filename:      ia_fft.h                                           */
/* Description:   Fast Fourrier Transform                            */
/*                                                                   */
/*********************************************************************/
#define CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include <stdio.h>

#ifndef _USE_MATH_DEFINES
	#define _USE_MATH_DEFINES
#endif

#include <math.h>
#include <ia/algo/ia_fft.h>

static int FFT_STATE=0, FFT_SIZE=0;
static ia_double_t *CN=0, *SN=0;

ia_complex_t CONJG( ia_complex_t a )
{
	a.im = -a.im; 
	return a;
}

ia_complex_t CMULT( ia_complex_t a, ia_complex_t b )
{
	ia_complex_t sum;
	sum.re = a.re * b.re - a.im * b.im;
	sum.im = a.re * b.im + a.im * b.re;
	return sum;
}

ia_complex_t CSMULT( ia_complex_t a, ia_double_t b )
{
	ia_complex_t sum;
	sum.re = a.re * b;
	sum.im = a.im * b;
	return sum;
}

/* calculates sin and cos values of 2Pi*i/N */
static void MRFFT_INIT( int  N 	/* N: Number of points */)
{
 ia_double_t TPN;
 int I;

 if (N!=FFT_SIZE)
 {
	if (SN) free(SN);
	if (CN) free(CN);
    SN = (ia_double_t *)malloc( N * sizeof(ia_double_t) );
	CN = (ia_double_t *)malloc( N * sizeof(ia_double_t) );

	TPN = 2.0 * M_PI / (ia_double_t)N;
	for (I=0; I!=N; I++)
	{ 
		SN[I] = -sin(TPN * (ia_double_t)I);  CN[I] = cos(TPN * (ia_double_t)I); 
	}
	FFT_SIZE=N;
 }
}


static void MRFFT( ia_complex_t *x, ia_complex_t *Y, int N, int direction)
{ /*MRFFT*/
/****************************************************************
 * appropriate for N not perfect power of 2
 * This subroutine accepts a ia_complex_t vector of samples, and
 * computes the corresponding ia_complex_t FFT, or inverse FFT.
 * This routine was derived from work done by Murlin S.
 * Corrington and CHein.  This algorithm is especially useful
 * for arbitrary transform lengths which are not a power of two.
 * The algorithm first chooses the most efficient radix sequence,
 * and then computes the transform accordingly.  Warnings can be
 * enabled for less desirable or relatively prime transform lengths.
 *
 * Inputs:	 x: input vector
 *		 N: Number of points
 *		 direction: (+1)=Forward DFT, (-1)=Inverse DFT
 * Output:	 Y: Output vector
 *
 *****************************************************************/

	int 	I, itmp, sw, nsw, n_stages, Aradix, Bradix, NAB, NDIV, No,
			row, col, ct,  A, B, C, Bcount, Ccount, CSTP, ABN, stage, 	
			FF[2][40], base[40], cntr, level;

	ia_double_t	ftmp, Real, Imag, *Xr[2], *Xi[2];

	/* Allocate some memory */
	for (I=0; I!=2; I++)
	{
		Xr[I] = (ia_double_t *)malloc( N * sizeof( ia_double_t ) );
		if (Xr[I]==0) printf("ERROR:%c Not Enough MEMORY Failure MRFFT Xr%d.\n", 7, I);
		
		Xi[I] = (ia_double_t *)malloc( N * sizeof( ia_double_t ) );
		if (Xi[I]==0) printf("ERROR:%c Not Enough MEMORY Failure MRFFT Xi%d.\n", 7, I);
	}

	for (I=0; I!=N; I++)  
	{  
		Xr[0][I] = x[I].re;  
		Xi[0][I] = x[I].im;  
	}

	if (direction<0)  
		for (I=0; I!=N; I++) SN[I] = -SN[I];
	
	No = N;  n_stages = 0;
	do
	{
		I = (int)sqrt((ia_double_t)No);
		while ( (ia_double_t)No/(ia_double_t)I != (ia_double_t)(No/I) ) I = I - 1;

		No = No/I;   
		FF[0][n_stages] = I;	
		FF[1][n_stages] = No;
		n_stages = n_stages + 1;
	} while ((No>5) && (I != 1));
	if (I>9) printf("	Innefficient transform length N (factor=%d).\n", I);

	sw = 1; nsw = 0;  stage = 0;
	while (stage!=n_stages)
	{ 
		for (I=0; I!=N; I++) 
		{ 
			Xr[sw][I] = 0.0;  
			Xi[sw][I] = 0.0; 
		}
		
		Aradix = FF[0][stage];  
		Bradix = FF[1][stage];

		/* Multiply A-matrix */
		NAB = N / (Aradix * Bradix);  
		NDIV = N / Aradix;
		
		row = 0;  ct = 0;
		while (row<N)
		{ 
			CSTP = 0;	A = Aradix;	ABN = 0;
			
			while (A!=0)
			{ 
				Bcount = Bradix - 1;	 B = 0;	 col = ct;
				do
				{ 
					Ccount = Aradix; 	 C = B;  Real = 0.0;	Imag = 0.0;
					while (Ccount!=0)
					{ 
						if (C>=N) C = C - N;
						Real = Real + CN[C] * Xr[nsw][col] - SN[C] * Xi[nsw][col];
						Imag = Imag + CN[C] * Xi[nsw][col] + SN[C] * Xr[nsw][col];
						col = col + Bradix; C = C + CSTP; Ccount = Ccount - 1;
					} 

					Xr[sw][row] = Real;   
					Xi[sw][row] = Imag;
					
					B = B + ABN;  row = row + 1; Bcount = Bcount - 1;
					if (Bcount!=-1) col = Bradix - Bcount - 1 + ct;
				} while (Bcount != -1); 

				CSTP = CSTP + NDIV;
				A = A - 1; ABN = ABN + NAB;
			} 
			ct = col - Bradix + 1;
		} 
		itmp = sw;  sw = nsw;  nsw = itmp;  stage = stage + 1;
	} 

	for (I=0; I!=N; I++) 
	{ 
		Xr[sw][I] = 0.0;  
		Xi[sw][I] = 0.0; 
	}

	Aradix = FF[0][n_stages-1];  Bradix = FF[1][n_stages-1];
	/* Multiply B-matrix */
	NDIV = N / Bradix;  row = 0;  col = 0;
	while (row < N-1)
	{ 
		CSTP = 0;  Bcount = Bradix;
		while (Bcount!=0)
		{ 
			Ccount = Bradix;  C = 0;  Real = 0.0;  Imag = 0.0;
			while (Ccount!=0)
			{ 
				if (C>=N) C = C - N;
				Real = Real + CN[C] * Xr[nsw][col] - SN[C] * Xi[nsw][col];
				Imag = Imag + CN[C] * Xi[nsw][col] + SN[C] * Xr[nsw][col];
				Ccount = Ccount - 1;    C = C + CSTP;  col = col + 1;
			} 

			Xr[sw][row] = Real;   
			Xi[sw][row] = Imag;
			
			CSTP = CSTP + NDIV;   Bcount = Bcount - 1;  row = row + 1;
			if (Bcount!=0) col = col - Bradix;
		} 
	} 

	cntr = 0;
	FF[1][n_stages] = 1;	
	FF[0][n_stages] = FF[1][n_stages-1];
	
	base[n_stages+1] = 0;	
	base[n_stages] = 0;
	
	level = n_stages;
	do
	{
		if (level==0)
		{ 
			Y[cntr].re = Xr[sw][base[0]];  
			Y[cntr].im = Xi[sw][base[0]];
			cntr = cntr + 1;    
			base[level] = base[level] + FF[1][level];
		}
		else
		{	
			level = level - 1;  base[level] = base[level+1]; 
		}
		
		while ((level!=n_stages) && (base[level]>=FF[0][level] * FF[1][level] ))
		{ 
			level = level + 1;  
			base[level] = base[level] + FF[1][level]; 
		}
	} while ( base[level] < FF[0][level] * FF[1][level] );

	if (direction>0)
	{
		ftmp = 1.0 / (ia_double_t)N;
		for (I=0; I!=N; I++)  
		{ 
			Y[I].re = ftmp * Y[I].re;   Y[I].im = ftmp * Y[I].im; 
		}
	}

	/* Return Sine table to original if it was negated. */
	if (direction<0)  
		for (I=0; I!=N; I++) SN[I] = -SN[I];
	for (I=0; I!=2; I++) 
	{ 
		free( Xr[I] );  
		free(Xi[I]); 
	}
} /*MRFFT*/

/* FFT routine for perfect powers of twos only. */
static void fft_init( int N)
{
	int k, mmax, m, step, i, j, sz;
	ia_double_t phi, sinphi, twid, pi2;
 
	sz = 1; m = 2;
	while (m<N) 
	{	
		sz = sz + 1; m = m << 1; 
	}
	
	if (m!=N) 
	{ 
		if (N>2502) printf("	(Note: %d is not a perfect power of two.)\n	( FFT may not be as efficient.)\n", N);
		
		MRFFT_INIT( N);

		FFT_STATE = 2;  
		FFT_SIZE = N;  
		
		return;
	}
	else FFT_STATE = 1;
	
	FFT_SIZE = N;
	
	if (SN) free(SN);
	if (CN) free(CN);
	SN = (ia_double_t *)malloc( N * sizeof(ia_double_t) );
	CN = (ia_double_t *)malloc( N * sizeof(ia_double_t) );

	pi2 = 0.5 * M_PI;
	mmax = 1;  k = 0;
	while (mmax < N)
	{ /*mmax*/
		step = mmax << 1;    
		phi = pi2 / (float)mmax;
		
		sinphi = sin(phi);   
		twid = sin(phi * 2.0);
		SN[k] = sinphi; 
		CN[k] = twid;
		
		k = k + 1;  m = 0;
		while (m < mmax)
		{ /*m*/
			i = m;
			while (i<N) 
			{ 
				j = i + mmax;  
				i = i + step; 
			}
			m = m + 1;
		} /*m*/
		
		mmax = step;
	} /*mmax*/
}

static void fft( ia_complex_t *data, int N, int direction)
{
	int i, step, j, m, mmax;
	ia_double_t sinphi, coef_i, coef_r, sci, scr, tmpr, tmpi, fdir, scale;
	int n2, k;
	ia_complex_t ctmp;

	n2 = N >> 1;
	j = 0;   i = 0;   
	fdir = -(ia_double_t)direction;

	/* Perform decimation in time by bit reversing up front. */
	while (i<N)
	{ /*i*/
		if (i<j) 
		{ 
			ctmp = data[j];  
			data[j] = data[i];  
			data[i] = ctmp; 
		}
			m = n2;
		while ( (j>m-1) && (m>=2) ) 
		{ 
			j = j - m;  m = m >> 1; 
		}
		j = j + m;  i = i + 1;
	} /*i*/
	
	mmax = 1;  k = 0;
	while (mmax < N)
	{ /*mmax*/
		step = mmax << 1;
		sinphi = fdir * SN[k];
		sci = fdir * CN[k];
		scr = -2.0 * sinphi * sinphi;
		k = k + 1;    m = 0;
		coef_r = 1.0;    coef_i = 0.0;
		
		while (m < mmax)
		{ /*m*/
			i = m;
			while (i<N)
			{ /*i*/
				j = i + mmax;
				tmpr = (coef_r * data[j].re) - (coef_i * data[j].im);
				tmpi = (coef_r * data[j].im) + (coef_i * data[j].re);
				data[j].re = data[i].re - tmpr;  data[j].im = data[i].im - tmpi;
				data[i].re = data[i].re + tmpr;  data[i].im = data[i].im + tmpi;
				i = i + step;
			} /*i*/
	
			tmpr = coef_r;  m = m + 1;
			coef_r = coef_r + (coef_r * scr) - (coef_i * sci);
			coef_i = coef_i + (coef_i * scr) + (tmpr * sci);
		} /*m*/
		mmax = step;
	} /*mmax*/

	if (direction>0)
	{
		scale = 1.0 / (ia_double_t)N;
		for (i=0; i!=N; i++) 
		{
			data[i].re = scale * data[i].re;
			data[i].im = scale * data[i].im;
		}
	}
} 


static void FFT( ia_complex_t *x, ia_complex_t *Y, int N, int direction)
{
	if ((FFT_STATE==0) || (N != FFT_SIZE))  
		fft_init( N);

	if (FFT_STATE==1) 
	{
		int i;
		for (i=0; i<N; i++) Y[i] = x[i];
		fft( Y, N, direction);
	}
	 else
		MRFFT( x, Y, N, direction);
}

void ia_fft(ia_int32_t m, ia_complex_p x, ia_int8_t direction, ia_double_t scale, ia_complex_p w)
{
	ia_int32_t i;
	ia_complex_p _w;

	_w = (ia_complex_p)malloc( m * sizeof(ia_complex_t) );
	
	FFT( x, _w, m, direction);

	for (i=0; i<m; i++) 
	{
		w[i] =CSMULT(_w[i], (ia_double_t)scale);
	}
	
	free(_w);
}
