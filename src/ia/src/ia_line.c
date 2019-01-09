/*********************************************************************/
/*                                                                   */
/* Copyright (C) 2005, Alexander Marinov, Nadezhda Zlateva           */
/*                                                                   */
/* Project:       ia                                                 */
/* Filename:      ia_line.c                                          */
/* Description:   Line module implementation                         */
/*                                                                   */
/*********************************************************************/

#include <math.h>
#include <ia/ia_line.h>

/*********************************************************************/
/*                        Local prototypes                           */
/*********************************************************************/

static ia_int32_t ia_line_clipping_code(ia_double_t x, ia_double_t y, ia_double_t xcl1, ia_double_t ycl1, ia_double_t xcl2, ia_double_t ycl2);
static ia_int32_t ia_line_clip(ia_double_t *x, ia_double_t *y, ia_double_t xclipl, ia_double_t yclipb, ia_double_t xclipr, ia_double_t yclipt);

static ia_int32_t ia_line_clipping_code(ia_double_t x, ia_double_t y, ia_double_t xcl1, ia_double_t ycl1, ia_double_t xcl2, ia_double_t ycl2)
{
	/*   Compute the endpoia_int32_t codes for clip(). */
	ia_int32_t code = 0;
	if (x < xcl1) code = code | 0x1;
	if (x > xcl2) code = code | 0x2;
	if (y < ycl1) code = code | 0x4;
	if (y > ycl2) code = code | 0x8;
	return code;
}

static ia_int32_t ia_line_clip(ia_double_t *x, ia_double_t *y, ia_double_t xclipl, ia_double_t yclipb, ia_double_t xclipr, ia_double_t yclipt)
{
	/*   
		Clipping routine: Cohen Sutherland algorithm.
		If Clip ==2 the segment is outside the boundary.
		If Clip ==1 the segment has one poia_int32_t outside the boundary.
		If Clip ==0 the segment is inside the boundary.

		_Input parameters:

		x[2], y[2] : Segment coordinates
		xclipl, yclipb, xclipr, yclipt : Clipping boundary

		_Output parameters:

		x[2], y[2] : New segment coordinates
	*/
	const ia_double_t P=10000;
	ia_int32_t clip = 0;
	ia_int32_t i, code1, code2;
	ia_double_t xt=0, yt=0;
	ia_int32_t clipped = 0; /* this variable could be used in a future version */

	for (i=0; i<2; i++)
	{
		if (fabs(xclipl-x[i]) <= fabs(xclipr-xclipl)/P) x[i] = xclipl;
		if (fabs(xclipr-x[i]) <= fabs(xclipr-xclipl)/P) x[i] = xclipr;
		if (fabs(yclipb-y[i]) <= fabs(yclipt-yclipb)/P) y[i] = yclipb;
		if (fabs(yclipt-y[i]) <= fabs(yclipt-yclipb)/P) y[i] = yclipt;
	}

	/* Compute the first endpoia_int32_t codes. */
	code1 = ia_line_clipping_code(x[0],y[0],xclipl,yclipb,xclipr,yclipt);
	code2 = ia_line_clipping_code(x[1],y[1],xclipl,yclipb,xclipr,yclipt);

	while(code1 + code2) 
	{
		ia_int32_t ic = code1;
		clipped = 1;

		/* The line lies entirely outside the clipping boundary */

		if (code1&code2) 
		{
			clip = 2;
			return clip;
		}

		/* The line is subdivided ia_int32_to several parts */
		if (ic == 0) 
		{
			ic = code2;
		}

		if (ic & 0x1) 
		{
			yt = y[0] + (y[1]-y[0])*(xclipl-x[0])/(x[1]-x[0]);
			xt = xclipl;
		}

		if (ic & 0x2) 
		{
			yt = y[0] + (y[1]-y[0])*(xclipr-x[0])/(x[1]-x[0]);
			xt = xclipr;
		}

		if (ic & 0x4) 
		{
			xt = x[0] + (x[1]-x[0])*(yclipb-y[0])/(y[1]-y[0]);
			yt = yclipb;
		}

		if (ic & 0x8) 
		{
			xt = x[0] + (x[1]-x[0])*(yclipt-y[0])/(y[1]-y[0]);
			yt = yclipt;
		}

		if (ic == code1) 
		{
			x[0]  = xt;
			y[0]  = yt;
			code1 = ia_line_clipping_code(xt,yt,xclipl,yclipb,xclipr,yclipt);
		} else 
		{
			x[1]  = xt;
			y[1]  = yt;
			code2 = ia_line_clipping_code(xt,yt,xclipl,yclipb,xclipr,yclipt);
		}
	}
	clip = clipped;
	return clip;
}

void ia_line_draw (ia_int32_t x0, ia_int32_t y0, ia_int32_t x1, ia_int32_t y1, ia_rect_p clip_rgn, ia_line_call_t line_callback, void* param)
{
	ia_int32_t x, y, dx, dy, sx, sy, decx, decy, ax, ay, max, var;

	/* clip line */
	{
		ia_double_t  x[2] = {x0, x1};
		ia_double_t  y[2] = {y0, y1};

		if (clip_rgn)
			if (ia_line_clip(x, y, clip_rgn->l, clip_rgn->t, clip_rgn->r, clip_rgn->b) == 2)
				return;

		x0 = (ia_int32_t)(x[0]);
		y0 = (ia_int32_t)(y[0]);
		x1 = (ia_int32_t)(x[1]);
		y1 = (ia_int32_t)(y[1]);
	}

	/* starting poia_int32_t of line */
	x = x0, y = y0;

	/* direction of line */
	dx = x1-x0, dy = y1-y0;

	/* increment or decrement depending on direction of line */
	sx = (dx > 0 ? 1 : (dx < 0 ? -1 : 0));
	sy = (dy > 0 ? 1 : (dy < 0 ? -1 : 0));

	/* decision parameters for voxel selection */
	if ( dx < 0 ) dx = -dx;
	if ( dy < 0 ) dy = -dy;
	ax = 2*dx, ay = 2*dy;

	/* determine largest direction component, single-step related variable */
	max = dx, var = 0;
	if ( dy > max ) { var = 1; }

	/* traverse Bresenham line */
	switch ( var )
	{
	case 0:  /* single-step in x-direction */
		for (decy=ay-dx; ; x += sx, decy += ay)
		{
			/* process pixel */
			line_callback(param, x, y);

			/* take Bresenham step */
			if ( x == x1 ) break;
			if ( decy >= 0 ) { decy -= ax; y += sy; }
		}
		break;
	case 1:  /* single-step in y-direction */
		for (decx=ax-dy; ; y += sy, decx += ax)
		{
			/* process pixel */
			line_callback(param, x, y);

			/* take Bresenham step */
			if ( y == y1 ) break;
			if ( decx >= 0 ) { decx -= ay; x += sx; }
		}
		break;
	}
}

