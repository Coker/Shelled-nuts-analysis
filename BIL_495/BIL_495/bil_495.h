#ifndef BIL_495_H
#define BIL_495_H

#include <QtGui/QMainWindow>
#include <QFileDialog>
#include "ui_bil_495.h"

class BIL_495 : public QMainWindow
{
	Q_OBJECT

public:
	BIL_495(QWidget *parent = 0, Qt::WFlags flags = 0);
	~BIL_495();

public slots:
	void slottt(void);
	void slottt2(void);
	void slottt3(void);
	void slottt4(void);

private:
	Ui::BIL_495Class ui;
	QFileDialog dialog;
};

#endif // BIL_495_H
