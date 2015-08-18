//This code simulates the original Viscek Model as is in the 1995 PRL - no repulsion, no turning angle, scalar noise and angle based implementation
// Angles are chosen over [-PI,PI] and are maintained so throughout.
// The size of spheres in the animations are not to scale.
#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<unistd.h>
using namespace std;

#define PI 3.14159


const int N=200;


float L,r_s=1,v=.03,delta_t=1,eta;


float simulate(float x[2][N],float y[2][N],float theta[2][N],long int T,FILE *fp);
void initialize(float x[2][N],float y[2][N],float theta[2][N]);
void update_pos(float x[2][N],float y[2][N],float theta[2][N]);
void update_vel(float x[2][N],float y[2][N],float theta[2][N]);
float distance(float x1,float x2,float y1,float y2);
void swap(float x[2][N],float y[2][N],float theta[2][N]);
float Orderparameter(float theta[2][N]);
float *correlation(float x[2][N],float y[2][N],float theta[2][N],float l,float delta_l);


void timeseriesplot();
void plot(FILE *fp);
void write_to_file(float x[2][N],float y[2][N]);



int main()
{
     float x[2][N],y[2][N],theta[2][N];
     int n=10;
     float v_a[n],mean,stdev;
     float density;
     char name[50];
     
     
     
     for(density = .05;density<=5;density+=0.5)
     {
	  L = sqrt(N/density);
	  
	  for(eta =.5;eta<=5;eta+=.5)
	  {
	       for(long int T = 15000; T<=15000; T+= 5000)
	       {
		    for(int i=0;i<1;i++)
		    {
			 FILE *timedat;
			 sprintf(name,"time_%.1f_%.1f_%ld_%d.dat",density,eta,T,i);
			 timedat = fopen(name,"w");
			 v_a[i]=simulate(x,y,theta,T,timedat);
			 fclose(timedat);
		    }
	
	       } 
	  }
	  
	 
     }
     
     
    // timeseriesplot();
     
     
     return 0;
}


float simulate(float x[2][N],float y[2][N],float theta[2][N],long int T,FILE *fp)
{
     
     FILE *gnupipe;
     gnupipe = popen("gnuplot -persistent","w");
   
     
     initialize(x,y,theta);     
     for(long int t=0;t<T;t++)
     {
	  update_pos(x,y,theta);
	  update_vel(x,y,theta);
	  
	  fprintf(fp,"%ld\t%f\n",t,Orderparameter(theta));
	  write_to_file(x,y);
	  
	  plot(gnupipe);
	  
	  swap(x,y,theta);
     }
     
     pclose(gnupipe);
    
     
     return (Orderparameter(theta));
}


void initialize(float x[2][N],float y[2][N],float theta[2][N])
{
     srand(time(NULL));
     
     for(int i=0;i<N;i++)
     {
	  x[0][i]= ( (float)rand()/(float)RAND_MAX )*L;
	  y[0][i]= ( ((float)rand())/((float)RAND_MAX) )*L;
	  theta[0][i] = -PI + ( ((float)rand())/ ((float)RAND_MAX) )*2*PI;   
     }
}

void update_pos(float x[2][N],float y[2][N],float theta[2][N])
{
     for(int i=0;i<N;i++)
     {
	  x[1][i] = x[0][i] + v*cos(theta[0][i])*delta_t;
	  y[1][i] = y[0][i] + v*sin(theta[0][i])*delta_t;
	  
	  if(x[1][i] > L)
	  {
	       x[1][i] -= L;
	  }
	  
	  if(x[1][i] < 0)
	  {
	       x[1][i] += L;
	  }
	  
	  if(y[1][i] > L)
	  {
	       y[1][i]-= L;
	  }
	  
	  if(y[1][i] < 0)
	  {
	       y[1][i] += L;
	  }
	  
     }  
}
void update_vel(float x[2][N],float y[2][N],float theta[2][N])
{
     for(int i=0; i<N;i++)
     {
	  float avgtheta =0;
	  float sumtheta = 0;
	  int count = 0;
	  float noisetheta;
	  
	  for(int j=0;j<N;j++)
	  {
	       float dx,dy;
	       dx = fabs(x[0][i]-x[0][j]);
	       dy = fabs(y[0][i]- y[0][j]);
	       if(dx > L/2) dx = L-dx;
	       if(dy > L/2) dy = L-dy;
	       
	       float d = sqrt( dx*dx +dy*dy );
	       
	       if(d < r_s)
	       {
		    sumtheta +=theta[0][j];
		    count++;
	       }
	  }
	  
	  avgtheta = sumtheta/count;
	  srand(time(NULL));
	  noisetheta = -eta/2 + ( ((float)rand())/ ((float)RAND_MAX) )*eta;
	  
	  theta[1][i] = avgtheta + noisetheta;  
	  
	  if(theta[1][i]>PI)
	  {
	       theta[1][i] = theta[1][i]-2*PI;
	  }
	  else if(theta[1][i]< -PI)
	  {
	       theta[1][i] = theta[1][i] + 2*PI;  
	  }
     }
}

