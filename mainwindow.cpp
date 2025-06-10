#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include<QMessageBox>
#include <QIntValidator>
#include <QRegularExpressionValidator>
#include<QLocale>
#include <QString>


QLocale brasil(QLocale::Portuguese, QLocale::Brazil);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Começa com binomial
    ui->stackedWidget->setCurrentIndex(0);

    ////Start binomial
    intValidator = new QIntValidator(0,9999,this);
    ui->tentativas->setValidator(intValidator);
    ui->sucessos->setValidator(intValidator);

    connect(ui->newLine, &QPushButton::clicked, this, &MainWindow::newLine);
    newLine();

    ////Start Poisson
    QRegularExpression db("^\\d{1,6}([,]\\d{1,2})?$");
    QValidator *doubleValidator = new QRegularExpressionValidator(db, this);
    ui->lambda->setValidator(doubleValidator);
    ui->nPoi->setValidator(intValidator);

    ui->nLabelPoi->hide();
    ui->nPoi->hide();

    connect(ui->newLinePoi, &QPushButton::clicked, this, &MainWindow::newLinePoi);
    newLinePoi();

}

MainWindow::~MainWindow()
{
    delete ui;
}


////////////////////Funções distribuição binomial/////////////////////////////////////////


void MainWindow::clearDstBinValues()
{
    ui->med->clear();
    ui->var->clear();
    ui->desvPad->clear();
    ui->cv->clear();

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

    if (grafico)
    {
        ui->layoutGrafico->removeWidget(grafico);
        grafico->deleteLater();
        grafico = nullptr;
    }

}

void MainWindow::refreshDstBinValues()
{
    dstBin1.setN(brasil.toInt(ui->tentativas->text()));
    dstBin1.setSuc(brasil.toInt(ui->sucessos->text()));

    dstBin1.calc();

    if (ui->medCheck->isChecked()) ui->med->setText(brasil.toString(dstBin1.getMed(), 'f', 2));
    if (ui->varCheck->isChecked()) ui->var->setText(brasil.toString(dstBin1.getVar(), 'f', 2));
    if (ui->desvPadCheck->isChecked()) ui->desvPad->setText(brasil.toString(dstBin1.getDesvPad(), 'f', 2));
    if (ui->cvCheck->isChecked()) ui->cv->setText(brasil.toString(dstBin1.getCv(), 'f', 2)+ " %");
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
    if((arg1 == Qt::Checked) && !(inputValuesChanged)) ui->cv->setText(brasil.toString(dstBin1.getCv(), 'f', 2) + "%");
    else ui->cv->clear();
}

