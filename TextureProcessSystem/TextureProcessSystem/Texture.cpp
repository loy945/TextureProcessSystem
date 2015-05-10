#include "StdAfx.h"
#include "Texture.h"
Texture::Texture(void)
{
}


Texture::~Texture(void)
{
}
bool Texture::addTE(TextureElement * te)
{
	/*for (int i = 0; i < tes.size(); i++)
	{
		if (tes[i]->index == te->index)
			return false;
	}*/
	tes.push_back(te);
	return true;
	
}
bool Texture::isConnected(int index1,int index2)
{
	TextureElement * te=tes.at(index1);
	for(int i=0;i<te->link.size();i++)
	{
		LinkData * node=te->link[i];
		if(node->index==index2)
			return true;
	}
	return false;

}

bool Texture::addLink(TextureElement*te1,TextureElement *te2)
{
	return true;
}
int Texture::countSides(int index)
{
	int count=0;
	//检测邻域有多少条环形变，即关联节点之间存在多少关系
	TextureElement * te1=tes.at(index);

	int i = 0, j = 0;
	for(i;i<te1->link.size();i++)
	{
		j = i + 1;
		if (j >= te1->link.size())
		{
			j = 0;
		}
		int index1 = te1->link[i]->index;
		int index2 = te1->link[j]->index;
		bool islink = isConnected(index1, index2);
		if (islink)
		{
			count++;
		}
	}
	

	return count;
}
bool Texture::isRingStruct(int index)
{
	/*cout<<"lines:"<<this->countSides(index)<<endl;
	cout<<"node:"<<this->tes.at(index)->link.size()<<endl;*/
	bool res=false;
	res=(this->countSides(index)==tes.at(index)->link.size());
	 //cout<<"node"<<index+1<<(res?"  true":" false")<<endl;
	 return res;
}
double Texture::getError(LinkData *  link1,LinkData * link2)
{
	double len1=link1->distance;
	double len2=link2->distance;
	double angle1=link1->angle;
	double angle2=link2->angle;
	//默认相同类型
	int cid1=1;
	int cid2=1;
//参数
	int cid=0;
	double xita=0;
	double l;
	double w1=50;
	double w2=1;
	double w3=1;
	double w4=50;
	double w5=1000;

	//类别
	if(cid1==cid1)
	{
		cid=0;
	}else
	{
		cid=1;
	}


	
	//L
	l=abs(len1-len2);
	//xita
	xita=abs(angle1-angle2);

	//return w1*q+w2*xita+w3*l+w4*cid+w5*subid;
	//returnError=w2*xita+w3*l+w4*cid+w5*subid;
	double returnError=w2*xita+w3*l+w4*cid;
	return returnError;
}

TextureElement * Texture::findOptimalMatchTextureElement(TextureElement *te,vector<int> &addList)
{
	
	double minEorror=999;
	double eorror;
	TextureElement  * res=NULL;
	vector<int>  f;
	int m_start=0;
	int m_steps=0;
	for(int i=0;i<tes.size();i++)
	{
		TextureElement * tei=tes.at(i);
		if(!isRingStruct(tei->index)) continue;
		eorror=differentBetweenTwoTextureElement(te,tei,f);
		if(eorror<minEorror)
		{
			minEorror=eorror;
			res=tei;
			addList.assign(f.begin(),f.end());
		}
		f.clear();
	}
	return res;
}
double Texture::differentBetweenTwoTextureElement(TextureElement *te1,TextureElement *te2,vector<int> &addList)//te2的连通信息完整，而te1的不完整，用te1的连通信息（4点，2个三角形）与te2的每个顶点局部连通关系（4点，2个三角形）作比较
{
	double error = 0;
	double minEorror = 9999;
	int size1 = te1->link.size();
	int size2 = te2->link.size();
	int i = 0;
	int j = 0;
	int tempError = 0;
	int tempMinError = 999;
	int minJ;
	vector<IndexPair> matchIndexs;
	matchIndexs.clear();
	bool isAdded = false;
	bool isAddedToAddList = false;
	if (size1>size2)
	{
		return 999;
	}
	for (i = 0; i<size1; i++)
	{
		tempError = 0;
		tempMinError = 999;
		minJ = 0;
		for (j = 0; j<size2; j++)
		{
			////如果te2中的第j个元素,已经有匹配了的元素了，则不参加匹配
			//vector<IndexPair>::iterator beginJ=matchIndexs.begin();
			//vector<IndexPair>::iterator endJ=matchIndexs.end();
			//vector <IndexPair>::iterator itJ=find_if(beginJ,endJ,j);//此处find有问题 3.17 
			//if(itJ==endJ)
			isAddedToAddList = false;
			for (int m = 0; m<matchIndexs.size(); m++)
			{
				if (matchIndexs.at(m).sampleIndex == j)
				{
					isAddedToAddList = true;
					break;
				}
			}
			if (isAddedToAddList)
			{

			}
			else
			{
				//用te1中的第i个元素，与te2中所有元素依次匹配
				tempError = getError(te1->link.at(i), te2->link.at(j));
				{
					if (tempError<tempMinError)
					{
						tempMinError = tempError;
						minJ = j;
					}
				}
			}

		}
		IndexPair ip;
		ip.sampleIndex = minJ;
		ip.targetIndex = i;
		matchIndexs.push_back(ip);
		error += tempMinError;
	}
	addList.clear();
	for (int k = 0; k<size2; k++)
	{
		isAdded = false;
		for (int n = 0; n<matchIndexs.size(); n++)
		{
			if (matchIndexs.at(n).sampleIndex == k)
			{
				isAdded = true;
				break;
			}
		}
		if (isAdded)
		{

		}
		else
		{
			addList.push_back(k);
		}

	}

	return error;
}
void Texture::textureSort()
{
	for(int i=0;i<tes.size();i++)
	{
		tes.at(i)->textureElementSort();
	}
}
