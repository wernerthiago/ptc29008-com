/*
 * APC220.cpp
 *
 *  Created on: Feb 23, 2016
 *      Author: werner
 */

#include "APC220.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>

using namespace std;

APC220::APC220() {
	// TODO Auto-generated constructor stub
	struct termios tio;
	struct termios stdio;
	struct termios old_stdio;

	tcgetattr(STDOUT_FILENO,&old_stdio);

	memset(&stdio,0,sizeof(stdio));
	stdio.c_iflag=0;
	stdio.c_oflag=0;
	stdio.c_cflag=0;
	stdio.c_lflag=0;
	stdio.c_cc[VMIN]=1;
	stdio.c_cc[VTIME]=0;
	tcsetattr(STDOUT_FILENO,TCSANOW,&stdio);
	tcsetattr(STDOUT_FILENO,TCSAFLUSH,&stdio);
	fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

	memset(&tio,0,sizeof(tio));
	tio.c_iflag=0;
	tio.c_oflag=0;
	tio.c_cflag=CS8|CREAD|CLOCAL;
	tio.c_lflag=0;
	tio.c_cc[VMIN]=1;
	tio.c_cc[VTIME]=5;

	tty_fd=open("/dev/ttyUSB0", O_RDWR | O_NONBLOCK);
	cfsetospeed(&tio,B9600);
	cfsetispeed(&tio,B9600);
}

APC220::~APC220() {
	// TODO Auto-generated destructor stub
}

void APC220::send(char* msg) {
	int i = 1;
	while(i < strlen(msg)+1){
		sendFSM(tty_fd,msg[i-1],i,strlen(msg));
		i++;
	}
	sleep(4);
	return;
}

void APC220::closed() {
	close(tty_fd);
}

char * APC220::receive(){
	char * mensagem;
	int i = 0;
	while(mensagem[i] = receiveFSM()){
		i++;
	}
	return mensagem;
}

char APC220::receiveFSM() {
	char data;
	int receive = read(tty_fd,&data,1);
	switch(data){
	case 0x7E:
		receive = read(tty_fd,&data,1);
		return data;
		break;
	case 0x7D:
		receive = read(tty_fd,&data,1);
		data = data ^ 0x20;
		return data;
		break;
	default:
		return data;
		break;
	}
}

void APC220::sendFSM(int tty_fd, char data, int count, int length) {
	char flag = 0x7E;
	char esc = 0x7D;
	switch(count){
	case 1:
		write(tty_fd,&flag,1);//0x7E
		write(tty_fd,&data,1);//Data
		break;
	default:
		if(((data == 0x7E) || (data == 0x7D)) && (count != length)){
			write(tty_fd,&esc,1); //0x7D
			data = data ^ 0x20;
			write(tty_fd,&data,1); //data XOR 0x20
		}else{
			if(count == length){
				write(tty_fd,&data,1);
				write(tty_fd,&flag,1);
			}else{
				write(tty_fd,&data,1);
			}
		}
		break;
	}
}