void MainWindow::on_grafCheck_checkStateChanged(const Qt::CheckState &arg1)
{
    if (inputValuesChanged == 0)
    {
        if((arg1 == Qt::Checked))
        {
            if(grafico) grafico->show();
            else
            {
                grafico = newChart([this](int x) {return dstBin1.getP(x);}, brasil.toInt(ui->tentativas->text()));
                ui->layoutGrafico->addWidget(grafico);
            }
        }
        else if (grafico) grafico->hide();
    }
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

        if (ui->grafCheck->isChecked() && grafico == nullptr)
            {
                grafico = newChart([this](int x) {return dstBin1.getP(x);}, brasil.toInt(ui->tentativas->text()));
                ui->layoutGrafico->addWidget(grafico);
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
            resultado->setText(brasil.toString((dstBin1.getP(valorMinimo))*100, 'f', 2) + " %");
            break;

        case 1: //No mínimo
            for(int x = valorMinimo; x <= n; x++) somaProb += dstBin1.getP(x);
            resultado->setText(brasil.toString(somaProb*100, 'f', 2) + " %");
            break;

        case 2: //Mais de
            for(int x = valorMinimo+1; x <= n; x++) somaProb += dstBin1.getP(x);
            resultado->setText(brasil.toString(somaProb*100, 'f', 2) + " %");
            break;

        case 3: //No máximo
            for(int x = 0; x <= valorMinimo; x++) somaProb += dstBin1.getP(x);
            resultado->setText(brasil.toString(somaProb*100, 'f', 2) + " %");
            break;

        case 4: //Menos de
            for(int x = 0; x <= valorMinimo-1; x++) somaProb += dstBin1.getP(x);
            resultado->setText(brasil.toString(somaProb*100, 'f', 2) + " %");
            break;

        case 5: //De - a
            if (valorMinimo > valorMaximo)
            {
                int temp = valorMinimo;
                valorMinimo = valorMaximo;
                valorMaximo = temp;
            }
            for(int x = valorMinimo; x <= valorMaximo; x++) somaProb += dstBin1.getP(x);
            resultado->setText(brasil.toString(somaProb*100, 'f', 2) + " %");
            break;

        case 6: //Entre e
            if (valorMinimo > valorMaximo)
            {
                int temp = valorMinimo;
                valorMinimo = valorMaximo;
                valorMaximo = temp;
            }
            for(int x = valorMinimo+1; x <= valorMaximo-1; x++) somaProb += dstBin1.getP(x);
            resultado->setText(brasil.toString(somaProb*100, 'f', 2) + " %");
            break;

        }

    }

}


void MainWindow::on_tentativas_textEdited(const QString &arg1)
{
    clearDstBinValues();
}


