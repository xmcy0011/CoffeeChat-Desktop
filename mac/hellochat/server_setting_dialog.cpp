#include "server_setting_dialog.h"

#include <QMessageBox>

#include "cim/cim.h"
#include "ui_server_setting_dialog.h"

ServerSettingDialog::ServerSettingDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ServerSettingDialog) {
    ui->setupUi(this);
}

ServerSettingDialog::~ServerSettingDialog() { delete ui; }

void ServerSettingDialog::on_pushButton_clicked() {
    std::string ip = ui->leServerIp->text().toStdString();
    int16_t port = ui->leGatePort->text().toInt();
    int16_t http_port = ui->leHttpPort->text().toInt();

    bool ret = cim::base::TcpClient::ping(ip, port);
    if (!ret) {
        QMessageBox::information(this, "Ping Result", "gate端口ping失败", QMessageBox::Yes);
        return;
    }

    ret = cim::base::TcpClient::ping(ip, http_port);
    if (!ret) {
        QMessageBox::information(this, "Ping Result", "http端口ping失败", QMessageBox::Yes);
    } else {
        QMessageBox::information(this, "Ping Result", "all is success", QMessageBox::Yes);
    }
}
