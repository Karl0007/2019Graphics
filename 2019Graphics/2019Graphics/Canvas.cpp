#include "Canvas.h"

const QString Canvas::m_logInfo[] = {
u8"画笔：直线；点击左键选择起点 右键取消",
u8"画笔：直线；点击左键选择终点 右键取消",
u8"画笔：椭圆；点击左键选择圆心 右键取消",
u8"画笔：椭圆；点击左键确定半径 右键取消",
u8"画笔：多边形；点击左键确定第一个点 右键取消",
u8"画笔：多边形；左键-继续加入 右键-完成绘制",
u8"画笔：曲线；点击左键确定第一个点 右键取消",
u8"画笔：曲线；左键-继续加入 右键-完成绘制",
u8"操作：裁剪；点击左键选择裁剪点",
u8"操作：裁剪；点击左键确定裁剪范围",
u8"操作：移动；按住左键拖动图形 点击右键确定",
u8"操作：移动；按住左键拖动图形 点击右键确定",
u8"操作：旋转；点击左键确定旋转中心",
u8"操作：旋转；按住左键拖动旋转 点击右键确定",
u8"操作：缩放；点击左键确定缩放中心",
u8"操作：缩放；按住左键拖动缩放 点击右键确定",
u8"点击右侧按钮选择操作"
};

Canvas::Canvas(QWidget *parent, Painter* p,MainWindow *w)
	: QWidget(parent), m_painter(p),m_mouseClick(false),m_window(w),m_state(Null)
{
	setFixedSize(m_painter->m_width, m_painter->m_height);
	setMouseTracking(true);
}

Canvas::~Canvas()
{
}

void Canvas::Reset(int w, int h) { 
	m_state = Null;
	m_painter->Reset(w, h); 
	//setFixedSize(w, h);
}


bool Canvas::SaveCanvas(QString const & dir, QString const & name)
{
	QDir qdir(dir);
	QImage img(m_painter->m_img, m_painter->m_width, m_painter->m_height, QImage::Format::Format_RGB888);
	if (!qdir.exists()) qdir.mkpath(dir);
	return img.save(dir + name);
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
}

void Canvas::mousePressEvent(QMouseEvent * e)
{
	int r = 0;
	if (e->button() == Qt::MouseButton::LeftButton) {
		switch (m_state)
		{
		case Canvas::State::TranslateStart:
		case Canvas::State::RotateStart:
		case Canvas::State::ScaleStart:
			m_zoom.setZero();
		case Canvas::State::LinePosStart:
		case Canvas::State::EllipsePos:
		case Canvas::State::PolygonStart:
		case Canvas::State::CurveStart:
		case Canvas::State::ClipStart:
			m_posList.clear();
			m_state = State(int(m_state) + 1);
		case Canvas::State::Polygoning:
		case Canvas::State::Curveing:
			m_posList.push_back(e->x());
			m_posList.push_back(e->y());
			m_window->UpdateLog();
			break;
		case Canvas::LinePosEnd:
		case Canvas::EllipseR:
			while (m_painter->m_hash.count(r = rand()));
			m_painter->m_hash[r] = m_painter->m_tmp;
			m_painter->m_tmp = nullptr;
			if (m_state == LinePosEnd) m_painter->m_line.insert(r);
			m_window->UpdateLog(u8"绘制成功！ 左键继续绘制 右键返回");
			SetState((State)((int)m_state - 1));
			//SetNull();
			break;
		case Canvas::ClipEnd:
			r = m_window->ui.CurrentID->currentText().toInt();
			m_painter->SetClip(r, std::min(m_posList[0], e->x()), std::min(m_posList[1], e->y()), std::max(m_posList[0], e->x()), std::max(m_posList[1], e->y()), m_window->ui.Cohen->isChecked());
			m_window->UpdateLog(u8"裁剪成功！");
			SetNull();
			break;
		default:
			break;
		}
	}
	else if (e->button() == Qt::MouseButton::RightButton) {
		if (m_state == Curveing || m_state == Polygoning) {
			while (m_painter->m_hash.count(r = rand()));
			m_painter->m_hash[r] = m_painter->m_tmp;
			m_painter->m_tmp = nullptr;
			m_window->UpdateLog(u8"绘制成功！左键继续绘制 右键返回");
			SetState((State)((int)m_state - 1));
			//SetNull();
		}else if (m_state == RotateEnd || m_state == ScaleEnd || m_state == TranslateEnd) {
			r = m_window->ui.CurrentID->currentText().toInt();
			delete m_painter->m_hash[r];
			m_painter->m_hash[r] = m_painter->m_tmp;
			m_painter->m_tmp = nullptr;
			m_window->UpdateLog(u8"操作成功！");
			SetNull();
		}
		else{
			delete m_painter->m_tmp;
			m_painter->m_tmp = nullptr;
			m_window->UpdateLog(u8"取消成功！");
			SetNull();
		}
	}
}

