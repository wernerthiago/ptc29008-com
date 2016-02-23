#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <iostream>
#include <sys/types.h>

using namespace std;

int main(){
	struct termios tio;
	struct termios stdio;
	struct termios old_stdio;
	int tty_fd;

	unsigned char c='D';
	tcgetattr(STDOUT_FILENO,&old_stdio);

	cout << "Enviando dados!" << endl;
	memset(&stdio,0,sizeof(stdio));
	stdio.c_iflag=0;
	stdio.c_oflag=0;
	stdio.c_cflag=0;
	stdio.c_lflag=0;
	stdio.c_cc[VMIN]=1;
	stdio.c_cc[VTIME]=0;
	tcsetattr(STDOUT_FILENO,TCSANOW,&stdio);
	tcsetattr(STDOUT_FILENO,TCSAFLUSH,&stdio);
	fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);       // make the reads non-blocking

	memset(&tio,0,sizeof(tio));
	tio.c_iflag=0;
	tio.c_oflag=0;
	tio.c_cflag=CS8|CREAD|CLOCAL;           // 8n1, see termios.h for more information
	tio.c_lflag=0;
	tio.c_cc[VMIN]=1;
	tio.c_cc[VTIME]=5;

	tty_fd=open("/dev/ttyUSB0", O_RDWR | O_NONBLOCK);
	cfsetospeed(&tio,B9600);            // 115200 baud
	cfsetispeed(&tio,B9600);            // 115200 baud

	tcsetattr(tty_fd,TCSANOW,&tio);

	write(STDOUT_FILENO,&c,1);              // if new data is available on the serial port, print it out
	char * frame = 0x7E;
	std::string msg = "Werner";
	strcat(frame,msg.c_str());
	int i = 0;
	while(i < strlen(msg)){
		write(tty_fd,&msg[i],1);
		cout << msg[i] << endl;
		i++;
	}
	sleep(2);
	close(tty_fd);
	tcsetattr(STDOUT_FILENO,TCSANOW,&old_stdio);

	return EXIT_SUCCESS;
}
