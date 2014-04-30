/*
 FILE: GOL3D.cpp
 ABOUT: GLUTGameOfLife3D
 AUTHOR: kevin
 EMAIL: keglme00@hs-esslingen.de
 DATE: 02.01.2012
 */

#include "GOL3D.h"
#include <GL/gl.h>
#include <stdlib.h>
#include <stdio.h>

#include <time.h>

namespace gliewe {

void GOL3D::_drawCube()
{
	glBegin(GL_QUADS);
	glVertex3f( 1.0f, 1.0f,-1.0f);          // Top Right Of The Quad (Top)
	glVertex3f(-1.0f, 1.0f,-1.0f);          // Top Left Of The Quad (Top)
	glVertex3f(-1.0f, 1.0f, 1.0f);          // Bottom Left Of The Quad (Top)
	glVertex3f( 1.0f, 1.0f, 1.0f);          // Bottom Right Of The Quad (Top)

	glVertex3f( 1.0f,-1.0f, 1.0f);          // Top Right Of The Quad (Bottom)
	glVertex3f(-1.0f,-1.0f, 1.0f);          // Top Left Of The Quad (Bottom)
	glVertex3f(-1.0f,-1.0f,-1.0f);          // Bottom Left Of The Quad (Bottom)
	glVertex3f( 1.0f,-1.0f,-1.0f);          // Bottom Right Of The Quad (Bottom)

	glVertex3f( 1.0f, 1.0f, 1.0f);          // Top Right Of The Quad (Front)
	glVertex3f(-1.0f, 1.0f, 1.0f);          // Top Left Of The Quad (Front)
	glVertex3f(-1.0f,-1.0f, 1.0f);          // Bottom Left Of The Quad (Front)
	glVertex3f( 1.0f,-1.0f, 1.0f);          // Bottom Right Of The Quad (Front)

	glVertex3f( 1.0f,-1.0f,-1.0f);          // Bottom Left Of The Quad (Back)
	glVertex3f(-1.0f,-1.0f,-1.0f);          // Bottom Right Of The Quad (Back)
	glVertex3f(-1.0f, 1.0f,-1.0f);          // Top Right Of The Quad (Back)
	glVertex3f( 1.0f, 1.0f,-1.0f);          // Top Left Of The Quad (Back)

	glVertex3f(-1.0f, 1.0f, 1.0f);          // Top Right Of The Quad (Left)
	glVertex3f(-1.0f, 1.0f,-1.0f);          // Top Left Of The Quad (Left)
	glVertex3f(-1.0f,-1.0f,-1.0f);          // Bottom Left Of The Quad (Left)
	glVertex3f(-1.0f,-1.0f, 1.0f);          // Bottom Right Of The Quad (Left)

    glVertex3f( 1.0f, 1.0f,-1.0f);          // Top Right Of The Quad (Right)
    glVertex3f( 1.0f, 1.0f, 1.0f);          // Top Left Of The Quad (Right)
    glVertex3f( 1.0f,-1.0f, 1.0f);          // Bottom Left Of The Quad (Right)
    glVertex3f( 1.0f,-1.0f,-1.0f);          // Bottom Right Of The Quad (Right)
    glEnd();                        // Done Drawing The Quad
}

float GOL3D::_ranf()         /* ranf() is uniform in 0..1 */
{
	return ((float)rand())/ (float)RAND_MAX; // 32767.0f;
}

bool GOL3D::_ranb(float level) { return this->_ranf() < level; }

void GOL3D::_lock()
{
	while(this->_locked)
		sleepx(10);
	this->_locked = true;
}
void GOL3D::_unlock()
{
	this->_locked = false;
}

bool GOL3D::_isAlive(Point3D p)
{
	if(p.x < 0) return false;
	if(p.y < 0) return false;
	if(p.z < 0) return false;

	if(p.x >= this->_sizeX) return false;
	if(p.y >= this->_sizeY) return false;
	if(p.z >= this->_sizeZ) return false;

	return ((this->_field[p.x])[p.y])[p.z] == 1;
}

char*** GOL3D::_newField()
{
	char*** field;
	field = new char**[this->_sizeX];
	for(int x = 0 ; x < this->_sizeX ; x++)
	{
		field[x] = new char*[this->_sizeY];
		for(int y = 0 ; y < this->_sizeY ; y++)
		{
			(field[x])[y] = new char[this->_sizeZ];
		}
	}
	return field;
}
void GOL3D::_deleteField(char*** field)
{
	for(int x = 0 ; x < this->_sizeX ; x++)
	{
		for(int y = 0 ; y < this->_sizeY ; y++)
		{
			delete[] (field[x])[y];
		}
		delete[] field[x];
	}
	delete[] field;
}

GOL3D::GOL3D(int sizeX, int sizeY, int sizeZ) {
	this->_neighbors[0] = Point3D(-1, -1, -1 );
	this->_neighbors[1] = Point3D(-1, -1, 0 );
	this->_neighbors[2] = Point3D(-1, -1, 1 );
	this->_neighbors[3] = Point3D(-1, 0, -1 );
	this->_neighbors[4] = Point3D(-1, 0, 0 );
	this->_neighbors[5] = Point3D(-1, 0, 1 );
	this->_neighbors[6] = Point3D(-1, 1, -1 );
	this->_neighbors[7] = Point3D(-1, 1, 0 );
	this->_neighbors[8] = Point3D(-1, 1, 1 );
	this->_neighbors[9] = Point3D(0, -1, -1 );
	this->_neighbors[10] = Point3D(0, -1, 0 );
	this->_neighbors[11] = Point3D(0, -1, 1 );
	this->_neighbors[12] = Point3D(0, 0, -1 );
	this->_neighbors[13] = Point3D(0, 0, 1 );
	this->_neighbors[14] = Point3D(0, 1, -1 );
	this->_neighbors[15] = Point3D(0, 1, 0 );
	this->_neighbors[16] = Point3D(0, 1, 1 );
	this->_neighbors[17] = Point3D(1, -1, -1 );
	this->_neighbors[18] = Point3D(1, -1, 0 );
	this->_neighbors[19] = Point3D(1, -1, 1 );
	this->_neighbors[20] = Point3D(1, 0, -1 );
	this->_neighbors[21] = Point3D(1, 0, 0 );
	this->_neighbors[22] = Point3D(1, 0, 1 );
	this->_neighbors[23] = Point3D(1, 1, -1 );
	this->_neighbors[24] = Point3D(1, 1, 0 );
	this->_neighbors[25] = Point3D(1, 1, 1 );


	this->_probability = 0.13;

	this->_sizeX = sizeX;
	this->_sizeY = sizeY;
	this->_sizeZ = sizeZ;

	this->_field = this->_newField();

	srand( (unsigned)time( NULL ) );

	this->_locked = false;
}

GOL3D::~GOL3D() {
	this->_deleteField(this->_field);
}

void GOL3D::Clear()
{
	this->_lock();
	for(int x = 0 ; x < this->_sizeX ; x++)
	{
		for(int y = 0 ; y < this->_sizeY ; y++)
		{
			for(int z = 0 ; z < this->_sizeZ ; z++)
			{
				char ran = this->_ranb(this->_probability) ? 1 : 0;
				((this->_field[x])[y])[z] = ran;
			}
		}
	}
	this->_unlock();
}
void GOL3D::Step()
{
	this->_lock();
	char*** field = this->_newField();
	for(int x = 0 ; x < this->_sizeX ; x++)
	{
		for(int y = 0 ; y < this->_sizeY ; y++)
		{
			for(int z = 0 ; z < this->_sizeZ ; z++)
			{
				int alive = 0;
				for(int n = 0 ; n < 26 ; n++)
				{
					Point3D *p = this->_neighbors + n;
					if(this->_isAlive(Point3D(x + p->x, y + p->y, z + p->z))) alive++;
				}

				if(alive >= 8 && alive <= 10)
				{
					((field[x])[y])[z] = 1;
					continue;
				}
				if(alive < 8 && alive >= 5)
				{
					((field[x])[y])[z] = ((this->_field[x])[y])[z];
					continue;
				}
				if(alive < 5)
				{
					((field[x])[y])[z] = 2;
					continue;
				}
				if(alive > 10)
				{
					((field[x])[y])[z] = 2;
					continue;
				}

			}
		}
	}

	this->_deleteField(this->_field);
	this->_field = field;
	this->_unlock();
}
void GOL3D::Draw()
{
	sleepx(11);
	this->_lock();

	glPushMatrix();

	glTranslatef(this->_sizeX/-2.0f,this->_sizeY/-2.0f,this->_sizeZ/-2.0f);

	for(int x = 0 ; x < this->_sizeX ; x++)
	{
		for(int y = 0 ; y < this->_sizeY ; y++)
		{
			for(int z = 0 ; z < this->_sizeZ ; z++)
			{
				if(this->_isAlive(Point3D(x,y,z)))
				{
					glColor4f((float)x/(float)this->_sizeX, (float)y/(float)this->_sizeY, (float)z/(float)this->_sizeZ, 0.85f);
					glPushMatrix();
					glTranslatef(x,y,z);
					glScalef(0.5,0.5,0.5);
					this->_drawCube();
					glPopMatrix();
				}
			}
		}
	}

	glPopMatrix();
	this->_unlock();

}
void GOL3D::setProbability(float level)
{
	this->_probability = level;
}
void GOL3D::DumpFild()
{
	this->_lock();
	for(int x = 0 ; x < this->_sizeX ; x++)
	{
		for(int y = 0 ; y < this->_sizeY ; y++)
		{
			for(int z = 0 ; z < this->_sizeZ ; z++)
			{
				char c = ((this->_field[x])[y])[z];
				char o = '+';

				if(c == 1) printf("#");
				else printf("+");


			}
			printf("\n");
		}
		printf("\n\n");
	}
	this->_unlock();
}

} /* namespace gliewe */
