#include "StdAfx.h"
#include "FindTextureElementPosition.h"
#include "cv.h"
#include "highgui.h"
#include <opencv2/opencv.hpp> 
#include "config.h"
#define pi 3.1415926
using namespace cv;

FindTextureElementPosition::FindTextureElementPosition(void)
{
}


FindTextureElementPosition::~FindTextureElementPosition(void)
{
}

void FindTextureElementPosition::init(CTextureProcessSystemDoc * pDoc)
{
	this->pDoc=pDoc;
	gl_face * theFace;
	m_sampleTexture=new SampleTexture();
	m_sampleTexture->initSampleTextureFromFile("x.txt","y.txt","TRI.txt");
	m_sampleTexture->textureSort();
	m_targetTexture=new TargetTexture();

	//��ӵ�һ��target��Ԫ
	/*pDoc->plyLoader.faceArry.at(pDoc->userSelectingTriangleIndex).isShowColorIn3D=true;
	pDoc->plyLoader.faceArry.at(pDoc->userSelectingTriangleIndex).textureclick=true;*/
	pDoc->plyLoader.faceArry.at(pDoc->userSelectingTriangleIndex).r=1;
	pDoc->plyLoader.faceArry.at(pDoc->userSelectingTriangleIndex).g=0;
	pDoc->plyLoader.faceArry.at(pDoc->userSelectingTriangleIndex).b=0;
	
	//�����һ����Ԫλ��
	Point3D p1, p2, p3,pCore;//��������������
	p1.x = pDoc->plyLoader.pointArry[pDoc->plyLoader.faceArry.at(pDoc->userSelectingTriangleIndex).ptnum[0]].x;
	p1.y = pDoc->plyLoader.pointArry[pDoc->plyLoader.faceArry.at(pDoc->userSelectingTriangleIndex).ptnum[0]].y;
	p1.z = pDoc->plyLoader.pointArry[pDoc->plyLoader.faceArry.at(pDoc->userSelectingTriangleIndex).ptnum[0]].z;
	p2.x = pDoc->plyLoader.pointArry[pDoc->plyLoader.faceArry.at(pDoc->userSelectingTriangleIndex).ptnum[1]].x;
	p2.y = pDoc->plyLoader.pointArry[pDoc->plyLoader.faceArry.at(pDoc->userSelectingTriangleIndex).ptnum[1]].y;
	p2.z = pDoc->plyLoader.pointArry[pDoc->plyLoader.faceArry.at(pDoc->userSelectingTriangleIndex).ptnum[1]].z;
	p3.x = pDoc->plyLoader.pointArry[pDoc->plyLoader.faceArry.at(pDoc->userSelectingTriangleIndex).ptnum[2]].x;
	p3.y = pDoc->plyLoader.pointArry[pDoc->plyLoader.faceArry.at(pDoc->userSelectingTriangleIndex).ptnum[2]].y;
	p3.z = pDoc->plyLoader.pointArry[pDoc->plyLoader.faceArry.at(pDoc->userSelectingTriangleIndex).ptnum[2]].z;
	pCore = (p1 + p2 + p3) / 3.0;

	float corx = pDoc->plyLoader.faceArry.at(pDoc->userSelectingTriangleIndex).corex;
	float cory = pDoc->plyLoader.faceArry.at(pDoc->userSelectingTriangleIndex).corey;
	float corz = pDoc->plyLoader.faceArry.at(pDoc->userSelectingTriangleIndex).corez;
	firstPoint.x = corx;
	firstPoint.y = cory;
	firstPoint.z = corz;
	//

 	TextureElement * te = new TextureElement(0, &(pDoc->plyLoader.faceArry.at(pDoc->userSelectingTriangleIndex)), 1);
	m_targetTexture->addTE(te);
	targetCenter=te;
	smapleCenter=m_sampleTexture->findFirstTextrueElement();
	//***************************
	P = addPointToSelectList(pDoc->markConnectedFace(0, 0, radius));
	for(int i=0;i<smapleCenter->link.size();i++)
	{
		LinkData *  ld=smapleCenter->link.at(i);
		//theFace=findFace(ld->distance,ld->angle,te,pDoc->markConnectedFace(0,0,1000));
		Point3D pt(0,0,0);
		getPoint(ld->distance, ld->angle, te, P, pt);
		theFace = &pDoc->plyLoader.faceArry[pt.beLongFaceID];

		TextureElement * tex = new TextureElement(m_targetTexture->tes.size(), theFace,1,pt);
		m_targetTexture->addTE(tex);
		m_targetTexture->addLink(targetCenter,tex);
	}
	//��ȫ��ͨ��Ϣ
	addlinks(targetCenter);

	//ͳ�Ƹ���
	int w=0;
	if(m_targetTexture->tes.size()>0)
	{
		w++;
	}
	m_targetTexture->textureSort();
	
	return;


}



