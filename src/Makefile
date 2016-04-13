CC=gcc
FLAGS=-Wall -O3

####
PROG=gpsLogger
EXTRA=nmea_rp.c uart_rp.c gps_rp.c

all: $(PROG)

$(PROG): $(PROG).c $(EXTRA) 
	$(CC) $(FLAGS) -o $(PROG) $(PROG).c $(EXTRA) -lm 

clean:
	rm -f $(PROG)
