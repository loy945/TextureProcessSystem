#pragma once
#include "Point3D.h"
#include <Eigen/Dense>  
#include <Eigen/LU>
using namespace Eigen;
using namespace Eigen::internal;
using namespace Eigen::Architecture;
class TriangleCoorTrans
{
public:
	TriangleCoorTrans();
	~TriangleCoorTrans();
	Point3D * m_centerPosXYCoord;
	Point3D * m_trianglePos[3];
	Point3D * m_centerPosUVCoord;

public:
	void init(Point3D ** trianglePos);
	//����������������ǰ�����ȳ�ʼ�������ζ���λ��
	Point3D * convertCoordXY2UV(Point3D * centerPosXYCoord);
	Point3D * convertCoordUV2XY(Point3D * centerPosUVCoord);

};

