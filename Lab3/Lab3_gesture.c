
#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

unsigned char read_gesture();
bool gesture_enable();
bool APDS9960_write(char address,char command);

    useconds_t delay = 2000;
  char *dev = "/dev/i2c-0";
	const int  fd = open(dev, O_RDWR );
	
int main()
{
	    int i;
        int r;
     //   int fd;
        //unsigned char command[1];
       // unsigned int value[4] = {0,0,0,0};    
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

        close(fd);
        return(0);
}

unsigned char read_gesture()
{
	unsigned char GF4_V,STATUS_V,GFLVL_V,GSTATUS_V,GUP_V,GDOWN_V,GLEFT_V,GRIGHT_V;
	unsigned char GF4 = 0xAB;
	unsigned char STATUS = 0x93;
	unsigned char GFLVL = 0xAE;
	unsigned char GSTATUS = 0xAF;
	unsigned char GUP = 0xFC;
	unsigned char GDOWN = 0xFD;
	unsigned char  GLEFT = 0xFE;
	unsigned char GRIGHT = 0xFF;
	
	while(1)
	{
		
		//sleep(2);
		
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
	 
	 if(((GSTATUS_V & 0x01) ==1) && ((STATUS_V & 0x02) == 2 ))
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
		
		 		 for(int i=1;i<=GFLVL_V;i++)
		 {
	 
	 sleep(1);
	  write(fd,&GUP,1);
	usleep(delay);
     read(fd,&GUP_V,1);
	 
	 printf("GUP: %d\n",GUP_V);
	 
	  write(fd,&GDOWN,1);
	usleep(delay);
     read(fd,&GDOWN_V,1);
	 
	  printf("GDOWN: %d\n",GDOWN_V);
	 
	 write(fd,&GLEFT,1);
	usleep(delay);
     read(fd,&GLEFT_V,1);
	   printf("GLEFT: %d\n",GLEFT_V);
	 
	  write(fd,&GRIGHT,1);
	usleep(delay);
     read(fd,&GRIGHT_V,1);
	   printf("GRIGHT: %d\n",GRIGHT_V);
		 }
		 if(!APDS9960_write(0xAB,0x00))
	{
		return false;
	}
		 break;
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
     read(fd,&GUP_V,1);
	 
	  write(fd,&GDOWN,1);
	usleep(delay);
     read(fd,&GDOWN_V,1);
	 
	 write(fd,&GLEFT,1);
	usleep(delay);
     read(fd,&GLEFT_V,1);
	 
	  write(fd,&GRIGHT,1);
	usleep(delay);
     read(fd,&GRIGHT_V,1);
		 }
		 		 if(!APDS9960_write(0xAB,0x00))
	{
		return false;
	}
		 
	 } 

      //return 1;		 
	}
	
}



bool gesture_enable()
{
	   /* Enable gesture mode
       Set ENABLE to 0 (power off)
       Set WTIME to 0xFF
       Set AUX to LED_BOOST_300
       Enable PON, WEN, PEN, GEN in ENABLE 
    */
		/*if(!APDS9960_write(APDS9960_WTIME,0xFF);
	{
		return false;
	}
		if(!APDS9960_write(APDS9960_PPULSE,DEFAULT_GESTURE_PPULSE);
	{
		return false;
	}
		if(!APDS9960_write(APDS9960_PPULSE,DEFAULT_GESTURE_PPULSE);
	{
		return false;
	}*/
	//Entry Threshold
	/*if(!APDS9960_write(0xA0,0x4D))
	{
		return false;
	}*/
	//Exit Threshold
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
	if(!APDS9960_write(0xA4,0x00))
	{
	return false;
	}
	if(!APDS9960_write(0xA5,0x00))
	{
	return false;
	}
	if(!APDS9960_write(0xA7,0x00))
	{
	return false;
	}
	if(!APDS9960_write(0xA9,0x00))
	{
	return false;
	}
	if(!APDS9960_write(0xA6,0x47))
	{
		return false;
	}
	if(!APDS9960_write(0xAA,0x03))
	{
		return false;
	}
	if(!APDS9960_write(0xAB,0x03))
	{
		return false;
	}
	if(!APDS9960_write(0xE7,0x00))
	{
		return false;
	}
	
		return true;

}

bool APDS9960_write(char address,char command)
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

bool APDS9960_read(char address, char command)
{
	int r = write(fd,&address,1);
	if(r<0)
	{
		printf("error wrinting to address: %d",address);
		return false;
	}
	else
	{
		usleep(delay);
		unsigned char value;
		r = read(fd,&value,1);
		if(r<0)
		{
			printf("error reading from address : %d",address);
		}
	    return true;
	}
}
	
	
	/*
	bool Gesture_Init()
{
	if(!APDS9960_write(APDS9960_GPENTH,DEFAULT_GPENTH);
	{
		return false;
	}
	if(!APDS9960_write(APDS9960_GPEXTH,DEFAULT_GPEXTH);
	{
		return false;
	}
	if(!APDS9960_write(APDS9960_GCONF1,DEFAULT_GCONF1);
	{
		return false;
	}
		if(!APDS9960_write(APDS9960_GGAIN,DEFAULT_GGAIN);
	{
		return false;
	}
		if(!APDS9960_write(APDS9960_GLDRIVE,DEFAULT_GLDRIVE);
	{
		return false;
	}
		if(!APDS9960_write(APDS9960_GWTIME,DEFAULT_GWTIME);
	{
		return false;
	}
		if(!APDS9960_write(APDS9960_GOFFSET_U,DEFAULT_GOFFSET);
	{
		return false;
	}
		if(!APDS9960_write(APDS9960_GOFFSET_D,DEFAULT_GOFFSET);
	{
		return false;
	}
		if(!APDS9960_write(APDS9960_GOFFSET_L,DEFAULT_GOFFSET);
	{
		return false;
	}
		if(!APDS9960_write(APDS9960_GOFFSET_R,DEFAULT_GOFFSET);
	{
		return false;
	}
	if(!APDS9960_write(APDS9960_GPULSE,DEFAULT_GPULSE);
	{
		return false;
	}
		if(!APDS9960_write(APDS9960_GCONF3,DEFAULT_GCONF3);
	{
		return false;
	}
		if(!APDS9960_write(APDS9960_G,DEFAULT_GOFFSET);
	{
		return false;
	}
		if(!APDS9960_write(APDS9960_GIEN,DEFAULT_GIEN);
	{
		return false;
	}
	
}*/