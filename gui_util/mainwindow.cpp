#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include "DictionaryHolder.h"
#include <filesystem>
#include "textworker.h"

namespace fs = std::filesystem;


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

    auto file = dialog.selectedFiles()[0];

    fs::path pth{file.toStdString()};
    auto ext = pth.extension();
    if (ext == ".txt")
    {
        auto new_count = TextWorker::Work(pth.string());
        ui->lbNewCount->setText(QString::number(new_count));
        ui->lbWordsCount->setText(QString::number(DictionaryHolder::GetData().words.size()));
    }





}

