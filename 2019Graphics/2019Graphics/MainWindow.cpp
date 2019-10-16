#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->setFixedSize(this->width(), this->height());
	this->setWindowTitle("Graphic 2019 V0.81");
	Curve::Init();
	m_canvas = new Canvas(ui.IMG,m_painter = new Painter(360, 360),this);
	UpdateID();
	UpdateLog();
	QObject::connect(ui.Clean, &QPushButton::pressed, [&]() {ResetCanvas(360, 360); UpdateID(); UpdateLog(u8"清除成功！"); });
	QObject::connect(ui.Test, &QPushButton::pressed, [&]() { 	UpdateLog(u8"刷新时间:" + QString::number(TestTime()) + "ms"); });
	QObject::connect(ui.OpenFile, &QPushButton::pressed, [&]() { QString log; OpenFile(log) ? UpdateLog(u8"文件读取成功！ 每帧绘制时间："+log) : UpdateLog(u8"文件解析失败！"+log); });
	QObject::connect(ui.SaveImg, &QPushButton::pressed, [&]() {SaveCanvas() ? UpdateLog(u8"保存成功！") : UpdateLog(u8"保存失败！"); });
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

bool MainWindow::SaveCanvas(QString const & name)
{
	m_painter->DrawAll();
	return m_canvas->SaveCanvas("./Results/", name + ".bmp");
}

bool MainWindow::SaveCanvas()
{
	m_painter->DrawAll();
	return m_canvas->SaveCanvas("", QFileDialog::getSaveFileName(this, "Save", QString(), "BMP Files(*.bmp)"));
}

void MainWindow::SetColor(Byte R, Byte G, Byte B)
{
	m_painter->SetColor(R, G, B);
}

bool MainWindow::OpenFile(QString &log)
{
	bool ok = true;
	int i = 0;
	auto path = QFileDialog::getOpenFileName(this);
	QFile file(path);
	ok &= file.open(QIODevice::ReadOnly | QIODevice::Text);
	while (!file.atEnd() && (ok &= Compile(file, log, i)));
	if (ok) log = QString::number(TestTime()) + "ms";
	m_canvas->update();
	UpdateID();
	return ok;
}

int MainWindow::TestTime()
{
	QTime time(0, 0, 0, 0);
	time.start();
	m_painter->DrawAll();
	return time.elapsed();
}

void MainWindow::UpdateID()
{
	ui.CurrentID->clear();
	ui.CurrentID->addItems(m_painter->GetIDList());
	CurrentIDChange("Null");
}

void MainWindow::UpdateLog()
{
	ui.Log->setText(Canvas::m_logInfo[int(m_canvas->m_state)]);
}

void MainWindow::UpdateLog(QString const & str)
{
	ui.Log->setText(str);
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

bool MainWindow::Compile(QFile &file, QString &log,int &line)
{
	++line;
	bool ok = true;
	stringstream ss(file.readLine().constData());
	string str;
	int a, b, c, d, e, f, g, h, i;
	ss >> str;
	if (str == "resetCanvas" || str == "r") {
		ss >> a >> b;
		ResetCanvas(a, b);
	}
	else if (str == "saveCanvas" || str == "s") {
		SaveCanvas(QString::fromStdString(getStr(ss)));
	}
	else if (str == "setColor" || str == "c") {
		ss >> a >> b >> c;
		SetColor(a, b, c);
	}
	else if (str == "drawLine" || str == "L") {
		ss >> a >> b >> c >> d >> e;
		m_painter->DrawLine(a, b, c, d, e, getStr(ss) == "DDA");
	}
	else if (str == "drawPolygon" || str == "P") {
		++line;
		ss >> a >> b;
		stringstream ss2(file.readLine().constData());
		m_painter->DrawPolygon(a, getVec(ss2, b), getStr(ss) == "DDA");
		if (!ss2.good()) {
			ok = false;
			log += "line" + QString::number(line) + QString(u8" 参数个数异常.") + "\n";
		}

	}
	else if (str == "drawEllipse" || str == "E") {
		ss >> a >> b >> c >> d >> e;
		m_painter->DrawEllipse(a, b, c, d, e);
	}
	else if (str == "drawCurve" || str == "C") {
		++line;
		ss >> a >> b;
		stringstream ss2(file.readLine().constData());
		m_painter->DrawCurve(a, getVec(ss2, b), getStr(ss) == "Bezier");
		if (!ss2.good()) {
			ok = false;
			log += "line" + QString::number(line) + QString(u8" 参数个数异常.") + "\n";
		}
	}
	else if (str == "translate" || str == "ot") {
		ss >> a >> b >> c;
		m_painter->Translate(a, b, c);
	}
	else if (str == "rotate" || str == "or") {
		ss >> a >> b >> c >> d;
		m_painter->Rotate(a, b, c, d);
	}
	else if (str == "scale" || str == "os") {
		ss >> a >> b >> c;
		m_painter->Scale(a, b, c, getFloat(ss));
	}
	else if (str == "clip" || str == "oc") {
		ss >> a >> b >> c >> d >> e;
		m_painter->SetClip(a, b, c, d, e, getStr(ss) == "Cohen-Sutherland");
	}
	else if (str != ""){
		ok = false;
		log += "line" + QString::number(line) + QString(u8" 非法操作:") + QString::fromStdString(str) + "\n";
	}else if (!ss.good() && str != "") {
		ok = false;
		log += "line" + QString::number(line)  + QString(u8" 参数个数异常.") + "\n";
	}
	return ok;
}

void MainWindow::StartLine()
{
	SetEnable(0);
	m_canvas->m_state = Canvas::State::LinePosStart;
	UpdateLog();
}

void MainWindow::StartPolygon()
{
	SetEnable(0);
	m_canvas->m_state = Canvas::State::PolygonStart;
	UpdateLog();
}

void MainWindow::StartEllispe()
{
	SetEnable(0);
	m_canvas->m_state = Canvas::State::EllipsePos;
	UpdateLog();
}

void MainWindow::StartCurve()
{
	SetEnable(0);
	m_canvas->m_state = Canvas::State::CurveStart;
	UpdateLog();
}

void MainWindow::StartRotate()
{
	SetEnable(0);
	m_canvas->m_state = Canvas::State::RotateStart;
	UpdateLog();
}

void MainWindow::StartScale()
{
	SetEnable(0);
	m_canvas->m_state = Canvas::State::ScaleStart;
	UpdateLog();
}

void MainWindow::StartTranslate()
{
	SetEnable(0);
	m_canvas->m_state = Canvas::State::TranslateStart;
	UpdateLog();
}

void MainWindow::StartClip()
{
	SetEnable(0);
	m_canvas->m_state = Canvas::State::ClipStart;
	UpdateLog();
}

void MainWindow::DeleteCur()
{
	m_painter->Delete(ui.CurrentID->currentText().toInt());
	UpdateID();
	m_painter->DrawAll();
	m_canvas->update();
	UpdateLog(u8"删除成功！");
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
