#include "user_register_dialog.h"

#include <QMessageBox>
#include <QRegExpValidator>
#include <QValidator>

#include "cim/cim.h"
#include "ui_user_register_dialog.h"

UserRegisterDialog::UserRegisterDialog(QWidget *parent) : QDialog(parent), ui(new Ui::UserRegisterDialog) {
    ui->setupUi(this);
}

UserRegisterDialog::~UserRegisterDialog() { delete ui; }

void UserRegisterDialog::on_pbOk_clicked() {
    QRegExp regx("^[a-zA-Z0-9_-]+@[a-zA-Z0-9_-]+(\\.[a-zA-Z0-9_-]+)+$");
    QRegExpValidator v(regx, ui->leEmail);

    QString str = ui->leEmail->text();
    int pos = 0;
    if (v.validate(str, pos) != QValidator::State::Acceptable) {
        QMessageBox::information(this, "Tips", "邮箱格式不正确", QMessageBox::Yes);
        return;
    }

    if (ui->lePwd->text().length() <= 3) {
        QMessageBox::information(this, "Tips", "请输入至少3位密码", QMessageBox::Yes);
        return;
    }

    cim::core::HttpResponseBase res;
    if (cim::core::UserManager::getInstance()->registerUser(str.toStdString(), ui->lePwd->text().toStdString(),
                                                            ui->leNick->text().toStdString(), res)) {
        QMessageBox::information(this, "Tips", "注册成功", QMessageBox::Yes);
        this->accept();
    } else {
        QMessageBox::information(this, "Tips", QString::fromStdString(std::string("注册失败：") + res.msg),
                                 QMessageBox::Yes);
    }
}

void UserRegisterDialog::on_pbOk_2_clicked() {
    std::string random_nick;
    cim::core::HttpResponseBase result;
    if (cim::core::UserManager::getInstance()->generateNickName(random_nick, result)) {
        ui->leNick->setText(QString::fromStdString(random_nick));
    } else {
        QMessageBox::information(this, "Tips", "获取昵称失败，请稍候重试", QMessageBox::Yes);
    }
}
