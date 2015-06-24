#include "StdAfx.h"
#include "TargetTexture.h"
#include "config.h"

TargetTexture::TargetTexture(void)
{
}


TargetTexture::~TargetTexture(void)
{
}
bool TargetTexture::addLink(TextureElement *te1, TextureElement *te2)
{
	if (te1->index == te2->index) return false;
	bool te1AlreadyIn = false;
	bool te2AlreadyIn = false;

	for (int i = 0; i < te1->link.size(); i++)
	{
		if (te1->link[i]->index == te2->index)
			te2AlreadyIn = true;
	}
	for (int i = 0; i < te2->link.size(); i++)
	{
		if (te2->link[i]->index == te1->index)
			te1AlreadyIn = true;
	}
	/*
	double dx=te2.pos[0]-te1.pos[0];
	double dy=te2.pos[1]-te1.pos[1];
	//double dz=te2->centre->z-te1->centre->z;
	//����ֻ����2D��� ���㳤�� ������
	double len=sqrt(dx*dx+dy*dy);
	double angle1=atan2f(dy,dx)/3.1415926*180;//atan -180<=angle<=180
	double angle2=atan2f(-dy,-dx)/3.1415926*180;//atan -180<=angle<=180
	*/
	//
	double dis1 = te1->getDisFrom(te2->pos)*lenFactor;
	double dis2 = te2->getDisFrom(te1->pos)*lenFactor;
	//ͶӰ������ƽ��
	double angle1 = te1->getAngleFrom(te2->pos);
	double angle2 = te2->getAngleFrom(te1->pos);
	//ͶӰ������ƽ��
	//double angle1=te1->getAngleFrom(te2->pos,te2->n);
	//double angle2=te2->getAngleFrom(te1->pos,te1->n);



	if (!te2AlreadyIn)
	{
		LinkData * ld1 = new LinkData();
		ld1->index = te2->index;
		ld1->linkElement = te2;
		ld1->angle = angle1;
		ld1->distance = dis1;
		te1->addLinkNode(ld1);
	}
	if (!te1AlreadyIn)
	{
		LinkData * ld2 = new LinkData();
		ld2->index = te1->index;
		ld2->linkElement = te1;
		ld2->angle = angle2;
		ld2->distance = dis2;
		te2->addLinkNode(ld2);
	}
	//�������

	if (!te2AlreadyIn)
	{
	Line l1;
	l1.start.x = te1->pos[0];
	l1.start.y = te1->pos[1];
	l1.start.z = te1->pos[2];
	l1.end.x = te2->pos[0];
	l1.end.y = te2->pos[1];
	l1.end.z = te2->pos[2];
	l1.startElement = te1;
	l1.endElement = te2;
	lines.push_back(l1);
	}
	return true;
}