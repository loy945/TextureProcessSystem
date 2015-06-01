#pragma once
#include "stdafx.h"
#include "Line.h"


Line::Line(void)
{
}


Line::~Line(void)
{
}
bool Line::isCross(Line l)
{
	double k1=func(l.start);
	double k2=func(l.end);
	double k3=l.func(start);
	double k4=l.func(end);
	if((k1*k2<0)&&(k3*k4<0))
	{
		return true;//½»²æ
	}
	return false;
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

bool isCross(Point3D pt1,Point3D pt2,Point3D pt3,Point3D pt4)
{
	Line l1;
	l1.start=pt1;
	l1.end=pt2;
	return l1.isCross(pt3,pt4);
}