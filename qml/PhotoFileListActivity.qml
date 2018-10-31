import QtQuick 2.9
import QtQuick.Controls 2.2
import "./widget"
import PhotoService 1.0

Rectangle {
    
    Component.onCompleted: {
        listView.model = service.loadFileList(identifier)
    }
    
    PhotoService {
        id: service
    }
    
    readonly property var rootView: StackView.view
    property string identifier: ""
    
    color: "white"
    
    TopBar {
        id: topbar
        title: qsTr("图片")
        onBack: {
            rootView.pop()
        }
    }
    
    ListView {
        id: listView
        width: parent.width
        anchors.top: topbar.bottom
        anchors.bottom: parent.bottom
        clip: true
        delegate: delegate
    }
    
    Component {
        id: delegate
        
        Rectangle {
            width: parent.width
            height: 200
            color: "white"
            
            Image {
                id: img
                width: 160
                height: 160
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 100
                source: "file://" + model.modelData.path
                sourceSize.width: 100
                sourceSize.height: 100
                fillMode: Image.PreserveAspectCrop
            }
            
            Label {
                anchors.left: img.right
                anchors.leftMargin: 200
                anchors.verticalCenter: parent.verticalCenter
                font.pointSize: 14
                color: "black"
                text: model.modelData.name
            }
            
            Rectangle {
                width: parent.width
                height: 1
                color: "lightgray"
                anchors.bottom: parent.bottom
            }
            
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    rootView.push("qrc:/qml/ImageCropActivity.qml", {"path":model.modelData.path})
                }
            }
            
        }
    }
}

