#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->setFixedSize(this->width(), this->height());
	this->setWindowTitle(QString("Graphic 2019 By karl07 ")+ VERSION);
	Curve::Init();
	m_canvas = new Canvas(ui.IMG,m_painter = new Painter(360, 360),this);
	ui.CurrentID->clear();
	ui.CurrentID->addItems(m_painter->GetIDList());
	UpdateID();
	UpdateLog();
	InitButtons();
	InitShortCuts();
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
	auto tmp = ui.CurrentID->currentText();
	//std::cout << "tmp:" << tmp.toStdString() << "\n";
	ui.CurrentID->clear();
	ui.CurrentID->addItems(m_painter->GetIDList());
	ChangeID(tmp);
}

void MainWindow::UpdateLog()
{
	ui.Log->setText(Canvas::m_logInfo[int(m_canvas->m_state)]);
}

void MainWindow::UpdateLog(QString const & str)
{
	ui.Log->setText(str);
}

void MainWindow::ChangeID(QString const & str)
{
	int id = str.toInt();
	bool b = str != "Null" && m_painter->m_hash.count(id);
	for (int i = 0; i <= m_painter->m_hash.size(); i++) {
		if (ui.CurrentID->itemText(i) == str) ui.CurrentID->setCurrentIndex(i);
	}
	ui.scale->setEnabled(b);
	ui.translate->setEnabled(b);
	ui.rotate->setEnabled(b);
	ui.Delete->setEnabled(b);
	ui.Copy->setEnabled(b);
	ui.clip->setEnabled(b && m_painter->m_line.count(id));
	m_painter->m_current = b ? m_painter->m_hash[id] : nullptr;
	m_painter->DrawAll();
	m_canvas->update();
}

void MainWindow::InitButtons()
{
	QObject::connect(ui.Clean, &QPushButton::pressed, [&]() {ResetCanvas(360, 360); UpdateID(); UpdateLog(u8"清除成功！"); });
	QObject::connect(ui.Test, &QPushButton::pressed, [&]() { 	UpdateLog(u8"刷新时间:" + QString::number(TestTime()) + "ms"); });
	QObject::connect(ui.OpenFile, &QPushButton::pressed, [&]() { QString log; OpenFile(log) ? UpdateLog(u8"文件读取成功！ 每帧绘制时间：" + log) : UpdateLog(u8"文件解析失败！" + log); });
	QObject::connect(ui.SaveImg, &QPushButton::pressed, [&]() {SaveCanvas() ? UpdateLog(u8"保存成功！") : UpdateLog(u8"保存失败！"); });
	QObject::connect(ui.CurrentID, &QComboBox::currentTextChanged, this, &MainWindow::CurrentIDChange);
	QObject::connect(ui.Line, &QPushButton::pressed, this, &MainWindow::StartLine);
	QObject::connect(ui.Curve, &QPushButton::pressed, this, &MainWindow::StartCurve);
	QObject::connect(ui.Polygon, &QPushButton::pressed, this, &MainWindow::StartPolygon);
	QObject::connect(ui.Ellipse, &QPushButton::pressed, this, &MainWindow::StartEllispe);
	QObject::connect(ui.translate, &QPushButton::pressed, this, &MainWindow::StartTranslate);
	QObject::connect(ui.rotate, &QPushButton::pressed, this, &MainWindow::StartRotate);
	QObject::connect(ui.scale, &QPushButton::pressed, this, &MainWindow::StartScale);
	QObject::connect(ui.clip, &QPushButton::pressed, this, &MainWindow::StartClip);
	QObject::connect(ui.Delete, &QPushButton::pressed, this, &MainWindow::DeleteCur);
	QObject::connect(ui.Copy, &QPushButton::pressed, this, &MainWindow::CopyCur);
}

