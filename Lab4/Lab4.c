#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <sys/stat.h>
#include <time.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#define ACK 0xF

bool APDS9960_write(unsigned char address,unsigned char command)
	{
		unsigned char command1[2] = {address,command};
		int r = write(fd,&command1,2);
		if(r<0)
		{
			printf("error wrinting to address: %d",address);
			return false;
		}
		else
		return true;
	}

void Imagecapture()
	{
	VideoCapture cap(0); // open the video camera no. 0

		if (!cap.isOpened())  // if not success, exit program
		{
			cout << "ERROR: Cannot open the video file" << endl;
			
		}
	   double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
	   double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video
	   cout << "Frame Size = " << dWidth << "x" << dHeight << endl;
	   vector<int> compression_params; //vector that stores the compression parameters of the image
	   compression_params.push_back(CV_IMWRITE_JPEG_QUALITY); //specify the compression technique
	   compression_params.push_back(95); //specify the jpeg quality
	   Mat img(dWidth, dHeight, CV_8UC1);
	   cap.read(img);
	   bool bSuccess = imwrite("/media/card/img.jpg", img, compression_params); //write the image to file
	   if ( !bSuccess )
	   {
			cout << "ERROR : Failed to save the image" << endl;
	   }
	}
/*-------------------------------------------------------------------------
Read_gesture() 
reads the gesture value from the APDS9960 and sense the gesture value and returns
the gesture value.
---------------------------------------------------------------------------*/
unsigned char read_gesture()
	{
		unsigned char GF4 = 0xAB;
		unsigned char  STATUS = 0x93;
		unsigned char GFLVL = 0xAE;
		unsigned char GSTATUS = 0xAF;
	    unsigned char GUP = 0xFC;
		unsigned char GDOWN = 0xFD;
		unsigned char  GLEFT = 0xFE;
	    unsigned char GRIGHT = 0xFF;
		unsigned char GF4_V,STATUS_V,GFLVL_V,GSTATUS_V;
		unsigned char GUP_V[32] , GDOWN_V[32], GLEFT_V[32] ,GRIGHT_V[32] ;
		unsigned char valid_up[1],valid_down[1],valid_left[1],valid_right[1];
		while(1)
		{
			write(fd,&GF4,1);
			usleep(delay);
			read(fd,&GF4_V,1);
			printf("Status : %d\n",GF4_V);
			write(fd,&STATUS,1);
			usleep(delay);
			read(fd,&STATUS_V,1);
			printf("Status : %d\n",STATUS_V);
		    write(fd,&GSTATUS,1);
		    usleep(delay);
		    read(fd,&GSTATUS_V,1);
		    printf("GSTATUS: %d\n",GSTATUS_V);
			unsigned char x = GSTATUS_V & 0x01;
			printf("x = %d",x);
			unsigned char y = STATUS_V & 0x02;
			printf("y = %d",y);
			if(((GSTATUS_V & 0x01) ==1) && ((STATUS_V & 0x04) == 4 ))
			{
				if(!APDS9960_write(0xAB,0x03))
				{
					return false;
				}
				sleep(1);
				printf("valid\n");
				write(fd,&GFLVL,1);
				usleep(delay);
				read(fd,&GFLVL_V,1);
				printf("GFLVL: %d\n",GFLVL_V);			
				for(int i=0;i<=GFLVL_V-1;i++) // for reading the 32 datasets
				{		 
					sleep(0.7);
					write(fd,&GUP,1);
					usleep(delay);
					read(fd,&GUP_V[i],1);		 
					printf("GUP: %d\n",GUP_V[i]);
					write(fd,&GDOWN,1);
					usleep(delay);
					read(fd,&GDOWN_V[i],1);
					printf("GDOWN: %d\n",GDOWN_V[i]);
					write(fd,&GLEFT,1);
					usleep(delay);
					read(fd,&GLEFT_V[i],1);
					printf("GLEFT: %d\n",GLEFT_V[i]);
					write(fd,&GRIGHT,1);
					usleep(delay);
					read(fd,&GRIGHT_V[i],1);
					printf("GRIGHT: %d\n",GRIGHT_V[i]);
				}
				if(!APDS9960_write(0xAB,0x00))
				{
					return false;
				}
				valid_up[1]={0};
				valid_down[1] = {0};
				valid_left[1] = {0};
				valid_right[1] = {0};
				for(int j=0;j<GFLVL_V-1;j++)
				{
					if(GUP_V[j] >50){valid_up[0] = GUP_V[j];}
					if(GDOWN_V[j] >50){valid_down[0] = GDOWN_V[j];}
					if(GLEFT_V[j] >50){valid_left[0] = GLEFT_V[j];}
					if(GRIGHT_V[j] >50){valid_right[0] = GRIGHT_V[j];}			
				}
				if((valid_up[0] == valid_down[0]) && (valid_left[0] == valid_right[0]) && (valid_down[0] == valid_left[0]))
				{cout << "Give a Gesture please"<<endl;}
				if((valid_down[0] < valid_up[0]) && (valid_left[0] > valid_right[0]))
				{
					cout << "UP GESTURE DETECTED" << endl;
					return UP;         
				}
				if((valid_down[0] > valid_up[0]) && (valid_left[0] > valid_right[0]))
				{
					cout << "Down GESTURE DETECTED" << endl;
					return DOWN;         
				}
				if((valid_down[0] > valid_up[0]) && (valid_left[0] < valid_right[0]))
				{
					cout << "Left GESTURE DETECTED" << endl;
					return LEFT;         
				}
				if((valid_down[0] < valid_up[0]) && (valid_left[0] < valid_right[0]))
				{
					cout << "Right GESTURE DETECTED" << endl;
					return RIGHT;         
				}
				else 
				{
					cout << "Wrong GESTURE DETECTED Please Try again" << endl;
					break;
				}
		 }
		 else
		 {
			 printf("not valid");
			 write(fd,&GFLVL,1);
			 usleep(delay);
			 read(fd,&GFLVL_V,1);
			 printf("GFLVL: %d\n",GFLVL_V);
			 for(int i=1;i<=GFLVL_V;i++)
			 {
					write(fd,&GUP,1);
					usleep(delay);
					read(fd,&GUP_V[i],1);		 
					write(fd,&GDOWN,1);
					usleep(delay);
					read(fd,&GDOWN_V[i],1);
					write(fd,&GLEFT,1);
					usleep(delay);
					read(fd,&GLEFT_V[i],1);
					write(fd,&GRIGHT,1);
					usleep(delay);
					read(fd,&GRIGHT_V[i],1);
			 }
			if(!APDS9960_write(0xAB,0x00))
			{
				return false;
			}
		} 	 
		}
		
	}
