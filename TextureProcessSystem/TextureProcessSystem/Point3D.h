#pragma once
#include <math.h>
class Point3D
{
public:
	Point3D(void);
	~Point3D(void);
	Point3D(float x, float y, float z){ this->x = x; this->y = y; this->z = z; beLongFaceID = 0; }
	void setValue(float x, float y, float z){ this->x = x; this->y = y; this->z = z; beLongFaceID = 0; }
	void setValue(Point3D &point){ this->x = point.x; this->y = point.y; this->z = point.z; this->beLongFaceID = point.beLongFaceID; }
	double getDistance(){
		return sqrt(this->x*this->x+this->y*this->y+this->z*this->z);
	
	}
	friend Point3D  operator - (Point3D &pt1,Point3D &pt2)
	{
	
		Point3D pt;
		pt.x=pt1.x-pt2.x;
		pt.y=pt1.y-pt2.y;
		pt.z=pt1.z-pt2.z;
		return pt;
	}
	
		friend Point3D  operator + (Point3D &pt1,Point3D &pt2)
	{
	
		Point3D pt;
		pt.x=pt1.x+pt2.x;
		pt.y=pt1.y+pt2.y;
		pt.z=pt1.z+pt2.z;
		return pt;
	}
	friend Point3D  operator * (Point3D &pt1, float k)
	{
		Point3D pt;
		pt.x = pt1.x * k;
		pt.y = pt1.y * k;
		pt.z = pt1.z * k;
		return pt;
	}
	friend Point3D  operator / (Point3D &pt1, float k)
	{
		Point3D pt;
		pt.x = pt1.x / k;
		pt.y = pt1.y / k;
		pt.z = pt1.z / k;
		return pt;
	}

	bool operator == (const Point3D & pt ) const
	{
		if((pow((this->x-pt.x),2)<0.01)&&(pow((this->y-pt.y),2)<0.01)&&(pow((this->z-pt.z),2)<0.01))
		{
			return true;
		}
		return false;
		
	}
	float x;
	float y;
	float z;
	int beLongFaceID;

};

