/*
 * @brief This is a simple application for testing UART communication with a GPS
 * connected to a RedPitaya
 *
 * @Author L. Horacio Arnaldi <lharnaldi@gmail.com>
 *
 * (c) LabDPR  http://labdpr.cab.cnea.gov.ar
 *
 * This part of code is written in C programming language.
 * Please visit http://en.wikipedia.org/wiki/C_(programming_language)
 * for more details on the language used herein.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <inttypes.h>
#include <string.h>

#include "uart_rp.h"

/* File descriptor definition */
int uart_fd = -1;

int rp_UartInit(void)
{
	uart_fd = open(PORTNAME, O_RDWR | O_NOCTTY | O_NDELAY);

  if(uart_fd == -1){
    fprintf(stderr, "Failed to open uart.\n");
    return -1;
  }
	return 0;
}

void rp_UartConfig(void)
{
    struct termios options;
    tcgetattr(uart_fd, &options);
		/*Configure the UART
			see http://pubs.opengroup.org/onlinepubs/007908799/xsh/termios.h.html */
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
		/* Setting attributes */
    tcflush(uart_fd, TCIFLUSH);
    tcsetattr(uart_fd, TCSANOW, &options);
}

int rp_UartPrintln(const char *line, int len)
{
    if (uart_fd != -1) {
        char *cpstr = (char *)malloc((len+1) * sizeof(char));
        strcpy(cpstr, line);
        cpstr[len-1] = '\r';
        cpstr[len] = '\n';

        int count = write(uart_fd, cpstr, len+1);
        if (count < 0) {
					fprintf(stderr, "UART TX error.\n");
					return -1;
        }
        free(cpstr);
    }
	return 0;
}

/* Read a line from UART */
int rp_UartReadln(char *buffer, int len)
{
	char c;
	char *b = buffer;
	int rx_length = -1;

	while(1) {
		rx_length = read(uart_fd, (void*)(&c), 1);
		
		if (rx_length <= 0) {
			//wait for messages
			 sleep(1);
		} else {
		  	if (c == '\n') {
		    	*b++ = '\0';
		      break;
		    }
		    *b++ = c;
		}
	}
	return 0;
}

void rp_UartClose(void)
{
	tcflush(uart_fd, TCIFLUSH);
	close(uart_fd);
}

