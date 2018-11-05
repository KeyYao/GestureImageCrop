import QtQuick 2.9
import QtQuick.Controls 2.2
import "./widget"
import PhotoService 1.0
import ImageThumbnailView 1.0

Rectangle {
    
    Component.onCompleted: {
        listView.model = service.loadAlbumList()
    }
    
    PhotoService {
        id: service
    }
    
    readonly property var stackView: StackView.view
    
    id: rootView
    color: "white"
    
    TopBar {
        id: topbar
        title: qsTr("相册")
        onBack: {
            stackView.pop()
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
            height: rootView.height * 0.1
            color: "white"
            
            ImageThumbnailView {
                id: img
                height: rootView.height * 0.09
                width: height
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: rootView.width * 0.08
                source: model.modelData.url
            }
            
            Label {
                anchors.left: img.right
                anchors.leftMargin: rootView.width * 0.18
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
                    stackView.push("qrc:/qml/PhotoFileListActivity.qml", {"identifier":model.modelData.identifier})
                }
            }
            
        }
    }
    
}
