#pragma once
#include "stdafx.h"
#include "Line.h"
#define MIN 0.00001

Line::Line(void)
{
}


Line::~Line(void)
{
}
bool Line::isCross(Line l)
{
	/*double k1=func(l.start);
	double k2=func(l.end);
	double k3=l.func(start);
	double k4=l.func(end);
	if((k1*k2<0)&&(k3*k4<0))
	{
		return true;//½»²æ
	}
	return false;*/

	Vector3f x1;
	Vector3f x2;
	Vector3f y1;
	Vector3f y2;

	x1(0) = this->start.x;
	x1(1) = this->start.y;
	x1(2) = this->start.z;
	x2(0) = this->end.x;
	x2(1) = this->end.y;
	x2(2) = this->end.z;

	y1(0) = l.start.x;
	y1(1) = l.start.y;
	y1(2) = l.start.z;
	y2(0) = l.end.x;
	y2(1) = l.end.y;
	y2(2) = l.end.z;

	return isCross(x1, x2, y1, y2);
}
double Line::func(Point3D pt)
{
	double x1=start.x;
	double y1=start.y;
	double x2=end.x;
	double y2=end.y;
	double x=pt.x;
	double y=pt.y;
	return ((y-y1)*(x2-x1)-(x-x1)*(y2-y1));
}

bool Line::operator == (Line l)
{
	//if(((this->start==l.start)&&(this->end==l.end))||((this->start==l.end)&&(this->end==l.start)))
	if(((this->startElement->index==l.startElement->index)&&(this->endElement->index==l.endElement->index))||(this->startElement->index==l.endElement->index)&&(this->endElement->index==l.startElement->index))
	{
		return true;
	}
	return false;
}

bool Line::isContains(Point3D point)
{
	if((this->start==point)||(this->end==point))
	{
		return true;
	}
	return false;
}

bool Line::isContains(TextureElement * te)
{
	if((this->startElement==te)||(this->endElement==te))
	{
		return true;
	}
	return false;
}
bool Line::isCross(Point3D pt1,Point3D pt2)
{
	Line l1;
	l1.start=pt1;
	l1.end=pt2;
	return this->isCross(l1);

}

bool Line::isCross(Point3D pt1, Point3D pt2, Point3D pt3, Point3D pt4)
{
	Line l1;
	l1.start=pt1;
	l1.end=pt2;
	return l1.isCross(pt3,pt4);
}
bool Line::isCross(Vector3f x1, Vector3f x2, Vector3f y1, Vector3f y2)
{
	int i = 0;
	Matrix3f k;
	if (isCrossAtBE(x1, x2, y1, y2))
	{
		return false;
	}

	for (i = 0; i < 3; i++)
	{
		k(i, 0) = x1(i) - x2(i);
		k(i, 1) = -(y1(i) - y2(i));
		k(i, 2) = x2(i) - y2(i);
	}
	FullPivLU<Matrix3f> lu_decomp(k);
	auto rank = lu_decomp.rank();
	if (rank == 3)
	{
		return false;
	}
	else
	{
		return true;
	}
}
bool Line::isCrossAtBE(Vector3f x1, Vector3f x2, Vector3f y1, Vector3f y2)
{
	if ((x1 - y1).norm() < MIN)
		return true;
	if ((x1 - y2).norm() < MIN)
		return true;
	if ((x2 - y1).norm() < MIN)
		return true;
	if ((x2 - y2).norm() < MIN)
		return true;
}