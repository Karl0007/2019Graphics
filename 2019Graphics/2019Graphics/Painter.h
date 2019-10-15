#pragma once
#include "Canvas.h"
#include "qobject.h"
#include "Head.h"
#include "Graph.h"

class Painter : public QObject
{
	friend class Canvas;

	Q_OBJECT

private:
	Hash m_hash;
	Set m_line;
	Byte m_rgb[3];
	Byte* m_img;
	int m_width, m_height, m_deep;

public:
	Painter(int w = 0, int h = 0, int d = 3) : m_img(new Byte[w*h*d]), m_width(w), m_deep(d), m_height(h) { for (int i = 0; i < w*h*d; i++) m_img[i] = 255; m_rgb[0] = m_rgb[1] = m_rgb[2] = 0; }
	~Painter() { delete m_img; for (auto i : m_hash) delete i.second; }

	Painter* Reset(int w = 0, int h = 0, int d = 3) {
		delete m_img; 
		m_img = new Byte[w*h*d];
		for (int i = 0; i < w*h*d; i++) m_img[i] = 255;
		m_width = w, m_deep = d, m_height = h; 
		for (auto i : m_hash) delete i.second;
		m_hash.clear();
		m_line.clear();
		return this;
	}
	void SetColor(Byte R, Byte G, Byte B) { m_rgb[0] = R, m_rgb[1] = G, m_rgb[2] = B; }
	Graph* DrawLine(int id, int x1, int y1, int x2, int y2, bool dda) { Delete(id); m_line.insert(id); return m_hash[id] = new Line(id, x1, y1, x2, y2, dda, m_rgb); }
	Graph* DrawEllipse(int id, int x, int y, int rx, int ry) { Delete(id); return m_hash[id] = new Ellipse(id, x, y, rx, ry , m_rgb); }
	Graph* DrawPolygon(int id, vector<int> const&p, bool dda) { Delete(id); return m_hash[id] = new Polygon(id, p, dda, m_rgb); }
	Graph* DrawCurve(int id, vector<int> const&p, bool bezier) { Delete(id); return m_hash[id] = new Curve(id, p, bezier, m_rgb); }
	void Translate(int id, int x, int y) { if (m_hash.count(id)) m_hash[id]->Translate(x, y); }
	void Rotate(int id, int x, int y, int r) { if (m_hash.count(id)) m_hash[id]->Rotate(x, y, r); }
	void Scale(int id, int x, int y, float s) { if (m_hash.count(id)) m_hash[id]->Scale(x, y, s, s); }
	void SetClip(int id, int x1, int y1, int x2, int y2, bool C) { if (m_line.count(id)) static_cast<Line*>(m_hash[id])->SetClip(x1, x2, y1, y2, C); };
	void Delete(int id) { if (m_hash.count(id)) { m_line.erase(id); delete m_hash[id]; } }
	void DrawAll();

	Painter(Painter const &) = delete;
	Painter(Painter &&) = delete;
	Painter operator=(Painter const &) = delete;
	Painter operator=(Painter &&) = delete;
};