/*------------------------------------------------
Gesture Enable function
Enables the Gesture sensor required register values
--------------------------------------------------*/	
bool gesture_enable()
	{
		if(!APDS9960_write(0xA1,0x00))
		{
			return false;
		}
		//Config1
		if(!APDS9960_write(0xA2,0x00))
		{
			return false;
		}
		//Config2
		if(!APDS9960_write(0xA3,0x41))
		{
			return false;
		}
		//Up Offste Register
		if(!APDS9960_write(0xA4,0x00))
		{
			return false;
		}
		//Down offset register
		if(!APDS9960_write(0xA5,0x00))
		{
			return false;
		}
		//Left offset register
		if(!APDS9960_write(0xA7,0x00))
		{
			return false;
		}
		//right offset register
		if(!APDS9960_write(0xA9,0x00))
		{
			return false;
		}
		//Pulse count length
		if(!APDS9960_write(0xA6,0x47))
		{
			return false;
		}
		//cofig3
		if(!APDS9960_write(0xAA,0x03))
		{
			return false;
		}
		//config 4
		if(!APDS9960_write(0xAB,0x03))
		{
			return false;
		}
		//clear interrupts
		if(!APDS9960_write(0xE7,0x00))
		{
			return false;
		}
			return true;

	}
/*----------------------------------------------------------------
Temperature()
Reads the temperature value from the sensor and returns the value.
------------------------------------------------------------------*/	
unsigned char Temperature()
	{
		int i;
		int r;
		int fd2;
		float result = 0.0;
		char value[2] ={0} ;
		char addr = 0x48;
		const char *dev = "/dev/i2c-0";
		fd2 = open(dev, O_RDWR );
		if(fd < 0)
		{
			perror("Opening i2c device node\n");
			return 1;
		}
		r = ioctl(fd2, I2C_SLAVE, addr);
		if(r < 0)
		{
			perror("Selecting i2c device\n");
		}
		for(i=0;i<2;i++)
		{
            r = read(fd2, &value[i], 1);
            if(r != 1)
            {
                perror("reading i2c device\n");
            }
            usleep(delay);
		}
		float  tlow =0;
		tlow = (float)(((value[0] << 8) | value[1]) >> 4);
		result = 0.0625*(tlow);
        printf("Temperature: %f\n",result);
        close(fd2);
		return result;
}
	

