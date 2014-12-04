#include "ip_algorithms.h"

#include <QColor>

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

QImage *smooth(const QImage& image, int thread_count)
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
