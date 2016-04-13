/*
 * @brief This is a simple parser programm for NMEA messages
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
#include <inttypes.h>
#include <string.h>
#include <math.h>

#include "nmea_rp.h"

void rp_NmeaParseGpgga(char *nmea, gpgga_t *loc)
{
    char *ptr = nmea;

    ptr = strchr(ptr, ',')+1; //skip time

    ptr = strchr(ptr, ',')+1;
    loc->latitude = atof(ptr);

    ptr = strchr(ptr, ',')+1;
    switch (ptr[0]) {
        case 'N':
            loc->lat = 'N';
            break;
        case 'S':
            loc->lat = 'S';
            break;
        case ',':
            loc->lat = '\0';
            break;
    }

    ptr = strchr(ptr, ',')+1;
    loc->longitude = atof(ptr);

    ptr = strchr(ptr, ',')+1;
    switch (ptr[0]) {
        case 'W':
            loc->lon = 'W';
            break;
        case 'E':
            loc->lon = 'E';
            break;
        case ',':
            loc->lon = '\0';
            break;
    }

    ptr = strchr(ptr, ',')+1;
    loc->quality = (uint8_t)atoi(ptr);

    ptr = strchr(ptr, ',')+1;
    loc->satellites = (uint8_t)atoi(ptr);

    ptr = strchr(ptr, ',')+1;

    ptr = strchr(ptr, ',')+1;
    loc->altitude = atof(ptr);
}

void rp_NmeaParseGprmc(char *nmea, gprmc_t *loc)
{
    char *ptr = nmea;

    ptr = strchr(ptr, ',')+1; //skip time
    ptr = strchr(ptr, ',')+1; //skip status

    ptr = strchr(ptr, ',')+1;
    loc->latitude = atof(ptr);

    ptr = strchr(ptr, ',')+1;
    switch (ptr[0]) {
        case 'N':
            loc->lat = 'N';
            break;
        case 'S':
            loc->lat = 'S';
            break;
        case ',':
            loc->lat = '\0';
            break;
    }

    ptr = strchr(ptr, ',')+1;
    loc->longitude = atof(ptr);

    ptr = strchr(ptr, ',')+1;
    switch (ptr[0]) {
        case 'W':
            loc->lon = 'W';
            break;
        case 'E':
            loc->lon = 'E';
            break;
        case ',':
            loc->lon = '\0';
            break;
    }

    ptr = strchr(ptr, ',')+1;
    loc->speed = atof(ptr);

    ptr = strchr(ptr, ',')+1;
    loc->course = atof(ptr);
}

/**
 * Get the message type (GPGGA, GPRMC, etc..)
 *
 * This function filters out also wrong packages (invalid checksum)
 *
 * @param message The NMEA message
 * @return The type of message if it is valid
 */
uint8_t rp_NmeaGetMessageType(const char *message)
{
    uint8_t checksum = 0;
    if ((checksum = rp_NmeaValidChecksum(message)) != _EMPTY) {
        return checksum;
    }

    if (strstr(message, NMEA_GPGGA_STR) != NULL) {
        return NMEA_GPGGA;
    }

    if (strstr(message, NMEA_GPRMC_STR) != NULL) {
        return NMEA_GPRMC;
    }

    return NMEA_UNKNOWN;
}

uint8_t rp_NmeaValidChecksum(const char *message) {
    uint8_t checksum= (uint8_t)strtol(strchr(message, '*')+1, NULL, 16);

    char p;
    uint8_t sum = 0;
    ++message;
    while ((p = *message++) != '*') {
        sum ^= p;
    }

    if (sum != checksum) {
        return NMEA_CHECKSUM_ERR;
    }

    return _EMPTY;
}

