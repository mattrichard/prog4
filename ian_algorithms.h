#ifndef IAN_ALGORITHMS_H
#define IAN_ALGORITHMS_H

#include <QImage>

QImage* sharpen(const QImage& image, int thread_count);
QImage* emboss(const QImage& image, int thread_count);
QImage* enhance_contrast(const QImage& image, int thread_count);
QImage* reduce_contrast(const QImage& image, int thread_count);
QImage* posterize(const QImage& image, int thread_count);
QImage* gamma(const QImage& image, int thread_count);
QImage* fft(const QImage& image, int thread_count);
#endif // IAN_ALGORITHMS_H
