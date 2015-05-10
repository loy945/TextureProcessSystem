#include "StdAfx.h"
#include "SampleTexture.h"
#include <fstream>

SampleTexture::SampleTexture(void)
{
}


SampleTexture::~SampleTexture(void)
{
}
bool SampleTexture::initSampleTextureFromFile(const char * fileNameX,const char * fileNameY,const char * fileNameTRI)
{
	vector<double> xPos;
	vector<double> yPos;
	vector<double *> tri;
	double tempValue;
	ifstream fin;
	fin.open(fileNameX,ios::app);
	while(!fin.eof())
	{
		fin>>tempValue;
		xPos.push_back(tempValue);
	}
	fin.close();

	fin.open(fileNameY,ios::app);
	while(!fin.eof())
	{
		fin>>tempValue;
		yPos.push_back(tempValue);
	}
	fin.close();


	//读取Delaunay三角剖分数据
	
	fin.open(fileNameTRI,ios::app);
	while(!fin.eof())
	{
		double *  triIndex=new double [3];
		fin>>triIndex[0]>>triIndex[1]>>triIndex[2];

		tri.push_back(triIndex);
	}
	fin.close();

	double width=0.1;
	double hight=0.1;
	for(int i=0;i<xPos.size();i++)
	{
		TextureElement * tex=new TextureElement(i);
		tex->textureIndex=i+1;
		tex->pos[0] = xPos.at(i);
		tex->pos[1] = yPos.at(i);
		tex->pos[2] = 0;

		this->tes.push_back(tex);
	}

	for(int i=0;i<tri.size();i++)
	{
		int index1=tri.at(i)[0]-1;
		int index2=tri.at(i)[1]-1;
		int index3=tri.at(i)[2]-1;
		this->addDelaunayTRI(tes.at(index1),tes.at(index2),tes.at(index3));
	}
	 //整理顺序 
   //textureSort();

 	return true;
}

TextureElement * SampleTexture::findFirstTextrueElement()
{
	int maxSize = 0;
	int mxSizeID = 0;
	for (int i = 0; i < tes.size(); i++)
	{
		if (tes[i]->link.size()>maxSize)
		{
			maxSize = tes[i]->link.size();
			mxSizeID = i;
		}
	}
	return tes[mxSizeID];
}
bool SampleTexture::addLink(TextureElement* te1,TextureElement * te2)
{
	if(te1->index==te2->index) return false;
	bool te1AlreadyIn=false;
	bool te2AlreadyIn=false;

	for (int i = 0; i<te1->link.size(); i++)
	{
		if (te1->link[i]->index == te2->index)
			te2AlreadyIn = true;
	}
	for (int i = 0; i<te2->link.size(); i++)
	{
		if (te2->link[i]->index == te1->index)
			te1AlreadyIn = true;
	}
	
	double dx=te2->pos[0]-te1->pos[0];
	double dy=te2->pos[1]-te1->pos[1];
	//double dz=te2->centre->z-te1->centre->z;
	//只考虑2D情况 计算长度 和象限 
	double dis1=sqrt(dx*dx+dy*dy);
	double dis2=sqrt(dx*dx+dy*dy);

	double angle1=atan2f(dy,dx)/3.1415926*180;//atan -180<=angle<=180
	double angle2=atan2f(-dy,-dx)/3.1415926*180;//atan -180<=angle<=180
	


	if(angle1<0)
	{
		angle1+=360;
	}
	if(angle2<0)
	{
		angle2+=360;
	}
	
	if(!te2AlreadyIn)
	{
		LinkData * ld1=new LinkData();
		ld1->index=te2->index;
		ld1->linkElement=te2;
		ld1->angle=angle1;
		ld1->distance=dis1;

		te1->addLinkNode(ld1);
	}
	if(!te1AlreadyIn)
	{
		LinkData * ld2=new LinkData();
		ld2->index=te1->index;
		ld2->linkElement=te1;
		ld2->angle=angle2;
		ld2->distance=dis2;

		te2->addLinkNode(ld2);
	}
	return true;
}
void SampleTexture::addDelaunayTRI(TextureElement *te1,TextureElement * te2,TextureElement * te3)
{
	this->addLink(te1,te2);
	this->addLink(te2,te3);
	this->addLink(te3,te1);

}