#pragma once
#include"stdafx.h"
#include "VertexPair.h"
#include "Point3D.h"
#include "plyloader.h"
class Polyhedron;

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
	vector<VertexPair* > * vfp;
	void init(Model_PLY * ply, vector<int> faceIndexs);
	double localPara(Model_PLY * ply, vector<int> faceIndexs, int indexCenter, Point3D * offset, float scale);
	void face_Parameterization(Model_PLY * ply, vector<int> faceIndexs);
	int find1by2(int index2, vector<VertexPair*> *v);
	int find2by1(int index1, vector<VertexPair*> *v);
	void updateTextureCoord();//����������Ϣ
public:
	Model_PLY * m_ply;
	int m_indexCenter;
	Point3D * m_2DOffset;//ָ�������������ڵ����λ��
	float m_scale;
	Polyhedron *mymesh;//չ��ģ����������
	int m_faceNums;//ѡ��չ����Ƭ����
	vector<bool> effectFaceVector;
};

