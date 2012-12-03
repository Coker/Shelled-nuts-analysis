/********************************************************************************
** Form generated from reading UI file 'bil_495.ui'
**
** Created: Sun 2. Dec 21:51:57 2012
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BIL_495_H
#define UI_BIL_495_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BIL_495Class
{
public:
    QWidget *centralWidget;
    QPushButton *Start;
    QLabel *original;
    QLabel *classfied;
    QFrame *line;
    QLineEdit *adress;
    QLabel *label_3;
    QLabel *label_4;
    QPushButton *browse;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *BIL_495Class)
    {
        if (BIL_495Class->objectName().isEmpty())
            BIL_495Class->setObjectName(QString::fromUtf8("BIL_495Class"));
        BIL_495Class->resize(1077, 600);
        centralWidget = new QWidget(BIL_495Class);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        Start = new QPushButton(centralWidget);
        Start->setObjectName(QString::fromUtf8("Start"));
        Start->setGeometry(QRect(400, 460, 271, 61));
        original = new QLabel(centralWidget);
        original->setObjectName(QString::fromUtf8("original"));
        original->setGeometry(QRect(10, 10, 501, 391));
        classfied = new QLabel(centralWidget);
        classfied->setObjectName(QString::fromUtf8("classfied"));
        classfied->setGeometry(QRect(550, 10, 501, 391));
        line = new QFrame(centralWidget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(520, 10, 20, 391));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);
        adress = new QLineEdit(centralWidget);
        adress->setObjectName(QString::fromUtf8("adress"));
        adress->setGeometry(QRect(330, 430, 401, 20));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(290, 430, 31, 21));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(470, 410, 111, 16));
        browse = new QPushButton(centralWidget);
        browse->setObjectName(QString::fromUtf8("browse"));
        browse->setGeometry(QRect(740, 430, 121, 21));
        BIL_495Class->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(BIL_495Class);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        BIL_495Class->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(BIL_495Class);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        BIL_495Class->setStatusBar(statusBar);

        retranslateUi(BIL_495Class);

        QMetaObject::connectSlotsByName(BIL_495Class);
    } // setupUi

    void retranslateUi(QMainWindow *BIL_495Class)
    {
        BIL_495Class->setWindowTitle(QApplication::translate("BIL_495Class", "BIL_495", 0, QApplication::UnicodeUTF8));
        Start->setText(QApplication::translate("BIL_495Class", "Start", 0, QApplication::UnicodeUTF8));
        original->setText(QApplication::translate("BIL_495Class", "original", 0, QApplication::UnicodeUTF8));
        classfied->setText(QApplication::translate("BIL_495Class", "classfied", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("BIL_495Class", "Path", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("BIL_495Class", "Please Enter the Path", 0, QApplication::UnicodeUTF8));
        browse->setText(QApplication::translate("BIL_495Class", "Browse", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class BIL_495Class: public Ui_BIL_495Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BIL_495_H
