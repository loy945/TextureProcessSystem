#pragma once
#include "TextureElement.h"
#include "IndexPair.h"
class Texture
{
public:
	Texture(void);
	~Texture(void);
	vector<TextureElement *> tes;

public:
	bool addTE(TextureElement * te);
	bool addLink(TextureElement * te1,TextureElement *te2);//���Ѿ����ڵ�te1 te2�����ͨ��ϵ
	double getError(LinkData * link1,LinkData * link2);
	void textureSort();
	bool isRingStruct(int index);
	int countSides(int index);
	bool isConnected(int index1,int index2);
	TextureElement * findOptimalMatchTextureElement(TextureElement *te,vector<int> &addList);
	double differentBetweenTwoTextureElement(TextureElement *te1,TextureElement *te2,vector<int>&addList);
	void addTEbyCenterTE(TextureElement * centerTE);
	
};

