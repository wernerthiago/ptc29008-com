#include "APC220.h"
#include "tun.h"

int main(){
	APC220 sender;
	tun tun;
	char data;
	char buffer[MAX_DADOS];
	int i = 0;
	sender.setPTC(PT1);

	int fd = tun.tun_alloc("tun0");
	if (tun.set_ip("tun0", "10.0.0.2", "10.0.0.1") < 0) {
		perror("so configurar a interface tun");
		return 0;
	}
	while(true){
		int n;

		if((n=read(fd,buffer+3,MAX_LENGTH)) > 0){
			cout << "Bytes: " << n << endl;
			sender.send(buffer);
		}
	}

	sender.closed();
	return 0;
}
