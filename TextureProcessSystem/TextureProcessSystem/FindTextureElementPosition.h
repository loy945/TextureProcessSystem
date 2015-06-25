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
	void buildTargetTextureElement(TextureElement * centerTE, vector<int> &matchF,vector<vector<int> >  &linkPairs);
	void addlinks(TextureElement * centerTE, vector<int> &matchF,vector<vector<int> >  &linkPairs);//������������λ�ö��ѹ̶��Ļ�Ԫ���������ж��α�
	TextureElement * theNearsetTE();
	void buildByStep();
	void amend();
	void amendTargetTE(TextureElement * te);//������ǰ��Ԫ��link����
	void detectCross1(TextureElement * te);//���1�ͱ߳�ͻ,��teΪ���ĵ�һȦ��Ԫ����һ�ͱ��н������Ӧ��Ԫֹͣ����
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

