#include "Graph.h"

#include <iostream>
using namespace std;

#define SAFE

bool Curve::init = false;
double Curve::Fac[Curve::Step][Curve::N][Curve::N] = {0};


Graph* Graph::Translate(int x, int y)
{
	Matrix3f tmp;
	tmp <<
		1, 0, x,
		0, 1, y,
		0, 0, 1;
	m_transform = tmp * m_transform;
	return this;
}

Graph* Graph::Rotate(int x, int y, int r)
{
	Translate(-x, -y);
	float th = r * 3.1415926 / 180;
	Matrix3f tmp;
	tmp <<
		cosf(th), -sinf(th), 0,
		sinf(th), cosf(th), 0,
		0, 0, 1;
	m_transform = tmp * m_transform;
	Translate(x, y);
	return this;
}

Graph* Graph::Scale(int x, int y, float sx,float sy)
{
	Translate(-x, -y);
	Matrix3f tmp;
	tmp <<
		sx, 0, 0,
		0, sy, 0,
		0, 0, 1;
	m_transform = tmp * m_transform;
	Translate(x, y);
	return this;
}

Graph * Graph::SetColor(Byte R, Byte G, Byte B)
{
	m_RGB[0] = R, m_RGB[1] = G, m_RGB[2] = B;
	return this;
}

void Graph::GetColor(Byte & R, Byte & G, Byte & B)
{
	R = m_RGB[0],G = m_RGB[1],B = m_RGB[2];

}

#ifdef  SAFE
#define SetColor(vec) if(vec(0)>=0 && vec(0)<w && vec(1)>=0 && vec(1)<h) memcpy(img + (vec(0)+vec(1)*w)*3,m_RGB,3)
#define SetColorXY(x,y) if(x>=0 && x<w && y>=0 && y<h) memcpy(img + (x+y*w)*3,m_RGB,3)
#else
#define SetColor(vec) memcpy(img + (vec(0)+vec(1)*w)*3,m_RGB,3)
#define SetColorXY(x,y) memcpy(img + (x+y*w)*3,m_RGB,3)
#endif

static void inline DrawLine(Byte *img, int w, int h, bool DDA, Vector2i st, Vector2i ed,Byte m_RGB[]) {
	Vector2i tmp = ed - st;
	int step = abs(tmp(0)) < abs(tmp(1));
	if (tmp(step) < 0) st.swap(ed), tmp *= -1;
	if (DDA) {
		float j = st(!step), delta = 1.0f*tmp(!step) / tmp(step);
		for (tmp = st; tmp(step) <= ed(step); tmp(step)++, j += delta) {
			tmp(!step) = j;
			SetColor(tmp);
		}
	}
	else {
		int dy = abs(tmp(!step) << 1), dydx = dy - abs(tmp(step) << 1), p = dy - abs(tmp(step));
		int x = tmp(!step) < 0 ? -1 : 1;
		for (tmp = st; tmp(step) <= ed(step); tmp(step)++) {
			SetColor(tmp);
			if (p > 0) tmp(!step) += x, p += dydx; else p += dy;
		}
	}
}

static inline Vector2i Transform(Matrix3f const& transform, Vector2i vec) {
	Vector3f v;
	Vector2i res;
	v << vec(0), vec(1), 1;
	v = transform * v;
	res << v(0)/v(2), v(1)/v(2);
	return res;
}


void Line::Draw(Byte *img, int w, int h)
{
	auto t1 = Transform(m_transform, st);
	auto t2 = Transform(m_transform, ed);
	if (Cohen ? ClipCohen(t1, t2) : ClipBarsky(t1, t2))
		DrawLine(img, w, h, DDA,t1,t2, m_RGB);
	//clean clip data
	xmin = ymin = 0;
	xmax = ymax = 1000;
	m_transform.setIdentity();
	st = t1, ed = t2;
}

#define Code(x,y) (((x<xmin)<<0) | ((x>xmax)<<1) | ((y<ymin)<<2) | ((y>ymax)<<3))
bool Line::ClipCohen(Vector2i& st,Vector2i &ed)
{
	int x1 = st(0), y1 = st(1), x2 = ed(0), y2 = ed(1);
	int c1 = Code(x1, y1), c2 = Code(x2, y2);
	while (c1 || c2) {
		if (c1&c2) {
			return false;
		}
		int code = c1 ? c1 : c2, x, y;
		if (code & 1)
			x = xmin, y = y1 + (y2 - y1)*(xmin - x1) / (x2 - x1);
		else if (code & 2) 
			x = xmax, y = y1 + (y2 - y1)*(xmax - x1) / (x2 - x1);
		else if (code & 4) 
			y = ymin, x = x1 + (x2 - x1)*(ymin - y1) / (y2 - y1);
		else if (code & 8) 
			y = ymax, x = x1 + (x2 - x1)*(ymax - y1) / (y2 - y1);
		if (code == c1) 
			x1 = x, y1 = y, c1 = Code(x, y);
		else
			x2 = x, y2 = y, c2 = Code(x, y);
	}
	st(0) = x1, st(1) = y1, ed(0) = x2, ed(1) = y2;
	return true;
}
#undef Code

