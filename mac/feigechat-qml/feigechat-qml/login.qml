import QtQuick
import QtQuick.Controls

Window{
    color: "#EDEDED"
    width: 380
    height: 500
    visible: true
    title: qsTr("登录")

    Image {
        id: imgLogo
        anchors.horizontalCenter: parent.horizontalCenter
        y: 21
        width: 140
        height: 140
        source: "/icon/feige.png"
    }

    Label{
        y: imgLogo.y + imgLogo.height + 5
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: 26
        text: "飞鸽IM"
    }

    Button {
        id: btnSetting
//        background: Image{
//            source: "/icon/btn-setting"
//        }
        text: "test"
        y: 21
        width: 20
        height: 20
        anchors.right: parent.right
        anchors.rightMargin: 10
    }

    TextField {
        id: tfUserName
        placeholderText: "邮箱"
        y: 255
        height: 50
        anchors.left: parent.left
        anchors.leftMargin: 43
        anchors.right: parent.right
        anchors.rightMargin: 43
        topPadding: 15
        leftPadding: 10
    }

    TextField {
        id: tfPwd
        placeholderText: "密码"
        y: tfUserName.y + tfUserName.height + 15
        height: 50
        anchors.left: parent.left
        anchors.leftMargin: 43
        anchors.right: parent.right
        anchors.rightMargin: 43
        topPadding: 15
        leftPadding: 10
        echoMode: TextField.Password
        passwordCharacter: "*"
    }

    Label{
        id: labLoginErr
        text: "timeout"
        y: tfPwd.y + tfPwd.height + 9
        color: "red"
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Button{
        id: btnLogin
        text: "登录"
        y: tfPwd.y + tfPwd.height + 38
        width: 94
        height: 37
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Button{
        id: btnRegister
        contentItem: Text {
            text: "注册"
            color: "blue"
            font: root_Button.font
            opacity: enabled ? 1.0 : 0.3
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
        flat: true
        y: btnLogin.y + btnLogin.height + 5
        width: 94
        height: 37
        anchors.horizontalCenter: parent.horizontalCenter
    }
}