void FindTextureElementPosition::getPoint(double dis, double angel, TextureElement * centerTE, vector<Point3D> P,Point3D &res)
{
	double error = 999;
	double minerror = 999;
	

	for (int i = 0; i < P.size();i++)
	{	
		error = getPointError(dis, angel, centerTE, P[i]);
		if (minerror>error)
		{
			minerror = error;
			res.setValue(P[i]);
		}
	}

	return ;
}
double FindTextureElementPosition::getPointError(double dis, double angel, TextureElement * centerTE, Point3D p)
{
	double k1 = 100;
	double k2 = 1;
	double distanceError;
	float pos[3];
	pos[0] = p.x;
	pos[1] = p.y;
	pos[2] = p.z;
	float disfromCenter = centerTE->getDisFrom(pos);
	distanceError = abs(dis - (disfromCenter * lenFactor));

	double angleError;
	//ͶӰ������ƽ����Ƕ�
	float angleFromCenter = centerTE->getAngleFrom(pos);
	//ͶӰ������ƽ����Ƕ�
	//float angleFromCenter = centerTE->getAngleFrom(pos,centerTE->n);

	angleError = abs(angel - angleFromCenter);
	if (angleError > 180)
	{
		angleError = 360 - angleError;
	}
	double error = k1*distanceError + k2*angleError;
	return error;
}
vector<Point3D> FindTextureElementPosition::addPointToSelectList(vector<gl_face *> F)
{
	Point3D pt[10];
	gl_face * f;
	vector<Point3D> res;
	for (int i = 0; i < F.size(); i++)
	{
		 f = F[i];
		//��������Ƭ��������
		for (int k = 0; k < 3; k++)
		{
			pt[k].x = pDoc->plyLoader.pointArry.at(f->ptnum[k]).x;
			pt[k].y = pDoc->plyLoader.pointArry.at(f->ptnum[k]).y;
			pt[k].z = pDoc->plyLoader.pointArry.at(f->ptnum[k]).z;
		}
		//��Ƭ������
		pt[3] = (pt[0] + pt[1] + pt[2]) / 3.0;
		//���е�
		pt[4] = (pt[0] + pt[1]) / 2.0;
		pt[5] = (pt[1] + pt[2]) / 2.0;
		pt[6] = (pt[2] + pt[0]) / 2.0;
		//���������ĵ��������ĵ�
		pt[7] = (pt[3] + pt[0]) / 2.0;
		pt[8] = (pt[3] + pt[1]) / 2.0;
		pt[9] = (pt[3] + pt[2]) / 2.0;

		for (int k = 0; k < 10; k++)
		{
			pt[k].beLongFaceID = f->facenum;
			res.push_back(pt[k]);
		}



	}
	return res;
}
void FindTextureElementPosition::draw()
{
	float w = 0.003, h = 0.003, d = 0.003;
	float x, y, z = 0;
	int i = 0, j = 0;

	glColor3f(1, 0, 1);
	glBegin(GL_TRIANGLES);
	Point3D p1, p2, p3,pCore;//��������������
	p1.x = pDoc->plyLoader.pointArry[pDoc->plyLoader.faceArry.at(pDoc->userSelectingTriangleIndex).ptnum[0]].x;
	p1.y = pDoc->plyLoader.pointArry[pDoc->plyLoader.faceArry.at(pDoc->userSelectingTriangleIndex).ptnum[0]].y;
	p1.z = pDoc->plyLoader.pointArry[pDoc->plyLoader.faceArry.at(pDoc->userSelectingTriangleIndex).ptnum[0]].z;
	p2.x = pDoc->plyLoader.pointArry[pDoc->plyLoader.faceArry.at(pDoc->userSelectingTriangleIndex).ptnum[1]].x;
	p2.y = pDoc->plyLoader.pointArry[pDoc->plyLoader.faceArry.at(pDoc->userSelectingTriangleIndex).ptnum[1]].y;
	p2.z = pDoc->plyLoader.pointArry[pDoc->plyLoader.faceArry.at(pDoc->userSelectingTriangleIndex).ptnum[1]].z;
	p3.x = pDoc->plyLoader.pointArry[pDoc->plyLoader.faceArry.at(pDoc->userSelectingTriangleIndex).ptnum[2]].x;
	p3.y = pDoc->plyLoader.pointArry[pDoc->plyLoader.faceArry.at(pDoc->userSelectingTriangleIndex).ptnum[2]].y;
	p3.z = pDoc->plyLoader.pointArry[pDoc->plyLoader.faceArry.at(pDoc->userSelectingTriangleIndex).ptnum[2]].z;
	glVertex3f(p1.x, p1.y, p1.z);
	glVertex3f(p2.x, p2.y, p2.z);
	glVertex3f(p3.x, p3.y, p3.z);
	glEnd();




	glColor3f(0, 0, 1);
	glBegin(GL_QUADS);

	for (i = 0; i < m_targetTexture->tes.size(); i++)
	{
		if (!m_targetTexture->tes[i]->isShow)
			continue;

		if (m_targetTexture->tes[i]->index == targetCenter->index)
		{
			glColor3f(1, 1, 1);
		}
		else if (m_targetTexture->tes[i]->isfixed)
		{
			glColor3f(0, 1, 0);
		}
		else 
		{
			bool latestaddin = false;
			for (j = 0; j < latestAddIn.size(); j++)
			{
				if (latestAddIn[j] == m_targetTexture->tes[i]->index)
				{
					latestaddin = true;
				}
			}
			if (latestaddin)
			{
				glColor3f(1, 0, 0);
			}
			else
			{
				glColor3f(0, 0, 1);
			}
			
		}
		x = m_targetTexture->tes[i]->pos[0];
		y = m_targetTexture->tes[i]->pos[1];
		z = m_targetTexture->tes[i]->pos[2];

	
		//front
		glVertex3f(x - w, y + h, z + d);
		glVertex3f(x - w, y - h, z + d);
		glVertex3f(x + w, y - h, z + d);
		glVertex3f(x + w, y + h, z + d);

		//bottom
		glVertex3f(x - w, y - h, z + d);
		glVertex3f(x + w, y - h, z + d);
		glVertex3f(x + w, y - h, z - d);
		glVertex3f(x - w, y - h, z - d);

		//back
		glVertex3f(x + w, y + h, z - d);
		glVertex3f(x + w, y - h, z - d);
		glVertex3f(x - w, y - h, z - d);
		glVertex3f(x - w, y + h, z - d);

		//top
		glVertex3f(x - w, y + h, z + d);
		glVertex3f(x + w, y + h, z + d);
		glVertex3f(x + w, y + h, z - d);
		glVertex3f(x - w, y + h, z - d);

		//left
		glVertex3f(x - w, y + h, z + d);
		glVertex3f(x - w, y - h, z + d);
		glVertex3f(x - w, y - h, z - d);
		glVertex3f(x - w, y + h, z - d);

		//right
		glVertex3f(x + w, y + h, z + d);
		glVertex3f(x + w, y - h, z + d);
		glVertex3f(x + w, y - h, z - d);
		glVertex3f(x + w, y + h, z - d);

	}
	glColor3f(1, 0, 1);
	w = 0.001, h = 0.001, d = 0.001;
	for (i = 0; i < P.size(); i++)
	{
		x = P[i].x;
		y = P[i].y;
		z = P[i].z;


		//front
		glVertex3f(x - w, y + h, z + d);
		glVertex3f(x - w, y - h, z + d);
		glVertex3f(x + w, y - h, z + d);
		glVertex3f(x + w, y + h, z + d);

		//bottom
		glVertex3f(x - w, y - h, z + d);
		glVertex3f(x + w, y - h, z + d);
		glVertex3f(x + w, y - h, z - d);
		glVertex3f(x - w, y - h, z - d);

		//back
		glVertex3f(x + w, y + h, z - d);
		glVertex3f(x + w, y - h, z - d);
		glVertex3f(x - w, y - h, z - d);
		glVertex3f(x - w, y + h, z - d);

		//top
		glVertex3f(x - w, y + h, z + d);
		glVertex3f(x + w, y + h, z + d);
		glVertex3f(x + w, y + h, z - d);
		glVertex3f(x - w, y + h, z - d);

		//left
		glVertex3f(x - w, y + h, z + d);
		glVertex3f(x - w, y - h, z + d);
		glVertex3f(x - w, y - h, z - d);
		glVertex3f(x - w, y + h, z - d);

		//right
		glVertex3f(x + w, y + h, z + d);
		glVertex3f(x + w, y - h, z + d);
		glVertex3f(x + w, y - h, z - d);
		glVertex3f(x + w, y + h, z - d);

	}

	glEnd(); // �ı��λ��ƽ���
	glFlush();//������Ⱦ
}
void FindTextureElementPosition::buildTargetTextureElement(TextureElement * centerTE)
{
	targetCenter = centerTE;
	latestAddIn.clear();
	vector<int> matchF;
	gl_face * theFace;
	TextureElement * p_OMTE;
	//�ֲ���ƥ���������Ԫ
	p_OMTE=m_sampleTexture->findOptimalMatchTextureElement(centerTE, matchF);
	//�����ѡ��
	P.clear();
	pDoc->userSelectingTriangleIndex = centerTE->face->facenum;
	P = addPointToSelectList(pDoc->markConnectedFace(0, 0, 50));
	//��ʼ���
	for (int i = 0; i < matchF.size(); i++)
	{
		LinkData *ld = p_OMTE->link[matchF[i]];
		Point3D pt(0, 0, 0);
		getPoint(ld->distance, ld->angle, centerTE, P, pt);
		theFace = &pDoc->plyLoader.faceArry[pt.beLongFaceID];

		TextureElement * tex = new TextureElement(m_targetTexture->tes.size(), theFace, 1, pt);
		m_targetTexture->addTE(tex);
		m_targetTexture->addLink(targetCenter, tex);
		latestAddIn.push_back(tex->index);
	}

}
void FindTextureElementPosition::addlinks(TextureElement * centerTE)
{
	centerTE->textureElementSort();
	int i = 0, j = 0;
	for (i = 0; i < centerTE->link.size(); i++)
	{
		j = i + 1;
		if (j >= centerTE->link.size())
		{
			j = 0;
		}
		m_targetTexture->addLink(centerTE->link[i]->linkElement, centerTE->link[j]->linkElement);
	}
	centerTE->isfixed = true;
}
TextureElement * FindTextureElementPosition::theNearsetTE()
{
	float distance = 9999, minDistance = 9999;
	TextureElement * res;
	for (int i = 0; i < m_targetTexture->tes.size(); i++)
	{
		if (!m_targetTexture->tes[i]->isfixed)
		{
			Point3D pt;
			pt .x= m_targetTexture->tes[i]->pos[0];
			pt. y= m_targetTexture->tes[i]->pos[1];
			pt. z= m_targetTexture->tes[i]->pos[2];
			distance = (pt - firstPoint).getDistance();
			if (distance < minDistance)
			{
				minDistance = distance;
				res = m_targetTexture->tes[i];
			}
		}
	}
	return res;
}
void FindTextureElementPosition::buildByStep()
{
	TextureElement * nexte = theNearsetTE();
	buildTargetTextureElement(nexte);
	nexte->textureElementSort();
	addlinks(targetCenter);
	amend();
	m_targetTexture->textureSort();
}
void FindTextureElementPosition::amend()
{
	Point3D pt1;
	Point3D pt2;
	for (int i = 0; i < latestAddIn.size(); i++)
	{
		int index = latestAddIn[i];
		TextureElement * te = m_targetTexture->tes[index];
		pt1.x = te->pos[0];
		pt1.y = te->pos[1];
		pt1.z = te->pos[2];
		for (int j = 0; j < m_targetTexture->tes.size(); j++)
		{
			if (m_targetTexture->tes[j]->index == te->index)
				continue;
			pt2.x = m_targetTexture->tes[j]->pos[0];
			pt2.y = m_targetTexture->tes[j]->pos[1];
			pt2.z = m_targetTexture->tes[j]->pos[2];
			Point3D pt3 = pt1 - pt2;
			float dis = pt3.getDistance();
			if (dis < minTEDistance)
			{
				te->isfixed = true;
				te->isShow = false;
			}
		}
	}

}