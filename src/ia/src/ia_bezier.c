/*********************************************************************/
/*                                                                   */
/* Copyright (C) 2005, Alexander Marinov, Nadezhda Zlateva           */
/*                                                                   */
/* Project:       ia                                                 */
/* Filename:      ia_bezier.c                                        */
/* Description:   Bezier curve module implementation                 */
/* Source:                                                           */
/* Paul Bourke, http://astronomy.swin.edu.au/~pbourke/curves/bezier/ */
/*                                                                   */
/*********************************************************************/

#include <math.h>
#include <ia/ia_bezier.h>

ia_pos_t ia_bezier_draw(ia_pos_p points, ia_int32_t n, ia_double_t mu)
{
	int k, kn, nn, nkn;
	double blend, muk, munk;
	double bx=0.0, by=0.0;
	ia_pos_t result;

	muk = 1;
	munk = pow(1-mu, (double)n);

	for (k=0; k<=n; k++) 
	{
		nn = n;
		kn = k;
		nkn = n - k;
		blend = muk * munk;
		muk *= mu;
		munk /= (1-mu);
		while (nn >= 1) 
		{
			blend *= nn;
			nn--;
			if (kn > 1) 
			{
				blend /= (double)kn;
				kn--;
			}
			if (nkn > 1) 
			{
				blend /= (double)nkn;
				nkn--;
			}
		}
		bx += points[k].x * blend;
		by += points[k].y * blend;
	}

	result.x=(int)bx;
	result.y=(int)by;

	return result;
}