void Canvas::mouseReleaseEvent(QMouseEvent * e)
{
	m_mouseClick = false;
	//m_zoom(0) += m_pressPos(0) - e->x();
	//m_zoom(1) += m_pressPos(1) - e->y();
}

void Canvas::mouseMoveEvent(QMouseEvent * e)
{
	if (m_mouseClick) {
		m_zoom(0) += m_pressPos(0) - e->x();
		m_zoom(1) += m_pressPos(1) - e->y();
	}
	if (m_mouseClick = (e->buttons() & Qt::LeftButton)) {
		m_pressPos(0) = e->x();
		m_pressPos(1) = e->y();
	}
	delete m_painter->m_tmp;
	auto ui = m_window->ui;
	Byte rgb[] = { ui.Red->value() ,ui.Green->value(),ui.Blue->value() };
	auto list = m_posList;
	switch (m_state)
	{
	case Canvas::LinePosEnd:
		m_painter->m_tmp = new Line(0, list[0], list[1], e->x(), e->y(), ui.DDA->isChecked(), rgb);
		break;
	case Canvas::EllipseR:
		m_painter->m_tmp = new Ellipse(0, list[0], list[1], abs(list[0]-e->x()), abs(list[1] - e->y()), rgb);
		break;
	case Canvas::Polygoning:
		list.push_back(e->x());
		list.push_back(e->y());
		m_painter->m_tmp = new Polygon(0, list, ui.DDA->isChecked(), rgb);
		break;
	case Canvas::Curveing:
		list.push_back(e->x());
		list.push_back(e->y());
		m_painter->m_tmp = new Curve(0, list, ui.Bezier->isChecked(), rgb);
		break;
	case Canvas::ClipEnd:
		m_painter->m_tmp = new Polygon(0, {std::min(list[0],e->x()),std::min(list[1],e->y()),std::max(list[0],e->x()),std::min(list[1],e->y()),std::max(list[0],e->x()),std::max(list[1],e->y()),std::min(list[0],e->x()),std::max(list[1],e->y()) }, ui.DDA->isChecked(), rgb);
		break;
	case Canvas::TranslateEnd:
		m_painter->m_tmp = m_painter->m_current->Copy()->Translate(-m_zoom(0), -m_zoom(1));
		break;
	case Canvas::RotateEnd:
		m_painter->m_tmp = m_painter->m_current->Copy()->Rotate(list[0],list[1], -m_zoom(1));
		break;
	case Canvas::ScaleEnd:
		m_painter->m_tmp = m_painter->m_current->Copy()->Scale(list[0], list[1], 1-m_zoom(0)/100.0f , 1+m_zoom(1)/ 100.0f);
		break;
	case Canvas::Null:
		break;
	default:
		break;
	}
	m_painter->DrawAll();
	update();
}

void Canvas::SetNull()
{
	delete m_painter->m_tmp;
	m_painter->m_tmp = nullptr;
	m_state = Null;
	m_zoom.setZero();
	m_window->SetEnable(true);
	m_window->UpdateID();
}

inline void Canvas::SetState(State s)
{
	delete m_painter->m_tmp;
	m_painter->m_tmp = nullptr;
	m_state = s;
	m_zoom.setZero();
	m_window->SetEnable(false);
	m_window->UpdateID();
}

