/*********************************************************************/
/*                                                                   */
/* Copyright (C) 2005, Alexander Marinov, Nadezhda Zlateva           */
/*                                                                   */
/* Project:       ia                                                 */
/* Filename:      ia_binarize.c                                      */
/* Description:   Binarization module                                */
/*                                                                   */
/*********************************************************************/
#define CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include <math.h>
#include <ia/ia_vector.h>
#include <ia/algo/ia_binarize.h>

void ia_binarize_level(ia_image_p img, ia_uint32_t level)
{
	ia_uint16_t i,j;
	ia_int32_t min;
	ia_uint32_t max;
	ia_bool_t is_signed=ia_format_signed(img->format);
	ia_format_min_max(img->format, &min, &max);
	for (i=0; i<img->height; i++)
	for (j=0; j<img->width; j++)
	{
		ia_uint32_t c= img->get_pixel(img, j, i);
		if ((is_signed && ((ia_int32_t)c >= (ia_int32_t)level)) || (!is_signed && (c>=level)) )
		{
			img->set_pixel(img, j, i, max);
		}
		else
		{
			img->set_pixel(img, j, i, min);
		}
	}
}

/* FIXME: implement OTSU! */
void ia_binarize_otsu_(ia_image_p img)
{
        ia_int32_t min;
        ia_uint32_t max;
        img->get_min_max(img, &min, &max);
        ia_binarize_level(img, min + ((max-min)>>1));
}


/* Binarization by Otsu */
/* long histogram[256]; -  gray level histogram 
	int leftpeak,rightpeak;
	int mingray,maxgray;
	double meangray;  
*/
#define HISTSMOOTHING 6 /* length of smoothing mask for histogram */

static void calchistogram(int x,int y, int dx,int dy, ia_image_p img, long histogram[256])
{
  int i,j,k,n,r;
  long tmp[256];
  long sum;
  int maxima,maxstart,maxend,nmax;
  double meangray;
  int leftpeak,rightpeak;
  int mingray,maxgray;
  int cols,rows;
 
  cols=img->width;
  rows=img->height;
  for(i=0;i<256;i++) histogram[i]=0;/* put zeros to histogram */
  
  meangray=0;
//  fprintf(stderr,"Calculating histogram\n");
  for(i=y;(i<y+dy)&&(i<rows);i++)/* calculate histogram */
    for(j=x;(j<x+dx)&&(j<cols);j++)
      {
        histogram[img->get_pixel(img, j, i)]++;
        meangray+=img->get_pixel(img, j, i);
      }
  meangray/=(dx*dy);/* calculate mean gray level */
  mingray=0; maxgray=155;
  for(i=0;i<256;i++)
    if(histogram[i]!=0) {mingray=i; break;}
  for(i=255;i>=0;i--)
    if(histogram[i]!=0) {maxgray=i; break;}
      /* smooth the histogram */
//  fprintf(stderr,"Smoothing histogram\n");      
  r=0;
  do      
    {
      if(r>0)/* skip smoothing on first run--maybe the histogram is already ok?*/
        {
          for(i=0;i<256;i++) tmp[i]=histogram[i];
          for(i=0;i<256;i++)
            {
              n=0;
              sum=0;
              for(k=i-HISTSMOOTHING;k<=i+HISTSMOOTHING;k++)
                {
                  if((k>=0)&&(k<256))
                    {
                      sum+=tmp[k];
                      n++;
                    }
                }
              histogram[i]=ceil(1.0*sum/n+0.5);
            }
        }
      /* count the number of local maxima in histogram */

      maxima=0;
      maxstart=0;
      maxend=255;

      if(histogram[1]<=histogram[0])/* check if the leftmost
                                           point is a maximum */
        {
          for(k=0;k<256;k++)
            {
              if(histogram[k]>histogram[0]) break;
              if(histogram[k]<histogram[0])
                { maxima++; maxstart=k; leftpeak=0; break; }
            }
        }
      if(histogram[254]<=histogram[255])/* check if the rightmost
                                         point is a maximum */
        {
          for(k=255;k>=0;k--)
            {
              if(histogram[k]>histogram[255]) break;
              if(histogram[k]<histogram[255])
                { maxima++; maxend=k; rightpeak=255; break; }
            }
        }
      nmax=0;
      for(i=maxstart;i<=maxend;i++)
        {
          int a,b;
          while((histogram[i]<histogram[i-1])&&(i<=maxend)) i++;
          while((histogram[i]>=histogram[i-1])&&(i<=maxend)) i++;
          a=i;
          while((histogram[i]==histogram[i-1])&&(i<=maxend)) i--;
          b=i;
          i=a;
          if(i<maxend)
            { 
              if(nmax==0) leftpeak=(a+b)/2; 
              else rightpeak=(a+b)/2;
              maxima++;          
              nmax++;
            }
          else
            {
              leftpeak=rightpeak=meangray;
              return;
            }
        }
      r++;
    }
  while(maxima>2);
//  fprintf(stderr,"Histogram smoothed by %d smoothing runs\n",r);
}

