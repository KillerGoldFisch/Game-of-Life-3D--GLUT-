/*
 FILE: GOL3D.h
 ABOUT: GLUTGameOfLife3D
 AUTHOR: kevin
 EMAIL: keglme00@hs-esslingen.de
 DATE: 02.01.2012
 */

#ifndef GOL3D_H_
#define GOL3D_H_

#ifndef WIN32
#include <unistd.h>
#else
#include <windows.h>
#endif
static int sleepx( unsigned long _t ) {
#ifdef WIN32
return sleep( _t );
#else
return usleep( 1000 * _t );
#endif
}

namespace gliewe {

struct Point3D
{
	int x;
	int y;
	int z;
	Point3D(){}
	Point3D(int X, int Y, int Z)
	{
		this->x = X;
		this->y = Y;
		this->z = Z;
	}
};

class GOL3D {
protected:
	bool _locked;

	void _drawCube();
	float _ranf();
	bool _ranb(float level);
	char*** _field;

	void _lock();
	void _unlock();

	int _sizeX;
	int _sizeY;
	int _sizeZ;

	float _probability;

	bool _isAlive(Point3D p);

	char*** _newField();
	void _deleteField(char*** field);

	Point3D _neighbors[26];

public:
	GOL3D(int sizeX, int sizeY, int sizeZ);
	virtual ~GOL3D();

	void Clear();
	void Step();
	void Draw();

	void setProbability(float level);

	void DumpFild();
};

} /* namespace gliewe */
#endif /* GOL3D_H_ */
