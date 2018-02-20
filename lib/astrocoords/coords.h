#ifndef COORDS_CONVERTER
#define COORDS_CONVERTER

/**
 * Helper classes for Stars coordinates converting
 * Implementation based on http://www.stargazing.net/kepler/altaz.html
 */

namespace astrocoords
{

class Angle
{
    float angle_;
    double radians_;

    void calcRadians();
public:
    Angle(float angle=0);
    Angle(float r, bool radiansFlag);

    static Angle fromDegrees(float d, float minutes=0, float seconds=0);
    static Angle fromHours(float hours, float minutes=0, float seconds=0);

    /**
     * @brief normalizeAngle shift angle to be in [0;360)
     */
    void normalizeAngle();

    /**
     * @brief d fetch angle in degrees
     * @return angle in degrees
     */
    float d();

    /**
     * @brief r fetch angle in radians
     * @return angle in radians
     */
    float r();

    friend class Converter;
};

class Converter
{
    Angle latitude_ = Angle(0);
    Angle longitude_ = Angle(0);
    float daysFromJ2000_ = 0;
    float UTCtime_ = 0;
public:
    void setLocation(Angle latitude, Angle longitude);
    void setUTCDateAndTime(int year, int month, int day, int hour, int minute, int seconds);

    bool equatorialToHorizontal(Angle rightAngle, Angle declination, Angle& altitude, Angle& azimuth);
};


}
#endif
