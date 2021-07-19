#include <QApplication>

#include "logindialog.h"
#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  LoginDialog login;
  if (login.exec() == QDialog::Accepted) {
    MainWindow w;
    w.show();
    return a.exec();
  } else {
    return 0;
  }
}
