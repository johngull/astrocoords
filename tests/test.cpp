#include <gtest/gtest.h>

#include "../lib/astrocoords/coords.h"

TEST(Angle, FromDegreeMinuteSecond)
{
    auto a = astrocoords::Angle::fromDegrees(0, 0, 10);
    EXPECT_FLOAT_EQ(10./3600., a.degrees());
    a = astrocoords::Angle::fromDegrees(0, 10, 0);
    EXPECT_FLOAT_EQ(10./60.,a.degrees());
    a = astrocoords::Angle::fromDegrees(135, 0, 0);
    EXPECT_FLOAT_EQ(135., a.degrees());
    a = astrocoords::Angle::fromDegrees(135, 5, 15);
    EXPECT_FLOAT_EQ(2.35772165, a.radians());
}

TEST(Angle, FromDegreeNegative)
{
    auto a = astrocoords::Angle::fromDegrees(-1, 55);
    EXPECT_FLOAT_EQ(-1.9166667, a.degrees());
}

TEST(Angle, FromHours)
{
    auto a = astrocoords::Angle::fromHours(12);
    EXPECT_FLOAT_EQ( 180., a.degrees());
    a = astrocoords::Angle::fromHours(24);
    EXPECT_FLOAT_EQ(360., a.degrees());
    a = astrocoords::Angle::fromHours(1);
    EXPECT_FLOAT_EQ(15., a.degrees());
    a = astrocoords::Angle::fromHours(7, 5, 15);
    EXPECT_FLOAT_EQ(1.85550316, a.radians());
    a = astrocoords::Angle::fromHours(16, 41.7);
    EXPECT_FLOAT_EQ(250.425, a.degrees());
}

TEST(Angle, NormalizeAngle)
{
    auto a = astrocoords::Angle::fromDegrees(-25);
    a.normalizeAngle();
    EXPECT_FLOAT_EQ(360-25, a.degrees());
    a = astrocoords::Angle::fromDegrees(3610);
    a.normalizeAngle();
    EXPECT_FLOAT_EQ(10, a.degrees());
}

TEST(Equatorial, EquatorialToHorizontal)
{
    astrocoords::Converter cconv;
    //example from http://www.stargazing.net/kepler/altaz.html

    //Birmingham
    cconv.setLocation(astrocoords::Angle::fromDegrees(52, 30),
                       astrocoords::Angle::fromDegrees(-1, 55)
                       );
    cconv.setDateAndUTCTime(1998, 8, 10, 23, 10, 0);

    astrocoords::Angle alt, az;
    //Messier object M13
    cconv.equatorialToHorizontal(astrocoords::Angle::fromHours(16, 41.7),
                                 astrocoords::Angle::fromDegrees(36, 28),
                                 alt, az);

    EXPECT_NEAR(49.2, alt.degrees(), 0.1);
    EXPECT_NEAR(269.1, az.degrees(), 0.1);
}


int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
