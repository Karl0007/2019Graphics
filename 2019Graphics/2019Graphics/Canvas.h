#pragma once

#include <QWidget>
#include <qpainter.h>
#include <qevent.h>
#include <iostream>
#include <qdir.h>
#include "Painter.h"
#include "Head.h"

class Painter;
class MainWindow;
class Canvas : public QWidget
{
	friend class MainWindow;
	Q_OBJECT

public:
	enum State
	{
		LinePosStart,
		LinePosEnd,
		EllipsePos,
		EllipseR,
		PolygonStart,
		Polygoning,
		CurveStart,
		Curveing,
		ClipStart,
		ClipEnd,
		TranslateStart,
		TranslateEnd,
		RotateStart,
		RotateEnd,
		ScaleStart,
		ScaleEnd,
		Null
	};
	Canvas(QWidget *parent = Q_NULLPTR,Painter* = nullptr, MainWindow *w = nullptr);
	~Canvas();

	void Reset(Painter* p) { m_painter = p; }
	void Reset(int w, int h);
	bool SaveCanvas(QString const & dir,QString const & name);
	void SetColor(Byte R,Byte G,Byte B);


private:
	static const QString m_logInfo[];
	MainWindow *m_window;
	Painter *m_painter;
	Vector2i m_pressPos;
	Vector2i m_zoom;
	vector<int> m_posList;
	State m_state;
	bool m_mouseClick;
	void paintEvent(QPaintEvent*);

	void mousePressEvent(QMouseEvent* e);
	//void mouseDoubleClickEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	inline void SetNull();
	inline void SetState(State s);

};
