#pragma once
class Point3fv3
{
public:
	Point3fv3();
	~Point3fv3();
	float cor[3][3];
	bool is_valid(float cor[3][3]);
	bool update;
	void setValue(Point3fv3 & pt3fv3);
};

