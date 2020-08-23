#include "mainwindow.h"
#include <QFile>
#include <QApplication>
#include <QLatin1String>
#include <QString>

int main(int argc, char *argv[]){

    QApplication Application(argc, argv);
    Application.setOrganizationName("Geekmors Enterprise");
    Application.setOrganizationDomain("www.geekmors.com");
    Application.setApplicationName("KYoutube Downloader");

    MainWindow Window;
    Window.setWindowTitle("KYoutube .mp4 Downloader");

    QFile styles(":/styles/assets/styles.qss");

    if(styles.open(QFile::ReadOnly)){
        QString sstyles = QLatin1String( styles.readAll() );
        Application.setStyleSheet(sstyles);
    }

    Window.show();
    return Application.exec();
}
