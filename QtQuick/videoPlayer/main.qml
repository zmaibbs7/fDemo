import QtQuick 2.15
import QtQuick.Window 2.15
import QtMultimedia 5.0

Window {
    width: 1920
    height: 1080
    visible: true
    title: qsTr("Hello World")

    Rectangle
    {
        anchors.fill: parent
        color: "black"
    }

    // 第一个视频播放器
    Video {
        id: videoPlayer1
        source: "file:///home/zmaibbs/Music/test.mp4"
        anchors.fill: parent
        autoPlay: true
        autoLoad: true
        visible: true // 初始时可见

        onStopped:
        {

            videoPlayer2.play()

            videoPlayer1.visible = false
            videoPlayer2.z = 2;
            videoPlayer1.play()
            videoPlayer1.pause()
            videoPlayer1.seek(0)
            videoPlayer1.z = 1
            videoPlayer1.visible = true

        }
    }

    // 第二个视频播放器
    Video {
        id: videoPlayer2
        source: "file:///home/zmaibbs/Music/test.mp4"
        anchors.fill: parent
        autoPlay: false
        autoLoad: true
        visible: true // 初始时不可见
        onStopped:
        {
            // videoPlayer1.visible = true

            videoPlayer1.play()

            videoPlayer2.visible = false
            videoPlayer1.z = 2;
            videoPlayer2.play()
            videoPlayer2.pause()
            videoPlayer2.seek(0)
            videoPlayer2.z = 1
            videoPlayer2.visible = true


        }
    }
}