void MainWindow::on_sucessos_textEdited(const QString &arg1)
{
    clearDstBinValues();
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



///////////////Troca binomial <-> possion///////////////////////////

void MainWindow::on_tipoDst_currentIndexChanged(int index)
{
    ui->stackedWidget->setCurrentIndex(index);
    if (index == 1)
    {
        clearDstPoiValues();
        ui->tentativas->clear();
        ui->sucessos->clear();
        ui->nLabelPoi->hide();
        ui->nPoi->hide();
        ui->nPoi->setText("10");
        dstPoi1.setN(10);

        ui->medCheckPoi->setChecked(0);
        ui->varCheckPoi->setChecked(0);
        ui->desvPadCheckPoi->setChecked(0);
        ui->cvCheckPoi->setChecked(0);
        ui->grafCheckPoi->setChecked(0);
    }
    else
    {
        ui->lambda->clear();
        clearDstBinValues();

        ui->medCheck->setChecked(0);
        ui->varCheck->setChecked(0);
        ui->desvPadCheck->setChecked(0);
        ui->cvCheck->setChecked(0);
        ui->grafCheck->setChecked(0);
    }

}

///////////////////Funções Poisson//////////////////////////

void MainWindow::clearDstPoiValues()
{
    ui->medPoi->clear();
    ui->varPoi->clear();
    ui->desvPadPoi->clear();
    ui->cvPoi->clear();

    inputValuesChangedPoi = 1;

    if (containersPoi.size() > 1)
    {
        for(int i = containersPoi.size()-1; i >= 1; i--)
        {
            deleteLinePoi(containersPoi.at(i));
        }
    }

    QWidget* container = containersPoi.at(0);
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

    if (grafico)
    {
        ui->layoutGrafico->removeWidget(grafico);
        grafico->deleteLater();
        grafico = nullptr;
    }
}

void MainWindow::refreshDstPoiValues()
{
    dstPoi1.setLambMedVar(brasil.toFloat(ui->lambda->text()));

    dstPoi1.calc();

    if (ui->medCheckPoi->isChecked()) ui->medPoi->setText(brasil.toString(dstPoi1.getLambMedVar(), 'f', 2));
    if (ui->varCheckPoi->isChecked()) ui->varPoi->setText(brasil.toString(dstPoi1.getLambMedVar(), 'f', 2));
    if (ui->desvPadCheckPoi->isChecked()) ui->desvPadPoi->setText(brasil.toString(dstPoi1.getDesvPad(), 'f', 2));
    if (ui->cvCheckPoi->isChecked()) ui->cvPoi->setText(brasil.toString(dstPoi1.getCv(), 'f', 2)+ " %");
}


void MainWindow::on_medCheckPoi_checkStateChanged(const Qt::CheckState &arg1)
{
    if((arg1 == Qt::Checked) && !(inputValuesChangedPoi)) ui->medPoi->setText(brasil.toString(dstPoi1.getLambMedVar(), 'f', 2));
    else ui->medPoi->clear();
}


void MainWindow::on_varCheckPoi_checkStateChanged(const Qt::CheckState &arg1)
{
    if((arg1 == Qt::Checked) && !(inputValuesChangedPoi)) ui->varPoi->setText(brasil.toString(dstPoi1.getLambMedVar(), 'f', 2));
    else ui->varPoi->clear();
}


void MainWindow::on_desvPadCheckPoi_checkStateChanged(const Qt::CheckState &arg1)
{
    if((arg1 == Qt::Checked) && !(inputValuesChangedPoi)) ui->desvPadPoi->setText(brasil.toString(dstPoi1.getDesvPad(), 'f', 2));
    else ui->desvPadPoi->clear();

}


void MainWindow::on_cvCheckPoi_checkStateChanged(const Qt::CheckState &arg1)
{
    if((arg1 == Qt::Checked) && !(inputValuesChangedPoi)) ui->cvPoi->setText(brasil.toString(dstPoi1.getCv(), 'f', 2)+" %");
    else ui->cvPoi->clear();

}

void MainWindow::on_grafCheckPoi_checkStateChanged(const Qt::CheckState &arg1)
{
    if(arg1 == Qt::Checked)
    {
        ui->nLabelPoi->show();
        ui->nPoi->show();
    }

    else
    {
        ui->nLabelPoi->hide();
        ui->nPoi->hide();
    }

    if (inputValuesChangedPoi == 0)
    {
        if (arg1 == Qt::Checked)
        {
            if (grafico) grafico->show();
            else
            {
                grafico = newChart([this](int x) { return dstPoi1.getP(x); }, brasil.toInt(ui->nPoi->text()));
                ui->layoutGrafico->addWidget(grafico);
            }
        }
        else if (grafico) grafico->hide();
    }

}


void MainWindow::on_pushButtonPoi_clicked()
{
    if(ui->lambda->text()=="")
    {
        QMessageBox::warning(this, "Aviso", "Digite a média de eventos por intervalo!");
    }
    else
    {
        refreshDstPoiValues();
        inputValuesChangedPoi = 0;

        for(int i=0; i < containersPoi.size(); i++)
        {
            refreshProbPoi(i);
        }

        if (ui->grafCheckPoi->isChecked() && grafico == nullptr)
        {
            grafico = newChart([this](int x) {return dstPoi1.getP(x);}, brasil.toInt(ui->nPoi->text()));
            ui->layoutGrafico->addWidget(grafico);
        }
    }


}

void MainWindow::refreshProbPoi(int i)
{
    QWidget* container = containersPoi.at(i);
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
            resultado->setText(brasil.toString((dstPoi1.getP(valorMinimo))*100, 'f', 2) + " %");
            break;

        case 1: //No mínimo
            for(int x = 0; x <= valorMinimo-1; x++) somaProb += dstPoi1.getP(x);
            resultado->setText(brasil.toString((1-somaProb)*100, 'f', 2) + " %");
            break;

        case 2: //Mais de
            for(int x = 0; x <= valorMinimo; x++) somaProb += dstPoi1.getP(x);
            resultado->setText(brasil.toString((1-somaProb)*100, 'f', 2) + " %");
            break;

        case 3: //No máximo
            for(int x = 0; x <= valorMinimo; x++) somaProb += dstPoi1.getP(x);
            resultado->setText(brasil.toString(somaProb*100, 'f', 2) + " %");
            break;

        case 4: //Menos de
            for(int x = 0; x <= valorMinimo-1; x++) somaProb += dstPoi1.getP(x);
            resultado->setText(brasil.toString(somaProb*100, 'f', 2) + " %");
            break;

        case 5: //De - a
            if (valorMinimo > valorMaximo)
            {
                int temp = valorMinimo;
                valorMinimo = valorMaximo;
                valorMaximo = temp;
            }
            for(int x = valorMinimo; x <= valorMaximo; x++) somaProb += dstPoi1.getP(x);
            resultado->setText(brasil.toString(somaProb*100, 'f', 2) + " %");
            break;

        case 6: //Entre e
            if (valorMinimo > valorMaximo)
            {
                int temp = valorMinimo;
                valorMinimo = valorMaximo;
                valorMaximo = temp;
            }
            for(int x = valorMinimo+1; x <= valorMaximo-1; x++) somaProb += dstPoi1.getP(x);
            resultado->setText(brasil.toString(somaProb*100, 'f', 2) + " %");
            break;

        }

    }

}

