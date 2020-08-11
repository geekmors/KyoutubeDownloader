#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "string"
#include <iostream>
#include <QDebug>
#include <QMessageBox>
#include <QSettings>
#include <QFileDialog>
#include <QMovie>

// Utility Function for formating savePath text eg. "Save Path: /some/path"
QString getNewLabelText(QString newSavePath){
    return "Save Path: "+newSavePath;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
        //retrieve the settings from file
    this->loadSettings()
        // set name of download program
        ->setYoutube_dl("youtube-dl.exe")
        //initializes new QProcess instance.
        ->setProc(new QProcess(this));

    ui->setupUi(this);
    ui->pushButton->setCursor(Qt::PointingHandCursor);
    // loads animation to be used while downloading video - loading animation
    ui->loadingLabel->setMovie(new QMovie(":/img/loading.gif"));
    ui->loadingLabel->hide();

    ui->changeSavePathToolButton->setCursor(Qt::PointingHandCursor);
    ui->savePath_label->setText( getNewLabelText(this->video_save_path) );



    connect(this->proc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(onProcessFinish(int, QProcess::ExitStatus)));
    connect(this->proc, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(onProcessError(QProcess::ProcessError)));
    connect(this->proc, SIGNAL(started()), this, SLOT(onProcessStart()));

}

MainWindow::~MainWindow(){
    this->saveSettings();
    delete ui;
}

MainWindow *MainWindow::setProc(QProcess *process){
    this->proc = process;
    return  this;
}

MainWindow *MainWindow::setYoutube_dl(QString program){
    this->youtube_dl = program;
    return this;
}

// gets settings from file to load into private variables.
 MainWindow * MainWindow::loadSettings(){
    QSettings settings(this);
    this->video_save_path = settings.value( "video_save_path", QApplication::applicationDirPath() ).toString();
    return this;
}
// saves the settings to file
MainWindow * MainWindow::saveSettings(){
    QSettings settings(this);
    settings.setValue("video_save_path", this->video_save_path);
    return this;
}
//shows a success dialogue
MainWindow * MainWindow::showSuccess(QString message, QString title){
    QMessageBox * messageBox = new QMessageBox(this);
    messageBox->setText(message);
    messageBox->setIcon(QMessageBox::Information);
    messageBox->setWindowTitle(title);
    messageBox->addButton(QMessageBox::Ok);
    messageBox->exec();
    return this;
}
//disables any inputs while video downloads
MainWindow * MainWindow::disableUIInputs(bool disable){

    ui->pushButton->setDisabled(disable);
    ui->lineEdit->setDisabled(disable);
    return this;
}
// shows an error dialogue
MainWindow * MainWindow::showError(QString error){
    QMessageBox * messageBox = new QMessageBox(this);
    messageBox->setText(error);
    messageBox->setIcon(QMessageBox::Warning);
    messageBox->setWindowTitle("Download Error");
    messageBox->addButton(QMessageBox::Ok);
    messageBox->exec();
    return this->downloadProcessEnd();
}
// begins the download process
MainWindow * MainWindow::downloadProcessStart(QString videoURL){
    ui->statusbar->showMessage(this->youtube_dl, 3000);

    //command: youtube-dl.exe ${youtubeVideoURL} -o ${videoSavePath}/videoTitle.mp4
    this->proc->start(this->youtube_dl, {videoURL, "-o", this->video_save_path+"/%(title)s.mp4"});

    return this;
}

// ends the download process by stoping the loading animation and re-enabling the window inputs.
MainWindow * MainWindow::downloadProcessEnd(){
    ui->loadingLabel->movie()->stop();
    ui->loadingLabel->hide();
    ui->lineEdit->clear();
    return this->disableUIInputs(false);
}

// Utility to get a better defined error to show when the download command fails.
QString MainWindow::getProcessErrorAsString(QProcess::ProcessError error){
    switch (error) {
        case 0:
        return QString("Failed to start");
        break;
    case 1:
        return QString("Started But Crashed");
        break;
     case 2:
        return QString("Process got tired of waiting");
        break;
     case 3:
        return QString("There was a read error");
        break;
     case 4:
        return QString("There was a write error");
        break;
      default:
        return QString("An Unknown Error Occurred");
    }
}

//Download button handler for QButton::clicked() signal
void MainWindow::on_pushButton_clicked(){

    QString videoUrl = ui->lineEdit->text();
    this->disableUIInputs(true);

    if(videoUrl.contains("https://www.youtube.com/watch?v="))
        this->downloadProcessStart(videoUrl);
    else
        this->showError("Download Error Occurred : "+ videoUrl +" "+ "not a valid youtube URL");

}
// error handler for QProcess::started() signal
void MainWindow::onProcessError(QProcess::ProcessError error){
    QString errorToString = this->getProcessErrorAsString(error);
    this->showError("Download Error Occurred :"+errorToString);
}
// finished handler for QProcess::finished() signal
void MainWindow::onProcessFinish(int exitCode, QProcess::ExitStatus exitStatus){

    if(!exitStatus && exitCode==0){
        this->showSuccess("Download Has Finished SuccessFully!", "Download Finished")
            ->downloadProcessEnd();
    }
    else
        this->showError("An Error Occurred while downloading the video");
}
// started handler for QProcess::started() signal
void MainWindow::onProcessStart(){
    ui->loadingLabel->show();
    ui->loadingLabel->movie()->start();
}

//changeSavePathToolButton click signal handler
void MainWindow::on_changeSavePathToolButton_clicked(){
    QString newSavePath = QFileDialog::getExistingDirectory(
        this,
        "Change where you save videos",
        this->video_save_path,
        QFileDialog::ShowDirsOnly
    );

    this->video_save_path = newSavePath == "" ? this->video_save_path : newSavePath;
    this->saveSettings();
    ui->savePath_label->setText( getNewLabelText(this->video_save_path) );
}
