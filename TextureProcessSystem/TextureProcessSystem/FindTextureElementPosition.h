#pragma once
#include "TextureProcessSystemDoc.h"
#include "TargetTexture.h"
#include "SampleTexture.h"
#include "Point3D.h"
#include "plyloader.h"
class FindTextureElementPosition
{
public:
	FindTextureElementPosition(void);
	~FindTextureElementPosition(void);
	void init(CTextureProcessSystemDoc * pDoc);
	void addTargetTE(gl_face * face,int textureIndex);
	void getPoint(double dis, double angel, TextureElement * centerTE, vector<Point3D> P, Point3D &res);
	double getPointError(double dis, double angel, TextureElement * centerTE, Point3D P);
	vector<Point3D> addPointToSelectList(vector<gl_face *> F);
	void buildTargetTextureElement(TextureElement * centerTE);
	void addlinks(TextureElement * centerTE);//对于所有种子位置都已固定的基元，补齐所有二形边
	TextureElement * theNearsetTE();
	void buildByStep();
	void amend();
public:
	SampleTexture * m_sampleTexture;
	TargetTexture * m_targetTexture;
	TextureElement * smapleCenter;
	TextureElement * targetCenter;
public:
	CTextureProcessSystemDoc * pDoc;
	vector<Point3D> P;
	Point3D firstPoint;
	vector<int> latestAddIn;
public:
	void draw();
};

