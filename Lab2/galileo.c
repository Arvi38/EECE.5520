#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#define MSG_RESET 0x0
#define MSG_PING 0x1
#define MSG_GET 0x2
#define MSG_TURN30 0x3
#define MSG_TURN90 0x04
#define MSG_TURN120 0x5

void Export()
{
	//export the pin 8 GPIO 40
		system("echo 40 > /sys/class/gpio/export");
		//export the pin 7 GPIO 38
        system("echo 38 > /sys/class/gpio/export");
		//export pin 6 GPIO 1 and SHIFTER GPIO 20
		system("echo 1 > /sys/class/gpio/export");
		system("echo 20 > /sys/class/gpio/export");
		//export pin 5 GPIO 0 and SHIFTER GPIO 18
		system("echo 0 > /sys/class/gpio/export");
		system("echo 18 > /sys/class/gpio/export");
		//export pin 4 GPIO 6 and SHIFTER GPIO 36
		system("echo 6 > /sys/class/gpio/export");
		system("echo 36 > /sys/class/gpio/export");
}
void UnExport()
{
	//export the pin 8 GPIO 40
		system("echo 40 > /sys/class/gpio/unexport");
		//export the pin 7 GPIO 38
        system("echo 38 > /sys/class/gpio/unexport");
		//export pin 6 GPIO 1 and SHIFTER GPIO 20
		system("echo 1 > /sys/class/gpio/unexport");
		system("echo 20 > /sys/class/gpio/unexport");
		//export pin 5 GPIO 0 and SHIFTER GPIO 18
		system("echo 0 > /sys/class/gpio/unexport");
		system("echo 18 > /sys/class/gpio/unexport");
		//export pin 4 GPIO 6 and SHIFTER GPIO 36
		system("echo 6 > /sys/class/gpio/unexport");
		system("echo 36 > /sys/class/gpio/unexport");
}
void SetGPIO_output()
{
     //setting pin8 as an output   
	system("echo out > /sys/class/gpio/gpio40/direction");
	//Setting pin7 as an output
	system("echo out > /sys/class/gpio/gpio38/direction");
	//setting pin6 as an output
	system("echo out > /sys/class/gpio/gpio1/direction");
	system("echo out > /sys/class/gpio/gpio20/direction");
	//setting pin5 as an output
	system("echo out > /sys/class/gpio/gpio0/direction");
	system("echo out > /sys/class/gpio/gpio18/direction");
	//setting pin4 as output
	system("echo out > /sys/class/gpio/gpio6/direction");
	system("echo out > /sys/class/gpio/gpio36/direction");         
}

void SetGPIO_Input()
{
	//Setting pin7 as an input
	system("echo in > /sys/class/gpio/gpio38/direction");
	//setting pin6 as an input
	system("echo in > /sys/class/gpio/gpio1/direction");
	system("echo in > /sys/class/gpio/gpio20/direction");
	//setting pin5 as an input
	system("echo in > /sys/class/gpio/gpio0/direction");
	system("echo in > /sys/class/gpio/gpio18/direction");
	//setting pin4 as input
	system("echo in > /sys/class/gpio/gpio6/direction");
	system("echo in > /sys/class/gpio/gpio36/direction");  
}