void MainWindow::newLinePoi()
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

    int i = containersPoi.size();
    connect(minimo, &QLineEdit::editingFinished, this, [this, minimo, i, resultado]()
            {
                if (inputValuesChangedPoi == 0) refreshProbPoi(i);
                dstPoi1.calcP(inputValuesChangedPoi, brasil.toInt(minimo->text()));
            });

    connect(minimo, &QLineEdit::textEdited, this, [this, resultado]()
            {
                resultado->clear();
            });

    connect(maximo, &QLineEdit::editingFinished, this, [this, maximo, i, resultado]()
            {
                if (inputValuesChangedPoi == 0) refreshProbPoi(i);
                dstPoi1.calcP(inputValuesChangedPoi, brasil.toInt(maximo->text()));
            });

    connect(maximo, &QLineEdit::textEdited, this, [this, resultado]()
            {
                resultado->clear();
            });

    containersPoi.append(container);

    layout->addWidget(deleteLineBtn);
    layout->addWidget(comparacao);
    layout->addWidget(minimo);
    layout->addWidget(comparacao2);
    layout->addWidget(maximo);
    layout->addWidget(resultado);

    maximo->hide();
    comparacao2->hide();

    // Adiciona o container ao layout principal
    ui->verticalLayoutProbPoi->addWidget(container);


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

void MainWindow::deleteLinePoi(QWidget* container)
{
    ui->verticalLayoutProbPoi->removeWidget(container);
    containersPoi.removeOne(container);
    container->deleteLater();
}





void MainWindow::on_lambda_textEdited(const QString &arg1)
{
    clearDstPoiValues();
}


void MainWindow::on_nPoi_editingFinished()
{
    dstPoi1.calcP(inputValuesChangedPoi, brasil.toInt(ui->nPoi->text()));
    if(inputValuesChangedPoi == 0)
    {
        grafico = newChart([this](int x) {return dstPoi1.getP(x);}, brasil.toInt(ui->nPoi->text()));
        ui->layoutGrafico->addWidget(grafico);
    }
}

void MainWindow::on_nPoi_textEdited(const QString &arg1)
{
    if(inputValuesChangedPoi == 0 && grafico)
    {
        ui->layoutGrafico->removeWidget(grafico);
        grafico->deleteLater();
        grafico = nullptr;
    }
}

QChartView* MainWindow::newChart(std::function<double(int)> getP, int n)
{
    QBarSet *set = new QBarSet("Probabilidades");

    QStringList categories;
    for (int i = 0; i <= n; i++) {
        *set << (getP(i) * 100.0);
        categories << QString::number(i);
    }

    QBarSeries *series = new QBarSeries();
    series->append(set);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Distribuição Discreta");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 100);
    axisY->setLabelFormat("%.2f %%");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    return chartView;
}





