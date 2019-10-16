#pragma once

#include <QtWidgets/QMainWindow>
#include <sstream>
#include <string>
#include <iostream>
#include "ui_Window.h"
#include "Canvas.h"
#include "Painter.h"
#include "Head.h"
#include "qfiledialog.h"
#include "qpushbutton.h"
#include "qdatetime.h"
using std::stringstream;
using std::string;

class Canvas;
class MainWindow : public QMainWindow
{
	friend class Canvas;
	Q_OBJECT

public:
	MainWindow(QWidget *parent = Q_NULLPTR);

	void ResetCanvas(int w, int h);
	bool SaveCanvas(QString const &name);
	bool SaveCanvas();
	void SetColor(Byte R, Byte G, Byte B);
	bool OpenFile(QString &log);
	int TestTime();
	void UpdateID();
	void UpdateLog();
	void UpdateLog(QString const & str);

private:
	bool Compile(QFile &file, QString &log,int &line);
	void SetEnable(bool b);


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

private slots:
	void CurrentIDChange(QString const &str);
	void StartLine();
	void StartPolygon();
	void StartEllispe();
	void StartCurve();
	void StartRotate();
	void StartScale();
	void StartTranslate();
	void StartClip();
	void DeleteCur();
};
