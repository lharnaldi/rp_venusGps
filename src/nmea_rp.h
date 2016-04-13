/**
 * $Id: $
 *
 * @brief GPS NMEA messages parser.
 *
 * @Author L. Horacio Arnaldi <lharnaldi@gmail.com>
 *
 * (c) LabDPR  http://labdpr.cab.cnea.gov.ar
 *
 * This part of code is written in C programming language.
 * Please visit http://en.wikipedia.org/wiki/C_(programming_language)
 * for more details on the language used herein.
 */

#ifndef _NMEA_RP_H_
#define _NMEA_RP_H_

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define _EMPTY 0x00
#define NMEA_GPRMC 0x01
#define NMEA_GPRMC_STR "$GPRMC"
#define NMEA_GPGGA 0x02
#define NMEA_GPGGA_STR "$GPGGA"
#define NMEA_UNKNOWN 0x00
#define _COMPLETED 0x03

#define NMEA_CHECKSUM_ERR 0x80
#define NMEA_MESSAGE_ERR 0xC0

typedef struct gpgga {
    double latitude;
    char lat;
    double longitude;
    char lon;
    uint8_t quality;
    uint8_t satellites;
    double altitude;
} gpgga_t;

typedef struct gprmc {
    double latitude;
    char lat;
    double longitude;
    char lon;
    double speed;
    double course;
} gprmc_t;

uint8_t rp_NmeaGetMessageType(const char *);
uint8_t rp_NmeaValidChecksum(const char *);
void rp_NmeaParseGpgga(char *, gpgga_t *);
void rp_NmeaParseGprmc(char *, gprmc_t *);

#endif

