#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMessageBox>
#include <QDebug>
#include <QLabel>
#include <QTextStream>
#include <QThread>

#include<QtCharts>
#include<QChartView>
#include<QLineSeries>

using namespace QtCharts;

//apt install libqt5serialport5-dev
//apt install libqt5charts5 libqt5charts5-dev qml-module-qtcharts

QSerialPort *serial;
auto series = new QLineSeries;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    ui->dial_v->setMaximum(0);
    ui->dial_v->setMinimum(0);
    ui->dial_v->setValue(0);

    ui->dial_a->setMaximum(0);
    ui->dial_a->setMinimum(0);
    ui->dial_a->setValue(0);

    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::gray);
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    QPalette pal_log = palette();
    pal_log.setColor(QPalette::Base, Qt::black); // BG
    pal_log.setColor(QPalette::Text, Qt::white); // (plain) TEXT
    ui->txtlog->setPalette(pal_log);

    QPalette palette_tabs;
    ui->tabWidget->setAutoFillBackground(true);
    palette_tabs.setColor(QPalette::Inactive,QPalette::Window ,Qt::darkBlue);
    ui->tabWidget->setPalette(palette_tabs);

    QPalette palette_volt;
    palette_volt.setColor(QPalette::Window, Qt::darkRed);
    palette_volt.setColor(QPalette::WindowText, Qt::red);

    ui->lcdVolt->setDigitCount(5);
    ui->lcdVolt->setAutoFillBackground(true);
    ui->lcdVolt->setPalette(palette_volt);
    ui->lcdVolt->setSegmentStyle(QLCDNumber::Flat);
    double v = 0;
    ui->lcdVolt->display(v);

    QPalette palette_ampere;
    palette_ampere.setColor(QPalette::Window, Qt::darkBlue);
    palette_ampere.setColor(QPalette::WindowText, Qt::blue);

    ui->lcdCurrent->setDigitCount(5);
    ui->lcdCurrent->setAutoFillBackground(true);
    ui->lcdCurrent->setPalette(palette_ampere);
    ui->lcdCurrent->setSegmentStyle(QLCDNumber::Flat);
    double a = 0;
    ui->lcdCurrent->display(a);

    getAvalilableSerialDevices();

    bar("Disconnected","NORMAL");
    ui->btn_connect->setText("Connect");

    serial = new QSerialPort(this);
    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
            SLOT(handleError(QSerialPort::SerialPortError)));

    // create a timer
    timer = new QTimer(this);

    // setup signal and slot
    connect(timer, SIGNAL(timeout()),
          this, SLOT(ChartTimer()));

    // msec
    timer->start(1000);

    DrawChart();

}

MainWindow::~MainWindow() {
    serial_close();
    delete ui;
}

void MainWindow::serial_close() {
    if (serial->isOpen()){
        QString reply = writeData("OUT OFF");
        serial->close();
    }
    timer->stop();

    ui->lbl_ar488_ver->setText("unknown");
    ui->lbl_device_Ver->setText("unknown");
    ui->lbl_rom_version->setText("unknown");
    ui->lbl_max_volt->setText("unknown");
    ui->lbl_max_ampere->setText("unknown");

}

void MainWindow::on_btn_connect_clicked()
{
    if(ui->btn_connect->text()=="Connect"){

        serial->setPortName(ui->cbserial->currentText());
        if (serial->open(QIODevice::ReadWrite)) {

            if (serial->setBaudRate(QSerialPort::Baud115200)
                    && serial->setDataBits(QSerialPort::Data8)
                    && serial->setParity(QSerialPort::NoParity)
                    && serial->setStopBits(QSerialPort::OneStop)
                    && serial->setFlowControl(QSerialPort::NoFlowControl)) {
                    serial->setDataTerminalReady(false);
                    bar(tr("Connected to %1 : %2, %3, %4, %5, %6")
                                               .arg(serial->portName()).arg(serial->baudRate()).arg(serial->dataBits())
                                               .arg(serial->parity()).arg(serial->stopBits()).arg(serial->flowControl()),"INFO");
                    ui->btn_connect->setText("Disconnect");

                    QString reply = ""; // Get Version from Arduino: e.g. AR488 GPIB controller, ver. 0.51.29, 18/03/2024

                    ui->statusbar->showMessage("Please wait! Search AR488 Controller");
                    serial->waitForReadyRead(1500);
                    reply = writeData("++ver");
                    if (reply.startsWith(("AR488"))) {

                        qDebug() << "AR488 firmware found";
                        ui->lbl_ar488_ver->setText(reply);
                        ui->statusbar->showMessage(reply);

                        reply = writeData("++addr");
                        ui->statusbar->showMessage(ui->statusbar->currentMessage() + " Adress:" + reply);

                        reply = writeData("++srqauto 1");

                        reply = writeData("++auto 2");

                        //Turn off volt output for setings
                        reply = writeData("OUT OFF");

                        reply = writeData("ID?");
                        ui->lbl_device_Ver->setText(reply);

                        reply = writeData("ROM?");
                        ui->lbl_rom_version->setText(reply);

                        //get max volt from device
                        reply = writeData("VMAX?");
                        ui->dial_v->setMaximum(get_int(reply).toDouble()*100);
                        ui->lbl_max_volt->setText(get_int(reply) + " V");

                        //get max Ampere from device
                        reply = writeData("IMAX?");
                        ui->dial_a->setMaximum(get_int(reply).toDouble()*100);
                        ui->lbl_max_ampere->setText(get_int(reply)+ " A");

                    } else {
                        qDebug() << "No AR488 Device found!";
                        bar("No AR488 Device found!","ERROR");
                    }

            } else {
                serial->close();
                QMessageBox::critical(this, tr("Error"), serial->errorString());
                bar("Open error","ERROR");
                ui->statusbar->showMessage(tr("Open error"));
            }

        } else {
            QMessageBox::critical(this, tr("Error"), serial->errorString());
            bar("Configure error","ERROR");
        }
    } else {
        serial_close();
        bar("Disconnected","NORMAL");
        ui->btn_connect->setText("Connect");
    }
}

