#pragma once
#include"stdafx.h"
#include "VertexPair.h"
#include "Point3D.h"
#include "plyloader.h"

//1.建立一个局部模型，顶底序号从0开始，
//2.有一个映射关系vertexPair,表示局部模型和PLY模型顶点序号对应关系
//3.按照PLY模型中的三角形关系(对应顶点序号)建立三角形面片
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

