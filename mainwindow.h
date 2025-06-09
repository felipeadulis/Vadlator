#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QtCharts>
#include<QChartView>
#include<QBarSet>
#include<QBarSeries>
#include <QList>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_medCheck_checkStateChanged(const Qt::CheckState &arg1);

    void on_varCheck_checkStateChanged(const Qt::CheckState &arg1);

    void on_desvPadCheck_checkStateChanged(const Qt::CheckState &arg1);

    void on_cvCheck_checkStateChanged(const Qt::CheckState &arg1);

    void on_pushButton_clicked();

    void on_tentativas_textEdited(const QString &arg1);

    void on_sucessos_textEdited(const QString &arg1);

    void on_tipoDst_currentIndexChanged(int index);

    void on_medCheckPoi_checkStateChanged(const Qt::CheckState &arg1);

    void on_varCheckPoi_checkStateChanged(const Qt::CheckState &arg1);

    void on_desvPadCheckPoi_checkStateChanged(const Qt::CheckState &arg1);

    void on_cvCheckPoi_checkStateChanged(const Qt::CheckState &arg1);

    void on_pushButtonPoi_clicked();

    void on_grafCheckPoi_checkStateChanged(const Qt::CheckState &arg1);

    void on_lambda_textEdited(const QString &arg1);

    void on_nPoi_editingFinished();

private:
    Ui::MainWindow *ui;

    bool inputValuesChanged = 1;

    QVector<QWidget*> containers;

    void newLine();
    void deleteLine(QWidget* container);

    void refreshProb(int i);

    void clearDstBinValues();

    void refreshDstBinValues();

    QIntValidator *intValidator;

    //distribuição Poisson
    bool inputValuesChangedPoi = 1;
    QVector<QWidget*> containersPoi;


    void clearDstPoiValues();

    void refreshDstPoiValues();

    void refreshProbPoi(int i);

    void newLinePoi();
    void deleteLinePoi(QWidget* container);


};
#endif // MAINWINDOW_H
