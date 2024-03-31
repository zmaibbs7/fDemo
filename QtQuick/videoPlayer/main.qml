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
        source: "C:/Users/zmaib/Videos/test.mp4"
        anchors.fill: parent
        autoPlay: true
        MouseArea
        {
            anchors.fill: parent
            onClicked:
            {
                console.log("videoPlayer.status = "+videoPlayer.status)
            }
        }
    }
}
