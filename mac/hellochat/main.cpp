#include <QApplication>

#include "cim/cim.h"
#include "logindialog.h"
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    cim::initChatKit();

    LoginDialog login;
    if (login.exec() == QDialog::Accepted) {
        MainWindow w;
        w.show();

        int ret = a.exec();
        // 释放SDK
        cim::cleanup();
        return ret;
    } else {
        cim::cleanup();
        return 0;
    }
}
