
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


#include "testquickcam.h"

int open_camera(const char *devicename)
{
	
    device_fd = open(devicename, O_RDWR);
    if(device_fd <= 0)
    {
	printf("Device %s couldn't be opened\n", devicename);
	return 0;
    }
    return 1;
}

void close_camera(void)
{
    close(device_fd);
}

void get_camera_info(void)
{
    ioctl(device_fd, VIDIOCGCAP, &vidcap);
    ioctl(device_fd, VIDIOCGWIN, &vidwin);
    ioctl(device_fd, VIDIOCGPICT, &vidpic);
    
    vidwin.clips = vidclips;
    vidwin.clipcount = 0;
}

void print_camera_info(void)
{

    printf("    *** Camera Info ***\n");
    printf("Name:           %s\n", vidcap.name);
    printf("Type:           %d\n", vidcap.type);
    printf("Minimum Width:  %d\n", vidcap.minwidth);
    printf("Maximum Width:  %d\n", vidcap.maxwidth);
    printf("Minimum Height: %d\n", vidcap.minheight);
    printf("Maximum Height: %d\n", vidcap.maxheight);
    printf("X:              %d\n", vidwin.x);
    printf("Y:              %d\n", vidwin.y);
    printf("Width:          %d\n", vidwin.width);
    printf("Height:         %d\n", vidwin.height);
    printf("Depth:          %d\n", vidpic.depth);

    if(vidcap.type & VID_TYPE_MONOCHROME)
	printf("Color           false\n");
    else
	printf("Color           true\n");	
}

static void hexdump_data(const unsigned char *data, int len)
{
    const int bytes_per_line = 32;
    char tmp[128];
    int i = 0, k = 0;

    for(i = 0; len > 0; i++, len--)
    {
	if(i > 0 && ((i % bytes_per_line) == 0))
	{
    	    printf("%s\n", tmp);
            k = 0;
        }
        if ((i % bytes_per_line) == 0)
    	    k += sprintf(&tmp[k], "[%04x]: ", i);
        k += sprintf(&tmp[k], "%02x ", data[i]);
    }
    
    if (k > 0)

	printf("%s\n", tmp);
}

struct HEADER{
	char type1;                 /* Magic identifier            */
	char type2;
	char size[4];
	char reserved1[2], reserved2[2];
	char offset[4];                     /* Offset to image data, bytes */
};

struct INFOHEADER{
   char size[4];               /* Header size in bytes      */
   char width[4],height[4];                /* Width and height of image */
   char planes[2];       /* Number of colour planes   */
   char bits[2];         /* Bits per pixel            */
   char compression[4];        /* Compression type          */
   char imagesize[4];          /* Image size in bytes       */
   char xresolution[4],yresolution[4];     /* Pixels per meter          */
   char ncolours[4];           /* Number of colours         */
   char importantcolours[4];   /* Important colours         */
};
                           
void set32(char p[4],unsigned int v)
{
	p[0]=v&0xff;
	p[1]=(v>>8)&0xff;
	p[2]=(v>>16)&0xff;
	p[3]=(v>>24)&0xff;
}

void set16(char p[2],unsigned short int v)
{
	p[0]=v&0xff;
	p[1]=(v>>8)&0xff;
}
            

