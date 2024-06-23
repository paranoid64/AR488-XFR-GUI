#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QtGlobal>
#include <QMainWindow>
#include <QPushButton>
#include <QtSerialPort/QSerialPort>

#include<QtCharts>
#include<QChartView>
#include<QLineSeries>

#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();
        QString readData() const;
        QString writeData(const QString data) const;
        QString get_int(const QString data) const;

    private:
        Ui::MainWindow *ui;
        QSerialPort *serial;
        QByteArray charBuffer;
        QTimer *timer;
        void getAvalilableSerialDevices();

    private slots:
      void handleError(QSerialPort::SerialPortError error);
      void on_btn_connect_clicked();
      void on_pushButton_clicked();
      void serial_close();
      void bar(QString msg, QString typ);
      void on_dial_v_valueChanged(int value);
      void on_dial_a_valueChanged(int value);
      void on_btn_out_clicked();
      void out_on();
      void out_off();
      void on_bnt_err_clicked();
      void on_bnt_failt_clicked();
      void ChartTimer();
      void DrawChart();
      void on_bnt_SerialDevices_clicked();
};
#endif // MAINWINDOW_H
