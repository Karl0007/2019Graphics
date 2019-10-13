#include "Graph.h"

#include <iostream>
using namespace std;

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

#define SetColor(vec) memcpy(img + (vec(0)+vec(1)*w)*3,m_RGB,3)

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
	DrawLine(img, w, h, DDA,Transform(m_transform,st),Transform(m_transform,ed), m_RGB);
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

#undef SetColor

void Polygon::Draw(Byte *img, int w, int h)
{
	for (int i = 0; i < points.size() - 1; i++) {
		DrawLine(img, w, h, DDA, Transform(m_transform, points[i]), Transform(m_transform, points[i+1]), m_RGB);
	}
	DrawLine(img, w, h, DDA, Transform(m_transform, points.front()), Transform(m_transform, points.back()), m_RGB);

}
