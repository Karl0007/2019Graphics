#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	m_canvas = new Canvas(this,m_painter = new Painter(100, 100));
	//for (float i = 0; i < 6.28; i += 0.05) {
	//	m_painter->DrawLine(rand(), 50, 50, 50 + sinf(i) * 30, 50 + cos(i) * 30, false);
	//}
	m_painter->DrawLine(1, 50, 50, 70, 70, false)->Scale(70,70,-1,1);
	m_painter->DrawEllipse(2, 50, 50, 40, 10)->Rotate(50,50,45)->Scale(0,0,0.5,0.5);
	m_painter->DrawPolygon(2, { 44,22,73,30,74,77,59,56,32,63,25,49}, false)->Rotate(50, 50, 45)->Scale(0, 0, 0.5, 0.5);

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

