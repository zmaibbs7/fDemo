import QtQuick 2.15
import QtQuick.Window 2.15
import MapPointListModel 1.0
Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    MapPointListModel
    {
        id:cusModel

        Component.onCompleted: cusModel.bind(MapModel.pointsModel())
    }

    ListView {
        id:lvb
        x:10
        width: 180; height: 200

        model:cusModel
        delegate: Text {
            text: "----" + name + " : " + index
        }
        Component.onDestruction: {
            console.log("lvb.destruction ")
        }
    }

    // ListView {
    //     id:lvb2
    //     x:100
    //     width: 180; height: 200

    //     model: MapModel.pointsFilterModel
    //     delegate: Text {
    //         text: "----" + name + " : " + index
    //     }
    //     Component.onDestruction: {
    //         // MapModel.pointsFilterModel.setFilterTypeID(1)
    //     }
    // }
}