int ack()
{
	system("./gpio_in.sh 16 14"); //Gpio pin 3
	system("./gpio_in.sh 36 6"); //Gpio pin 4
	system("./gpio_in.sh 18 0"); //Gpio pin 5
	system("./gpio_in.sh 20 1"); //Gpio pin 6
	FILE *data = fopen("out.txt","rb");
	int buffer[4];
    fread(buffer,1,4,data);	
	int value = buffer[0] + buffer[1]<<1 + buffer[2]<<2 + buffer[3]<<3;
	return value;
	fclose(data);
}

int ADC()
{
	system("./gpio_in.sh 16 14"); //Gpio pin 3
	system("./gpio_in.sh 36 6"); //Gpio pin 4
	system("./gpio_in.sh 18 0"); //Gpio pin 5
	system("./gpio_in.sh 20 1"); //Gpio pin 6
	FILE *data = fopen("out.txt","rb");
	int buffer[10];
    fread(buffer,1,10,data);	
	int value = buffer[0] + buffer[1]<<1 + buffer[2]<<2 + buffer[3]<<3 +buffer[4]<<4 + buffer[5]<<5 + buffer[6]<<6 + buffer[7]<<7 + buffer[8]<<8 + buffer[9]<<9;
	return value;
	fclose(data);
}
void *Interface(void *Interfaceid)
{
  int cmd,a;
  while(1)
  {
	  printf("Give any one of the command \n 1.Reset 2.Ping 3.PIC LDR VALUE 4.TURN 30 5.TURN 90 6.TURN 120\n");
      scanf("%d",&cmd);
  switch(cmd)
  {
    case 1: //Reset cmd is 0 for pic
	       system("./gpio_out.sh 16 14 0");//Gpio pin 3
		   system("./gpio_out.sh 36 6 0"); //Gpio pin 4 
		   system("./gpio_out.sh 0 18 0"); //Gpio pin 5
		   system("./gpio_out.sh 1 20 0"); //Gpio pin 6
		   a= ack();
		   if (a!=ACK)
		   {
				printf("cannot find pic");
		   }
		   break;
	case 2: //ping cmd is 1 for pic
	       system("./gpio_out.sh 16 14 1"); //Gpio pin 3
		   system("./gpio_out.sh 36 6 0"); //Gpio pin 4 
		   system("./gpio_out.sh 0 18 0"); //Gpio pin 5
		   system("./gpio_out.sh 1 20 0"); //Gpio pin 6
		   a = ack();
		   if (a!=ACK)
		   {
			   printf("Pic is not available");
		   }
		   break;
	case 3: //ping cmd is 2 for pic ldr value
	       system("./gpio_out.sh 16 14 0"); //Gpio pin 3
		   system("./gpio_out.sh 36 6 1"); //Gpio pin 4 
		   system("./gpio_out.sh 0 18 0"); //Gpio pin 5
		   system("./gpio_out.sh 1 20 0"); //Gpio pin 6
		   a = ack();
		   if (a!=ACK)
		   {
			   printf("Pic ldr value is not available");
		   }
		   int adc = ADC();
		   printf("ADC value :%d\n",adc);
		   break;
	case 4: //ping cmd is 3 for turning motor by 30
	       system("./gpio_out.sh 16 14 1"); //Gpio pin 3
		   system("./gpio_out.sh 36 6 1"); //Gpio pin 4 
		   system("./gpio_out.sh 0 18 0"); //Gpio pin 5
		   system("./gpio_out.sh 1 20 0"); //Gpio pin 6
		   a = ack();
		   if (a!=ACK)
		   {
			   printf("motor is not available");
		   }
		   break;
	case 5: //ping cmd is 4 for turning motor by 60
	       system("./gpio_out.sh 16 14 0"); //Gpio pin 3
		   system("./gpio_out.sh 36 6 0"); //Gpio pin 4 
		   system("./gpio_out.sh 0 18 1"); //Gpio pin 5
		   system("./gpio_out.sh 1 20 0"); //Gpio pin 6
		   a = ack();
		   if (a!=ACK)
		   {
			   printf("motor is not available");
		   }
		   break;
	case 6: //ping cmd is 5 for turning motor by 120
	       system("./gpio_out.sh 16 14 1"); //Gpio pin 3
		   system("./gpio_out.sh 36 6 0"); //Gpio pin 4 
		   system("./gpio_out.sh 0 18 1"); //Gpio pin 5
		   system("./gpio_out.sh 1 20 0"); //Gpio pin 6
		   a = ack();
		   if (a!=ACK)
		   {
			   printf("motor is not available");
		   }
		   break;
    }
	//create thread 1 & 2
  }
}

