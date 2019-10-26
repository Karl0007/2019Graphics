#pragma once
#include "Head.h"
#include <cstring>
#include <vector>
#include <iostream>
using std::vector;

class Graph
{
protected:
	//变换矩阵
	Matrix3f m_transform;
	//颜色信息
	Byte m_RGB[3];
	//编号
	int m_id;

public:

	Graph(int id, Byte RGB[]) : m_id(id) { m_transform.setIdentity(); m_RGB[0] = RGB[0]; m_RGB[1] = RGB[1]; m_RGB[2] = RGB[2]; }
	virtual ~Graph(){ }
	
	//在w*h的img数组中绘制当前图元
	virtual void Draw(Byte *img,int w,int h) = 0;
	//复制当前图元
	virtual Graph* Copy(int id) = 0;
	//平移变换
	Graph* Translate(int x, int y);
	//旋转变换
	Graph* Rotate(int x, int y,int r);
	//缩放变换
	Graph* Scale(int x, int y, float sx,float sy);
	Graph* SetColor(Byte R, Byte G, Byte B);
	void GetColor(Byte &R, Byte &G, Byte &B);
	int GetId();
	void SetID(int id) { m_id = id; }

};

class Line : public Graph
{
private:
	Vector2i st, ed;
	int xmin, xmax, ymin, ymax;
	bool DDA,Cohen;
public:
	Line(int id, int x1, int y1, int x2, int y2,bool dda,Byte RGB[]) : Graph(id, RGB),DDA(dda),xmin(0),ymin(0),xmax(1000),ymax(1000),Cohen(true) {
		st << x1, y1;
		ed << x2, y2;
	}
	~Line(){}
	void Draw(Byte *,int w,int h);
	void SetClip(int xn, int xx, int yn, int yx, bool C) { xmin = xn, xmax = xx, ymin = yn, ymax = yx, Cohen = C; }
	bool ToDelete() { return xmin == xmax && ymin == ymax; }
	bool ClipCohen(Vector2i& st, Vector2i &ed);
	bool ClipBarsky(Vector2i& st, Vector2i &ed);
	Line* Copy(int id) { auto tmp = new Line(*this); tmp->m_id = id; return tmp; }

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
	Ellipse* Copy(int id) { auto tmp = new Ellipse(*this); tmp->m_id = id; return tmp; }

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
	Polygon* Copy(int id) { auto tmp = new Polygon(*this); tmp->m_id = id; return tmp; }

};

class Curve : public Graph
{
private:
	vector<Vector2i> points;
	bool bezier;
	static const int Step = 2500;
	static const int N = 100;
	static const int K = 3;
	static bool init;
	static double Fac[Step][N][N];

public:
	static void Init();

	Curve(int id, vector<int> const&p, bool bezier, Byte RGB[]) : Graph(id, RGB), bezier(bezier) {
		if (!init) Init();
		for (int i = 0; i < p.size(); i += 2) {
			Vector2i tmp;
			tmp(0) = p[i], tmp(1) = p[i + 1];
			points.push_back(std::move(tmp));
		}
	}
	~Curve() {}
	void Draw(Byte *img, int w, int h);
	Curve* Copy(int id) { auto tmp = new Curve(*this); tmp->m_id = id; return tmp; }
};