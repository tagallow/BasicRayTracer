/*
 * Texture.h
 *
 *  Created on: Nov 11, 2009
 *      Author: ashesh
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <QtGui/QImage>

class Texture
{
public:
	Texture();
	virtual ~Texture();

	unsigned int getTextureID();
	bool createImage(char *filename);
	QString getName();
	void setName(QString& name);
        void lookup(float x,float y,float& r,float& g,float& b)
        {
                float c1[3],c2[3];
                int i,j;

                y = image->height()-1-y;

                //by default, repeat
                x = x-(int)x;
                y = y-(int)y;

                x *= image->width();
                y *= image->height();

                i = y;
                j = x;

                if (j<0)
                    j = 0;


                c1[0] = (1-(x-j))*qRed(image->pixel(j,i)) + (x-j)*qRed(image->pixel((j+1)%image->width(),i));
                c2[0] = (1-(x-j))*qRed(image->pixel(j,(i+1)%image->height())) + (x-j)*qRed(image->pixel((j+1)%image->width(),(i+1)%image->height()));

                c1[1] = (1-(x-j))*qGreen(image->pixel(j,i)) + (x-j)*qGreen(image->pixel((j+1)%image->width(),i));
                c2[1] = (1-(x-j))*qGreen(image->pixel(j,(i+1)%image->height())) + (x-j)*qGreen(image->pixel((j+1)%image->width(),(i+1)%image->height()));

                c1[2] = (1-(x-j))*qBlue(image->pixel(j,i)) + (x-j)*qBlue(image->pixel((j+1)%image->width(),i));
                c2[2] = (1-(x-j))*qBlue(image->pixel(j,(i+1)%image->height())) + (x-j)*qBlue(image->pixel((j+1)%image->width(),(i+1)%image->height()));




                r = (1-(y-i))*c1[0] + (y-i)*c2[0];
                g = (1-(y-i))*c1[1] + (y-i)*c2[1];
                b = (1-(y-i))*c1[2] + (y-i)*c2[2];

                r/=255;
                g/=255;
                b/=255;
        }
	
protected:
	unsigned int texID;
	QImage *image;
	QString name;

	void deleteImage();
};

#endif /* TEXTURE_H_ */
