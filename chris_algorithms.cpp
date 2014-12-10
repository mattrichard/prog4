#include "chris_algorithms.h"

#include <QColor>

#include <cmath>
#include <time.h>

using namespace std;

/******************************************************************************
 * Function: brighten
 * Description: Sets the value to be added to each pixel to 20 in for the universal
 * brighten_darken function
 * Parameters:
 *   image - the image to process on
 *   thread_count - the number of threads to use
 * Returns: the brightened image after its computed
 *****************************************************************************/
QImage* brighten(const QImage &image, int thread_count)
{
    int bright = 20;
    return brighten_darken(image, thread_count, bright, 255);
}

/******************************************************************************
 * Function: darken
 * Description: Sets the value to be added to each pixel to -20 in for the universal
 * brighten_darken function
 * Parameters:
 *   image - the image to process on
 *   thread_count - the number of threads to use
 * Returns: the darkened image after its computed
 *****************************************************************************/
QImage* darken(const QImage &image, int thread_count)
{
    int dark = -20;
    return brighten_darken(image, thread_count, dark, 0);
}

/******************************************************************************
 * Function: brighten
 * Description: Adds teh value passed in to each pixel value in parrallel
 * Parameters:
 *   image - the image to process on
 *   thread_count - the number of threads to use
 *   value - the integer value to be added to each pixel value
 *   limit - 0 if the image is getting darkened so no value is less than 0 or
 *           255 if the image is getting brightened so no value is greater than 255
 * Returns: the brightened or darkened image after its computed
 *****************************************************************************/
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

/******************************************************************************
 * Function: negate
 * Description: Negates each pixel value ( 255 - pixel_value) in parrallel
 * for the image.
 * Parameters:
 *   image - the image to process on
 *   thread_count - the number of threads to use
 * Returns: The negated image
 *****************************************************************************/
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

/******************************************************************************
 * Function: binary_threshold
 * Description: takes a grayscale image and calculates a simple binary threshold
 * in parallel
 * Parameters:
 *   image - the image to process on
 *   thread_count - the number of threads to use
 * Returns: the darkened image after its computed
 *****************************************************************************/
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

/******************************************************************************
 * Function: noise
 * Description: Adds salt and pepper noise randomly through the image in parallel
 * Parameters:
 *   image - the image to process on
 *   thread_count - the number of threads to use
 * Returns: the darkened image after its computed
 *****************************************************************************/
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
            //Commented out since this line adds random white and black horizontal lines across image
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
