import QtQuick 2.9
import QtQuick.Controls 2.2
import PhotoView 1.0

Rectangle {
    
    property string source: ""
    property real maxScale: 3
    
    function crop() {
        photoView.crop()
    }
    
    color: "transparent"
    signal cropSuccess(var path)
    onMaxScaleChanged: {
        photoView.maxScale = maxScale
    }
    
    PhotoView {
        id: photoView
        anchors.fill: parent
        onSignal_loadingStarted: {
            loadingView.startAnimation()
            
        }
        onSignal_loadingEnded: {
            loadingView.stopAnimation()
        }
        onSignal_cropAreaInited: {
            photoView.setSrc(source)
        }
        onSignal_cropSuccess: {
            emit: cropSuccess(path)
        }
    }
    
    PinchArea {
        anchors.fill: parent
        
        onPinchStarted: {
            photoView.scaleBegan(pinch.center.x, pinch.center.y)
        }
        
        onPinchUpdated: {
            photoView.scaleUpdate(pinch.scale)
        }
        
        onPinchFinished: {
            photoView.scaleEnded()
        }
        
        MouseArea {
            property int touchDownX: 0
            property int touchDownY: 0
            
            anchors.fill: parent
            
            onPressed: {
                touchDownX = mouse.x
                touchDownY = mouse.y
                photoView.moveBegan()
            }
            
            onPositionChanged: {
                photoView.moveUpdate(mouse.x - touchDownX, mouse.y - touchDownY)
            }
            
            onReleased: {
                touchDownX = 0
                touchDownY = 0
                photoView.moveEnded()
            }
            
        }
        
    }
    
    LoadingView {
        id: loadingView
        width: 50
        height: 50
        petalCount: 12
        petalWidth: 3
        petalLength: width / 6
        petalColor: "#ffffff"
        anchors.centerIn: parent
    }
    
}
