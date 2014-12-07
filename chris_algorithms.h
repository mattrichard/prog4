#ifndef CHRIS_ALGORITHMS_H
#define CHRIS_ALGORITHMS_H


#include <QImage>

QImage* brighten(const QImage& image, int thread_count);

QImage* darken(const QImage& image, int thread_count);

QImage* brighten_darken(const QImage& image, const int &thread_count, const int &value, const int &limit);

QImage* negate(const QImage& image, const int& thread_count);

QImage* binary_threshold(const QImage& image, const int& thread_count);

QImage* noise( const QImage& image, const int& thread_count);

#endif // CHRIS_ALGORITHMS_H
