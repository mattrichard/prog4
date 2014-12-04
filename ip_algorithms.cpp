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