void swap(float x[2][N],float y[2][N],float theta[2][N])
{
     for(int i=0;i<N;i++)
     {
	  x[0][i] = x[1][i];
	  y[0][i] = y[1][i];
	  theta[0][i] = theta[1][i]; 
     }
}

float Orderparameter(float theta[2][N])
{
     float sumcos,sumsin;
     sumcos =0;
     sumsin =0;
     
     for(int i=0;i<N;i++)
     {
	  sumsin += sin(theta[1][i]);
	  sumcos += cos(theta[1][i]);
     }
     
     return((sqrt(sumsin*sumsin + sumcos*sumcos))/N);
}

float distance(float x1,float x2,float y1,float y2)
{
     float dx,dy;
     dx = fabs(x1-x2);
     dy = fabs(y1-y2);
     
     if(dx > L/2)
	  dx = L-dx;
     if( dy > L/2)
	  dy = L-dy;;
     
     return( sqrt(dx*dx + dy*dy) );
}

void write_to_file(float x[2][N],float y[2][N])
{
     FILE *fp;
     
     fp = fopen("data1.temp","w");
     
     for(int i=0;i<N;i++)
     {
	  fprintf(fp,"%f\t%f\t",x[1][i],y[1][i]);
     }
     
     fprintf(fp,"\n");
     
     fclose(fp);
}

void plot(FILE *fp)
{
     int i=1;
     double l=1;
     float x;
     x= 8.9*10/L;
     
     fprintf(fp,"set terminal wxt size 600,600\n");
     fprintf(fp,"set size square 1,1\n");
     fprintf(fp,"set key off\n");
     fprintf(fp,"unset xtic \n");
     fprintf(fp,"unset ytic \n");
     fprintf(fp,"set grid xtic\n");
     fprintf(fp,"set grid ytic\n");
     fprintf(fp,"set xrange [0:%lf]\n",L);
     fprintf(fp,"set yrange [0:%lf]\n",L);
     
     fprintf(fp,"plot \'data1.temp\' u %d:%d w points pointtype 6 ps %.1f,",1,2,x);
     for(i=3;i<(2*N-1);i+=2)
     {
	  fprintf(fp,"\'data1.temp\' u %d:%d w points pointtype 6 ps %.1f,",i,i+1,x);  
     }
     
     
     fprintf(fp,"\'data1.temp\' u %d:%d w points pointtype 6 ps %.1f\n",i,i+1,x);
     
     
     usleep(10000);
     
     //pclose(gnuplotPipe);
     
}

float *correlation(float x[2][N],float y[2][N],float theta[2][N],float l = 0,float delta_l = L/100)
{
     float* p;
     p = (float *)malloc( sizeof(float)*(int)( (L-l)/delta_l ) );
     int k = 0;
     while(l<=L && k<= sizeof(float)*(int)( (L-l)/delta_l ))
     {
	  int count = 0;
	  float sum = 0;
	  for(int i=0;i<N;i++)
	  {
	       for(int j=0;j<N;j++)
	       {
		    float d = distance(x[1][i],x[1][j],y[1][i],y[1][j]);
		    if(d > l && d < (l+delta_l) )
		    {
			count++;
			sum += cos(theta[1][i])*cos(theta[1][j]) + sin(theta[1][i])*sin(theta[1][j]);
		    }
	       }
	  }
	  
	  p[k] = sum/count;
	  k++;
	  l += delta_l;
     }
     
     return p;
	  
}

void timeseriesplot()
{
     char name[50],output[50];
      FILE *fp;
      fp = popen("gnuplot","w");
     
     
     
     for(float density = 1;density<=5;density+=0.5)
     {  
	  for(float eta =0;eta<=5;eta+=.5)
	  {
	       for(long int T = 5000; T<=15000; T+= 5000)
	       {
		    
		    FILE *fp;
		    fp = popen("gnuplot","w");
		    
			 
			 
			 
			 fprintf(fp,"set terminal png\n");
			 fprintf(fp,"set xrange [0:%ld]\n",T);
			 fprintf(fp,"set yrange [0:1.2]\n");
			 fprintf(fp,"set xlabel 'T'\n");
			 fprintf(fp,"set ylabel 'Order Parameter'\n");
			 fprintf(fp,"set title \'D_%.1f,eta_%.1f,T_%ld\'",density,eta,T);
			 sprintf(output,"time_%.1f_%.1f_%ld.png",density,eta,T);
			 fprintf(fp,"set output \"%s\"",output);
			 sprintf(name,"time_%.1f_%.1f_%ld_0.dat",density,eta,T);
			 
			 fprintf(fp,"plot \'%s\' u 1:2,",name);
			 for(int i=1;i<9;i++)
			 {
			      sprintf(name,"time_%.1f_%.1f_%ld_%d.dat",density,eta,T,i);
			      fprintf(fp,"\'%s\' u 1:2,",name);
			 
			 }
			 sprintf(name,"time_%.1f_%.1f_%ld_9.dat",density,eta,T);
			 fprintf(fp,"\'%s\' u 1:2 \n",name);
			 
			 fclose(fp);
		    
	       } 
	  }
	  
	  
     }



}
     