static void calcsimplehistogram(int x,int y, int dx,int dy, ia_image_p img, long histogram[256])
{
  int i,j,k,n,r;
  long tmp[256];
  long sum;
  int maxima,maxstart,maxend;
  int cols,rows;
   double meangray;
  int leftpeak,rightpeak;
  int mingray,maxgray;

  cols=img->width; 
  rows=img->height;

  for(i=0;i<256;i++) histogram[i]=0;/* put zeros to histogram */
  
  meangray=0;
  for(i=y; (i<y+dy)&&(i<rows); i++)/* calculate histogram */
    for(j=x; (j<x+dx)&&(j<cols); j++)
      {
		  
        histogram[img->get_pixel(img, j, i)]++;
        meangray+=img->get_pixel(img, j, i);
      }
  meangray/=(dx*dy);/* calculate mean gray level */
  mingray=0; maxgray=155;
  for(i=0;i<256;i++)
    if(histogram[i]!=0) {mingray=i; break;}
  for(i=255;i>=0;i--)
    if(histogram[i]!=0) {maxgray=i; break;}
}

/* otsu */
static double calc_eta(int t,double *Xa,double *Xb, long histogram[256])
{
  #define H histogram
  static double Na,Nb,N;
  static double p1,p2;
  static double xa,xb,x,s1,s2,h1,h2;
  static int update_needed=0;
  double sigma_w, sigma_b;
  int i,j;
  
  if((t==1)||(update_needed))
    {
  /* calculate means, number of pixels--if not first iteration,
     use cached values */
      Na=0;Nb=0;N=0;xa=0;xb=0;x=0;s1=0;s2=0;h1=0;h2=0;
      for(i=0;i<256;i++)
        {
          if(H[i]==0) continue;
          N+=H[i];
          if(i<=t)
            {
              Na+=H[i];
              xa+=H[i]*i;
            }
          else
            {
              Nb+=H[i];
              xb+=H[i]*i;
            }
          x+=H[i]*i;
        }
      if((Na==0)||(Nb==0))
        {
          update_needed=1;
          return 1E20;
        }
      p1=Na/N;
      p2=Nb/N;
      xa/=Na;
      xb/=Nb;
      x/=N;
    }
  else
    {
      double suma,sumb,oldxa=xa,oldxb=xb;
      double dxa,dxb;

      suma=xa*Na+1.0*H[t]*t;
      xa=suma/(Na+H[t]);

      sumb=xb*Nb-1.0*H[t]*t;
      xb=sumb/(Nb-H[t]);
      
      dxa=xa-oldxa;
      dxb=xb-oldxb;
      
      h1=h1+2.0*t*H[t];
      h2=h2-2.0*t*H[t];
      
      s1=s1*Na+pow(1.0*t-oldxa,2)*H[t]+(Na+H[t])*(oldxa*dxa*2+dxa*dxa)-dxa*h1;
      s2=s2*Nb-pow(1.0*t-oldxb,2)*H[t]+(Nb-H[t])*(oldxb*dxb*2+dxb*dxb)-dxb*h2;
      
      Na+=H[t];
      Nb-=H[t];
      
      if(Nb==0)
        {
          update_needed=1;
          return 1E20;
        }
      
      s1/=Na;
      s2/=Nb;
      
      p1=Na/N;
      p2=Nb/N;
      
    }
  (*Xa)=xa;
  (*Xb)=xb;
  sigma_b=p1*(xa-x)*(xa-x)+p2*(xb-x)*(xb-x);
  
  sigma_w=0;
  
  if((t==1)||(update_needed))
    {
//      fprintf(stderr,"Updating, t=%d,s2=%lf\n",t,s2);
      for(i=0;i<256;i++)
        {
          if(H[i]==0) continue;
          if(i<=t)
            {
              s1+=pow(xa-i,2)*H[i];
              h1+=2.0*i*H[i];
            }
          else
            {
              s2+=pow(xb-i,2)*H[i];
              h2+=2.0*i*H[i];
            }
        }
      s1/=Na;
      s2/=Nb;
      update_needed=0;
    }
  sigma_w=p1*s1+p2*s2;
//  fprintf(stderr,"t=%d,Na=%lf,Nb=%lf,N=%lf,p1=%lf,p2=%lf,xa=%lf,xb=%lf,x=%lf,s1=%lf,s2=%lf\n",
//          t,Na,Nb,N,p1,p2,xa,xb,x,s1,s2);
   
  return sigma_w/sigma_b;
  #undef H
}

