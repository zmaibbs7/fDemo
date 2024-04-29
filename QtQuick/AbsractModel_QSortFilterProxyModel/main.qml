import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import MapPointsFilterProxyModel 1.0
Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Text
    {
        // text: MapModelManager
    }

    // MapPointsFilterProxyModel
    // {
    //     id:filterModel
    //     sourceModel: MapPoinstsModel
    //     filterTypeID:1
    //     Component.onCompleted: {
    //         console.log("hahah filterModel.filterID = "+filterModel.filterTypeID)
    //     }
    // }

    // MapPointsFilterProxyModel
    // {
    //     id:filterModel1
    //     sourceModel: MapPoinstsModel
    //     filterTypeID:1
    //     Component.onCompleted: {
    //         console.log("hahah filterModel.filterID = "+filterModel.filterTypeID)
    //     }
    // }

    // ListView {
    //     id:lvb
    //     x:10
    //     width: 180; height: 200

    //     model:filterModel
    //     delegate: Text {
    //         text: "----" + name + " : " + index
    //     }

    // }
    // ListView {
    //     id:lvb1
    //     x:100
    //     width: 180; height: 200

    //     model:filterModel1
    //     delegate: Text {
    //         text: "----" + name + " : " + index
    //     }
    // }
    // ListView {
    //     id:lvb2
    //     x:300
    //     width: 180; height: 200

    //     model:MapPointsFilterProxyModel{sourceModel: MapPoinstsModel}
    //     delegate: Text {
    //         text: "----" + name + " : " + index
    //     }
    // }

    // Button
    // {
    //     x:0
    //     y:200

    //     text: "remove"
    //     onClicked: filterModel.removeMapPoint(0)
    // }
}
