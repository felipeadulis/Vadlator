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

    intValidator = new QIntValidator(0,9999,this);

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
    if((ui->tentativas->text()=="") || (ui->sucessos->text()==""))
    {
        QMessageBox::warning(this, "Aviso", "Digite o número de tentativas e sucessos!");
    }
    else if(brasil.toInt(ui->sucessos->text())==0)
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

        for(int i=0; i < containers.size(); i++)
        {
            refreshProb(i);
        }
    }
}

void MainWindow::refreshProb(int i)
{
    int n = brasil.toInt(ui->tentativas->text());
    QWidget* container = containers.at(i);
    QHBoxLayout* layout = qobject_cast<QHBoxLayout*>(container->layout());
    QLineEdit* minimo = qobject_cast<QLineEdit*>(layout->itemAt(2)->widget());
    QLineEdit* maximo = qobject_cast<QLineEdit*>(layout->itemAt(4)->widget());
    if (minimo->text() != "" || maximo->text() != "")
    {
        int valorMinimo = brasil.toInt(minimo->text());
        int valorMaximo = brasil.toInt(maximo->text());
        QLabel* resultado = qobject_cast<QLabel*>(layout->itemAt(5)->widget());
        QComboBox* comparacao = qobject_cast<QComboBox*>(layout->itemAt(1)->widget());
        uint8_t comparacaoIndex = comparacao->currentIndex();

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


void MainWindow::on_tentativas_textEdited(const QString &arg1)
{
    clearDstBinValues();
    inputValuesChanged = 1;

    if (containers.size() > 1)
    {
        for(int i = containers.size()-1; i >= 1; i--)
        {
            deleteLine(containers.at(i));
        }
    }

    QWidget* container = containers.at(0);
    QHBoxLayout* layout = qobject_cast<QHBoxLayout*>(container->layout());
    QComboBox* comparacao = qobject_cast<QComboBox*>(layout->itemAt(1)->widget());
    QLineEdit* minimo = qobject_cast<QLineEdit*>(layout->itemAt(2)->widget());
    QLabel* comparacao2 = qobject_cast<QLabel*>(layout->itemAt(3)->widget());
    QLineEdit* maximo = qobject_cast<QLineEdit*>(layout->itemAt(4)->widget());
    QLabel* resultado = qobject_cast<QLabel*>(layout->itemAt(5)->widget());

    comparacao->setCurrentIndex(0);
    minimo->setText("");
    maximo->setText("");
    resultado->setText("");
    comparacao2->hide();
    maximo->hide();

}


void MainWindow::on_sucessos_textEdited(const QString &arg1)
{
    clearDstBinValues();
    inputValuesChanged = 1;

    if (containers.size() > 1)
    {
        for(int i = containers.size()-1; i >= 1; i--)
        {
            deleteLine(containers.at(i));
        }
    }

    QWidget* container = containers.at(0);
    QHBoxLayout* layout = qobject_cast<QHBoxLayout*>(container->layout());
    QComboBox* comparacao = qobject_cast<QComboBox*>(layout->itemAt(1)->widget());
    QLineEdit* minimo = qobject_cast<QLineEdit*>(layout->itemAt(2)->widget());
    QLabel* comparacao2 = qobject_cast<QLabel*>(layout->itemAt(3)->widget());
    QLineEdit* maximo = qobject_cast<QLineEdit*>(layout->itemAt(4)->widget());
    QLabel* resultado = qobject_cast<QLabel*>(layout->itemAt(5)->widget());

    comparacao->setCurrentIndex(0);
    minimo->setText("");
    maximo->setText("");
    resultado->setText("");
    comparacao2->hide();
    maximo->hide();
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
    minimo->setValidator(intValidator);

    QLabel* comparacao2 = new QLabel();

    QLineEdit* maximo = new QLineEdit();
    //maximo->setPlaceholderText("Máximo");
    maximo->setValidator(intValidator);

    QLabel* resultado = new QLabel();
    resultado->setFixedSize(70, 25);

    int i = containers.size();
    connect(minimo, &QLineEdit::editingFinished, this, [this, minimo, i, resultado]()
            {
                int valor = brasil.toInt(minimo->text());
                QString n = ui->tentativas->text();
                if (n != "" && valor > brasil.toInt(n))
                {
                    QMessageBox::warning(nullptr, "Aviso", "Valor deve estar entre 0 e " + n);
                    minimo->clear();
                    resultado->clear();
                }
                else if (inputValuesChanged == 0) refreshProb(i);
            });

    connect(minimo, &QLineEdit::textEdited, this, [this, resultado]()
            {
                resultado->clear();
            });

    connect(maximo, &QLineEdit::editingFinished, this, [this, maximo, i, resultado]()
            {
                int valor = brasil.toInt(maximo->text());
                QString n = ui->tentativas->text();
                if (n != "" && valor > brasil.toInt(n))
                {
                    QMessageBox::warning(nullptr, "Aviso", "Valor deve estar entre 0 e " + n);
                    maximo->clear();
                    resultado->clear();
                }
                else if (inputValuesChanged == 0) refreshProb(i);
            });

    connect(maximo, &QLineEdit::textEdited, this, [this, resultado]()
            {
                resultado->clear();
            });

    containers.append(container);

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


    connect(comparacao, &QComboBox::highlighted, this, [this, comparacao, container, minimo, maximo, resultado, comparacao2](int index) {
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
        minimo->clear();
        maximo->clear();
        resultado->clear();
    });

    // Quando clicar no botão "-", remover essa linha
    connect(deleteLineBtn, &QPushButton::clicked, this, [this, container]() {
        deleteLine(container);
    });
}

void MainWindow::deleteLine(QWidget* container)
{
    ui->verticalLayoutProb->removeWidget(container);
    containers.removeOne(container);
    container->deleteLater();
}


