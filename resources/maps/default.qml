// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import TileTools 1.0
import Box2D 1.0

GameTileMap {
    id: map
    width: cellX * gridSize + 1
    height: cellY * gridSize + 1
    gridSize: 32

    property int cellX: 150
    property int cellY: 15

    World {
        id: world
        width: parent.width
        height: parent.height
        //gravity: Qt.point(0,0,0)

//        Body {
//            bodyType: Body.Dynamic
//            x: 100
//            y: 100
//            rotation: 32
//            fixtures: Box { width: 32; height: 32; density: 1 }
//        }
        Body {
            bodyType: Body.Static

            fixtures: ChainLoop {
                id: boundary
                density: 1
                friction: 0.3
                restitution: 0.5
                vertices: [
                    Qt.point(0, 0),
                    Qt.point(0, map.height),
                    Qt.point(map.width, map.height),
                    Qt.point(map.width, 0)
                ]
            }
        }
        DebugDraw {
            id: debugDraw
            world: world
            anchors.fill: world
            opacity: 0.75
            visible: true
        }
//        MouseArea {
//            id: debugMouseArea
//            anchors.fill: world
//            onPressed: debugDraw.visible = !debugDraw.visible
//        }
    }

    GameCharacter {
        id: avatar1
        x: 64
        y: 64
        width: 32
        height: 32
        Image {
            width: parent.width
            height: parent.height
            source: "../images/tokens/pog16.png"
        }
    }
    GameCharacter {
        id: avatar2
        x: 300
        y: 300
        width: 64
        height: 64
        Image {
            width: parent.width
            height: parent.height
            source: "../images/tokens/pog17.png"
        }
    }
    Component.onCompleted: {
//        avatar.createObject( map, {"x": 64, "y": 64} );
//        avatar.createObject( map, {"x": 128, "y": 128} );
    }
}