int ia_binarize_otsu(ia_image_p img)
{
  int i,j,k,l,t,tmin=0;
  double eta=1E20,a;
  double xa,xb,xamin,xbmin;
  double mean;
  long histogram[256];
  int cols,rows;
 
  cols=img->width;
  rows=img->height;
	
  //calchistogram(0,0,cols,rows, img, histogram);
  calcsimplehistogram(0,0,cols,rows, img, histogram);
  //printhistogram(histogram);
  
  for(t=1;t<255;t++)
    {
      if((t==1)||(histogram[t]!=0))
        a=calc_eta(t,&xa,&xb,histogram);
      if((a>eta)&&(a==1E20)) break;
      if(a<eta)
        {
          eta=a;
          tmin=t;
          xamin=xa;
          xbmin=xb;
        }
    }

   for(i=0; i<rows; i++) //h
    for(j=0; j<cols; j++) //w
      {
		if(img->get_pixel(img, j, i)<tmin) 
		{
			img->set_pixel(img, j, i, 0);
		}
        else 
		{
			img->set_pixel(img, j, i, 255);
		}
      }

	  return tmin;
}

static void calcsimplehistogram_line(int x, int dx, ia_int32_t *line, long histogram[256])
{
  int i;
  double meangray;
  int mingray,maxgray;

  for(i=0;i<256;i++) histogram[i]=0;/* put zeros to histogram */
  
  meangray=0;
  for(i=x; i<x+dx; i++)
  {
    histogram[line[i]]++;
    meangray+=line[i];
  }
  
  meangray/=dx;/* calculate mean gray level */
  mingray=0; maxgray=155;
  
  for(i=0;i<256;i++)
    if(histogram[i]!=0) {mingray=i; break;}
  for(i=255;i>=0;i--)
    if(histogram[i]!=0) {maxgray=i; break;}
}


