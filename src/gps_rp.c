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
#include <math.h>
#include "gps_rp.h"
#include "nmea_rp.h"
#include "uart_rp.h"

void gps_init(void) {
  rp_UartInit();
  rp_UartConfig();
}

void gps_on(void) {
}

// Compute the GPS location using decimal scale
void gps_location(loc_t *coord) {
  uint8_t status = _EMPTY;
  while(status != _COMPLETED) {
    gpgga_t gpgga;
    gprmc_t gprmc;
    char buffer[256];

    rp_UartReadln(buffer, 256);
    switch (rp_NmeaGetMessageType(buffer)) {
      case NMEA_GPGGA:
        rp_NmeaParseGpgga(buffer, &gpgga);

        gps_convert_deg_to_dec(&(gpgga.latitude), gpgga.lat, &(gpgga.longitude), gpgga.lon);

        coord->latitude = gpgga.latitude;
        coord->longitude = gpgga.longitude;
        coord->altitude = gpgga.altitude;

        status |= NMEA_GPGGA;
        break;
      case NMEA_GPRMC:
        rp_NmeaParseGprmc(buffer, &gprmc);

        coord->speed = gprmc.speed;
        coord->course = gprmc.course;

        status |= NMEA_GPRMC;
        break;
    }
  }
}

void gps_off(void) {
  //Write off
  rp_UartClose();
}

// Convert lat e lon to decimals (from deg)
void gps_convert_deg_to_dec(double *latitude, char ns,  double *longitude, char we)
{
  double lat = (ns == 'N') ? *latitude : -1 * (*latitude);
  double lon = (we == 'E') ? *longitude : -1 * (*longitude);

  *latitude = gps_deg_dec(lat);
  *longitude = gps_deg_dec(lon);
}

double gps_deg_dec(double deg_point)
{
  double ddeg;
  double sec = modf(deg_point, &ddeg)*60;
  int deg = (int)(ddeg/100);
  int min = (int)(deg_point-(deg*100));

  double absdlat = round(deg * 1000000.);
  double absmlat = round(min * 1000000.);
  double absslat = round(sec * 1000000.);

  return round(absdlat + (absmlat/60) + (absslat/3600)) /1000000;
}

