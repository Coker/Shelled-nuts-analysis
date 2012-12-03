#include "bil_495.h"
#include <cv.h>
#include <highgui.h>
#include <QDebug>

BIL_495::BIL_495(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	QImage ceviz, findik;
    
	ceviz.load("ceviz.jpg");
	ui.original->setPixmap(QPixmap::fromImage(ceviz));

	findik.load("findik.jpg");
	ui.classfied->setPixmap(QPixmap::fromImage(findik));
	dialog.setViewMode(QFileDialog::Detail);
	
	QObject::connect(ui.Start, SIGNAL(pressed()), this, SLOT(slottt()));
	QObject::connect(ui.Start, SIGNAL(released()), this, SLOT(slottt2()));
	QObject::connect(&dialog, SIGNAL(accepted()), this, SLOT(slottt3()));
	QObject::connect(ui.browse, SIGNAL(pressed()), this, SLOT(slottt4()));
}

BIL_495::~BIL_495()
{

}

void BIL_495::slottt(void)
{
	ui.Start->setText("basildi");	
}

void BIL_495::slottt2(void)
{
	ui.Start->setText("cekildi");
}

void BIL_495::slottt3(void)
{	
	QString str = (dialog.selectedFiles()).at(0).toLocal8Bit().constData();
	ui.adress->setText(str);
}

void BIL_495::slottt4(void)
{
	dialog.setVisible(true);
}