#pragma once
#include "StdAfx.h"
#include "LocalParameterization.h"
#include "stdafx.h"
#include"Point2d.h"
#include"IDList.h"
#include"PointTool.h"
#include"PolarList.h"
#include"IDSet.h"
#include"PCBCGSolver.h"
#include"Polyhedron.h"
#include"Parameterization.h"

LocalParameterization::LocalParameterization()
{
}


LocalParameterization::~LocalParameterization()
{
}
void LocalParameterization::init(Model_PLY * ply, vector<int> faceIndexs)
{
	//����ӳ���ϵ
	vvp = new vector<VertexPair *>;
	int index = 0;
	for (int  i = 0; i < ply->pointArry.size(); i++)
	{
		bool findFace = false;
		if (!findFace)
		{
			for (int j = 0; j < ply->pointArry[i].beLongToFaceIndex.size(); j++)
			{
				if (!findFace)
				{
					for (int k = 0; k < faceIndexs.size(); k++)
					{
						if (ply->pointArry[i].beLongToFaceIndex[j] == faceIndexs[k])
						{
							VertexPair * vp = new VertexPair();
							vp->index1 = ply->pointArry[i].pointNum;
							vp->index2 = index;
							index++;
							vvp->push_back(vp);
							findFace = true;
							break;
						}
					}
				}
			}
		}
		
	}
	//
	face_Parameterization(ply, faceIndexs);
	return;
}
void LocalParameterization::face_Parameterization(Model_PLY * ply, vector<int> faceIndexs)
{
	Polyhedron *mymesh = new Polyhedron();
	int dV = 0;
	int dF = 0;
	int i, j;
	int di = 0;
	int dj = 0;
	int dk = 0;
	double dx = 0.0;
	double dy = 0.0;
	double dz = 0.0;
	int ptNum = 0;
	int index = 0;
	int ptnum1 = 0;
	int ptnum2 = 0;
	int ptnum3 = 0;
	dV = vvp->size();
	dF = faceIndexs.size();
	mymesh->memoryallocate(dV, dF);
	for (i = 0; i<mymesh->numberV; i++){
		ptNum = find1by2(i);
		dx = ply->pointArry[ptNum].x;
		dy = ply->pointArry[ptNum].y;
		dz = ply->pointArry[ptNum].z;
		mymesh->setPoint(i, dx, dy, dz);
	}
	int val = 3;
	for (i = 0; i<mymesh->numberF; i++){
		index = faceIndexs[i];
		ptnum1 = ply->faceArry[index].ptnum[0];
		ptnum2 = ply->faceArry[index].ptnum[1];
		ptnum3 = ply->faceArry[index].ptnum[2];
		di = find2by1(ptnum1);
		dj = find2by1(ptnum2);
		dk = find2by1(ptnum3);
		mymesh->setFace(i, di, dj, dk);
		mymesh->IDtool->AppendVFSort(i, mymesh->FHead[mymesh->Face[i][0]], mymesh->FTail[mymesh->Face[i][0]]);
		mymesh->IDtool->AppendVFSort(i, mymesh->FHead[mymesh->Face[i][1]], mymesh->FTail[mymesh->Face[i][1]]);
		mymesh->IDtool->AppendVFSort(i, mymesh->FHead[mymesh->Face[i][2]], mymesh->FTail[mymesh->Face[i][2]]);
	}

	FILE *out = fopen("pro-convert.ply2", "w");
	fprintf(out, "%d\n", mymesh->numberV);
	fprintf(out, "%d\n", mymesh->numberF);
	for (i = 0; i<mymesh->numberV; i++){
		fprintf(out, "%lf %lf %lf\n", mymesh->point[i]->x, mymesh->point[i]->y, mymesh->point[i]->z);
	}
	for (i = 0; i<mymesh->numberF; i++)
		fprintf(out, "3 %d %d %d\n", mymesh->Face[i][0], mymesh->Face[i][1], mymesh->Face[i][2]);
	fclose(out);

	/* feature analysis */
	mymesh->SetBoundaryLines();
	mymesh->setAreaMap3D();
	mymesh->param();
	mymesh->writemesh("after-convert.ply2");
	for (i = 0; i<mymesh->numberV; i++)
	{
		ply->pointArry[find1by2(i)].u = mymesh->pU[i];
		ply->pointArry[find1by2(i)].v = mymesh->pV[i];
	}
	

}
int LocalParameterization::find1by2(int index2)
{
	for (int i = 0; i < vvp->size(); i++)
	{
		if (vvp->at(i)->index2 == index2)
			return vvp->at(i)->index1;
	}
}
int LocalParameterization::find2by1(int index1)
{
	for (int i = 0; i < vvp->size(); i++)
	{
		if (vvp->at(i)->index1 == index1)
			return vvp->at(i)->index2;
	}
}