#include "coords.h"

#include <time.h>
#include <math.h>

using namespace std;
using namespace astrocoords;

constexpr float PI = 3.14159265359;

Angle::Angle(float angle) : angle_(angle)
{
    calcRadians();
}

Angle::Angle(float radians, bool) : radians_(radians)
{
    angle_ = radians_*180.f/PI;
}

void Angle::calcRadians()
{
    radians_ = angle_*PI/180.f;
}

Angle Angle::fromDegrees(float degrees, float minutes, float seconds)
{
    float fr = minutes/60.f + seconds/3600.f;
    return Angle(degrees + (degrees>=0 ? fr : -fr));
}

Angle Angle::fromHours(float hours, float minutes, float seconds)
{
    return fromDegrees(hours*15, minutes*15, seconds*15);
}

void Angle::normalizeAngle()
{
    bool changed = false;
    while(angle_<0)
    {
        angle_ += 360;
        changed = true;
    }
    while (angle_>=360)
    {
        angle_ -= 360;
        changed = true;
    }

    if(changed)
        calcRadians();
}

float Angle::d()
{
    return angle_;
}

float Angle::r()
{
    return radians_;
}

void Converter::setLocation(Angle latitude, Angle longitude)
{
    latitude_ = latitude;
    longitude_ = longitude;
}

void Converter::setUTCDateAndTime(int year, int month, int day, int hour, int minute, int seconds)
{
    tm JS2000;   //1200 hrs UT on Jan 1st 2000 AD
    JS2000.tm_year = 2000-1900;
    JS2000.tm_mon = 0;
    JS2000.tm_mday = 1;
    JS2000.tm_hour = 12;
    JS2000.tm_min = 0;
    JS2000.tm_sec = 0;

    tm now;
    now.tm_year = year-1900;
    now.tm_mon = month-1; // month is 0-11 in std::tm
    now.tm_mday = day; //year - 1900 based
    now.tm_hour = hour;
    now.tm_min = minute;
    now.tm_sec = seconds;
    time_t tjs2000 = mktime(&JS2000);
    time_t tnow = mktime(&now);

    daysFromJ2000_ = static_cast<float>(tnow-tjs2000)/86400.f;

    UTCtime_ = hour + minute/60.f + seconds/3600.f;
}


bool Converter::equatorialToHorizontal(Angle rightAngle, Angle declination, Angle &altitude, Angle &azimuth)
{
    Angle localTime = Angle(100.46f + 0.985647f * daysFromJ2000_ + longitude_.d() + 15*UTCtime_);
    localTime.normalizeAngle();

    Angle hourAngle = Angle(localTime.d()-rightAngle.d());
    hourAngle.normalizeAngle();

    altitude = Angle(asin( sin(declination.r())*sin(latitude_.r()) + cos(declination.r())*cos(latitude_.r())*cos(hourAngle.r()) ), true);

    float A = acos( (sin(declination.r()) - sin(altitude.r())*sin(latitude_.r()))/(cos(altitude.r())*cos(latitude_.r())) );
    if(sin(hourAngle.r())>=0)
        A = 2*PI - A;
    azimuth = Angle(A, true);

    return true;
}
