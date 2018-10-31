import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Window {
    visible: true
    height: Screen.height
    width: Screen.width
    color: "transparent"
    
    StackView {
        property int animationDuration: 200
        
        id: stackView
        anchors.fill: parent
        focus: true
        
        initialItem: WelcomeActivity {
            
        }
        
        Keys.onBackPressed: {
            if (stackView.depth > 1) {
                pop()
                event.accepted = true
            } else {
                event.accepted = false
            }
        }
        
        onCurrentItemChanged: {
            if (currentItem != null) {
                currentItem.forceActiveFocus()
            }
        }
        
        pushEnter: Transition {
            PropertyAnimation {
                property: "x"
                from: stackView.width
                to: 0
                duration: stackView.animationDuration
            }
        }
        
        pushExit: Transition {
            PropertyAnimation {
                property: "x"
                from: 0
                to: -stackView.width / 4
                duration: stackView.animationDuration
            }
        }
        
        popEnter: Transition {
            PropertyAnimation {
                property: "x"
                from: -stackView.width / 4
                to: 0
                duration: stackView.animationDuration
            }
        }
        
        popExit: Transition {
            PropertyAnimation {
                property: "x"
                from: 0
                to: stackView.width
                duration: stackView.animationDuration
            }
        }
        
    }
    
}
