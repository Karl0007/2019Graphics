#pragma once

#include <QWidget>
#include <qpainter.h>
#include <qevent.h>
#include <iostream>
#include "Painter.h"
#include "Head.h"

class Painter;
class Canvas : public QWidget
{
	Q_OBJECT

public:

	Canvas(QWidget *parent = Q_NULLPTR,Painter* = nullptr);
	~Canvas();

	void Reset(Painter* p) { m_painter = p; }
	void Reset(int w, int h);
	void SaveCanvas(QString const & str);
	void SetColor(Byte R,Byte G,Byte B);


private:
	Painter *m_painter;
	void paintEvent(QPaintEvent*);
	void mousePressEvent(QMouseEvent* e);
};
