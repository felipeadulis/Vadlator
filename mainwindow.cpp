#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "dstbin.h"
#include<QMessageBox>
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

    connect(ui->newLine, &QPushButton::clicked, this, &MainWindow::newLine);
    newLine();

}

MainWindow::~MainWindow()
{
    delete ui;
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
    if((arg1 == Qt::Checked) && !(inputValuesChanged)) ui->med->setText(brasil.toString(dstBin1.getMed(), 'f', 2));
    else ui->med->clear();
}


void MainWindow::on_varCheck_checkStateChanged(const Qt::CheckState &arg1)
{
    if((arg1 == Qt::Checked) && !(inputValuesChanged)) ui->var->setText(brasil.toString(dstBin1.getVar(), 'f', 2));
    else ui->var->clear();
}


void MainWindow::on_desvPadCheck_checkStateChanged(const Qt::CheckState &arg1)
{
    if((arg1 == Qt::Checked) && !(inputValuesChanged)) ui->desvPad->setText(brasil.toString(dstBin1.getDesvPad(), 'f', 2));
    else ui->desvPad->clear();
}


void MainWindow::on_cvCheck_checkStateChanged(const Qt::CheckState &arg1)
{
    if((arg1 == Qt::Checked) && !(inputValuesChanged)) ui->cv->setText(brasil.toString(dstBin1.getCv(), 'f', 2));
    else ui->cv->clear();
}


void MainWindow::on_pushButton_clicked()
{
    if((ui->tentativas->text()!="") && (ui->sucessos->text()!=""))
    {
        if(brasil.toInt(ui->sucessos->text())==0)
        {
            QMessageBox::warning(this, "Aviso", "Divisão por zero!");
        }
        else if (brasil.toInt(ui->sucessos->text()) > brasil.toInt(ui->tentativas->text()))
        {
            QMessageBox::warning(this, "Aviso", "Número de sucessos maior que número de tentativas!");
        }
        else
        {
            refreshDstBinValues();
            inputValuesChanged = 0;
        }
    }

}


void MainWindow::on_tentativas_textEdited(const QString &arg1)
{
    clearDstBinValues();
    inputValuesChanged = 1;

}


void MainWindow::on_sucessos_textEdited(const QString &arg1)
{
    clearDstBinValues();
    inputValuesChanged = 1;
}


void MainWindow::newLine()
{
    QWidget* container = new QWidget(this);
    QHBoxLayout* layout = new QHBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);

    //Botão para remover esta linha
    QPushButton* deleteLineBtn = new QPushButton("-");
    deleteLineBtn->setFixedWidth(25);

    QLineEdit* minimo = new QLineEdit();
    minimo->setPlaceholderText("Mínimo");

    QComboBox* comparacao = new QComboBox();
    comparacao->addItems({"Exatamente", "No mínimo", "Mais de", "No máximo",
                          "Menos de", "Entre - e", "De - a"});

    QLineEdit* maximo = new QLineEdit();
    maximo->setPlaceholderText("Máximo");

    layout->addWidget(deleteLineBtn);
    layout->addWidget(minimo);
    layout->addWidget(comparacao);
    layout->addWidget(maximo);

    // Adiciona o container ao layout principal
    ui->verticalLayoutProb->addWidget(container);

    // Quando clicar no botão "-", remover essa linha
    connect(deleteLineBtn, &QPushButton::clicked, this, [this, container]() {
        deleteLine(container);
    });
}

void MainWindow::deleteLine(QWidget* container)
{
    ui->verticalLayoutProb->removeWidget(container);
    container->deleteLater();
}


