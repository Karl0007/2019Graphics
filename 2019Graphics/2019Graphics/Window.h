#pragma once

#include <QWidget>
#include "ui_Window.h"

class Window : public QWidget
{
	Q_OBJECT

public:
	Window(QWidget *parent = Q_NULLPTR);
	~Window();

private:
	Ui::Window ui;
};
