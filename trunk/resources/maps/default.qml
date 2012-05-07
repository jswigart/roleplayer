// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import TileTools 1.0

GameTileMap {
    id: map
    width: cellX * gridSize + 1
    height: cellY * gridSize + 1
    gridSize: 32

    property int cellX: 15
    property int cellY: 10

//    Component {
//        id: avatar
//        Rectangle {
//            id: gfx
//            width: 32
//            height: 32
//            color: "#f70000"
//        }
//    }

    GameCharacter {
        id: avatar
        x: 64
        y: 64
        width: 64
        height: 64
        Rectangle {
            id: gfx
            width: 32
            height: 32
            color: "#f70000"
        }
    }

    Component.onCompleted: {
//        avatar.createObject( map, {"x": 64, "y": 64} );
//        avatar.createObject( map, {"x": 128, "y": 128} );
    }
}

