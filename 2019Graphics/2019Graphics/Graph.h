#pragma once
#include "Head.h"
#include <cstring>
#include <vector>
using std::vector;

class Graph
{
protected:

	Matrix3f m_transform;
	Byte m_RGB[3];
	int m_id;

public:

	Graph(int id, Byte RGB[]) : m_id(id) { m_transform.setIdentity(); m_RGB[0] = RGB[0]; m_RGB[1] = RGB[1]; m_RGB[2] = RGB[2]; }
	virtual ~Graph(){ }
	
	virtual void Draw(Byte *,int w,int h) = 0;
	Graph* Translate(int x, int y);
	Graph* Rotate(int x, int y,int r);
	Graph* Scale(int x, int y, float sx,float sy);

};

class Line : public Graph
{
private:
	Vector2i st, ed;
	bool DDA;
public:
	Line(int id, int x1, int y1, int x2, int y2,bool dda,Byte RGB[]) : Graph(id, RGB),DDA(dda) {
		st << x1, y1;
		ed << x2, y2;
	}
	~Line(){}
	void Draw(Byte *,int w,int h);
};

class Ellipse : public Graph
{
private:
	int rx, ry;

public:
	Ellipse(int id, int x, int y, int rx, int ry, Byte RGB[]) : Graph(id, RGB),rx(rx),ry(ry) {
		Translate(x, y);
	}
	~Ellipse() {}
	void Draw(Byte *, int w, int h);
};

class Polygon : public Graph
{
private:
	vector<Vector2i> points;
	bool DDA;
public:
	Polygon(int id,vector<int> const&p, bool dda, Byte RGB[]) : Graph(id, RGB), DDA(dda) {
		for (int i = 0; i < p.size(); i+=2) {
			Vector2i tmp;
			tmp(0) = p[i], tmp(1) = p[i + 1];
			points.push_back(std::move(tmp));
		}
	}
	~Polygon() {}
	void Draw(Byte *, int w, int h);
};