static void calchistogram_line(int x, int dx, ia_int32_t *line, long histogram[256])
{
  int i,k,n,r;
  long tmp[256];
  long sum;
  int maxima,maxstart,maxend,nmax;
  double meangray;
  int leftpeak,rightpeak;
  int mingray,maxgray;
  
  for(i=0;i<256;i++) histogram[i]=0;/* put zeros to histogram */
  
  meangray=0;
//  fprintf(stderr,"Calculating histogram\n");
  for(i=x; i<x+dx; i++)
  {
	histogram[line[i]]++;
    meangray+=line[i];
  }
  meangray/=dx;/* calculate mean gray level */
  mingray=0; maxgray=155;
  for(i=0;i<256;i++)
    if(histogram[i]!=0) {mingray=i; break;}
  for(i=255;i>=0;i--)
    if(histogram[i]!=0) {maxgray=i; break;}
      /* smooth the histogram */
//  fprintf(stderr,"Smoothing histogram\n");      
  r=0;
  do      
    {
      if(r>0)/* skip smoothing on first run--maybe the histogram is already ok?*/
        {
          for(i=0;i<256;i++) tmp[i]=histogram[i];
          for(i=0;i<256;i++)
            {
              n=0;
              sum=0;
              for(k=i-HISTSMOOTHING;k<=i+HISTSMOOTHING;k++)
                {
                  if((k>=0)&&(k<256))
                    {
                      sum+=tmp[k];
                      n++;
                    }
                }
              histogram[i]=ceil(1.0*sum/n+0.5);
            }
        }
      /* count the number of local maxima in histogram */

      maxima=0;
      maxstart=0;
      maxend=255;

      if(histogram[1]<=histogram[0])/* check if the leftmost
                                           point is a maximum */
        {
          for(k=0;k<256;k++)
            {
              if(histogram[k]>histogram[0]) break;
              if(histogram[k]<histogram[0])
                { maxima++; maxstart=k; leftpeak=0; break; }
            }
        }
      if(histogram[254]<=histogram[255])/* check if the rightmost
                                         point is a maximum */
        {
          for(k=255;k>=0;k--)
            {
              if(histogram[k]>histogram[255]) break;
              if(histogram[k]<histogram[255])
                { maxima++; maxend=k; rightpeak=255; break; }
            }
        }
      nmax=0;
      for(i=maxstart;i<=maxend;i++)
        {
          int a,b;
          while((histogram[i]<histogram[i-1])&&(i<=maxend)) i++;
          while((histogram[i]>=histogram[i-1])&&(i<=maxend)) i++;
          a=i;
          while((histogram[i]==histogram[i-1])&&(i<=maxend)) i--;
          b=i;
          i=a;
          if(i<maxend)
            { 
              if(nmax==0) leftpeak=(a+b)/2; 
              else rightpeak=(a+b)/2;
              maxima++;          
              nmax++;
            }
          else
            {
              leftpeak=rightpeak=meangray;
              return;
            }
        }
      r++;
    }
  while(maxima>2);
//  fprintf(stderr,"Histogram smoothed by %d smoothing runs\n",r);
}


void ia_binarize_line_otsu(ia_int32_t *line, ia_int32_t size)
{
  int i,j,k,l,t,tmin;
  double eta=1E20,a;
  double xa,xb,xamin,xbmin;
  double mean;
  long histogram[256];

  //calchistogram_line(0, size, line, histogram);
  calcsimplehistogram_line(0, size, line, histogram);
  
  for(t=1;t<255;t++)
    {
      if((t==1)||(histogram[t]!=0))
        a=calc_eta(t,&xa,&xb,histogram);
      if((a>eta)&&(a==1E20)) break;
      if(a<eta)
        {
          eta=a;
          tmin=t;
          xamin=xa;
          xbmin=xb;
        }
    }

   for(i=0; i<size; i++) 
   {
		if(line[i]<tmin) 
		{
			line[i]=0;
		}
        else 
		{
			line[i]=255;
		}
   }
}
/* End of Binarization by Otsu */

