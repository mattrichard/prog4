#include "ian_algorithms.h"
#include "ip_algorithms.h"
#include <QColor>
#include <cmath>

QImage* sharpen(const QImage& image, int thread_count)
{
    QImage* newImage = new QImage(image.size(), image.format());
    QSize size = newImage->size();

    float mask[3][3];
    int row,col;

    //Mask
    //0  -1  0
    //-1  1 -1
    //0  -1  0
    for(row = 0; row < 3; row++)
        for(int col = 0; col < 3; col++)
            mask[row][col] = -1;
    mask[1][1] = 5;
    for(row = 0; row < 3; row+=2)
        for(col = 0; col < 3; col+=2)
            mask[row][col] = 0;

#   pragma omp parallel for num_threads(thread_count) default(none) \
        shared(size, image, newImage, mask) private(row,col)
    for(row = 1; row < size.height()-1; row++)
    {
        for(col = 1; col < size.width()-1; col++)
        {
            float r=0,g=0,b=0,a;

            // Convolve
            for(int i = -1; i <= 1; i++)
            {
                for(int j = -1; j <= 1; j++)
                {
                    QColor color = QColor(image.pixel( col + j, row + i));

                    //grab the original transparency value
                    if(i == 0 && j == 0) a = color.alpha();

                    //update the colors
                    r+=color.red()*mask[i+1][j+1];
                    b+=color.blue()*mask[i+1][j+1];
                    g+=color.green()*mask[i+1][j+1];
                }
            }
            if(r > 255)r = 255;
            if(r < 0)r = 0;
            if(g < 0) g = 0;
            if(g > 255) g = 255;
            if(b < 0) b = 0;
            if(b > 255) b = 255;

            QColor color = QColor(image.pixel(col, row));
            color.setRed((int)r);
            color.setGreen((int)g);
            color.setBlue((int)b);
            color.setAlpha((int)a);

            newImage->setPixel(col, row, color.rgb());
        }
    }

    return newImage;
}

QImage* enhance_contrast(const QImage& image, int thread_count)
{
    QImage* newImage = new QImage(image.size(), image.format());
    QSize size = newImage->size();

    int row,col;

#   pragma omp parallel for num_threads(thread_count) default(none) \
        shared(size, image, newImage) private(row,col)
    for(row = 0; row < size.height(); row++)
    {
        for(col = 0; col < size.width(); col++)
        {
            QColor color = QColor(image.pixel(col, row));
            float r = (color.red()-64.0)*(255.0/(128.0));
            float g = (color.green()-64.0)*(255.0/(128.0));
            float b = (color.blue()-64.0)*(255.0/(128.0));
            if(r > 255)r=255;
            if(r < 0) r = 0;
            if(g > 255)g=255;
            if(g < 0) g = 0;
            if(b > 255)b=255;
            if(b < 0) b = 0;
            color.setRed((int)r);
            color.setGreen((int)g);
            color.setBlue((int)b);

            newImage->setPixel(col, row, color.rgb());
        }
    }

    return newImage;
}

QImage* reduce_contrast(const QImage& image, int thread_count)
{
    QImage* newImage = new QImage(image.size(), image.format());
    QSize size = newImage->size();

    int row,col;

#   pragma omp parallel for num_threads(thread_count) default(none) \
        shared(size, image, newImage) private(row,col)
    for(row = 0; row < size.height(); row++)
    {
        for(col = 0; col < size.width(); col++)
        {
            QColor color = QColor(image.pixel(col, row));
            float r = (color.red())*(128/(255.0)) + 64;
            float g = (color.green())*(128/(255.0)) + 64;
            float b = (color.blue())*(128/(255.0)) + 64;
            if(r > 255)r=255;
            if(r < 0) r = 0;
            if(g > 255)g=255;
            if(g < 0) g = 0;
            if(b > 255)b=255;
            if(b < 0) b = 0;
            color.setRed((int)r);
            color.setGreen((int)g);
            color.setBlue((int)b);


            newImage->setPixel(col, row, color.rgb());
        }
    }

    return newImage;
}

QImage* emboss(const QImage& image, int thread_count)
{
    QImage* newImage = new QImage(image.size(), image.format());
    QSize size = newImage->size();

    int row,col;

    //Mask
    //1   0
    //0  -1
    //actually easier to just hardcode this one

#   pragma omp parallel for num_threads(thread_count) default(none) \
        shared(size, image, newImage) private(row,col)
    for(row = 0; row < size.height()-1; row++)
    {
        for(col = 0; col < size.width()-1; col++)
        {
            QColor color1 = QColor(image.pixel(col, row));
            QColor color2 = QColor(image.pixel(col+1, row+1));
            float val = qGray(color1.red(),color1.green(),color1.blue()) - qGray(color2.red(),color2.green(),color2.blue()) + 128;
            if( val > 255) val=255;
            if(val < 0) val = 0;
            color1.setRgb((int)val,(int)val,(int)val);


            newImage->setPixel(col, row, color1.rgb());
        }
    }

    return newImage;
}

