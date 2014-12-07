#include "chris_algorithms.h"

#include <QColor>

#include <cmath>
#include <time.h>

using namespace std;


QImage* brighten(const QImage &image, int thread_count)
{
    int bright = 20;
    return brighten_darken(image, thread_count, bright, 255);
}

QImage* darken(const QImage &image, int thread_count)
{
    int dark = -20;
    return brighten_darken(image, thread_count, dark, 0);
}

QImage* brighten_darken(const QImage& image, const int &thread_count, const int &value, const int &limit)
{
    QImage* newImage = new QImage(image.size(), image.format());
    QSize size = newImage->size();

    int r;
    int red, green, blue;

#   pragma omp parallel for num_threads(thread_count) default(none) \
        shared(size, image, newImage,value, limit) private(r, red, green, blue)
    for(r = 0; r < size.height(); r++)
    {
        for(int c = 0; c < size.width(); c++)
        {
            QColor color = QColor(image.pixel(c, r));

            color.getRgb(&red, &green, &blue);

            red+=value;
            green+=value;
            blue+=value;

            if( red <  255 && red >0)
                color.setRed( red );
            else
                color.setRed(limit);

            if( green < 255 && green > 0)
                color.setGreen( green );
            else
                color.setGreen(limit);

            if(blue < 255 && blue > 0)
                color.setBlue( blue );
            else
                color.setBlue(limit);

            newImage->setPixel(c, r, color.rgb());


        }
    }

    return newImage;
}

QImage* negate(const QImage& image, const int& thread_count)
{
    QImage* newImage = new QImage(image.size(), image.format());
    QSize size = newImage->size();
    int r;
    int red, green, blue;

#   pragma omp parallel for num_threads(thread_count) default(none) \
        shared(size, image, newImage) private(r, red, green, blue)
    for(r = 0; r < size.height(); r++)
    {
        for(int c = 0; c < size.width(); c++)
        {
            QColor color = QColor(image.pixel(c, r));
            color.getRgb(&red, &green, &blue);

            red   = 255 - red;
            green = 255 - green;
            blue  = 255 - blue;

            color.setRgb(red, green, blue);
            newImage->setPixel(c, r, color.rgb());
        }
    }

    return newImage;
}
QImage* binary_threshold(const QImage& image, const int& thread_count)
{
    QImage* newImage = new QImage(image.size(), image.format());
    QSize size = newImage->size();
    int r;
    int red, green, blue;

#   pragma omp parallel for num_threads(thread_count) default(none) \
        shared(size, image, newImage) private(r, red, green, blue)
    for(r = 0; r < size.height(); r++)
    {
        for(int c = 0; c < size.width(); c++)
        {
            QColor color = QColor(image.pixel(c, r));
            color.getRgb(&red, &green, &blue);

            red   = ( red   > 127 ) ? 255 : 0;
            blue  = ( blue  > 127 ) ? 255 : 0;
            green = ( green > 127 ) ? 255 : 0;

            color.setRgb(red, green, blue);
            newImage->setPixel(c, r, color.rgb());
        }
    }
    return newImage;
}

QImage* noise( const QImage& image, const int& thread_count)
{
    QImage* newImage = new QImage(image.size(), image.format());
    QSize size = newImage->size();
    int r;
    int red, green, blue;
#   pragma omp parallel for num_threads(thread_count) default(none) \
        shared(size, image, newImage, thread_count) private(r, red, green, blue)
    for(r = 0; r < size.height(); r++)
    {
        for(int c = 0; c < size.width(); c++)
        {
            //unsigned int my_time =  time(NULL)*r*r + c*r*c/(thread_count+r+c*r + time(NULL));
            unsigned int my_time = time(NULL) * r + c;
            int x = rand_r(&my_time) % 100;
            QColor color = QColor(image.pixel(c, r));
            color.getRgb(&red, &green, &blue);
            red   = (( x == 99 ) ? 255 : ((x == 23) ? 0 : red));
            green = (( x == 99 ) ? 255 : ((x == 23) ? 0 : green));
            blue  = (( x == 99 ) ? 255 : ((x == 23) ? 0 : blue));

            color.setRgb(red, green, blue);

            newImage->setPixel(c, r, color.rgb());


        }
    }
    return newImage;
}
