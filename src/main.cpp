#include "mainwindow.h"
#include "version.h"
#include <QApplication>

#include <QChartView>
#include <QLineSeries>

QT_CHARTS_USE_NAMESPACE

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    a.setApplicationName(VER_PRODUCTNAME_STR);
    a.setOrganizationName(VER_COMPANYNAME_STR);
    a.setOrganizationDomain(VER_COMPANYDOMAIN_STR);

    MainWindow w;
    //w.resize(800,600);
    w.show();
    return a.exec();
}
