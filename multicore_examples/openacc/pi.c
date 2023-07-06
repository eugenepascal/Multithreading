#include <stdio.h>
#include <openacc.h>

#define N 20000
#define vl 1024

/* from https://stackoverflow.com/questions/38796020/gcc-openacc-compiled-program-does-not-function-properly */

int main(int argc, char** argv)
{
  double pi = 0.0f;
  long long i;
  int change = 0;

  printf("Number of NVidia devices: %d\n", acc_get_num_devices(acc_device_nvidia));
  printf("Number of Radeon devices: %d\n", acc_get_num_devices(acc_device_radeon));

  #pragma acc parallel 
  {
    change = 1;
    #pragma acc loop reduction(+:pi) private(i)
    for (i=0; i<N; i++)
    {
      double t= (double)((i+0.5)/N);
      pi +=4.0/(1.0+t*t);
    }
  }

  printf("Change: %d\n", change);
  printf("pi=%11.10f\n",pi/N);

  pi = 0.0;

  for (i=0; i<N; i++)
  {
    double t= (double)((i+0.5)/N);
    pi +=4.0/(1.0+t*t);
  }

  printf("pi=%11.10f\n",pi/N);

  return 0;
}

