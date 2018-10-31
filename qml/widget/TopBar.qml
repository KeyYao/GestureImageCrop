import QtQuick 2.9
import QtQuick.Controls 2.2

Rectangle {
    
    property string title: ""
    property string rightIcon: ""
    
    width: parent.width
    height: 140
    color: "white"
    
    signal back()
    signal rightClick()
    
    Rectangle {
        width: 100
        height: parent.height
        anchors.left: parent.left
        Image {
            width: 55
            height: 55
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
        width: 100
        height: parent.height
        anchors.right: parent.right
        visible: rightIcon != ""
        Image {
            width: 55
            height: 55
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
