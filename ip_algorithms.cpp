#include "ip_algorithms.h"

#include <QColor>

#include <cmath>

using namespace std;

QImage* grayscale(const QImage& image, int thread_count)
{
    QImage* newImage = new QImage(image.size(), image.format());
    QSize size = newImage->size();

    int r;

#   pragma omp parallel for num_threads(thread_count) default(none) \
        shared(size, image, newImage) private(r)
    for(r = 0; r < size.height(); r++)
    {
        for(int c = 0; c < size.width(); c++)
        {
            QColor color = QColor(image.pixel(c, r));

            int gray = (color.red() + color.green() + color.blue()) / 3.0;

            color.setRed(gray);
            color.setGreen(gray);
            color.setBlue(gray);

            newImage->setPixel(c, r, color.rgb());
        }
    }

    return newImage;
}

QImage* smooth(const QImage& image, int thread_count)
{
    QImage* newImage = new QImage(image.size(), image.format());
    QSize size = newImage->size();

    float mask[3][3];
    int r;

    for(r = 0; r < 3; r++)
        for(int c = 0; c < 3; c++)
            mask[r][c] = 1.0 / 9.0;

#   pragma omp parallel for num_threads(thread_count) default(none) \
        shared(size, image, newImage, mask) private(r)
    for(r = 0; r < size.height(); r++)
    {
        for(int c = 0; c < size.width(); c++)
        {
            float value = 0;

            // Convolve
            for(int i = -1; i <= 1; i++)
            {
                for(int j = -1; j <= 1; j++)
                {
                    QColor color = QColor(image.pixel((c + j + size.width()) % size.width(), (r + i + size.height()) % size.height()));
                    value += mask[i+1][j+1] * color.value();
                }
            }

            QColor color = QColor(image.pixel(c, r));

            color.setHsv(color.hue(), color.saturation(), (int)value);

            newImage->setPixel(c, r, color.rgb());
        }
    }

    return newImage;
}

QImage* gradient(const QImage& image, int thread_count)
{
    QImage grayscaleImage = image.convertToFormat(QImage::Format_Mono);

    QImage* newImage = new QImage(image.size(), image.format());
    QSize size = newImage->size();

    float xmask[3][3] = {{-1/4.0, -2/4.0, -1/4.0}, {0, 0, 0}, {1/4.0, 2/4.0, 1/4.0}};
    float ymask[3][3] = {{-1/4.0, 0, 1/4.0}, {-2/4.0, 0, 2/4.0}, {-1/4.0, 0, 1/4.0}};
    int r;

#   pragma omp parallel for num_threads(thread_count) default(none) \
        shared(size, grayscaleImage, newImage, xmask, ymask) private(r)
    for(r = 0; r < size.height(); r++)
    {
        for(int c = 0; c < size.width(); c++)
        {
            float xvalue = 0;
            float yvalue = 0;
            float magnitude;

            // Convolve x mask
            for(int i = -1; i <= 1; i++)
            {
                for(int j = -1; j <= 1; j++)
                {
                    QColor color = QColor(grayscaleImage.pixel((c + j + size.width()) % size.width(), (r + i + size.height()) % size.height()));
                    xvalue += xmask[i+1][j+1] * color.value();
                }
            }

            // Convolve y mask
            for(int i = -1; i <= 1; i++)
            {
                for(int j = -1; j <= 1; j++)
                {
                    QColor color = QColor(grayscaleImage.pixel((c + j + size.width()) % size.width(), (r + i + size.height()) % size.height()));
                    yvalue += ymask[i+1][j+1] * color.value();
                }
            }

            magnitude = sqrt(xvalue*xvalue + yvalue*yvalue);

            if(magnitude < 0)
                magnitude = 0;
            else if(magnitude > 255)
                magnitude = 255;

            QColor color = QColor(grayscaleImage.pixel(c, r));
            color.setHsv(color.hue(), color.saturation(), (int)magnitude);

            newImage->setPixel(c, r, color.rgb());
        }
    }

    return newImage;
    //return new QImage(temp);
}
