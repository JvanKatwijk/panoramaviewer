/********************************************************************************
** Form generated from reading UI file 'dabstick-widget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DABSTICK_2D_WIDGET_H
#define UI_DABSTICK_2D_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_dabstickWidget
{
public:
    QFrame *contents;
    QLCDNumber *rateDisplay;
    QLabel *label;
    QSpinBox *f_correction;
    QSpinBox *KhzOffset;
    QLabel *label_3;
    QLabel *label_4;
    QComboBox *rateSelector;
    QSlider *gainSlider;
    QCheckBox *agcChecker;
    QLCDNumber *showGain;

    void setupUi(QWidget *dabstickWidget)
    {
        if (dabstickWidget->objectName().isEmpty())
            dabstickWidget->setObjectName(QString::fromUtf8("dabstickWidget"));
        dabstickWidget->resize(223, 242);
        contents = new QFrame(dabstickWidget);
        contents->setObjectName(QString::fromUtf8("contents"));
        contents->setGeometry(QRect(0, 0, 221, 231));
        contents->setFrameShape(QFrame::StyledPanel);
        contents->setFrameShadow(QFrame::Raised);
        rateDisplay = new QLCDNumber(contents);
        rateDisplay->setObjectName(QString::fromUtf8("rateDisplay"));
        rateDisplay->setGeometry(QRect(0, 80, 121, 21));
        rateDisplay->setDigitCount(7);
        rateDisplay->setSegmentStyle(QLCDNumber::Flat);
        label = new QLabel(contents);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 150, 101, 21));
        f_correction = new QSpinBox(contents);
        f_correction->setObjectName(QString::fromUtf8("f_correction"));
        f_correction->setGeometry(QRect(0, 20, 91, 21));
        f_correction->setMinimum(-100);
        f_correction->setMaximum(100);
        KhzOffset = new QSpinBox(contents);
        KhzOffset->setObjectName(QString::fromUtf8("KhzOffset"));
        KhzOffset->setGeometry(QRect(0, 40, 91, 21));
        KhzOffset->setMaximum(1000000);
        label_3 = new QLabel(contents);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(100, 40, 41, 21));
        label_4 = new QLabel(contents);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(100, 20, 51, 21));
        rateSelector = new QComboBox(contents);
        rateSelector->addItem(QString());
        rateSelector->addItem(QString());
        rateSelector->addItem(QString());
        rateSelector->addItem(QString());
        rateSelector->addItem(QString());
        rateSelector->addItem(QString());
        rateSelector->setObjectName(QString::fromUtf8("rateSelector"));
        rateSelector->setGeometry(QRect(0, 100, 121, 29));
        gainSlider = new QSlider(contents);
        gainSlider->setObjectName(QString::fromUtf8("gainSlider"));
        gainSlider->setGeometry(QRect(180, 39, 20, 151));
        gainSlider->setOrientation(Qt::Vertical);
        agcChecker = new QCheckBox(contents);
        agcChecker->setObjectName(QString::fromUtf8("agcChecker"));
        agcChecker->setGeometry(QRect(154, 190, 51, 26));
        showGain = new QLCDNumber(contents);
        showGain->setObjectName(QString::fromUtf8("showGain"));
        showGain->setGeometry(QRect(170, 10, 41, 21));
        showGain->setDigitCount(2);
        showGain->setSegmentStyle(QLCDNumber::Flat);

        retranslateUi(dabstickWidget);

        QMetaObject::connectSlotsByName(dabstickWidget);
    } // setupUi

    void retranslateUi(QWidget *dabstickWidget)
    {
        dabstickWidget->setWindowTitle(QCoreApplication::translate("dabstickWidget", "Form", nullptr));
        label->setText(QCoreApplication::translate("dabstickWidget", "dabstick", nullptr));
        label_3->setText(QCoreApplication::translate("dabstickWidget", "KHz", nullptr));
        label_4->setText(QCoreApplication::translate("dabstickWidget", "ppm", nullptr));
        rateSelector->setItemText(0, QCoreApplication::translate("dabstickWidget", "2000", nullptr));
        rateSelector->setItemText(1, QCoreApplication::translate("dabstickWidget", "2500", nullptr));
        rateSelector->setItemText(2, QCoreApplication::translate("dabstickWidget", "960", nullptr));
        rateSelector->setItemText(3, QCoreApplication::translate("dabstickWidget", "1200", nullptr));
        rateSelector->setItemText(4, QCoreApplication::translate("dabstickWidget", "1536", nullptr));
        rateSelector->setItemText(5, QCoreApplication::translate("dabstickWidget", "3000", nullptr));

        agcChecker->setText(QCoreApplication::translate("dabstickWidget", "agc", nullptr));
    } // retranslateUi

};

namespace Ui {
    class dabstickWidget: public Ui_dabstickWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DABSTICK_2D_WIDGET_H
