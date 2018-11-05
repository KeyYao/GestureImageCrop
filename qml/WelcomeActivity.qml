import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Rectangle {
    
    readonly property var stackView: StackView.view
    
    color: "white"
    
    Button {
        width: parent.width * 0.5
        height: parent.height * 0.1
        anchors.centerIn: parent
        text: qsTr("选择图片")
        onClicked: {
            stackView.push("qrc:/qml/PhotoAlbumListActivity.qml")
        }
    }
    
}
