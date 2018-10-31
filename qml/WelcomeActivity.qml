import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Rectangle {
    
    readonly property var rootView: StackView.view
    
    color: "white"
    
    Button {
        width: 400
        height: 100
        anchors.centerIn: parent
        text: qsTr("选择图片")
        onClicked: {
            rootView.push("qrc:/qml/PhotoAlbumListActivity.qml")
        }
    }
    
}
