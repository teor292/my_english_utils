#include "mainwindow.h"

#include <QApplication>
#include "globlogger.h"

int main(int argc, char *argv[])
{
    GlobLogger::Initialize();
    LOGI("===== START =====");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    auto result = a.exec();
    LOGI("====== END ======");
    return result;
}
