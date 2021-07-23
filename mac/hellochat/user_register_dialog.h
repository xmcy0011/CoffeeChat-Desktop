#ifndef USER_REGISTER_DIALOG_H
#define USER_REGISTER_DIALOG_H

#include <QDialog>

namespace Ui {
class UserRegisterDialog;
}

class UserRegisterDialog : public QDialog {
    Q_OBJECT

public:
    explicit UserRegisterDialog(QWidget *parent = nullptr);
    ~UserRegisterDialog();

private slots:
    void on_pbOk_clicked();

    void on_pbOk_2_clicked();

private:
    Ui::UserRegisterDialog *ui;
};

#endif // USER_REGISTER_DIALOG_H
