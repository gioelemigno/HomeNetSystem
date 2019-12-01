#include "USART_Linux.h"

#ifdef __linux__

#include <stdlib.h>

#ifdef _PC_
#define MINI_UART "/dev/ttyUSB0"
#endif

#ifdef _RPiZW_
#define MINI_UART "/dev/ttyS0"
#include "MAX485.h"
#endif

#define MODE O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK
#define SIZE_BUF 1024

#define DELAY_PRE_TX      0//5
#define DELAY_POST_TX     0 //1000*1E-03 //0

typedef struct receiver_t{
		uint8_t buffer[SIZE_BUF];		//buffer
		uint16_t index;					//index buffer reader
		uint16_t size;					//written byte in buffer
}receiver_t;

receiver_t receiver;
int fd = -1;
struct termios options;

////////////////////////////////////////////////////////////////////////////////

static void USART_atexit(void){

	int resC = close(fd);
	if(resC!=-1) printf("Close USART\n");
	else printf("Error during close USART\n");

#ifdef _RPiZW_
	enableRX();
	printf("Put low enable pin of RS485 (enableRX())\n");
#endif
}

void check(int fd, const char* msg);
static void printHexBuffer(uint8_t* buf, int size);
static double getTime_us();

res_t USART_read_byte(uint8_t* data, double* timer){
	static uint8_t num_zero=0;  //avoid buffer full of zero
  bool serial_available = false;

  if(receiver.index == receiver.size || receiver.size == 0 ){ //buffer vuoto
    int resI = ioctl(fd, FIONREAD, &serial_available);
    check(resI, "SerialAvailable");

    if(!serial_available){
      struct timespec ts;
      clock_gettime(CLOCK_MONOTONIC, &ts);
      double timeStart = ts.tv_sec*1E06+ts.tv_nsec*1E-03;
      double elapsed = 0;

      while(!serial_available){ //wait for data
        resI = ioctl(fd, FIONREAD, &serial_available);
        check(resI, "SerialAvailable");

        clock_gettime(CLOCK_MONOTONIC, &ts);
        elapsed = (ts.tv_sec*1E06+ts.tv_nsec*1E-03) - timeStart;

        if( elapsed > *timer ){
        //  #if defined(PRINT) && defined(DEBUG_USART_READ)
            printf("passati %lf ms\n", elapsed*1E-03);
        //  #endif
          *timer=0;
          error=ERR_TIME;
          return -1;
        }
      }

      *timer -= elapsed;
    }

		ssize_t resR = read(fd, receiver.buffer, SIZE_BUF);
    check(resR, "USART_read_byte");

		#if defined(PRINT) && defined(DEBUG_USART_READ)
  		printf("\t\t\tDEBUG USART_read_byte(): ");
  		printf("\t\tresR: %d\t-> ", resR);
  		printHexBuffer(receiver.buffer, resR);
		#endif

		receiver.size = resR;
		receiver.index = 0;
	}

	uint8_t tmp = receiver.buffer[receiver.index];
  receiver.index++;
	if(tmp==0) num_zero++;
	else	num_zero=0;

	#if defined(PRINT) && defined(DEBUG_USART_READ)
		printf("USART_read_byte() read: %x\n", tmp);
	#endif

	*data=tmp;

  return 0;
}

