#include "stdio.h"
#include "stdlib.h"
#include "math.h"

#define ADDataCnt 10

double ADData[ADDataCnt];

void InitADData(double RealValue, unsigned int Cnt)
{
        unsigned int i;
        for( i =0; i < Cnt; i++)
        {
               // ADData = RealValue + 0.2*rand()/(double)RAND_MAX - 0.1;
        }        
}

double frand() 
{      
  return 2*((rand()/(double)RAND_MAX) - 0.5);  //随机噪声
}  

double KalmanFilter(unsigned int DataCnt,double *Data,double ProcessNiose_Q,double MeasureNoise_R,double InitialPrediction)
{
        unsigned int i;
        double R = MeasureNoise_R;
        double Q = ProcessNiose_Q;
        double x_last = *Data;
        double x_mid = x_last;
        double x_now;
        double p_last = InitialPrediction;
        double p_mid ;
        double p_now;
        double kg;

        for(i=0;i<DataCnt;i++)
        {
                x_mid=x_last; //x_last=x(k-1|k-1),x_mid=x(k|k-1)
                p_mid=p_last+Q; //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=噪声
                kg=p_mid/(p_mid+R); //kg为kalman filter，R为噪声
//                z_measure=z_real+frand()*0.03;//测量值
                x_now=x_mid+kg*(*(Data+i)-x_mid);//估计出的最优值
                p_now=(1-kg)*p_mid;//最优值对应的covariance

                p_last = p_now; //更新covariance值
                x_last = x_now; //更新系统状态值
        }
        printf("data: xnow \n");
        return x_now;

}



int main(int argc, char** argv)
{
  FILE *fp;
  fp = fopen("mag.txt", "r");
  char buf[100];

  if(NULL == fp)
    printf("open file error \n");
  
  unsigned int i = 0;
  double KalmanFilterReturn;
//  InitADData(2.1,ADDataCnt);
//  printf("ADData[%d] Is : %lf\n",i,ADData);

        float x_last=0;   
        float p_last=0.02;
        float Q=0.018;
        float R=1.542;
        float kg;
        float x_mid;
        float x_now;
        float p_mid;
        float p_now;
        float z_real=0.56;//0.56
        float z_measure;
        float sumerror_kalman=0;
        float sumerror_measure=0;
       // int i;
       // x_last=z_real+frand()*0.03;
        x_mid=x_last;
 //  KalmanFilterReturn = KalmanFilter(ADDataCnt,ADData,0,0.005,10);
//  printf("\n Kalman Output : %lf\n",KalmanFilterReturn);
  double mag = 0;
  while( !feof(fp))
  {
     fgets(buf, 100, fp);
 //  printf("%s \n", buf);
     sscanf(buf, "%s %s %lf\n",buf, buf, &mag);
    // printf("%f \n", mag);
  
     x_mid=x_last;    //x_last=x(k-1|k-1),x_mid=x(k|k-1)
     p_mid=p_last+Q;  //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=噪声
     kg=p_mid/(p_mid+R); //kg为kalman filter，R为噪声
   //  z_measure=z_real+frand()*0.03;//测量值
     z_measure = mag;
     x_now=x_mid+kg*(z_measure-x_mid);//估计出的最优值
     p_now=(1-kg)*p_mid;//最优值对应的covariance

   //  printf("Real     position: %6.3f \n",z_real);  //显示真值
   //  printf("Mesaured position: %6.3f [diff:%.3f]\n",z_measure,fabs(z_real-z_measure));  //显示测量值以及真值与测量值之间的误差
    // printf("Kalman   position: %6.3f [diff:%.3f]\n",x_now,fabs(z_real - x_now));  //显示kalman估计值以及真值和卡尔曼估计值的误差
     sumerror_kalman += fabs(z_real - x_now);  //kalman估计值的累积误差
     sumerror_measure += fabs(z_real-z_measure);  //真值与测量值的累积误差
     p_last = p_now;  //更新covariance值
     x_last = x_now;  //更新系统状态值
     printf("%f,%f\n", x_now, z_measure);

  }

  return 0;
}
