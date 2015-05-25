#include "stdafx.h"
#include<stdio.h>
#include<math.h>
#include"Point3D.h"
#include"Point2d.h"
#include"IDList.h"
#include"PointTool.h"
#include"PolarList.h"
#include"IDSet.h"
#include"PCBCGSolver.h"
#include"Polyhedron.h"
// Main 
class Parameterization
{
public:
	Polyhedron *mymesh;//Ä£ÐÍ
	Parameterization();
	Parameterization(Polyhedron *mymesh)
	{
		mymesh->param();
		mymesh->writemesh("manneq1_convert.ply2");
		delete mymesh;
	}
	
};


