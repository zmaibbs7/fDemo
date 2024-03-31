import QtQuick 2.15
import QtQuick.Window 2.15
import QtMultimedia 5.15

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    // Video {
    //     id: videoPlayer
    //     source: "C:/Users/zmaib/Videos/test.mp4"
    //     anchors.fill: parent
    //     autoPlay: true
    //     onErrorChanged: console.log("Error playing video:", error +  " - " +  errorString)
    //     MouseArea
    //     {
    //         anchors.fill: parent
    //         onClicked:
    //         {
    //             console.log("videoPlayer.status = "+videoPlayer.status)
    //         }
    //     }
    // }

    VideoOutput {
        source: mediaPlayer

        MediaPlayer {
            id: mediaPlayer
            autoPlay: true
            source: "C:/Users/zmaib/Videos/test.avi"
            loops: Audio.Infinite
        }

        function play() { mediaPlayer.play() }
        function pause() { mediaPlayer.pause() }
        function stop() { mediaPlayer.stop() }

        function toggleplay() {
            if (isRunning) {
                pause()
                isRunning = false
            } else {
                play()
                isRunning = true
            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: toggleplay()
        }
    }
}
