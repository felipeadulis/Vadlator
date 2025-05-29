#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "dstbin.h"
#include <QIntValidator>
#include <QRegularExpressionValidator>
#include<QLocale>
#include <QString>

QLocale brasil(QLocale::Portuguese, QLocale::Brazil);

dstBin dstBin1;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QIntValidator *intValidator = new QIntValidator(0,99999,this);

    ui->tentativas->setValidator(intValidator);
    ui->sucessos->setValidator(intValidator);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_tentativas_editingFinished()
{
    if(ui->tentativas->text()!="")
    {
        if(ui->sucessos->text()!="") refreshDstBinValues();
    }
    else clearDstBinValues();
}


void MainWindow::on_sucessos_editingFinished()
{
    if(ui->sucessos->text()!="")
    {
        if(ui->tentativas->text()!="") refreshDstBinValues();
    }
    else clearDstBinValues();
}

void MainWindow::clearDstBinValues()
{
    ui->med->clear();
    ui->var->clear();
    ui->desvPad->clear();
    ui->cv->clear();
}

void MainWindow::refreshDstBinValues()
{
    dstBin1.setN(brasil.toInt(ui->tentativas->text()));
    dstBin1.setSuc(brasil.toInt(ui->sucessos->text()));

    dstBin1.calc();

    if (ui->medCheck->isChecked()) ui->med->setText(brasil.toString(dstBin1.getMed(), 'f', 2));
    if (ui->varCheck->isChecked()) ui->var->setText(brasil.toString(dstBin1.getVar(), 'f', 2));
    if (ui->desvPadCheck->isChecked()) ui->desvPad->setText(brasil.toString(dstBin1.getDesvPad(), 'f', 2));
    if (ui->cvCheck->isChecked()) ui->cv->setText(brasil.toString(dstBin1.getCv(), 'f', 2));
}


void MainWindow::on_medCheck_checkStateChanged(const Qt::CheckState &arg1)
{
    arg1 == Qt::Checked ? ui->med->setText(brasil.toString(dstBin1.getMed(), 'f', 2)) : ui->med->clear();
}


void MainWindow::on_varCheck_checkStateChanged(const Qt::CheckState &arg1)
{
    arg1 == Qt::Checked ? ui->var->setText(brasil.toString(dstBin1.getVar(), 'f', 2)) : ui->var->clear();
}


void MainWindow::on_desvPadCheck_checkStateChanged(const Qt::CheckState &arg1)
{
    arg1 == Qt::Checked ? ui->desvPad->setText(brasil.toString(dstBin1.getDesvPad(), 'f', 2)) : ui->desvPad->clear();
}


void MainWindow::on_cvCheck_checkStateChanged(const Qt::CheckState &arg1)
{
    arg1 == Qt::Checked ? ui->cv->setText(brasil.toString(dstBin1.getCv(), 'f', 2)) : ui->cv->clear();
}


void MainWindow::on_pushButton_clicked()
{
    if((ui->tentativas->text()!="") && (ui->tentativas->text()!="") && (brasil.toInt(ui->tentativas->text())))
    {
        refreshDstBinValues();
    }

}

