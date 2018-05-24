#ifndef UTM_H_
#define UTM_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <math.h>
#include <stdio.h>

typedef struct tagUTMCorr
{
        double x;
        double y;
}UTMCoor;

typedef struct tagWGS84Corr
{
        double lat;
        double log;
}WGS84Corr;

inline double DegToRad (double deg);
inline double RadToDeg (double rad);
double ArcLengthOfMeridian (double phi);
inline double UTMCentralMeridian (int zone);
double FootpointLatitude (double y);
void MapLatLonToXY (double phi, double lambda, double lambda0, UTMCoor* xy);
void MapXYToLatLon (double x, double y, double lambda0, WGS84Corr* philambda);
void LatLonToUTMXY (double lat, double lon, int zone, UTMCoor* xy);
void UTMXYToLatLon (double x, double y, int zone, int southhemi, WGS84Corr* latlon);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
