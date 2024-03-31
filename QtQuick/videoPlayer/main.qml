import QtQuick 2.15
import QtQuick.Window 2.15
import QtMultimedia 5.15

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Video {
        id: videoPlayer
        source: "C:/Users/zmaib/Videos/test.mp4" // 指定视频文件路径
        anchors.fill: parent
        autoPlay: true // 视频加载后自动播放
        onStatusChanged: {
            if (videoPlayer.status == MediaPlayer.PlayingState) {
                console.log("视频正在播放");
            }
        }
    }
}
