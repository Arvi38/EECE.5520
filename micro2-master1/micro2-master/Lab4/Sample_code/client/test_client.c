// Last Modification : Ioannis Smanis 11/13/2017
/* Since this program depends on libcurl (CURL library), you may need to 
 * find the gcc compilation flags for CURL lib here: https://ubuntuforums.org/showthread.php?t=1175115
 */



#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <sys/stat.h>



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



int main(void){

	const char* hostname="ec2-54-202-113-131.us-west-2.compute.amazonaws.com"; // Server Hostname or IP address
	const int   port=8000;                  // Server Service Port Number
	const int   id=12;
	const char* password="password";
	const char* name="Zubir";
	const int   adcval=123;
	const char* status="About to Die";
	const char* timestamp="2017-11-09_09:12:34";
	const char* filename="picture0XX.jpg";  // captured picture name + incremented file number 

	char buf[1024];
     
    

     //............
     // use sprintf() call here to fill out the data "buf":
	 // use the provided URL Protocol in the lab description: replace the "server_hostname", "portnumber", "var_xxxx" with the related format specifiers "%d" or "%s" 
     //...................
	




	// ========== Don't bother the lines below
	FILE *fp;
	struct stat num;
	stat(filename, &num);
	int size = 0; num.st_size;
	char *buffer = (char*)malloc(size);
    // =========== Don't bother the above lines

	HTTP_POST(buf, buffer, size);
	fclose(fp);
	return 0;
}
