#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->setFixedSize(this->width(), this->height());
	this->setWindowTitle("Graphic 2019 V0.7");
	Curve::Init();
	m_canvas = new Canvas(ui.IMG,m_painter = new Painter(360, 360),this);
	UpdateID();
	QObject::connect(ui.Clean, &QPushButton::pressed, [&](){ResetCanvas(360, 360);});
	QObject::connect(ui.OpenFile, &QPushButton::pressed, [&]() {OpenFile(); });
	QObject::connect(ui.SaveImg, &QPushButton::pressed, [&]() {SaveCanvas(); });
	QObject::connect(ui.CurrentID, &QComboBox::currentTextChanged,this,&MainWindow::CurrentIDChange);
	QObject::connect(ui.Line, &QPushButton::pressed,this, &MainWindow::StartLine);
	QObject::connect(ui.Curve, &QPushButton::pressed,this, &MainWindow::StartCurve);
	QObject::connect(ui.Polygon, &QPushButton::pressed,this, &MainWindow::StartPolygon);
	QObject::connect(ui.Ellipse, &QPushButton::pressed,this, &MainWindow::StartEllispe);
	QObject::connect(ui.translate, &QPushButton::pressed,this, &MainWindow::StartTranslate);
	QObject::connect(ui.rotate, &QPushButton::pressed,this, &MainWindow::StartRotate);
	QObject::connect(ui.scale, &QPushButton::pressed,this, &MainWindow::StartScale);
	QObject::connect(ui.clip, &QPushButton::pressed,this, &MainWindow::StartClip);
	QObject::connect(ui.Delete, &QPushButton::pressed, this, &MainWindow::DeleteCur);
}


void MainWindow::ResetCanvas(int w, int h)
{
	m_canvas->Reset(w, h);
	m_canvas->update();
}

void MainWindow::SaveCanvas(QString const & name)
{
	m_canvas->SaveCanvas("./Results/", name);
}

void MainWindow::SaveCanvas()
{
	m_canvas->SaveCanvas("", QFileDialog::getSaveFileName(this, "Save", QString(), "BMP Files(*.bmp)"));

}

void MainWindow::SetColor(Byte R, Byte G, Byte B)
{
	m_painter->SetColor(R, G, B);
}

void MainWindow::OpenFile()
{
	auto path = QFileDialog::getOpenFileName(this);
	QFile file(path);
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	while (!file.atEnd())
		Compile(file);
	m_painter->DrawAll();
	m_canvas->update();
	UpdateID();
}

void MainWindow::UpdateID()
{
	ui.CurrentID->clear();
	ui.CurrentID->addItems(m_painter->GetIDList());
	CurrentIDChange("Null");
}

void MainWindow::CurrentIDChange(QString const & str)
{
	int id = str.toInt();
	bool b = m_painter->m_hash.count(id) && str != "Null";
	ui.scale->setEnabled(b);
	ui.translate->setEnabled(b);
	ui.rotate->setEnabled(b);
	ui.Delete->setEnabled(b);
	ui.clip->setEnabled(b && m_painter->m_line.count(id));
	m_painter->m_current = b ? m_painter->m_hash[id] : nullptr;
	m_painter->DrawAll();
	m_canvas->update();
}

void MainWindow::Compile(QFile &file)
{
	stringstream ss(file.readLine().constData());
	string str;
	int a, b, c, d, e, f, g, h, i;
	ss >> str;
	if (str == "resetCanvas") {
		ss >> a >> b;
		ResetCanvas(a, b);
	}
	else if (str == "saveCanvas") {
		SaveCanvas(QString::fromStdString(getStr(ss)));
	}
	else if (str == "setColor") {
		ss >> a >> b >> c;
		SetColor(a, b, c);
	}
	else if (str == "drawLine") {
		ss >> a >> b >> c >> d >> e;
		m_painter->DrawLine(a, b, c, d, e, getStr(ss) == "DDA");
	}
	else if (str == "drawPolygon") {
		ss >> a >> b;
		stringstream ss2(file.readLine().constData());
		m_painter->DrawPolygon(a, getVec(ss2, b), getStr(ss) == "DDA");
	}
	else if (str == "drawEllipse") {
		ss >> a >> b >> c >> d >> e;
		m_painter->DrawEllipse(a, b, c, d, e);
	}
	else if (str == "drawCurve") {
		ss >> a >> b;
		stringstream ss2(file.readLine().constData());
		m_painter->DrawCurve(a, getVec(ss2, b), getStr(ss) == "Bezier");
	}
	else if (str == "translate") {
		ss >> a >> b >> c;
		m_painter->Translate(a, b, c);
	}
	else if (str == "rotate") {
		ss >> a >> b >> c >> d;
		m_painter->Rotate(a, b, c, d);
	}
	else if (str == "scale") {
		ss >> a >> b >> c;
		m_painter->Scale(a, b, c, getFloat(ss));
	}
	else if (str == "clip") {
		ss >> a >> b >> c >> d >> e;
		m_painter->SetClip(a, b, c, d, e, getStr(ss) == "Cohen-Sutherland");
	}
}

void MainWindow::StartLine()
{
	SetEnable(0);
	m_canvas->m_state = Canvas::State::LinePosStart;
}

void MainWindow::StartPolygon()
{
	SetEnable(0);
	m_canvas->m_state = Canvas::State::PolygonStart;
}

void MainWindow::StartEllispe()
{
	SetEnable(0);
	m_canvas->m_state = Canvas::State::EllipsePos;
}

void MainWindow::StartCurve()
{
	SetEnable(0);
	m_canvas->m_state = Canvas::State::CurveStart;
}

void MainWindow::StartRotate()
{
	SetEnable(0);
	m_canvas->m_state = Canvas::State::RotateStart;
}

void MainWindow::StartScale()
{
	SetEnable(0);
	m_canvas->m_state = Canvas::State::ScaleStart;
}

void MainWindow::StartTranslate()
{
	SetEnable(0);
	m_canvas->m_state = Canvas::State::TranslateStart;
}

void MainWindow::StartClip()
{
	SetEnable(0);
	m_canvas->m_state = Canvas::State::ClipStart;
}

void MainWindow::DeleteCur()
{
	m_painter->Delete(ui.CurrentID->currentText().toInt());
	UpdateID();
	m_painter->DrawAll();
	m_canvas->update();
}

void MainWindow::SetEnable(bool b)
{
	ui.Bezier->setEnabled(b);
	ui.Blue->setEnabled(b);
	ui.DDA->setEnabled(b);
	ui.Clean->setEnabled(b);
	ui.clip->setEnabled(b);
	ui.Cohen->setEnabled(b);
	ui.CurrentID->setEnabled(b);
	ui.Curve->setEnabled(b);
	ui.Delete->setEnabled(b);
	ui.Ellipse->setEnabled(b);
	ui.Green->setEnabled(b);
	ui.Line->setEnabled(b);
	ui.OpenFile->setEnabled(b);
	ui.Polygon->setEnabled(b);
	ui.Red->setEnabled(b);
	ui.rotate->setEnabled(b);
	ui.SaveImg->setEnabled(b);
	ui.scale->setEnabled(b);
	ui.translate->setEnabled(b);
	if (b) CurrentIDChange(ui.CurrentID->currentText());
}
