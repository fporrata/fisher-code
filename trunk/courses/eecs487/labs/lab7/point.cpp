
#include "point.h"

Vec const Vec::X(1,0,0);
Vec const Vec::Y(0,1,0);
Vec const Vec::Z(0,0,1);


Pt const Pt::ORIGIN(0,0,0);

ostream & operator << (ostream & stream, Ray ray)
{
		stream << "* Ray info: \n";
		stream << "** Point: " << ray.p.x << " " << ray.p.y << " " << ray.p.z << endl;
		stream << "** Dir: 	 " << ray.v.x << " " << ray.v.y << " " << ray.v.z << endl;
		
		return stream;
}
