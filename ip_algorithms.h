#ifndef IP_ALGORITHMS_H
#define IP_ALGORITHMS_H

#include <QImage>

QImage* grayscale(const QImage& image, int thread_count);

QImage* smooth(const QImage& image, int thread_count);

QImage* gradient(const QImage& image, int thread_count);

#endif // IP_ALGORITHMS_H
