#pragma once
#include "Point3D.h"
#include "TextureElement.h"
class Line
{
public:
	Line(void);
	~Line(void);
	Point3D start;
	Point3D end;
	TextureElement * startElement;
	TextureElement * endElement;

public:
	bool isCross(Line l);
	bool isCross(Point3D pt1,Point3D pt2);
	bool isCross(Point3D pt1,Point3D pt2,Point3D pt3,Point3D pt4);
	double func(Point3D pt);
	bool operator == (Line l);
	bool isContains(Point3D point);
	bool isContains(TextureElement * te);

};

