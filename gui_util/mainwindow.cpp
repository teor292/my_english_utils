#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include "DictionaryHolder.h"
#include <filesystem>
#include "textworker.h"
#include "srtworker.h"
#include "globlogger.h"
#include <QMessageBox>

namespace fs = std::filesystem;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto count = DictionaryHolder::Load();

    LOGD("Count of words in dictionary: {}", count);

    ui->lbWordsCount->setText(QString::number(count));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btOpen_clicked()
{
    const QStringList filters({"Subtitles (*.srt)",
                               "Text files (*.txt)",
                              });
    QFileDialog dialog{this, "Choose src/txt file"};
    dialog.setNameFilters(filters);
    dialog.setFileMode(QFileDialog::FileMode::ExistingFile);
    if (!dialog.exec()) return;

    auto files = dialog.selectedFiles();
    auto file = files[0];

    LOGD("Select file: {}", file.toStdString());

    fs::path pth{file.toStdString()};
    auto ext = pth.extension();
    try
    {
        if (ext == ".txt")
        {
            auto new_count = TextWorker::Work(pth.string());
            show_new_data_(new_count);
        }
        else if (ext == ".srt")
        {
            auto new_count = SrtWorker::Work(pth.string());
            show_new_data_(new_count);
        }
        else
        {
            LOGW("Unknown extension: {}", ext.string());
        }
    }
    catch (const std::exception &e)
    {
        LOGE("Exception in working process: {}", e.what());
        QMessageBox box{QMessageBox::Icon::Critical,
                       "Error",
                       "Error while working, see log for description",
                       QMessageBox::StandardButton::Ok,
                       this};
        box.exec();
    }

}

 void MainWindow::show_new_data_(size_t count_new)
 {
    ui->lbNewCount->setText(QString::number(count_new));
    ui->lbWordsCount->setText(QString::number(DictionaryHolder::GetData().words.size()));
 }

