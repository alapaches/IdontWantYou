#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    RefreshList();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::RefreshList()
{
    QFile file(filename);
    QStringList lines;
    QString match;
    ui->blockedSitesListWidget->clear();
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
                qDebug() << match;
                ui->blockedSitesListWidget->addItem(match);
            }
        }
    } else {
        ui->statusbar->showMessage("Erreur lors de l'ouverture du fichier");
    }
}

void MainWindow::on_blockButton_clicked()
{
    ui->statusbar->clearMessage();
    QString sitePrefix = "127.0.0.1 ";
    QString siteToBlock = sitePrefix + ui->siteToBlockLineEdit->text();
    QString hostFileName = hostDir+QDir::separator()+"hosts";
    QString backupFileName = qApp->applicationDirPath()+"/hosts.xeno.bck";
    QFile hostFile(hostFileName);
    QFile backupFile(backupFileName);
    ui->blockedSitesListWidget->addItem(ui->siteToBlockLineEdit->text());
    ui->blockedSitesListWidget->scrollToBottom();
    if(backupFile.exists()) {
        backupFile.remove();
    }
    hostFile.copy(backupFileName);
    if(backupFile.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream stream(&backupFile);
        stream << Qt::endl << siteToBlock;
        backupFile.close();
    }
    QProcess process;
    process.setProgram("cmd.exe");
    process.setArguments({"/C", qApp->applicationDirPath()+"/apply_changes.bat"});
    process.setWorkingDirectory(qApp->applicationDirPath());
    process.setStandardOutputFile(QProcess::nullDevice());
    process.setStandardErrorFile(QProcess::nullDevice());
    process.start();
    process.waitForFinished();
    if(process.exitCode()== 0) {
        ui->statusbar->showMessage("Site bloqué avec succès !");
        RefreshList();
    }
}

void MainWindow::on_actionAdministrateur_triggered()
{
    QDialog dialog(this);
    auto *layoutDialog = new QVBoxLayout(&dialog);
    auto *lineEdit = new QLineEdit(&dialog);
    auto *label = new QLabel("Entrer le mot de passe", &dialog);

    lineEdit->setEchoMode(QLineEdit::Password);

    layoutDialog->addWidget(label);
    layoutDialog->addWidget(lineEdit);
    layoutDialog->addStretch();

    connect(lineEdit, &QLineEdit::editingFinished, [&]() {
        dialog.done(lineEdit->text() == "Sg3afi46");
        ui->actionReset->setVisible(true);
    });

    dialog.resize(250, 100);
    dialog.exec();
}

void MainWindow::on_actionReset_triggered()
{
    QProcess process;
    process.setProgram("cmd.exe");
    process.setArguments({"/C", qApp->applicationDirPath()+"/flush.bat"});
    process.setWorkingDirectory(qApp->applicationDirPath());
    process.setStandardOutputFile(QProcess::nullDevice());
    process.setStandardErrorFile(QProcess::nullDevice());
    process.startDetached();
    process.waitForFinished(true);
}