void read_test(int quiet)
{
	int server_sockfd, client_sockfd;	
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;
	//unsigned char *buffer = malloc(vidcap.maxwidth * vidcap.maxheight * 3);
    int len = 0;
	
	server_sockfd = socket(PF_INET, SOCK_STREAM,0);
	bzero(&server_address,sizeof(server_address));	
	server_address.sin_family= PF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(8888);
	
	bind(server_sockfd,(struct sockaddr *)&server_address, sizeof(server_address));	
		
	listen(server_sockfd,5);
	while(1)
	{
		
		int client_len;
		char command;
		
		struct video_mbuf vidbuf;
    	struct video_mmap vm;
    	int numframe = 0;
    	unsigned char *buffer;
  	
    			
		printf("webcam server waiting...\n");		
		client_len=sizeof(client_address);		
		client_sockfd = accept(server_sockfd,(struct sockaddr *)&client_address, &client_len);
		
    	ioctl(device_fd, VIDIOCGMBUF, &vidbuf);
    	buffer = mmap(NULL, vidbuf.size, PROT_READ, MAP_SHARED, device_fd, 0);

		//printf("vidbuf.size=%d\n",vidbuf.size);
		vm.format = VIDEO_PALETTE_RGB24;			
    	vm.frame  = 0;
    	vm.width  = 352;
    	//vm.width  = 176;
    	vm.height = 292;
    	//vm.height =146;
    	ioctl(device_fd, VIDIOCMCAPTURE, &vm);
		
		while(1)
		{
			//printf("read page\n");
			read(client_sockfd,&command,1);
			if(command==1)
			{		
				//len = read(device_fd, buffer, vidcap.maxwidth * vidcap.maxheight * 3);		
    			ioctl(device_fd, VIDIOCSYNC, &numframe);						
				
				write(client_sockfd, buffer, vidcap.maxwidth * vidcap.maxheight * 3);		
			}
			else
			{
				close(client_sockfd);
				break;
			}
		}
		
	}
	close(server_sockfd);
	
    
    
    

#if 0    
    if(!quiet)
    {
	printf(" *** read() test ***\n");
	printf("Read length: %d\n", len);
	printf("Raw data: \n\n");
	hexdump_data(buffer, len);
	}
#endif
#if 0    
    {
    FILE *f;
    struct HEADER hdr;
    struct INFOHEADER inf;
    unsigned int tmp;
    int i;    	
    hdr.type1='B';
    hdr.type2='M';
	tmp=(vidcap.maxwidth*vidcap.maxheight*3+sizeof(hdr)+sizeof(inf));
	set32(hdr.size,tmp);
	set16(hdr.reserved1,0);
	set16(hdr.reserved2,0);
	tmp=(sizeof(hdr)+sizeof(inf));
	set32(hdr.offset,tmp);
		
	tmp=(sizeof(inf));
	set32(inf.size,tmp);
	
   	tmp=(vidcap.maxwidth);
   	set32(inf.width,tmp);
   	
   	tmp=(vidcap.maxheight);
   	set32(inf.height,tmp);
   	
   	set16(inf.planes,1);
   	set16(inf.bits,24);
   	set32(inf.compression,0);
   	   	
   	tmp=(vidcap.maxwidth*vidcap.maxheight*3);
   	set32(inf.imagesize,tmp);
   	
   	set32(inf.xresolution,3780);
   	set32(inf.yresolution,3780);
	set32(inf.ncolours,0);
	set32(inf.importantcolours,0);   	
   	
    	
    f=fopen("test.bmp","wb");
    fwrite(&hdr,sizeof(hdr),1,f);
    fwrite(&inf,sizeof(inf),1,f);
    	
    for(i=vidcap.maxheight-1;i>=0;i--)
		fwrite(buffer+i*3*vidcap.maxwidth,vidcap.maxwidth * 3,1,f);
	
    fclose(f);    
	}
#endif //write to bmp file
}

#if 0
void mmap_test(int quiet)
{
    struct video_mbuf vidbuf;
    struct video_mmap vm;
    int numframe = 0;
    unsigned char *buffer;
    ioctl(device_fd, VIDIOCGMBUF, &vidbuf);
    buffer = mmap(NULL, vidbuf.size, PROT_READ, MAP_SHARED, device_fd, 0);

    vm.format = VIDEO_PALETTE_RGB24;
    vm.frame  = 0;
    vm.width  = 352;
    vm.height = 288;
    ioctl(device_fd, VIDIOCMCAPTURE, &vm);

    ioctl(device_fd, VIDIOCSYNC, &numframe);

    if(!quiet)
    {
	printf(" *** mmap() test ***\n");
	printf("Read length: %d\n", vidbuf.size);
	printf("Raw data: \n\n");
	hexdump_data(buffer, vidbuf.size);
    }
}
#endif

int main(int argc, char *argv[])
{
	FILE *f;
	f=fopen("/var/run/quickcam.pid","w");
	fprintf(f,"%d",getpid());
	fclose(f);

  	if(open_camera("/dev/video0") == 1)
	{
		get_camera_info();
		//print_camera_info();
		read_test(0);
		//mmap_test(0);
		close_camera();
	}
	
	unlink("/var/run/quickcam.pid");
    
    exit(1);
}

