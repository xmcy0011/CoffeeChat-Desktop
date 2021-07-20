#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog {
  Q_OBJECT

 public:
  explicit LoginDialog(QWidget *parent = nullptr);
  ~LoginDialog();

 private slots:
  void on_pushButton_clicked();

  void on_pushButton_2_clicked();

 private:
  Ui::LoginDialog *ui;
};

#endif  // LOGINDIALOG_H
