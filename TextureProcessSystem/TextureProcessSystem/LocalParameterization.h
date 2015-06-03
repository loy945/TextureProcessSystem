#pragma once
#include"stdafx.h"
#include "VertexPair.h"
#include "Point3D.h"
#include "plyloader.h"

//1.����һ���ֲ�ģ�ͣ�������Ŵ�0��ʼ��
//2.��һ��ӳ���ϵvertexPair,��ʾ�ֲ�ģ�ͺ�PLYģ�Ͷ�����Ŷ�Ӧ��ϵ
//3.����PLYģ���е������ι�ϵ(��Ӧ�������)������������Ƭ
class LocalParameterization
{
public:
	LocalParameterization();
	~LocalParameterization();
public:
	vector<VertexPair* > * vvp;
	void init(Model_PLY * ply, vector<int> faceIndexs);
	void face_Parameterization(Model_PLY * ply, vector<int> faceIndexs);
	int find1by2(int index2);
	int find2by1(int index1);
};

