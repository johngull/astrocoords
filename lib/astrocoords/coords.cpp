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

float Angle::degrees()
{
    return angle_;
}

float Angle::radians()
{
    return radians_;
}

void Converter::setLocation(Angle latitude, Angle longitude)
{
    latitude_ = latitude;
    longitude_ = longitude;
}

void Converter::setDateAndUTCTime(int year, int month, int day, int hour, int minute, int seconds)
{
    tm JS2000{0, 0, 12, 1, 0, 2000, 0};   //1200 hrs UT on Jan 1st 2000 AD
    tm now{seconds, minute, hour, day, month-1, year, 0};   // month is 0-11 in std::tm
    time_t tjs2000 = mktime(&JS2000);
    time_t tnow = mktime(&now);

    daysFromJ2000_ = static_cast<float>(tnow-tjs2000)/86400.f;

    UTCtime_ = hour + minute/60.f + seconds/3600.f;
}


bool Converter::equatorialToHorizontal(Angle rightAngle, Angle declination, Angle &altitude, Angle &azimuth)
{
    Angle localTime = Angle(100.46f + 0.985647f * daysFromJ2000_ + longitude_.degrees() + 15*UTCtime_);
    localTime.normalizeAngle();

    Angle hourAngle = Angle(localTime.degrees()-rightAngle.degrees());
    hourAngle.normalizeAngle();

    altitude = Angle(asin( sin(declination.radians())*sin(latitude_.radians()) + cos(declination.radians())*cos(latitude_.radians())*cos(hourAngle.radians()) ), true);

    float A = acos( (sin(declination.radians()) - sin(altitude.radians())*sin(latitude_.radians()))/(cos(altitude.radians())*cos(latitude_.radians())) );
    if(sin(hourAngle.radians())>=0)
        A = 2*PI - A;
    azimuth = Angle(A, true);

    return true;
}
