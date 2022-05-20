import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    MouseArea{
        anchors.fill: parent
        onClicked: {
            console.log("mouse clicked!")
            Qt.quit()
        }
    }

    Rectangle{
        height: 320
        width: 400

        Button{
            id: btnOk
            text: "退出"
            width: 100
            height: 50
            anchors.left: parent.left
            anchors.leftMargin: 5
            onClicked: {
                console.log("onclicked")
                Qt.quit();
            }
        }
    }

    Rectangle{
        width: 200
        height: 200
        x: 200
        y: 50
        color: "blue"
        border.color: "#808080"
        border.width: 2
        radius: 12
    }
}
