import QtQuick 2.9

Item {
    
    width: 100
    height: 100
    visible: false
    
    property int petalCount: 8
    property int petalWidth: 4
    property int rotateDuration: 1000
    property int petalLength: (width / 2) * (3.3 / 8.5)
    property color petalColor: Qt.rgba(255, 255, 255, 1)
    
    Canvas {
        id: canvas
        anchors.fill: parent
        
        onPaint: {
            var ctx = getContext("2d");
            ctx.reset();
            
            var centerX = width / 2
            var centerY = height / 2
            var degrees = Math.PI * 2 / petalCount;
            petalLength = centerX / 8 * 5
            
            ctx.translate(centerX, centerY)
            for(var i = 0 ; i < petalCount ; i ++) {
                var alpha = parseFloat(i + 1) / parseFloat(petalCount)

                var paintColor = petalColor
                paintColor.a = alpha
                ctx.fillStyle = paintColor
                
                ctx.moveTo(0, 0)
                ctx.beginPath();
                ctx.roundedRect(-(petalWidth / 2), -(centerY - petalLength), petalWidth, -petalLength, petalWidth, petalWidth)
                ctx.fill()
                
                ctx.rotate(degrees);
            }
            ctx.save();
        }
    }
    
    RotationAnimation {
        id: anim
        target: canvas
        duration: rotateDuration
        loops: Animation.Infinite
        from: 0
        to: 360
        
    }
    
    function startAnimation() {
        anim.start()
        visible = true
    }
    
    function stopAnimation() {
        anim.stop()
        visible = false
    }
    
    function isRunning() {
        return anim.running
    }
    
}