QImage* posterize(const QImage& image, int thread_count)
{
    QImage* newImage = new QImage(image.size(), image.format());
    QSize size = newImage->size();

    const int levels = 4;
    int interval = 256/levels;
    int quanta = 255/(levels-1);
    int row,col;

#   pragma omp parallel for num_threads(thread_count) default(none) \
        shared(size, image, newImage,interval,quanta) private(row,col)
    for(row = 0; row < size.height(); row++)
    {
        for(col = 0; col < size.width(); col++)
        {
            QColor color = QColor(image.pixel(col, row));
            int r = (color.red()/interval)*quanta;
            int g = (color.green()/interval)*quanta;
            int b = (color.blue()/interval)*quanta;

            color.setRed(r);
            color.setGreen(g);
            color.setBlue(b);


            newImage->setPixel(col, row, color.rgb());
        }
    }

    return newImage;
}

QImage* gamma(const QImage& image, int thread_count)
{
    QImage* newImage = new QImage(image.size(), image.format());
    QSize size = newImage->size();

    const double gamma = 0.5;
    int row,col;

#   pragma omp parallel for num_threads(thread_count) default(none) \
        shared(size, image, newImage) private(row,col)
    for(row = 0; row < size.height(); row++)
    {
        for(col = 0; col < size.width(); col++)
        {
            QColor color = QColor(image.pixel(col, row));
            int r = pow(color.red()/255.0,gamma)*255+0.5;
            int g = pow(color.green()/255.0,gamma)*255+0.5;
            int b = pow(color.blue()/255.0,gamma)*255+0.5;

            color.setRed(r);
            color.setGreen(g);
            color.setBlue(b);


            newImage->setPixel(col, row, color.rgb());
        }
    }

    return newImage;
}

QImage* fft(const QImage& image, int thread_count)
{
    //start off by grayscaling the image
    QImage* newImage = grayscale(image,thread_count);
    QSize size = newImage->size();

    double * real = new double[size.height()*size.width()];
    double * complex = new double[size.height()*size.width()];
    double * real2 = new double[size.height()*size.width()];
    double * complex2 = new double[size.height()*size.width()];
    double * magnitude = new double[size.height()*size.width()];

    //initialize the arrays to all 0s
    for(int i = 0; i < size.width()*size.height(); i++)
    {
        real[i] = 0;
        real2[i] = 0;
        complex[i] = 0;
        complex2[i] = 0;
        magnitude[i] = 0;
    }

    //First do a 1D fft on each row - this parallelizes nicely
#   pragma omp parallel for num_threads(thread_count) default(none) \
        shared(real,complex,image,size)
    for(int r = 0; r < size.height(); r++)
    {
        //for each element in the row
        for(int c = 0; c < size.width(); c++)
        {
            //loop through the whole row and do the formula
            for(int i = 0; i < size.width(); i++)
            {
                QColor color = QColor(image.pixel(i,r));
                real[r*size.width()+c] +=  color.value()*cos(-2*M_PI*c*i/size.width());
                complex[r*size.width()+c] += color.value()*sin(-2*M_PI*c*i/size.width());
            }
        }
    }

    //now go through each column and do an fft on the column
#   pragma omp parallel for num_threads(thread_count) default(none) \
        shared(real2,complex2,real,complex,size)
    for(int c = 0; c < size.width(); c++)
    {
        //for each element of the column
        for(int r = 0; r < size.height(); r++)
        {
            //loop through the whole column and do the formula
            for(int i = 0; i < size.height(); i++)
            {
                real2[r*size.width()+c] +=  real[i*size.width()+c]*cos(-2*M_PI*r*i/size.height());
                complex2[r*size.width()+c] += complex[i*size.width()+c]*sin(-2*M_PI*r*i/size.height());
            }
        }
    }

    double max=-1,min=-1;
    //get the magnitude, and take the log to scale things nicely
#   pragma omp parallel for num_threads(thread_count) default(none) \
        shared(magnitude,complex2,real2,size) reduction(max:max) reduction(min:min)
    for(int r = 0; r < size.height(); r++)
    {
        for(int c = 0; c < size.width(); c++)
        {
            magnitude[r*size.width()+c] = log(sqrt(real2[r*size.width()+c]*real2[r*size.width()+c] + complex2[r*size.width()+c]*complex2[r*size.width()+c]));
            if(max == -1)
                max = magnitude[r*size.width()+c];
            if(min == -1)
                min = magnitude[r*size.width()+c];
            if(magnitude[r*size.width()+c] > max)
                max = magnitude[r*size.width()+c];
            if(magnitude[r*size.width()+c] < min)
                min = magnitude[r*size.width()+c];
        }
    }

    //the log gives us really small numbers, normalize before setting the pixels in the image


    //go through and set each pixel in the new image
#   pragma omp parallel for num_threads(thread_count) default(none) \
        shared(min,max,newImage,magnitude,size)
    for(int r = 0; r < size.height(); r++)
    {
        for(int c = 0; c < size.width(); c++)
        {
            double norm_mag = (magnitude[r*size.width()+c]-min)/(max-min)*255;
            QColor color;
            color.setRgb(norm_mag,norm_mag,norm_mag);
            newImage->setPixel((c+size.width()/2)%size.width(),(r+size.height()/2)%size.height(),color.rgb());
        }
    }

    delete[] real;
    delete[] real2;
    delete[] complex;
    delete[] complex2;
    delete[] magnitude;

    return newImage;
}























