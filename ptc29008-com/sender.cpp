#include "APC220.h"
#include "tun.h"

int main(){
	APC220 sender;
	tun tun;

	char * payload = "werner1";
	sender.setPTC(PT1);
	cout << "SEQ" << sender.getSEQ() << ": " << payload << endl;
	bool aux = sender.send(payload);

	payload = "werner2";
	cout << "SEQ" << sender.getSEQ() << ": " << payload << endl;
	aux = sender.send(payload);

	tun.tun_alloc("tun0");
	if (tun.set_ip("tun0", "10.0.0.2", "10.0.0.1") < 0) {
		perror("so configurar a interface tun");
		return 0;
	}
	while(1);

	sender.closed();
	return 0;
}
