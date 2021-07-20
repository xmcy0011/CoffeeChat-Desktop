#include "server_setting_dialog.h"

#include "ui_server_setting_dialog.h"

ServerSettingDialog::ServerSettingDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::ServerSettingDialog) {
  ui->setupUi(this);
}

ServerSettingDialog::~ServerSettingDialog() { delete ui; }

void ServerSettingDialog::on_pushButton_clicked() {}
