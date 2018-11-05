import QtQuick 2.9
import QtQuick.Controls 2.2

Rectangle {
    
    property string title: ""
    property string rightIcon: ""
    
    id: rootView
    width: parent.width
    height: parent.height * 0.075
    color: "white"
    
    signal back()
    signal rightClick()
    
    Rectangle {
        width: parent.width * 0.094
        height: parent.height
        anchors.left: parent.left
        Image {
            width: rootView.width * 0.05
            height: width
            anchors.centerIn: parent
            source: "qrc:/res/ic_back_black.png"
        }
        
        MouseArea {
            anchors.fill: parent
            onClicked: {
                back()
            }
        }
    }
    
    Rectangle {
        width: parent.width * 0.094
        height: parent.height
        anchors.right: parent.right
        visible: rightIcon != ""
        Image {
            width: rootView.width * 0.05
            height: width
            anchors.centerIn: parent
            source: rightIcon
        }
        
        MouseArea {
            anchors.fill: parent
            onClicked: {
                rightClick()
            }
        }
    }
    
    Label {
        anchors.centerIn: parent
        text: title
        font.pointSize: 16
        color: "black"
    }
    
    Rectangle {
        width: parent.width
        height: 1
        color: "gray"
        anchors.bottom: parent.bottom
    }
    
}
