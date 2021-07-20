#ifndef SERVER_SETTING_DIALOG_H
#define SERVER_SETTING_DIALOG_H

#include <QDialog>

namespace Ui {
class ServerSettingDialog;
}

class ServerSettingDialog : public QDialog {
  Q_OBJECT

 public:
  explicit ServerSettingDialog(QWidget *parent = nullptr);
  ~ServerSettingDialog();

 private slots:
  void on_pushButton_clicked();

 private:
  Ui::ServerSettingDialog *ui;
};

#endif  // SERVER_SETTING_DIALOG_H