void MainWindow::bar(QString msg, QString typ) {

    if(typ=="ERROR")
        ui->statusbar->setStyleSheet("background-color: rgb(255, 0, 0);");
    else if(typ=="INFO")
        ui->statusbar->setStyleSheet("background-color: rgb(0, 255, 0);");
    else if(typ=="NORMAL")
        ui->statusbar->setStyleSheet("background-color: rgb(255, 255, 255);");

    ui->statusbar->showMessage(msg);
}

QString MainWindow::readData() const {
    QByteArray data = "";
         while (serial->waitForReadyRead(250))
             data.append(serial->readAll());
     qDebug()<< data;
     ui->txtlog->append(data);
    return data;
}

QString MainWindow::writeData(const QString data) const {
    ui->txtlog->append(data);
    QByteArray br = data.toUpper().toUtf8();
    serial->write(br+"\n");
    qDebug()<< br;
    return readData();
}

void MainWindow::handleError(QSerialPort::SerialPortError error) {
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        serial->close();
    }
}

void MainWindow::on_pushButton_clicked()
{
    writeData(ui->txtcmd->text());
}

void MainWindow::on_dial_v_valueChanged(int value)
{
    double v = (double)((double)value / 100);
    ui->lcdVolt->display(v);
    out_off();
}

void MainWindow::on_dial_a_valueChanged(int value)
{
    double a = (double)((double)value / 100);
    ui->lcdCurrent->display(a);
    out_off();
}

void MainWindow::on_btn_out_clicked()
{
    if(ui->btn_out->text()=="OUT ON")
        out_on();
    else
        out_off();
}

void MainWindow::out_on()
{
    if (serial->isOpen()){
        QString reply = "";
        reply = writeData("VSET " + QString::number(ui->lcdVolt->value()) + "V");
        reply = writeData("ISET " + QString::number(ui->lcdCurrent->value()) + "A");
        reply = writeData("OUT ON");
        ui->btn_out->setText("OUT OFF");
    }
}

void MainWindow::out_off()
{
    if(ui->btn_out->text()=="OUT OFF"){
        if (serial->isOpen()){
            QString reply = writeData("OUT OFF");
            ui->btn_out->setText("OUT ON");
        }
    }
}

void MainWindow::on_bnt_err_clicked()
{
    QString reply = writeData("ERR?");
    qint16 err = get_int(reply).toInt();
    QString msg ="";

    if(err==0)
        msg="No Errors";
    else if(err==4)
        msg="Unrecognized Character\nImproper Number\nUnrecognized String\nSyntax Error";
    else if(err==5)
        msg="Number Out of Range";
    else if(err==6)
        msg="Attempt to Exceed Soft Limits";
    else if(err==7)
        msg="Improper Soft Limit";
    else if(err==8)
        msg="Data Requested without a Query Being Sent";
    else if(err==9)
        msg="OVP Set Below Output";
    else if(err==10)
        msg="Slave Processor Not Responding";
    else if(err==12)
        msg="Illegal Calibration";
    else
        msg="unknown error" + QString::number(err);

    qDebug() << msg;
    ui->txtlog->append(msg);
}


void MainWindow::on_bnt_failt_clicked()
{
    writeData("FAILD?");
}

//Extract Value from GIBR reply
QString MainWindow::get_int(const QString data) const {
    if(data.length()>0){
        QStringList tmp = data.split(' ');
        if(tmp.count()>0)
            return tmp[1];
        else
            return 0;
     }
     else
        return 0;
}

void MainWindow::ChartTimer()
{
    if( ui->btn_out->text()=="OUT OFF" && serial->isOpen()){
        QString reply = writeData("VOUT?");
        double val = get_int(reply).toDouble();

        QDateTime momentInTime = QDateTime::currentDateTime();
        series->append(momentInTime.toMSecsSinceEpoch(), val);

        if(series->count()>10)
            series->remove(0);

        DrawChart();
    }
}

void MainWindow::DrawChart(){

    auto chart = new QChart;
    chart->legend()->hide();
    chart->addSeries(series);
    //chart->createDefaultAxes();
    //chart->setTitle("Volt");

    auto axisX = new QDateTimeAxis;
    axisX->setTickCount(10);
    axisX->setFormat("hh:mm:ss");
    axisX->setTitleText("Time");
    chart->addAxis(axisX, Qt::AlignBottom);
    axisX->setLabelsAngle(-90);
    series->attachAxis(axisX);

    auto axisY = new QValueAxis;
    axisY->setLabelFormat("%.3f");
    axisY->setTitleText("Volt");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    ui->graphicsView->setChart(chart);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
}

void MainWindow::getAvalilableSerialDevices()
{
    qDebug() << "Number of available ports: " << QSerialPortInfo::availablePorts().length();
    ui->cbserial->clear();
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
      ui->cbserial->addItem(serialPortInfo.portName());
    }
}

void MainWindow::on_bnt_SerialDevices_clicked()
{
    getAvalilableSerialDevices();
}

