#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QRegExpValidator>
#include <QValidator>

#include "cim/cim.h"

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

    void onLoginResult(const CIM::Login::CIMAuthRsp &rsp);

    void onLoginTimeout();

    void on_pushButton_3_clicked();

private:
    Ui::LoginDialog *ui;
    QValidator *email_validator_;
};

#endif // LOGINDIALOG_H
