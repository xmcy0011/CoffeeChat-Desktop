#include "server_setting_dialog.h"

#include <QMessageBox>

#include "cim/cim.h"
#include "ui_server_setting_dialog.h"

ServerSettingDialog::ServerSettingDialog(QWidget* parent) : QDialog(parent), ui(new Ui::ServerSettingDialog) {
    ui->setupUi(this);

    cim::ServerInfoConfig config = cim::getChatKitConfig().serverInfo;
    ui->leServerIp->setText(QString::fromStdString(config.ip));
    ui->leGatePort->setText(QString::fromStdString(std::to_string(config.gatePort)));
    ui->leHttpPort->setText(QString::fromStdString(std::to_string(config.httpPort)));
}

ServerSettingDialog::~ServerSettingDialog() { delete ui; }

void ServerSettingDialog::on_pushButton_clicked() {
    std::string ip = ui->leServerIp->text().toStdString();
    int16_t port = ui->leGatePort->text().toInt();
    int16_t http_port = ui->leHttpPort->text().toInt();

    bool ret = cim::base::TcpClient::ping(ip, port);
    if (!ret) {
        QMessageBox::information(this, "Ping", "gate端口ping失败", QMessageBox::Yes);
        return;
    }

    ret = cim::base::TcpClient::ping(ip, http_port);
    if (!ret) {
        QMessageBox::information(this, "Ping", "http端口ping失败", QMessageBox::Yes);
    } else {
        QMessageBox::information(this, "Ping", "all is success", QMessageBox::Yes);

        cim::ServerInfoConfig config{};
        config.ip = ip;
        config.gatePort = port;
        config.httpPort = http_port;
        cim::setChatKitServerInfo(config);
        close();
    }
}
