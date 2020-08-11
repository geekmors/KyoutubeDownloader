#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QString>
#include <QSettings>
#include <QLabel>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    MainWindow * setProc(QProcess * process);
    MainWindow * setYoutube_dl(QString program);

private slots:
    void on_pushButton_clicked();
    void onProcessError(QProcess::ProcessError error);
    void onProcessFinish(int exitCode, QProcess::ExitStatus exitStatus);
    void onProcessStart();
    void on_changeSavePathToolButton_clicked();

private:
    Ui::MainWindow *ui;
    MainWindow * disableUIInputs(bool disable);
    MainWindow * showError(QString error);
    MainWindow * downloadProcessStart(QString videoURL);
    MainWindow * downloadProcessEnd();
    MainWindow * loadSettings();
    MainWindow * saveSettings();
    MainWindow * showSuccess(QString message, QString title);
    QString getProcessErrorAsString(QProcess::ProcessError error);
    QString youtube_dl;
    QString video_save_path;
    QProcess * proc;

};
#endif // MAINWINDOW_H
