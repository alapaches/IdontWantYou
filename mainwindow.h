#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <QFile>
#include <QFileDevice>
#include <QDebug>
#include <QStringListModel>
#include <QDir>
#include <QSettings>
#include <QProcess>
#include <QVBoxLayout>
#include <QDialog>
#include "qt_windows.h"
#include "qwindowdefs_win.h"
#include <shellapi.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool RefreshList();

private slots:
    void on_blockButton_clicked();

    void on_actionAdministrateur_triggered();

    void on_actionReset_triggered();

private:
    Ui::MainWindow *ui;
    QString hostDir = "C:/Windows/System32/drivers/etc";
    QString filename="C:\\Windows\\System32\\drivers\\etc\\hosts";
    QStringList lines;

    QString backup="C:\\Windows\\System32\\drivers\\etc\\hosts.xeno.bck";

};
#endif // MAINWINDOW_H
