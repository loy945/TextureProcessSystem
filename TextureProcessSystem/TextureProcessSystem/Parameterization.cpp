/*
A fast and simple stretch-minimizing mesh parameterization C++ code
Copyright:(c) Shin Yoshizawa, 2004
E-mail: shin.yoshizawa@mpi-sb.mpg.de
URL: http://www.mpi-sb.mpg.de/~shin
Affiliation: Max-Planck-Institut fuer Informatik: Computer Graphics Group 
 Stuhlsatzenhausweg 85, 66123 Saarbruecken, Germany
 Phone +49 681 9325-408 Fax +49 681 9325-499 

 All right is reserved by Shin Yoshizawa.
This C++ sources are allowed for only primary user of 
research and educational purposes. Don't use secondary: copy, distribution, 
diversion, business purpose, and etc.. 
 */
#include "stdafx.h"
#include"Point3D.h"
#include"Point2d.h"
#include"IDList.h"
#include"PointTool.h"
#include"PolarList.h"
#include"IDSet.h"
#include"PCBCGSolver.h"
#include"Polyhedron.h"
#include"Parameterization.h"
// Main 
/*int main(int argc,char *argv[]){
  
  if(argv[1]==NULL)return 0;
  Polyhedron *mymesh = new Polyhedron();
  mymesh->readmesh(argv[1]);
  mymesh->param();
  if(argv[2]==NULL){
    delete mymesh;
    return 0;
  }
  mymesh->writemesh(argv[2]);
  delete mymesh;
  return 0;
}*/
void Parameterization::setparameterization(char * inPLY2, char * outPLY2)
{
	Polyhedron *mymesh = new Polyhedron();
	mymesh->readmesh(inPLY2);
	

	mymesh->param();
	mymesh->writemesh(outPLY2);
}
void Parameterization::face_Parameterization(Model_PLY * ply, vector<int> faceIndexs)
{
	Polyhedron *mymesh = new Polyhedron();
	//获取选择面片的数据
	int i = 0;
	float dx, dy, dz;
	int di, dj, dk;
	int pointNum,faceNum;
	faceNum = faceIndexs.size();

	//传入数据
	//顶点
	for (i = 0; i<ply->pointArry.size(); i++)
	{
		for (int j = 0; j < faceNum; j++)
		{
			for (int k = 0; k < ply->pointArry[i].beLongToFaceIndex.size();k++)
			if (ply->pointArry[i].beLongToFaceIndex[k] == faceIndexs[j])
			{
				dx = ply->pointArry[i].x;
				dy = ply->pointArry[i].y;
				dz = ply->pointArry[i].z;
				mymesh->setPoint(i, dx, dy, dz);
			}
		}	
	}
	//面
	for (i = 0; i < faceNum; i++)
	{
		di = ply->faceArry[faceIndexs[i]].ptnum[0];
		dj = ply->faceArry[faceIndexs[i]].ptnum[1];
		dk = ply->faceArry[faceIndexs[i]].ptnum[2];
		mymesh->setFace(i, di, dj, dk);
		mymesh->IDtool->AppendVFSort(i, mymesh->FHead[mymesh->Face[i][0]], mymesh->FTail[mymesh->Face[i][0]]);
		mymesh->IDtool->AppendVFSort(i, mymesh->FHead[mymesh->Face[i][1]], mymesh->FTail[mymesh->Face[i][1]]);
		mymesh->IDtool->AppendVFSort(i, mymesh->FHead[mymesh->Face[i][2]], mymesh->FTail[mymesh->Face[i][2]]);
	}

	mymesh->param();
	mymesh->writemesh("my_result.ply2");
}