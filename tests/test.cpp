#include <gtest/gtest.h>

#include "../lib/astrocoords/coords.h"

TEST(Angle, FromDegreeMinuteSecond)
{
    auto a = astrocoords::Angle::fromDegrees(0, 0, 10);
    EXPECT_FLOAT_EQ(10./3600., a.d());
    a = astrocoords::Angle::fromDegrees(0, 10, 0);
    EXPECT_FLOAT_EQ(10./60.,a.d());
    a = astrocoords::Angle::fromDegrees(135, 0, 0);
    EXPECT_FLOAT_EQ(135., a.d());
    a = astrocoords::Angle::fromDegrees(135, 5, 15);
    EXPECT_FLOAT_EQ(2.35772165, a.r());
}

TEST(Angle, FromDegreeNegative)
{
    auto a = astrocoords::Angle::fromDegrees(-1, 55);
    EXPECT_FLOAT_EQ(-1.9166667, a.d());
}

TEST(Angle, FromHours)
{
    auto a = astrocoords::Angle::fromHours(12);
    EXPECT_FLOAT_EQ( 180., a.d());
    a = astrocoords::Angle::fromHours(24);
    EXPECT_FLOAT_EQ(360., a.d());
    a = astrocoords::Angle::fromHours(1);
    EXPECT_FLOAT_EQ(15., a.d());
    a = astrocoords::Angle::fromHours(7, 5, 15);
    EXPECT_FLOAT_EQ(1.85550316, a.r());
    a = astrocoords::Angle::fromHours(16, 41.7);
    EXPECT_FLOAT_EQ(250.425, a.d());
}

TEST(Angle, NormalizeAngle)
{
    auto a = astrocoords::Angle::fromDegrees(-25);
    a.normalizeAngle();
    EXPECT_FLOAT_EQ(360-25, a.d());
    a = astrocoords::Angle::fromDegrees(3610);
    a.normalizeAngle();
    EXPECT_FLOAT_EQ(10, a.d());
}

TEST(Equatorial, EquatorialToHorizontalExample)
{
    astrocoords::Converter cconv;
    //example from http://www.stargazing.net/kepler/altaz.html

    //Birmingham
    cconv.setLocation(astrocoords::Angle::fromDegrees(52, 30),
                       astrocoords::Angle::fromDegrees(-1, 55)
                       );
    cconv.setUTCDateAndTime(1998, 8, 10, 23, 10, 0);

    astrocoords::Angle alt, az;
    //Messier object M13
    cconv.equatorialToHorizontal(astrocoords::Angle::fromHours(16, 41.7),
                                 astrocoords::Angle::fromDegrees(36, 28),
                                 alt, az);

    EXPECT_NEAR(49.2, alt.d(), 0.1);
    EXPECT_NEAR(269.1, az.d(), 0.1);
}

TEST(Equatorial, EquatorialToHorizontalPolar)
{
    astrocoords::Converter cconv;
    //Cherkasy, UA
    cconv.setLocation(astrocoords::Angle::fromDegrees(49, 26, 40),
                      astrocoords::Angle::fromDegrees(32, 03, 35)
                      );
    //18:57UTC, 19th February 2018
    cconv.setUTCDateAndTime(2018, 2, 19, 18, 57, 0);
    //Polaris (alpha Ursa minor)
    astrocoords::Angle alt, az;
    cconv.equatorialToHorizontal(astrocoords::Angle::fromHours(2, 32),
                                 astrocoords::Angle::fromDegrees(89, 16),
                                 alt, az);

    EXPECT_NEAR(49.716666667, alt.d(), 0.1);
    EXPECT_NEAR(358.9, az.d(), 0.1);


    //18:57UTC, 19th February 2018
    cconv.setUTCDateAndTime(2018, 2, 19, 15, 50, 0);
    //Polaris (alpha Ursa minor)
    cconv.equatorialToHorizontal(astrocoords::Angle::fromHours(2, 32),
                                 astrocoords::Angle::fromDegrees(89, 16),
                                 alt, az);
    EXPECT_NEAR(50.133333333, alt.d(), 0.1);
    EXPECT_NEAR(359.6, az.d(), 0.1);
}

TEST(Equatorial, EquatorialToHorizontalBetaOrion)
{
    astrocoords::Converter cconv;
    //Cherkasy, UA
    cconv.setLocation(astrocoords::Angle::fromDegrees(49, 26, 40),
                      astrocoords::Angle::fromDegrees(32, 03, 35)
                      );
    //18:57UTC, 19th February 2018
    cconv.setUTCDateAndTime(2018, 2, 19, 15, 50, 0);
    //beta-orion
    astrocoords::Angle alt, az;
    cconv.equatorialToHorizontal(astrocoords::Angle::fromHours(5, 14, 32.27210),
                                 astrocoords::Angle::fromDegrees(-8, 12, 5.8981),
                                 alt, az);

    EXPECT_NEAR(29.883333333, alt.d(), 0.1);
    EXPECT_NEAR(157.6, az.d(), 0.1);
}


int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
