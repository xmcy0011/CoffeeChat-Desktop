#include "logindialog.h"

#include "server_setting_dialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent), ui(new Ui::LoginDialog) {
    ui->setupUi(this);
    ui->labError->setVisible(false);
}

LoginDialog::~LoginDialog() { delete ui; }

void LoginDialog::on_pushButton_clicked() {}

void LoginDialog::on_pushButton_2_clicked() {
    ServerSettingDialog dialog;
    dialog.exec();
}