res_t USART_send_buffer(uint8_t* buffer, uint8_t size_buffer){

 	usleep(DELAY_PRE_TX*1E03);

	int resTF = tcflush (fd, TCIOFLUSH); //Flush the serial buffers (both tx & rx)
	check(resTF, "tcflush()");

	#ifdef _RPiZW_
	enableTX();
	#endif

	//							double time_start_TX = getTime_us();
//	int count = 0;

	//ioctl(fd, TIOCOUTQ, &count);
	//printf("Previous write there are %d bytes in tx buffer\n", count);

  int resW = write(fd, buffer, size_buffer);
  check(resW, "USART_send_buffer");

	//ioctl(fd, TIOCOUTQ, &count);
	//printf("After write there are %d bytes in tx buffer\n", count);


	int resD = tcdrain(fd); // wait for trasmit end
	check(resD, "tcdrain");

//	ioctl(fd, TIOCOUTQ, &count);
//	printf("After tcdrain there are %d bytes in tx buffer\n", count);

		//						double elapsed = getTime_us() - time_start_TX;
/*
	int bytes_in_tx_buffer;
	int res = ioctl(fd, TIOCOUTQ, &bytes_in_tx_buffer);
	check(res, "ioctl count bytes in tx buffer");

	while(bytes_in_tx_buffer != 0){
		res = ioctl(fd, TIOCOUTQ, &bytes_in_tx_buffer);
		check(res, "ioctl count bytes in tx buffer");
	}
*/
	usleep(DELAY_POST_TX*1E03);
	#ifdef _RPiZW_
	//	double time_start_disTX = getTime_us();
	enableRX();
	//	double elapsed_enableRX = getTime_us() - time_start_disTX;
	//		printf("DISABLE TX REQUIRED %.2lfms\n", elapsed_enableRX*1E-03);
	#endif

	#if defined(PRINT) && defined(DEBUG_USART_WRITE)
		printf("\t\t\tSend ->\t");
		printHexBuffer(buffer, size_buffer);
		printf("\n");
	#endif

		//				printf("USART TX REQUIRED %.2lfms\n", elapsed*1E-03);
  return 0;
}
res_t USART_init(unsigned long baud_rate){

	#ifndef _PC_
	MAX485_init();
	enableRX();
	#endif

  receiver.size=0;
  receiver.index=0;

	speed_t myBaud ;
	int res;
  int status;
  fd = open(MINI_UART, MODE);
  check(fd, "Open");


	switch(baud_rate){
    case      50:	myBaud =      B50 ; break ;
    case      75:	myBaud =      B75 ; break ;
    case     110:	myBaud =     B110 ; break ;
    case     134:	myBaud =     B134 ; break ;
    case     150:	myBaud =     B150 ; break ;
    case     200:	myBaud =     B200 ; break ;
    case     300:	myBaud =     B300 ; break ;
    case     600:	myBaud =     B600 ; break ;
    case    1200:	myBaud =    B1200 ; break ;
    case    1800:	myBaud =    B1800 ; break ;
    case    2400:	myBaud =    B2400 ; break ;
    case    4800:	myBaud =    B4800 ; break ;
    case    9600:	myBaud =    B9600 ; break ;
    case   19200:	myBaud =   B19200 ; break ;
    case   38400:	myBaud =   B38400 ; break ;
    case   57600:	myBaud =   B57600 ; break ;
    case  115200:	myBaud =  B115200 ; break ;
    case  230400:	myBaud =  B230400 ; break ;
    case  460800:	myBaud =  B460800 ; break ;
    case  500000:	myBaud =  B500000 ; break ;
    case  576000:	myBaud =  B576000 ; break ;
    case  921600:	myBaud =  B921600 ; break ;
    case 1000000:	myBaud = B1000000 ; break ;
    case 1152000:	myBaud = B1152000 ; break ;
    case 1500000:	myBaud = B1500000 ; break ;
    case 2000000:	myBaud = B2000000 ; break ;
    case 2500000:	myBaud = B2500000 ; break ;
    case 3000000:	myBaud = B3000000 ; break ;
    case 3500000:	myBaud = B3500000 ; break ;
    case 4000000:	myBaud = B4000000 ; break ;

    default:
      return -1 ;
  }
  res = fcntl (fd, F_SETFL, O_RDWR);
  check(res, "fcntl");

  // Get and modify current options:

  res=tcgetattr (fd, &options) ;
  check(res, "tcgetattr");

  cfmakeraw   (&options) ;
  check(res, "tcgetattr");

  res=cfsetispeed (&options, myBaud) ;
  check(res, "cfsetispeed");

  res=cfsetospeed (&options, myBaud) ;
  check(res, "cfsetospeed");


  options.c_cflag |= (CLOCAL | CREAD) ;
  options.c_cflag &= ~PARENB ;

  //options.c_cflag &= ~CSTOPB ;
	options.c_cflag |= CSTOPB ;

  options.c_cflag &= ~CSIZE ;
  options.c_cflag |= CS8 ;
  options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG) ;
  options.c_oflag &= ~OPOST ;

  options.c_cc [VMIN]  =   0 ;
  options.c_cc [VTIME] = 100 ;	// Ten seconds (100 deciseconds)

  res=tcsetattr (fd, TCSANOW, &options) ;
  check(res, "tcsetattr");

  res=ioctl (fd, TIOCMGET, &status);
  check(res, "ioctl");

  status |= TIOCM_DTR ;
  status |= TIOCM_RTS ;

  res=ioctl (fd, TIOCMSET, &status);
  check(res, "ioctl");


	int resAt = atexit(USART_atexit);
	if(resAt != 0){
		printf("Error atexit()\n");
	}

  usleep (10000) ;	// 10mS
  return 0;
}

static void printHexBuffer(uint8_t* buf, int size){
  int i;
  for(i=0; i<size;i++){
    printf("%x ", buf[i]);
  }
  printf("\n");
}

static double getTime_us(){
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return ts.tv_sec*1E06+ts.tv_nsec*1E-03;
}

void check(int fd, const char* msg){
	if(fd != -1) return;
	perror(msg);
	exit(EXIT_FAILURE);
}
#endif
