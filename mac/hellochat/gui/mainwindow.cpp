#include "mainwindow.h"

#include "./ui_mainwindow.h"
#include "cim/core/chat_manager.h"

using namespace cim::core;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) { ui->setupUi(this); }

MainWindow::~MainWindow() { delete ui; }

// 发送按钮被点击
void MainWindow::on_pushButton_clicked() {
    // ChatManager::getInstance().sendMessage(12, 1, "hello");
    // ui->listWidget->addItem("我->1:hello");
}

void MainWindow::onRecvMsg(CIM::Message::CIMMsgData &msg_data) {}