void ia_binarize_adaptive_triangular_linear_interpolation(ia_image_p img, int subimage_w, int subimage_h)
{
	//ia_image_p threshold;
	//int ssize, sx, sy, sw, sh, i, j;
	//int subimage_center_x, subimage_center_y;
	//if (!img->is_gray)
	//{
	//	/* accept only gray images */
	//	return ;
	//}
	//threshold=ia_image_new(img->width, img->height, img->format, IA_IMAGE_GRAY);
	//if (subimage_w == 0 && subimage_h == 0)
	//{
	//	/* default subimage size */
	//	subimage_w=subimage_h=7;
	//}
	//subimage_center_x=(subimage_w+1)>>1;
	//subimage_center_y=(subimage_h+1)>>1;
	//
	///* find threshold values for each subimage center */
	//sw=img->width / subimage_w;
	//sh=img->height / subimage_h;
	//ssize=sw*sh;
	//for (sy=0; sy<img->height; sy+=subimage_h)
	//	for (sx=0; sx<img->width; sx+=subimage_w)
	//	{
	//		int average=0;
	//		for (i=sy; i<sy+subimage_h; i++)
	//			for (j=sx; j<sx+subimage_w; j++)
	//			{
	//				average+=img->get_pixel(img, j, i);
	//			}
	//		average/=ssize;
	//		threshold->set_pixel(threshold, sx+subimage_center_x, sy+subimage_center_y, average);
	//	}

	///* triangular interpolation for all pixels */
	//for (i=0; i<img->height; i++)
	//	for (j=0; j<img->width; j++)
	//	{
	//		if (j>=subimage_center_x && i>=subimage_center_y && j<img->width-subimage_center_x && i<img->height-subimage_center_y)
	//		{
	//			ia_vector_p t1, t2, t3, o1, o2, o3, x, y, m;
	//			double k1, k2;
	//			double ssx, ssy;
	//			ia_int32_t tx=j-subimage_center_x;
	//			ia_int32_t ty=i-subimage_center_y;
	//			t1=ia_vector_new(3, IAT_DOUBLE);
	//			t2=ia_vector_new(3, IAT_DOUBLE);
	//			t3=ia_vector_new(3, IAT_DOUBLE);
	//			o1=ia_vector_new(3, IAT_DOUBLE);
	//			o2=ia_vector_new(3, IAT_DOUBLE);
	//			o3=ia_vector_new(3, IAT_DOUBLE);
	//			x=ia_vector_new(3, IAT_DOUBLE);
	//			y=ia_vector_new(3, IAT_DOUBLE);
	//			m=ia_vector_new(3, IAT_DOUBLE);

	//			sx=tx/subimage_w;
	//			sy=ty/subimage_h;
	//			ssx=sx*subimage_w;
	//			ssy=sy*subimage_h;
	//			if ((double)ty < (double)tx*((double)subimage_h/subimage_w))
	//			{
	//				/* top triangle */
	//				t1->set(t1, 0, ia_double_tovalue(IAT_DOUBLE, ssx));
	//				t1->set(t1, 1, ia_double_tovalue(IAT_DOUBLE, ssy));
	//				t2->set(t2, 0, ia_double_tovalue(IAT_DOUBLE, ssx+subimage_w));
	//				t2->set(t2, 1, ia_double_tovalue(IAT_DOUBLE, ssy));
	//				t3->set(t3, 0, ia_double_tovalue(IAT_DOUBLE, ssx));
	//				t3->set(t3, 1, ia_double_tovalue(IAT_DOUBLE, ssy+subimage_h));
	//			}
	//			else
	//			{
	//				/* bottom triangle */
	//				t1->set(t1, 0, ia_double_tovalue(IAT_DOUBLE, sx+1));
	//				t1->set(t1, 1, ia_double_tovalue(IAT_DOUBLE, sy+1));
	//				t2->set(t2, 0, ia_double_tovalue(IAT_DOUBLE, sx+1));
	//				t2->set(t2, 1, ia_double_tovalue(IAT_DOUBLE, sy));
	//				t3->set(t3, 0, ia_double_tovalue(IAT_DOUBLE, sx));
	//				t3->set(t3, 1, ia_double_tovalue(IAT_DOUBLE, sy+1));
	//			}
	//			t1->set(t1, 2, ia_double_tovalue(IAT_DOUBLE, (ia_double_t)threshold->get_pixel(threshold, (ia_int32_t)t1->get(t1, 0).d, (ia_int32_t)t1->get(t1, 1).d)));
	//			t2->set(t2, 2, ia_double_tovalue(IAT_DOUBLE, (ia_double_t)threshold->get_pixel(threshold, (ia_int32_t)t2->get(t2, 0).d, (ia_int32_t)t1->get(t2, 1).d)));
	//			t3->set(t3, 2, ia_double_tovalue(IAT_DOUBLE, (ia_double_t)threshold->get_pixel(threshold, (ia_int32_t)t3->get(t3, 0).d, (ia_int32_t)t1->get(t3, 1).d)));

	//			o1->set(o1, 0, t1->get(t1, 0));
	//			o1->set(o1, 1, t1->get(t1, 0));
	//			o1->set(o1, 2, ia_value_0);

	//			t1->sub(t1, o1);
	//			t2->sub(t2, o1);
	//			t3->sub(t3, o1);

	//			m->set(m, 0, j);
	//			m->set(m, 1, i);
	//			m->set(m, 2, 0);

	//			m->sub(m, o1);
	//		
	//		}
	//		else
	//		{
	//			/* calculate threshold values around the image border */
	//		}
	//	}
}
