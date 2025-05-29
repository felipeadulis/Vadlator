#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QtCharts>
#include<QChartView>
#include<QBarSet>
#include<QBarSeries>

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

    void clearDstBinValues();
    void refreshDstBinValues();

private slots:
    void on_tentativas_editingFinished();

    void on_sucessos_editingFinished();

    void on_medCheck_checkStateChanged(const Qt::CheckState &arg1);

    void on_varCheck_checkStateChanged(const Qt::CheckState &arg1);

    void on_desvPadCheck_checkStateChanged(const Qt::CheckState &arg1);

    void on_cvCheck_checkStateChanged(const Qt::CheckState &arg1);

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