bool Line::ClipBarsky(Vector2i & st, Vector2i & ed)
{
	int x1 = st(0), y1 = st(1), x2 = ed(0), y2 = ed(1);
	Vector2i delta = ed - st;
	double u1 = 0, u2 = 1;
	int p[]{ -delta(0),delta(0),-delta(1),delta(1) };
	int q[]{ x1 - xmin,xmax - x1,y1 - ymin,ymax - y1 };
	for (int i = 0; i < 4; i++) {
		if (p[i] == 0) {
			if (q[i] < 0) return false;
			if (i == 0 || i == 1)
				st(1) = max(ymin, min(y1, y2)), ed(1) = min(ymax, max(y1, y2));
			else
				st(0) = max(xmin, min(x1, x2)), ed(0) = min(xmax, max(x1, x2));
			return true;
		}
		if (p[i] < 0) {
			u1 = max(u1, 1.0* q[i] / p[i]);
		}else{
			u2 = min(u2, 1.0* q[i] / p[i]);
		}
	}
	if (u1 > u2) return false;
	st(0) = x1 + int(u1*delta(0)), st(1) = y1 + int(u1*delta(1)), ed(0) = x1 + int(u2*delta(0)), ed(1) = y1 + int(u2*delta(1));
	return true;
}



void Ellipse::Draw(Byte *img, int w, int h)
{
	static const int sx[] = { 1,-1,1,-1 };
	static const int sy[] = { 1,1,-1,-1 };
	Vector2i res;
	Vector2i tmp;

	float ry2 = ry * ry, rx2 = rx * rx, rx2ry2 = rx2 * ry2;
	int ry2i = ry * ry, rx2i = rx * rx;
	res << 0, ry;
	for (; res(0)*ry2i < res(1)*rx2i && res(0) <= rx; res(0)++) {
		for (int p = 0; p < 4; p++) {
			tmp(0) = res(0)*sx[p], tmp(1) = res(1)*sy[p];
			tmp = Transform(m_transform, tmp);
			SetColor(tmp);
		}
		res(1) -= (ry2 * (res(0) + 1)*(res(0) + 1) + rx2 * (res(1) - 0.5f)*(res(1) - 0.5f) - rx2ry2 >= 0);
	}
	for (; res(1)>=0 ; res(1)--) {
		for (int p = 0; p < 4; p++) {
			tmp(0) = res(0)*sx[p], tmp(1) = res(1)*sy[p];
			tmp = Transform(m_transform, tmp);
			SetColor(tmp);
		}
		res(0) += (ry2 * (res(0) + 0.5f)*(res(0) + 0.5f) + rx2 * (res(1) - 1)*(res(1) - 1) - rx2ry2 < 0);
	}
}


void Polygon::Draw(Byte *img, int w, int h)
{
	for (int i = 0; i < points.size() - 1; i++) {
		DrawLine(img, w, h, DDA, Transform(m_transform, points[i]), Transform(m_transform, points[i+1]), m_RGB);
	}
	DrawLine(img, w, h, DDA, Transform(m_transform, points.front()), Transform(m_transform, points.back()), m_RGB);

}

void Curve::Init()
{
	if (init) return;
	init = true;
	for (int s = 0; s < Step; s++) {
		double u = 1.0 * s / (Step - 1);
		double nu = 1.0 - u;
		Fac[s][1][0] = u;
		Fac[s][1][1] = nu;
		for (int i = 2; i < N; i++) {
			Fac[s][i][0] = Fac[s][i - 1][0] * u;
			Fac[s][i][i] = Fac[s][i - 1][i - 1] * nu;
			for (int j = 1; j < i; j++) {
				Fac[s][i][j] = Fac[s][i-1][j-1]*nu + Fac[s][i-1][j]*u;
			}
		}
	}
}

void Curve::Draw(Byte * img, int w, int h)
{
	auto p = points;
	for (auto &i : p) i = Transform(m_transform, i);
	if (bezier) {
		int N = p.size()-1;
		for (int s = 0; s < Step; s++) {
			double x = 0, y = 0;
			for (int i = 0; i <= N; i++) {
				x += Fac[s][N][i] * p[i](0);
				y += Fac[s][N][i] * p[i](1);
			}
			SetColorXY(int(x), int(y));
		}
	}
	else {
		int M = p.size() + K + 1;
		double *U = new double[M];
		double st = 1.0 / Step;
		for (int i = 0; i < M; i++) U[i] = 1.0*i / (M - 1);
		for (double t = U[K-1]; t < U[p.size()]; t+= st) {
			double x = 0, y = 0;
			for (int i = 0; i < p.size(); i++) {
				double B[K] = {};
				for (int j = i; j < i+K ; j++ ) {
					B[j - i] = (t <= U[j + 1] && U[j] <= t);
				}
				for (int p = 1; p < K; p++) {
					for (int j = 0; j < K-p ; j++) {
						double k1 = (t - U[j + i]) < 1e-8 ? 0 : (t - U[j + i]) / (U[j + p + i] - U[j + i]);
						double k2 = (U[i + p + j + 1] - t) < 1e-8 ? 0 : (U[i + p + j + 1] - t) / (U[i + p + j +1] - U[i + j + 1]);
						B[j] = B[j] * k1 + B[j + 1] * k2;
					}
				}
				x += B[0] * p[i](0);
				y += B[0] * p[i](1);
			}
			SetColorXY(int(x), int(y));
		}
		delete U;
	}
}

#undef SetColor
#undef SetColorXY