void MainWindow::InitShortCuts()
{
	addShortCuts("ctrl+r", [&]() {ResetCanvas(360, 360); UpdateID(); UpdateLog(u8"清除成功！"); });
	addShortCuts("ctrl+t", [&]() { 	UpdateLog(u8"刷新时间:" + QString::number(TestTime()) + "ms"); });
	addShortCuts("ctrl+o", [&]() { QString log; OpenFile(log) ? UpdateLog(u8"文件读取成功！ 每帧绘制时间：" + log) : UpdateLog(u8"文件解析失败！" + log); });
	addShortCuts("ctrl+s", [&]() {SaveCanvas() ? UpdateLog(u8"保存成功！") : UpdateLog(u8"保存失败！"); });
	addShortCuts("1", [&]() {this->StartLine(); });
	addShortCuts("2", [&]() {this->StartEllispe(); });
	addShortCuts("3", [&]() {this->StartPolygon(); });
	addShortCuts("4", [&]() {this->StartCurve(); });
	addShortCuts("q", [&]() {if (ui.translate->isEnabled()) this->StartTranslate(); });
	addShortCuts("w", [&]() {if (ui.rotate->isEnabled()) this->StartRotate(); });
	addShortCuts("e", [&]() {if (ui.scale->isEnabled())this->StartScale(); });
	addShortCuts("r", [&]() {if (ui.clip->isEnabled())this->StartClip(); });
	addShortCuts("ctrl+c", [&]() {if (ui.Copy->isEnabled())this->CopyCur(); });
	addShortCuts("ctrl+d", [&]() {if (ui.Delete->isEnabled())this->DeleteCur(); });
}

void MainWindow::addShortCuts(QString const & key, std::function<void(void)> fun)
{
	QShortcut *sc = new QShortcut(this);
	sc->setKey(key);
	sc->setAutoRepeat(false);
	QObject::connect(sc, &QShortcut::activated,fun);
}

void MainWindow::ChangeIDIndex(int x)
{
	int index = ui.CurrentID->currentIndex()+x;
	if (index >= 0 && index <= m_painter->m_hash.size())
		ui.CurrentID->setCurrentIndex(index);
}

void MainWindow::CurrentIDChange(QString const & str)
{
	ChangeID(str);
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
	//SetEnable(0);
	//m_canvas->m_state = Canvas::State::LinePosStart;
	//UpdateLog();
	m_canvas->SetState(Canvas::State::LinePosStart, true);
}

void MainWindow::StartPolygon()
{
	m_canvas->SetState(Canvas::State::PolygonStart, true);
}

void MainWindow::StartEllispe()
{
	m_canvas->SetState(Canvas::State::EllipsePos, true);
	m_painter->DrawAll();
	m_canvas->update();
}

void MainWindow::StartCurve()
{
	m_canvas->SetState(Canvas::State::CurveStart, true);
	m_painter->DrawAll();
	m_canvas->update();
}

void MainWindow::StartRotate()
{
	m_canvas->SetState(Canvas::State::RotateStart, true);
	m_painter->DrawAll();
	m_canvas->update();
}

void MainWindow::StartScale()
{
	m_canvas->SetState(Canvas::State::ScaleStart, true);
	m_painter->DrawAll();
	m_canvas->update();
}

void MainWindow::StartTranslate()
{
	m_canvas->SetState(Canvas::State::TranslateStart, true);
	m_painter->DrawAll();
	m_canvas->update();
}

void MainWindow::StartClip()
{
	m_canvas->SetState(Canvas::State::ClipStart, true);
	m_painter->DrawAll();
	m_canvas->update();
}

void MainWindow::DeleteCur()
{
	m_painter->Delete(ui.CurrentID->currentText().toInt());
	UpdateID();
	m_painter->DrawAll();
	m_canvas->update();
	UpdateLog(u8"删除成功！");
}

void MainWindow::CopyCur()
{
	m_canvas->Copy(m_painter->m_current);
	StartTranslate();
	UpdateID();
	m_painter->DrawAll();
	m_canvas->update();
}

void MainWindow::SetEnable(bool b)
{
	//ui.Bezier->setEnabled(b);
	//ui.Blue->setEnabled(b);
	//ui.DDA->setEnabled(b);
	//ui.Clean->setEnabled(b);
	//ui.clip->setEnabled(b);
	//ui.Cohen->setEnabled(b);
	//ui.CurrentID->setEnabled(b);
	//ui.Curve->setEnabled(b);
	//ui.Delete->setEnabled(b);
	//ui.Ellipse->setEnabled(b);
	//ui.Green->setEnabled(b);
	//ui.Line->setEnabled(b);
	//ui.OpenFile->setEnabled(b);
	//ui.Polygon->setEnabled(b);
	//ui.Red->setEnabled(b);
	//ui.rotate->setEnabled(b);
	//ui.SaveImg->setEnabled(b);
	//ui.scale->setEnabled(b);
	//ui.translate->setEnabled(b);
	//if (b) CurrentIDChange(ui.CurrentID->currentText());
}
