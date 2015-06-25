#include "StdAfx.h"
#include "FindTextureElementPosition.h"
#include "cv.h"
#include "highgui.h"
#include <opencv2/opencv.hpp> 
#include "config.h"
#include "VectorIndexPair.h"
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

	//添加第一个target基元
	/*pDoc->plyLoader.faceArry.at(pDoc->userSelectingTriangleIndex).isShowColorIn3D=true;
	pDoc->plyLoader.faceArry.at(pDoc->userSelectingTriangleIndex).textureclick=true;*/
	pDoc->plyLoader.faceArry.at(pDoc->userSelectingTriangleIndex).r=1;
	pDoc->plyLoader.faceArry.at(pDoc->userSelectingTriangleIndex).g=0;
	pDoc->plyLoader.faceArry.at(pDoc->userSelectingTriangleIndex).b=0;
	
	//计算第一个基元位置
	Point3D p1, p2, p3,pCore;//三角形三个顶点
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
	//补全连通信息
	vector<int> matchF;
	vector<vector<int> >  linkPairs;
	for (int i = 0; i<targetCenter->link.size(); i++)
	{
		vector<int> a;
		a.push_back(i);
		a.push_back(i+1);
		matchF.push_back(i);
		linkPairs.push_back(a);
	}
	addlinks(targetCenter, matchF, linkPairs);

	//统计个数
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
	double k1 = 50;
	double k2 = 1;
	double distanceError;
	float pos[3];
	pos[0] = p.x;
	pos[1] = p.y;
	pos[2] = p.z;
	float disfromCenter = centerTE->getDisFrom(pos);
	distanceError = abs(dis - (disfromCenter * lenFactor));

	double angleError;
	//投影到中心平面求角度
	float angleFromCenter = centerTE->getAngleFrom(pos);
	//投影到折中平面求角度
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
		//三角形面片的三个点
		for (int k = 0; k < 3; k++)
		{
			pt[k].x = pDoc->plyLoader.pointArry.at(f->ptnum[k]).x;
			pt[k].y = pDoc->plyLoader.pointArry.at(f->ptnum[k]).y;
			pt[k].z = pDoc->plyLoader.pointArry.at(f->ptnum[k]).z;
		}
		//面片的重心
		pt[3] = (pt[0] + pt[1] + pt[2]) / 3.0;
		//边中点
		pt[4] = (pt[0] + pt[1]) / 2.0;
		pt[5] = (pt[1] + pt[2]) / 2.0;
		pt[6] = (pt[2] + pt[0]) / 2.0;
		//顶点与重心的连线中心点
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
	Point3D p1, p2, p3,pCore;//三角形三个顶点
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

	glEnd(); // 四边形绘制结束
	//画线
	glColor3f(1, 1, 1);
	glBegin(GL_LINES);
	for (i = 0; i < m_targetTexture->lines.size(); i++)
	{
		if (m_targetTexture->lines[i].startElement->isShow&&m_targetTexture->lines[i].endElement->isShow)
		{
		glVertex3f(m_targetTexture->lines[i].start.x, m_targetTexture->lines[i].start.y, m_targetTexture->lines[i].start.z);
		glVertex3f(m_targetTexture->lines[i].end.x, m_targetTexture->lines[i].end.y, m_targetTexture->lines[i].end.z);
		}
	}
	glEnd();
	glFlush();//立即渲染
}
void FindTextureElementPosition::amendTargetTE(TextureElement * te)
{
	for (int i = 0; i < te->link.size(); i++)
	{
		te->link[i]->angle = te->getAngleFrom(te->link[i]->linkElement->pos);
		te->link[i]->distance = te->getDisFrom(te->link[i]->linkElement->pos);
	}
}
void FindTextureElementPosition::buildTargetTextureElement(TextureElement * centerTE, vector<int> &matchF,vector<vector<int> >  &linkPairs)
{
	targetCenter = centerTE;
	//修正当前基元的link数据
	amendTargetTE(centerTE);
	latestAddIn.clear();
	
	gl_face * theFace;
	TextureElement * p_OMTE;
	//分布最匹配的样本基元
	p_OMTE = m_sampleTexture->findOptimalMatchTextureElement(centerTE, matchF, linkPairs);

	//顶点候选集
	P.clear();
	pDoc->userSelectingTriangleIndex = centerTE->face->facenum;
	P = addPointToSelectList(pDoc->markConnectedFace(0, 0, radius));
	//开始添加
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
		vector<int> linkPair;
		linkPair.push_back(matchF[i]);
		linkPair.push_back(tex->index);
		linkPairs.push_back(linkPair);
		
	}
	
}
void FindTextureElementPosition::addlinks(TextureElement * centerTE, vector<int> &matchF,vector<vector<int> >  &linkPairs)
{
	int i = 0, j = 0,k = 0;
	/*for (i = 0; i < centerTE->link.size(); i++)
	{
		j = i + 1;
		if (j >= centerTE->link.size())
		{LINK
			j = 0;
		}
		if (centerTE->link[i]->linkElement->isShow&&centerTE->link[j]->linkElement->isShow)
		{
			m_targetTexture->addLink(centerTE->link[i]->linkElement, centerTE->link[j]->linkElement);
		}
		
	}*/
	int index = 0;
	int pindex = 0;
	int aindex = 0;
	for (i = 0; i < matchF.size(); i++)
	{
		j = matchF[i] - 1;
		if (j < 0)
		{
			j = linkPairs.size() - 1;
		}
		k = matchF[i] + 1;
		if (k>linkPairs.size() - 1)
		{
			k = 0;
		}
		VectorIndexPair(linkPairs, matchF[i], 2, index);
		VectorIndexPair(linkPairs, j, 2, pindex);
		VectorIndexPair(linkPairs, k, 2, aindex);

 		m_targetTexture->addLink(m_targetTexture->tes[index], m_targetTexture->tes[pindex]);
		m_targetTexture->addLink(m_targetTexture->tes[index], m_targetTexture->tes[aindex]);

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
	vector<int> matchF;
	vector<vector<int> >  linkPairs;
	buildTargetTextureElement(nexte, matchF, linkPairs);
	//nexte->textureElementSort();
	//amend();
	addlinks(targetCenter, matchF, linkPairs);
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

void FindTextureElementPosition::detectCross1(TextureElement * te)
{
	int i, j;

	i = 0;
	for (j = 0; j<targetCenter->link.size(); j++)
	{
		//中心te为起点，第j个邻域基元tte为终点，
		LinkData * cn = targetCenter->link.at(j);
		TextureElement * tte = cn->linkElement;
		Line l1;
		l1.startElement = targetCenter;
		l1.endElement = tte;
		l1.start = targetCenter->pos;
		l1.end = tte->pos;
		//与所有line求交点
		for (i = 0; i<m_targetTexture->lines.size(); i++)
		{
			if (l1.isCross(m_targetTexture->lines.at(i)))
			{
				bool deleteConnection = true;
				Line l2 = m_targetTexture->lines.at(i);
				for (int k = 0; k<targetCenter->link.size(); k++)
				{
					LinkData * cnk = targetCenter->link.at(k);
					TextureElement * ttek = cn->linkElement;
					if (l2.isContains(ttek))
					{
						deleteConnection = false;
						break;
					}

				}
				if (deleteConnection)
				{
					//若存在交叉，则对应基元不再扩张。
					tte->isfixed = true;
				/*	bool isFixed = true;
					for (vector<TextureElement *>::iterator it = m_targetTexture->recentlyAdd.begin(); it != m_targetTexture->recentlyAdd.end();)   //for循环中不要it++
					{
						if (*it == tte)
						{
							isFixed = false;//新加入的
						}
						it++;

					}

					if (isFixed)
					{
						m_targetTexture->deleteConnectRation(tte->index, targetCenter->index);
						m_targetTexture->deleteLines(tte, targetCenter);
					}
					else
					{
						m_targetTexture->deleteNode(tte->index);
					}
				
					detectCross1();
					return;*/
				}
			}
		}

		/*m_targetTexture->addLines(l1);*/
	}
	return;
}