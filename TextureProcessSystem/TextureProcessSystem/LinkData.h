#pragma once
class TextureElement;
class LinkData
{
public:
	LinkData(void);
	~LinkData(void);
public:
	int index;
	double distance;
	double angle;
	TextureElement * linkElement;

public:
	bool operator == (const int index)
	{
		if(this->index==index)
		{
			return true;
		}else
		{
			return false;
		}
	}
};