int main()
{
	int msg;
	printf("select the number of the command: \n1.MSG-RESET \n2.MSG-MSG-PING \n3.MSG-GET \n4.MSG-TURN30 \n5.MSG-TURN90 \n6.MSGTURN120\n");
    scanf("%d",&msg);
   switch(msg)
   {
	   case 1:
	   Export();
	   SetGPIO_output();
	    system("echo 0 > /sys/class/gpio/gpio40/value");
		system("echo 0 > /sys/class/gpio/gpio6/value");
		system("echo 0 > /sys/class/gpio/gpio0/value");
		system("echo 0 > /sys/class/gpio/gpio1/value");
		system("echo 0 > /sys/class/gpio/gpio38/value");
		system("echo 1 > /sys/class/gpio/gpio40/value");
        usleep(10000);		
		system("echo 0 > /sys/class/gpio/gpio40/value");
	    UnExport();
		Export();
	    SetGPIO_Input();
		system("echo 0 > /sys/class/gpio/gpio40/value");
		system("echo 1 > /sys/class/gpio/gpio40/value");
		system("cat /sys/class/gpio/gpio6/value");	
		system("cat /sys/class/gpio/gpio0/value");
		system("cat /sys/class/gpio/gpio1/value");	
		system("cat /sys/class/gpio/gpio38/value");	
		usleep(10000);
		system("echo 0 > /sys/class/gpio/gpio40/value");
	    UnExport();
		break;
		case 2:
		Export();
	    SetGPIO_output();
		system("echo 0 > /sys/class/gpio/gpio40/value");
		system("echo 1 > /sys/class/gpio/gpio6/value");
		system("echo 0 > /sys/class/gpio/gpio0/value");
		system("echo 0 > /sys/class/gpio/gpio1/value");
		system("echo 0 > /sys/class/gpio/gpio38/value");
	    system("echo 1 > /sys/class/gpio/gpio40/value");
		usleep(10000);
		system("echo 0 > /sys/class/gpio/gpio40/value");
		UnExport();
		Export();
	    SetGPIO_Input();
		system("echo 0 > /sys/class/gpio/gpio40/value");
		//usleep(10000);
		system("echo 1 > /sys/class/gpio/gpio40/value");
		system("cat /sys/class/gpio/gpio6/value");	
		system("cat /sys/class/gpio/gpio0/value");
		system("cat /sys/class/gpio/gpio1/value");	
		system("cat /sys/class/gpio/gpio38/value");	
		usleep(10000);
		system("echo 0 > /sys/class/gpio/gpio40/value");
	    UnExport();
		break;
		case 3:
		Export();
	    SetGPIO_output();
		system("echo 0 > /sys/class/gpio/gpio40/value");
		system("echo 0 > /sys/class/gpio/gpio6/value");
		system("echo 1 > /sys/class/gpio/gpio0/value");
		system("echo 0 > /sys/class/gpio/gpio1/value");
		system("echo 0 > /sys/class/gpio/gpio38/value");
		system("echo 1 > /sys/class/gpio/gpio40/value");
		usleep(10000);
		system("echo 0 > /sys/class/gpio/gpio40/value");
		UnExport();
		Export();
	    SetGPIO_Input();
		system("echo 0 > /sys/class/gpio/gpio40/value");
		system("echo 1 > /sys/class/gpio/gpio40/value");
		system("cat /sys/class/gpio/gpio6/value");	
		system("cat /sys/class/gpio/gpio0/value");
		system("cat /sys/class/gpio/gpio1/value");	
		system("cat /sys/class/gpio/gpio38/value");	
		usleep(10000);
		system("echo 0 > /sys/class/gpio/gpio40/value");
		system("echo 1 > /sys/class/gpio/gpio40/value");
		system("cat /sys/class/gpio/gpio6/value");	
		system("cat /sys/class/gpio/gpio0/value");
		system("cat /sys/class/gpio/gpio1/value");	
		system("cat /sys/class/gpio/gpio38/value");	
		usleep(10000);
		system("echo 0 > /sys/class/gpio/gpio40/value");
		system("echo 1 > /sys/class/gpio/gpio40/value");
		system("cat /sys/class/gpio/gpio6/value");	
		system("cat /sys/class/gpio/gpio0/value");
		system("cat /sys/class/gpio/gpio1/value");	
		system("cat /sys/class/gpio/gpio38/value");	
		usleep(10000);
		system("echo 0 > /sys/class/gpio/gpio40/value");
		system("echo 1 > /sys/class/gpio/gpio40/value");
		system("cat /sys/class/gpio/gpio6/value");	
		system("cat /sys/class/gpio/gpio0/value");
		system("cat /sys/class/gpio/gpio1/value");	
		system("cat /sys/class/gpio/gpio38/value");	
		usleep(10000);
		system("echo 0 > /sys/class/gpio/gpio40/value");
		UnExport(); 
		break; 
		case 4:
				Export();
	    SetGPIO_output();
		system("echo 0 > /sys/class/gpio/gpio40/value");
		system("echo 1 > /sys/class/gpio/gpio6/value");
		system("echo 1 > /sys/class/gpio/gpio0/value");
		system("echo 0 > /sys/class/gpio/gpio1/value");
		system("echo 0 > /sys/class/gpio/gpio38/value");
		system("echo 1 > /sys/class/gpio/gpio40/value");
		usleep(10000);
		system("echo 0 > /sys/class/gpio/gpio40/value");
		UnExport();
		Export();
	    SetGPIO_Input();
	 system("echo 0 > /sys/class/gpio/gpio40/value");
		usleep(10000);
		system("echo 1 > /sys/class/gpio/gpio40/value");
		system("cat /sys/class/gpio/gpio6/value");	
		system("cat /sys/class/gpio/gpio0/value");
		system("cat /sys/class/gpio/gpio1/value");	
		system("cat /sys/class/gpio/gpio38/value");	
		system("echo 0 > /sys/class/gpio/gpio40/value");
	    UnExport();
	    UnExport();
		break;
		case 5:
				Export();
	    SetGPIO_output();
		system("echo 0 > /sys/class/gpio/gpio40/value");
		system("echo 0 > /sys/class/gpio/gpio6/value");
		system("echo 0 > /sys/class/gpio/gpio0/value");
		system("echo 1 > /sys/class/gpio/gpio1/value");
		system("echo 0 > /sys/class/gpio/gpio38/value");
		system("echo 1 > /sys/class/gpio/gpio40/value");
		usleep(10000);
		system("echo 0 > /sys/class/gpio/gpio40/value");
		UnExport();
		Export();
	    SetGPIO_Input();
	  system("echo 0 > /sys/class/gpio/gpio40/value");
		usleep(10000);
		system("echo 1 > /sys/class/gpio/gpio40/value");
		system("cat /sys/class/gpio/gpio6/value");	
		system("cat /sys/class/gpio/gpio0/value");
		system("cat /sys/class/gpio/gpio1/value");	
		system("cat /sys/class/gpio/gpio38/value");	
		system("echo 0 > /sys/class/gpio/gpio40/value");
	    UnExport();
	    UnExport(); 
		break;
		case 6:
				Export();
	    SetGPIO_output();
		system("echo 0 > /sys/class/gpio/gpio40/value");
		system("echo 1 > /sys/class/gpio/gpio6/value");
		system("echo 0 > /sys/class/gpio/gpio0/value");
		system("echo 1 > /sys/class/gpio/gpio1/value");
		system("echo 0 > /sys/class/gpio/gpio38/value");
	    system("echo 1 > /sys/class/gpio/gpio40/value");
		usleep(10000);
		system("echo 0 > /sys/class/gpio/gpio40/value");
		UnExport();
		Export();
	    SetGPIO_Input();
	 system("echo 0 > /sys/class/gpio/gpio40/value");
		usleep(10000);
		system("echo 1 > /sys/class/gpio/gpio40/value");
		system("cat /sys/class/gpio/gpio6/value");	
		system("cat /sys/class/gpio/gpio0/value");
		system("cat /sys/class/gpio/gpio1/value");	
		system("cat /sys/class/gpio/gpio38/value");	
		system("echo 0 > /sys/class/gpio/gpio40/value");
	    UnExport();
		break;
   }
}

