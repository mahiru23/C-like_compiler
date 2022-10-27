#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include "QDir"
#include "QFileDialog"
#include "QTextStream"
#include "QFile"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("中间代码生成器");

    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(InputFile()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(DoGra()));

    connect(&(GA.lr),SIGNAL(OutStr1(QString,QString)),this,SLOT(out_str1(QString,QString)));
    connect(&GA,SIGNAL(OutStr2(QString)),this,SLOT(out_str2(QString)));
    connect(&GA,SIGNAL(OutStr3(QString)),this,SLOT(out_str3(QString)));
    connect(&GA,SIGNAL(OutStr4(QString)),this,SLOT(out_str4(QString)));




}

MainWindow::~MainWindow()
{
    delete ui;
}



/*从文件夹中选择文件，并将其内容导入plain text edit和文件str*/
void MainWindow::InputFile()
{
    QString path=QDir::toNativeSeparators(QFileDialog::getOpenFileName(this,tr("Save path"),QDir::currentPath()));  //文件路径
        if(!path.isEmpty())
        {
            QFile *file=new QFile;   //申请一个文件指针
            file->setFileName(path);   //设置文件路径
            bool ok=file->open(QIODevice::ReadOnly);
                if(ok)
                {
                    QTextStream in(file);
                    ui->plainTextEdit->clear();
                    ui->plainTextEdit->appendPlainText(in.readAll());
                    file->close();
                }
        }
}

void MainWindow::DoGra()
{
    GA.CreateGrammerAnalyser();
    ui->textBrowser->clear();
    ui->textBrowser_2->clear();
    ui->textBrowser_3->clear();
    ui->textBrowser_4->clear();
    ui->textBrowser_5->clear();


    GA.lr.strFile=ui->plainTextEdit->toPlainText();
    GA.lr.strToken = "";
    GA.lr.chPoint = 0;
    GA.lr.IdPoint = 0;
    GA.lr.ConstPoint = 0;
    GA.lr.AnalyseSuccess = 0;
    GA.lr.StrLength = 0;
    GA.lr.EndFile=GA.lr.strFile.length();

    GA.lr.PreAnalyse();
    ui->textBrowser->setText(GA.lr.strFile);

    GA.lr.strToken = "";
    GA.lr.chPoint = 0;
    GA.lr.IdPoint = 0;
    GA.lr.ConstPoint = 0;
    GA.lr.AnalyseSuccess = 0;
    GA.lr.StrLength = 0;
    GA.lr.EndFile=GA.lr.strFile.length();

   // ui->textBrowser->clear();

    while (1)
    {
        GA.lr.strToken = "";
        GA.lr.Analyse();
        if (GA.lr.chPoint >= GA.lr.EndFile||GA.lr.AnalyseSuccess==1)
            break;
    }

    GA.ip=0;
    GA.nextquad=GA.start_pos;
    GA.inter_var=0;
    GA.vtb_place=0;

    LinkList QL_pass1;
    LinkList QL_pass2;
    NoPass NUM_pass;//用于表达式等的传递序号
    GA.QL_pass1=&QL_pass1;
    GA.QL_pass2=&QL_pass2;
    GA.NUM_pass=&NUM_pass;

    GA.func_new=1;
    GA.func_num=0;
    GA.label_num=0;
    GA.whe_use_func=0;

    file1.setFileName("inter_code.txt");
    file2.setFileName("assembly.asm");
    file1.open(QIODevice::WriteOnly |QIODevice::Text| QFile::Truncate);
    file2.open(QIODevice::WriteOnly |QIODevice::Text| QFile::Truncate);



    GA.P("Program",1);
    GA.print_all_Quaternion();
    GA.gen_mips();

    file1.close();
    file2.close();

}




void MainWindow::out_str1(QString str1,QString str2)
{
    QString ss="";
    ss+="<$";
    ss+=str1;
    ss+=" , ";
    ss+=str2;
    ss+=">\n";
    ui->textBrowser_2->insertPlainText(ss);
}


void MainWindow::out_str2(QString str)
{
    str+='\n';
    ui->textBrowser_3->insertPlainText(str);
}

void MainWindow::out_str3(QString str)
{
    //str+='\n';
    QTextStream stream(&file1);
    stream<<str;

    ui->textBrowser_4->insertPlainText(str);
}

void MainWindow::out_str4(QString str)
{
    //str+='\n';
    QTextStream stream(&file2);
    stream<<str;
    ui->textBrowser_5->insertPlainText(str);
}


