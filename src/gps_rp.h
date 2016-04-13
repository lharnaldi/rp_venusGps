/**
 * $Id: $
 *
 * @brief GPS stuffs module.
 *
 * @Author L. Horacio Arnaldi <lharnaldi@gmail.com>
 *
 * (c) LabDPR  http://labdpr.cab.cnea.gov.ar
 *
 * This part of code is written in C programming language.
 * Please visit http://en.wikipedia.org/wiki/C_(programming_language)
 * for more details on the language used herein.
 */

#ifndef _GPS_RP_H_
#define _GPS_RP_H_

typedef struct location {
    double latitude;
    double longitude;
    double speed;
    double altitude;
    double course;
} loc_t;

// Initialize device
void gps_init(void);
// Activate device
void gps_on(void);
// Get the actual location
void gps_location(loc_t *);


// Turn off device (low-power consumption)
void gps_off(void);

// -------------------------------------------------------------------------
// Internal functions
// -------------------------------------------------------------------------

// convert deg to decimal deg latitude, (N/S), longitude, (W/E)
void gps_convert_deg_to_dec(double *, char, double *, char);
double gps_deg_dec(double);

#endif
