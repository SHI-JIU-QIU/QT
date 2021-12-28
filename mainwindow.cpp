#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "math.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    digitBTNs={{Qt::Key_0,ui->btnNum0},
            {Qt::Key_1,ui->btnNum1},
            {Qt::Key_2,ui->btnNum2},
            {Qt::Key_3,ui->btnNum3},
            {Qt::Key_4,ui->btnNum4},
            {Qt::Key_5,ui->btnNum5},
            {Qt::Key_6,ui->btnNum6},
            {Qt::Key_7,ui->btnNum7},
            {Qt::Key_8,ui->btnNum8},
            {Qt::Key_9,ui->btnNum9},};

    foreach(auto btn,digitBTNs)
        connect(btn,SIGNAL(clicked()),this,SLOT(btnNumClicked()));


//    connect(ui->btnNum0,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
//    connect(ui->btnNum1,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
//    connect(ui->btnNum2,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
//    connect(ui->btnNum3,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
//    connect(ui->btnNum4,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
//    connect(ui->btnNum5,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
//    connect(ui->btnNum6,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
//    connect(ui->btnNum7,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
//    connect(ui->btnNum8,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
//    connect(ui->btnNum9,SIGNAL(clicked()),this,SLOT(btnNumClicked()));


    connect(ui->btnPlus,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));
    connect(ui->btnMinus,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));
    connect(ui->btnMultiple,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));
    connect(ui->btnDivide,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));

    connect(ui->btnPercentage,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnSqrt,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnSquare,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnInverse,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::calculation(bool *ok)
{
    double result=0;
    if(operands.size()==1)
    {
        double operand1=operands.front().toDouble();
        result=operand1;
    }


    if(operands.size()==2&&opcodes.size()>0)
    {
        double operand1=operands.front().toDouble();
        operands.pop_front();
        double operand2=operands.front().toDouble();
        operands.pop_front();

        QString op=opcodes.front();
        opcodes.pop_front();


        if(op=="+")
        {
            result=operand1+operand2;
        }
        else if(op=="-")
        {
             result=operand1-operand2;
        }
        else if(op=="✕")
        {
            result=operand1*operand2;

        }
        else
        {
            result=operand1/operand2;

        }
        operands.push_back(QString::number(result));

        ui->statusbar->showMessage(QString("calculation is in progress:operands is %1,opcode is %2").arg(operands.size()).arg(opcodes.size()));
    }
    else
    {
        ui->statusbar->showMessage(QString("operands is %1,opcode is %2").arg(operands.size()).arg(opcodes.size()));
    }
    return QString::number(result);

}

void MainWindow::btnNumClicked()
{
    QString digit=qobject_cast<QPushButton *>(sender())->text();
    if(digit=="0"&&operand=="0")
        digit="";

    if(operand=="0"&&digit!="0")
        operand="";

    operand+=digit;

    ui->display->setText(operand);

}

void MainWindow::btnBinaryOperatorClicked()
{
    ui->statusbar->showMessage("last operand "+operand);
    QString opcode=qobject_cast<QPushButton *>(sender())->text();

    if(operand!="")
    {
        if(operands.size()==0||operands.size()==1)
        {
            operands.push_back(operand);
            operand="";
        }
    }

    if(opcodes.size()==0)
    {
        opcodes.push_back(opcode);
    }
     else if(opcodes.size()>0&&operands.size()!=2)
    {
        opcodes.pop_front();
        opcodes.push_back(opcode);
    }

     QString result=calculation();
     if(opcodes.size()==0)
        opcodes.push_back(opcode);
     ui->display->setText(result);

}

void MainWindow::btnUnaryOperatorClicked()
{
    if(operands.size()!=0&&operand!="")
    {
        operands.pop_front();
        operands.push_back(operand);
        operand="";

    }
    else if(operand!=""){
        operands.push_back(operand);
        operand="";
    }
    if(operands.size()>0)
    {
        double result=operands.front().toDouble();
        operands.pop_front();
        QString op=qobject_cast<QPushButton*>(sender())->text();

        if(op=="%")
            result/=100.0;
        else if(op=="1/x")
            result=1/result;
        else if(op=="x^2")
            result*=result;
        else if(op=="√")
            result=sqrt(result);

        ui->display->setText(QString::number(result));

        operands.push_back(QString::number(result));
    }


}


void MainWindow::on_btnPeriod_clicked()
{
    if(operand=="")
        operand="0"+qobject_cast<QPushButton *>(sender())->text();

    if(!operand.contains("."))
        operand+=qobject_cast<QPushButton *>(sender())->text();
    ui->display->setText(operand);
}

void MainWindow::on_btnDel_clicked()
{
    operand=operand.left(operand.length()-1);
    ui->display->setText(operand);
}



void MainWindow::on_btnEqual_clicked()
{
    if(operand!="")
        operands.push_back(operand);
    operand="";
    QString result=calculation();
    ui->display->setText(result);

    operands.pop_front();
}

void MainWindow::on_btnSign_clicked()
{
    if(operand!="")
    {
        if(operand.toDouble()>0)
            operand="-"+operand;
        else if(operand.toDouble()<0)
            operand=operand.right(operand.length()-1);


    }

    ui->display->setText(operand);
}

void MainWindow::on_btnClearAll_clicked()
{
    operands.clear();
    opcodes.clear();
    operand="";
    opcode="";
    ui->display->setText(operand);
}

void MainWindow::on_btnClear_clicked()
{
    operand="";
    ui->display->setText(operand);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    foreach(auto btnKey,digitBTNs.keys())
    {
        if(event->key()==btnKey)
            digitBTNs[btnKey]->animateClick(100);
    }

    if(event->key()==Qt::Key_Plus)
        ui->btnPlus->animateClick(100);
    if(event->key()==Qt::Key_Minus)
        ui->btnMinus->animateClick(100);
    if(event->key()==Qt::Key_Asterisk)
        ui->btnMultiple->animateClick(100);
    if(event->key()==Qt::Key_Slash)
        ui->btnDivide->animateClick(100);
    if(event->key()==Qt::Key_Enter)
        ui->btnEqual->animateClick(100);
    if(event->key()==Qt::Key_Period)
        ui->btnPeriod->animateClick(100);
}
