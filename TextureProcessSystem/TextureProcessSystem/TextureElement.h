#pragma once
#include <algorithm>
#include <vector>
#include "LinkData.h"
#include "plyloader.h"
#include "Point3D.h"
using namespace std;
class TextureElement
{
public:
	TextureElement(int index);
	TextureElement(int index, gl_face * face, int textureIndex);
	//TextureElement(int index, gl_face * face, int textureIndex, float position[]);
	TextureElement(int index, gl_face * face, int textureIndex, Point3D &pt);
	~TextureElement(void);
public:
	GLuint textureIndex;
	int index;
	float pos[3];
	vector <LinkData *> link;
	float n[3];//法线方向
	gl_face * face;
	bool isfixed;
	bool isShow;
public:
	bool init(gl_face * face);
	void addLinkNode(LinkData * node);
	void deleteNdoe(LinkData * node);
	//重载==运算，index是唯一标示符
	bool operator == (const int index)
	{
		if(this->index==index)
		{
			return true;
		}else
		{
			return false;
		}
	}
	double getDisFrom(float * position);
	double getAngleFrom(float * position);
	double getAngleFrom(float * position, float * n1);

	double vecTimes(float * n1,float * n2)//向量点乘
	{
		double sun=0;
		for(int i=0;i<3;i++)
		{
			sun+=n1[i]*n[2];
		}
		return sun;
	}
	void textureElementSort();
	void projection(float *n, float * position, float * planeCenterPos, float res[3]);
	

};

