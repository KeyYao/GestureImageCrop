import QtQuick 2.9
import QtQuick.Controls 2.2
import "./widget"

Rectangle {
    
    Component.onCompleted: {
        cropView.source = path
    }
    
    readonly property var stackView: StackView.view
    property string path: ""
    
    color: "white"
    
    TopBar {
        id: topbar
        title: qsTr("裁剪")
        rightIcon: "qrc:/res/ic_done_black.png"
        onBack: {
            stackView.pop()
        }
        onRightClick: {
            cropView.crop()
        }
    }
    
    GestureCropView {
        id: cropView
        width: parent.width
        anchors.top: topbar.bottom
        anchors.bottom: parent.bottom
        maxScale: 4
        onCropSuccess: {
            console.log("save path: " + path)
        }
    }
    
}
