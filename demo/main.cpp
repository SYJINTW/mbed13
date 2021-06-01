#include "mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"

Ticker servo_ticker;
PwmOut pin5(D5), pin6(D6);
BufferedSerial pc(USBTX, USBRX);

BBCar car(pin5, pin6, servo_ticker);

int main() {

   double pwm_table0[] = {-150, -120, -90, -60, -30, 0, 30, 60, 90, 120, 150};
   double speed_table0[] = {-10.445, -9.812, -9.647, -9.408, -5.900, 0.000, 5.501, 10.443, 10.603, 11.081, 10.523};
   double pwm_table1[] = {-150, -120, -90, -60, -30, 0, 30, 60, 90, 120, 150};
   double speed_table1[] = {-10.125, -9.646, -8.769, -6.059, -1.276, 0.000, 5.900, 10.843, 11.880, 11.401, 12.199};

   car.setCalibTable(11, pwm_table0, speed_table0, 11, pwm_table1, speed_table1);

   car.goStraightCalib(10);
   ThisThread::sleep_for(3s);
   car.stop();

   char buf[256], outbuf[256];
   FILE *devin = fdopen(&pc, "r");
   FILE *devout = fdopen(&pc, "w");
   while (1) {
      memset(buf, 0, 256);
      for( int i = 0; ; i++ ) {
         char recv = fgetc(devin);
         if(recv == '\n') {
            printf("\r\n");
            break;
         }
         buf[i] = fputc(recv, devout);
      }
      RPC::call(buf, outbuf);
      printf("%s\r\n", outbuf);
   }
}
