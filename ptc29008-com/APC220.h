/*
 * APC220.h
 *
 *  Created on: Feb 23, 2016
 *      Author: werner
 */

#ifndef APC220_H_
#define APC220_H_

class APC220 {
public:
	APC220();
	virtual ~APC220();
	void send(char * msg);
	void closed();
	void sendFSM(int tty_fd, char data, int count, int length);
private:
	int tty_fd;
};

#endif /* APC220_H_ */
