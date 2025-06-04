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

    QIntValidator *intValidator = new QIntValidator(0,9999,this);

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

            int n = brasil.toInt(ui->tentativas->text());
            for(int i=0; i < layouts.size(); i++)
            {
                QHBoxLayout* layout = layouts.at(i);
                QComboBox* comparacao = qobject_cast<QComboBox*>(layout->itemAt(1)->widget());
                uint8_t comparacaoIndex = comparacao->currentIndex();
                QLineEdit* minimo = qobject_cast<QLineEdit*>(layout->itemAt(2)->widget());
                int valorMinimo = brasil.toInt(minimo->text());
                QLineEdit* maximo = qobject_cast<QLineEdit*>(layout->itemAt(4)->widget());
                int valorMaximo = brasil.toInt(maximo->text());
                QLabel* resultado = qobject_cast<QLabel*>(layout->itemAt(5)->widget());

                float somaProb = 0;
                switch(comparacaoIndex)
                {
                case 0: //Igual a
                    resultado->setText(brasil.toString((dstBin1.getP(valorMinimo)), 'f', 2) + " %");
                    break;

                case 1: //No mínimo
                    for(int x = valorMinimo; x <= n; x++) somaProb += dstBin1.getP(x);
                    resultado->setText(brasil.toString(somaProb, 'f', 2) + " %");
                    break;

                case 2: //Mais de
                    for(int x = valorMinimo+1; x <= n; x++) somaProb += dstBin1.getP(x);
                    resultado->setText(brasil.toString(somaProb, 'f', 2) + " %");
                    break;

                case 3: //No máximo
                    for(int x = 0; x <= valorMinimo; x++) somaProb += dstBin1.getP(x);
                    resultado->setText(brasil.toString(somaProb, 'f', 2) + " %");
                    break;

                case 4: //Menos de
                    for(int x = 0; x <= valorMinimo-1; x++) somaProb += dstBin1.getP(x);
                    resultado->setText(brasil.toString(somaProb, 'f', 2) + " %");
                    break;

                case 5: //De - a
                    for(int x = valorMinimo; x <= valorMaximo; x++) somaProb += dstBin1.getP(x);
                    resultado->setText(brasil.toString(somaProb, 'f', 2) + " %");
                    break;

                case 6: //Entre e
                    for(int x = valorMinimo+1; x <= valorMaximo-1; x++) somaProb += dstBin1.getP(x);
                    resultado->setText(brasil.toString(somaProb, 'f', 2) + " %");
                    break;

                }
            }
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

    QComboBox* comparacao = new QComboBox();
    comparacao->addItems({"Igual a", "No mínimo", "Mais de", "No máximo",
                          "Menos de", "De", "Entre"});

    QLineEdit* minimo = new QLineEdit();
    //minimo->setPlaceholderText("Mínimo");

    QLabel* comparacao2 = new QLabel();

    QLineEdit* maximo = new QLineEdit();
    //maximo->setPlaceholderText("Máximo");

    QLabel* resultado = new QLabel();
    resultado->setFixedSize(70, 25);

    layouts.append(layout);

    layout->addWidget(deleteLineBtn);
    layout->addWidget(comparacao);
    layout->addWidget(minimo);
    layout->addWidget(comparacao2);
    layout->addWidget(maximo);
    layout->addWidget(resultado);

    maximo->hide();
    comparacao2->hide();

    // Adiciona o container ao layout principal
    ui->verticalLayoutProb->addWidget(container);


    connect(comparacao, &QComboBox::highlighted, this, [this, comparacao, container, maximo, comparacao2](int index) {
        if(index >= 0 && index <= 4)
        {
            maximo->hide();
            comparacao2->hide();
        }
        else if(index == 5)
        {
            maximo->show();
            comparacao2->setText("a");
            comparacao2->show();

        }
        else
        {
            maximo->show();
            comparacao2->setText("e");
            comparacao2->show();
        }
    });

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


