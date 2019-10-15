#include "Canvas.h"

Canvas::Canvas(QWidget *parent, Painter* p)
	: QWidget(parent), m_painter(p)
{
	setFixedSize(m_painter->m_width, m_painter->m_height);
}

Canvas::~Canvas()
{
}

void Canvas::Reset(int w, int h) { 
	m_painter->Reset(w, h); 
	setFixedSize(w, h);
}


bool Canvas::SaveCanvas(QString const & dir, QString const & name)
{
	QDir qdir(dir);
	QImage img(m_painter->m_img, m_painter->m_width, m_painter->m_height, QImage::Format::Format_RGB888);
	if (!qdir.exists()) qdir.mkpath(dir);
	return img.save(dir + name + ".bmp");
}

void Canvas::SetColor(Byte R, Byte G, Byte B)
{
	m_painter->SetColor(R,G,B);
}

void Canvas::paintEvent(QPaintEvent * e)
{
	QImage img(m_painter->m_img, m_painter->m_width, m_painter->m_height, QImage::Format::Format_RGB888);
	QPainter paint(this);
	paint.drawImage(0, 0, img);
	std::cout << "paint" << std::endl;
}

void Canvas::mousePressEvent(QMouseEvent * e)
{
	m_painter->DrawAll();
	update();
}

