#include "ian_algorithms.h"
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
