#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	m_canvas = new Canvas(this,m_painter = new Painter(300, 300));
	Curve::Init();
	//for (float i = 0; i < 6.28; i += 0.05) {
	//	m_painter->DrawLine(rand(), 50, 50, 50 + sinf(i) * 30, 50 + cos(i) * 30, false);
	//}
	//m_painter->DrawLine(1, 50, 50, 70, 70, false)->Scale(70,70,-1,1);
	//m_painter->DrawEllipse(2, 50, 50, 40, 10)->Rotate(50,50,45)->Scale(0,0,0.5,0.5);
	//m_painter->DrawPolygon(3, { 44,22,73,30,74,77,59,56,32,63,25,49}, false)->Rotate(50, 50, 45)->Scale(0, 0, 0.5, 0.5);
	//m_painter->DrawCurve(4, { 28,34,9,86,61,4,129,42 }, true);//->Rotate(50, 50, 45)->Scale(0, 0, 0.5, 0.5);
	//m_painter->DrawCurve(5, { 114,88,60,92,99,3,42,24 }, true);//->Rotate(50, 50, 45)->Scale(0, 0, 0.5, 0.5);
	//m_painter->DrawCurve(5, { 50,50,70,80,90,90 }, true);//->Rotate(50, 50, 45)->Scale(0, 0, 0.5, 0.5);
	//m_painter->SetColor(255, 0, 0);
	//m_painter->DrawPolygon(6, { 50,50,70,70,90,90 }, true);//->Rotate(50, 50, 45)->Scale(0, 0, 0.5, 0.5);
	//m_painter->DrawCurve(4, { 20,30,20,30,20,30,50,50,80,30,80,30,80,30 }, false)->Translate(30,30)->Scale(0,0,2,2);//->Rotate(50, 50, 45)->Scale(0, 0, 0.5, 0.5);
	m_painter->SetColor(0, 255, 0);
	//m_painter->DrawPolygon(3, { 20,30,20,30,20,30,50,50,80,30,80,30,80,30 }, false);//->Rotate(50, 50, 45)->Scale(0, 0, 0.5, 0.5);
	m_painter->DrawLine(1,0, 70,100 ,70, false)->Rotate(50,50,0);
	m_painter->SetClip(1, 60, 60, 80, 80, false);
	
}


void MainWindow::ResetCanvas(int w, int h)
{
	m_canvas->Reset(w, h);
}

void MainWindow::SaveCanvas(QString const & name)
{
	m_canvas->SaveCanvas(name);
}

void MainWindow::SetColor(Byte R, Byte G, Byte B)
{
	m_painter->SetColor(R, G, B);
}

