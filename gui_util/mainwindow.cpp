#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include "DictionaryHolder.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto count = DictionaryHolder::Load();

    ui->lbWordsCount->setText(QString::number(count));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btOpen_clicked()
{
    const QStringList filters({"Subtitles (*.src)",
                               "Text files (*.txt)",
                              });
    QFileDialog dialog{this, "Choose src/txt file"};
    dialog.setNameFilters(filters);
    dialog.setFileMode(QFileDialog::FileMode::ExistingFile);
    if (!dialog.exec()) return;

    auto file = dialog.selectedFiles();


}