void *Sensors(void *Sensorsid)
{
	while(1)
	{ 
	        unsigned char Temp_value = Temperature();
			char *dev = "/dev/i2c-0";
	        const int  fd = open(dev, O_RDWR );
            int i,r;
			int addr = 0x39;
			if(fd < 0)
			{
					perror("\nOpening i2c device node\n");
					return 1;
			}
			r = ioctl(fd, I2C_SLAVE, addr);
			if(r < 0)
			{
					perror("\nSelecting i2c device\n");
					return 1;
			}
			gesture_enable();
			r = APDS9960_write(0x80,0x4D);
			if(r<0)
			{
					perror("\ngesture engine not started\n");
			}
			printf("\ngesture engine started\n");
			usleep(delay);
			unsigned char value = read_gesture();
			if(!APDS9960_write(0xAB,0x00))
			{
				return 1;
			}
			if(!APDS9960_write(0xE7,0x00))
			{
				return false;
			}
			if(!APDS9960_write(0x80,0x00))
			{
				return 1;
			}
			if((Temp_value>23) ||(value == UP))
			{
				Imagecapture();
				cout<<"Gesture Recognised and Picture taken" << endl;
			}
			else
			{
				cout <<"Gesture Not Correct or Recognised" << endl;
			}
			close(fd);
			return(0);	  
	}
  	
}

void HTTP_POST(const char* url, const char* image, int size){
	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();
	if(curl){
		curl_easy_setopt(curl, CURLOPT_URL, url);
                curl_easy_setopt(curl, CURLOPT_POST, 1);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE,(long) size);
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, image);
		res = curl_easy_perform(curl);
		if(res != CURLE_OK)
      			fprintf(stderr, "curl_easy_perform() failed: %s\n",
              			curl_easy_strerror(res));
		curl_easy_cleanup(curl);
	}
}

char *time_stamp(){

char *timestamp = (char *)malloc(sizeof(char) * 16);
time_t ltime;
ltime=time(NULL);
struct tm *tm;
tm=localtime(&ltime);

sprintf(timestamp,"%04d%02d%02d%02d%02d%02d", tm->tm_year+1900, tm->tm_mon+1, 
    tm->tm_mday, tm->tm_hour-5, tm->tm_min, tm->tm_sec);
return timestamp;
}

void *Client(void *clientid)
{
	const char* hostname="ec2-54-202-113-131.us-west-2.compute.amazonaws.com"; // Server Hostname or IP address
	const int   port=8000;                  // Server Service Port Number
	const int   id=12;
	const char* password="password";
	const char* name="Zubir";
	const int   adcval=123;
	const char* status="HelloAll";
	const char* timestamp=time_stamp();
	const char* filename="picture01.jpg";  // captured picture name + incremented file number 

	char buf[1024];
     
    
    sprintf(buf,"http://%s:%d/update?id=%d&password=%s&name=%s&data=%d&status=%s&timestamp=%s&filename=%s",
	hostname,
	port,
	id,
	password,
	name,
	adcval,
	status,
	timestamp,
	filename);
     //............
     // use sprintf() call here to fill out the data "buf":
	 // use the provided URL Protocol in the lab description: replace the "server_hostname", "portnumber", "var_xxxx" with the related format specifiers "%d" or "%s" 
     //...................
	

	// ========== Don't bother the lines below
	FILE *fp;
	struct stat num;
	stat(filename, &num);
	int size = 100; num.st_size;
	char *buffer = (char*)malloc(size);
    // =========== Don't bother the above lines
	HTTP_POST(buf, buffer, size);
	//fclose(fp);
}




int main(void)
{
	pthread_attr_t attr;
	pthread_t thread_client,thread_Interface,thread_Sensors;
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
	pthread_create(&thread_Interface,&attr,Interface,NULL);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
	pthread_create(&thread_Sensors,&attr,Sensors,NULL);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
	pthread_create(&thread_client,&attr,Client,NULL);
	
	pthread_attr_destroy(&attr);
	pthread_join(thread_Interface,NULL);
	pthread_join(thread_Interface,NULL);
	pthread_join(thread_client,NULL);
	
	return 0;
	
}