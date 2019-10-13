#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "Canvas.h"
#include "Painter.h"
#include "Head.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = Q_NULLPTR);

	void ResetCanvas(int w, int h);
	void SaveCanvas(QString const &name);
	void SetColor(Byte R, Byte G, Byte B);

private:
	Ui::MainWindowClass ui;
	Canvas* m_canvas;
	Painter* m_painter;
};
