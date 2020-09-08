#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QFile file(filename);
    QStringList lines;
    QStringList test;
    test << "toto" << "tata" << "titi" << "tete" << "toto" << "tata" << "titi" << "tete" << "toto" << "tata" << "titi" << "tete";
    QString match;
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            lines << in.readLine();
        }
        file.close();
        QRegularExpression regex("[0-9]{1,3}.{1}[0-9]{1,3}.{1}[0-9]{1,3}.{1}[0-9]{1,3}\\s{1}(.*)");
        QRegularExpressionMatchIterator it;
        foreach (QString element, lines) {
            it = regex.globalMatch(element);
            while(it.hasNext()) {
                QRegularExpressionMatch qre_match = it.next();
                match = qre_match.captured(1);
                ui->blockedSitesListWidget->addItem(match);
            }
        }
    } else {
        ui->statusbar->showMessage("Erreur lors de l'ouverture du fichier");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_blockButton_clicked()
{
    QString sitePrefix = "127.0.0.1 ";
    QString siteToBlock = sitePrefix + ui->siteToBlockLineEdit->text();
    QString hostFileName = hostDir+QDir::separator()+"hosts";
    QString backupFileName = qApp->applicationDirPath()+"/hosts.xeno.bck";
    QFile hostFile(hostFileName);
    QFile backupFile(backupFileName);
    ui->blockedSitesListWidget->addItem(ui->siteToBlockLineEdit->text());
    ui->blockedSitesListWidget->scrollToBottom();
    if(!backupFile.exists()) {
        hostFile.copy(backupFileName);
    }
    if(backupFile.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream stream(&backupFile);
        stream << siteToBlock << Qt::endl;
        ui->statusbar->showMessage("Site bloqué avec succès !");
        backupFile.close();
    }

//    ShellExecute(0, LPCWSTR("runas"), LPCWSTR("cmd"), 0, 0, SW_SHOWNORMAL);
//    ShellExecute(Handle, 'runas', 'cmd.exe', nil, nil, SW_SHOWNORMAL);
    ShellExecute(NULL, L"runas", L"cmd", L"copy C:/hosts.xeno.bck C:/Windows/System32/drivers/etc/hosts.xeno", 0, SW_SHOWNORMAL);
//    system("robocopy hosts.xeno.bck C:\\Windows\\System32\\drivers\\etc\\hosts.xeno");
//    system("pause");
//    cmdProcess->start("Robocopy",argumentsCmdProcess);
}
