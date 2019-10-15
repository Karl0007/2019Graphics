#pragma once

#include <QtWidgets/QMainWindow>
#include <sstream>
#include <string>
#include <iostream>
#include "ui_MainWindow.h"
#include "ui_Window.h"
#include "Canvas.h"
#include "Painter.h"
#include "Head.h"
#include "qfiledialog.h"
using std::stringstream;
using std::string;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = Q_NULLPTR);

	void ResetCanvas(int w, int h);
	void SaveCanvas(QString const &name);
	void SetColor(Byte R, Byte G, Byte B);
	void OpenFile();

private:
	void Compile(QFile &file);
	static inline string getStr(stringstream &ss) { string s; ss >> s; return s; }
	static inline int getInt(stringstream &ss) { int i; ss >> i;  return i; }
	static inline int getFloat(stringstream &ss) { float f; ss >> f;  return f; }
	static inline vector<int> getVec(stringstream &ss,int l) { 
		vector<int> tmp;
		for (int i = 0; i < l*2; i++) {
			tmp.push_back(getInt(ss));
		}
		return tmp;
	}

	Ui::Window ui;
	Canvas* m_canvas;
	Painter* m_painter;
};
