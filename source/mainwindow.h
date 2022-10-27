#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include    <QTextBlock>
#include    <QLabel>
#include "grammeranalyser.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    GrammerAnalyser GA;
    QFile file1;
    QFile file2;



public slots:
    void InputFile();

    void DoGra();

    void out_str1(QString str1,QString str2);

    void out_str2(QString str);

    void out_str3(QString str);

    void out_str4(QString str);


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
