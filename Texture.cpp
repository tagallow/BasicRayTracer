/*
 * Texture.cpp
 *
 *  Created on: Nov 11, 2009
 *      Author: ashesh
 */

#include "Texture.h"
#include <QtGUI/QWidget>
#include <QtOpenGL>
#include <QColor>

Texture::Texture()
{
	// TODO Auto-generated constructor stub
	image = NULL;
        texID = 0;
}

Texture::~Texture()
{
	// TODO Auto-generated destructor stub
	deleteImage();
}

unsigned int Texture::getTextureID()
{
	return texID;
}

bool Texture::createImage(char *filename)
{
	deleteImage();

	image = new QImage(QString(filename));

	if (image->isNull())
		return false;

	QImage q;

	q = QGLWidget::convertToGLFormat(*image);

        if (texID!=0)
            glDeleteTextures(1,&texID);

	glGenTextures(1,&texID);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texID);

        gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGB,q.width(),q.height(),GL_RGBA,GL_UNSIGNED_BYTE,q.bits());
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glDisable(GL_TEXTURE_2D);

	return true;
}

void Texture::setName(QString& name)
{
	this->name = name;
}

QString Texture::getName()
{
	return name;
}



void Texture::deleteImage()
{
	if (image)
		delete image;
        if (texID!=0)
            glDeleteTextures(1,&texID);
}
