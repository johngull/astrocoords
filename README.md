# astrocoords
Minimalistic C++ library for converting astronomical coordinates

Example of usage:
``` c++
astrocoords::Converter cconv;

//beta-orinon in Cherkasy, UA at 22:00UTC, 17th February 2018
cconv.setLocation(astrocoords::Angle::fromDegrees(49, 26, 40),
                  astrocoords::Angle::fromDegrees(32, 03, 35)
                  );
cconv.setDateAndUTCTime(2018, 2, 17, 22, 0, 0);
//beta-orion
astrocoords::Angle alt, az;
cconv.equatorialToHorizontal(astrocoords::Angle::fromHours(5, 14, 32.27210),
                             astrocoords::Angle::fromDegrees(-8, 12, 5.8981),
                             alt, az);

//use alt.degrees() and az.degrees()

```
