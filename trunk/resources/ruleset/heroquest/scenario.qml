import QtQuick 1.1
import Box2D 1.0

// Hero options
// Movement - roll move d 6
// Actions
//  - Attack an adjacent monster
//  - Cast a spell on monster or hero that can be seen
//  - Search for treasure in the current room
//  - Search for secret doors in the room or corridor
//  - Search for traps in the room or corridor
//  - Disarm a trap on the square he is on
// caught by trap, drinking potion, picking things up, opening doors, looking through doors/corridors dont count as actions, can be used mid movement

// DM options
// Movement - fixed move squares per monster type, allowed to not move
// Actions
//  - Attack an adjacent hero
//  - Cast a spell on hero that can be seen by monster

World {
    id: world
    anchors.fill: parent

    Component {
        id: woodenBox

        Body {
            width: 100;
            height: 100;
            sleepingAllowed: false

            fixtures: Box {
                anchors.fill: parent
                density: 1;
                friction: 0.3;
                restitution: 0.5;
            }

            Image { anchors.fill: parent; source: "woodenbox.png" }
        }
    }

    Body {
        id: ground
        height: 20
        anchors { left: parent.left; right: parent.right; top: parent.bottom }
        bodyType: Body.Static
        fixtures: Box { anchors.fill: parent }
    }
    Body {
        id: ceiling
        height: 20
        anchors { left: parent.left; right: parent.right; bottom: parent.top }
        bodyType: Body.Static
        fixtures: Box { anchors.fill: parent }
    }
    Body {
        id: leftWall
        width: 20
        anchors { right: parent.left; bottom: ground.top; top: ceiling.bottom }
        bodyType: Body.Static
        fixtures: Box { anchors.fill: parent }
    }
    Body {
        id: rightWall
        width: 20
        anchors { left: parent.right; bottom: ground.top; top: ceiling.bottom }
        bodyType: Body.Static
        fixtures: Box { anchors.fill: parent }
    }

    DebugDraw {
        id: debugDraw
        world: world
        anchors.fill: world
        opacity: 0.75
        visible: true
    }
    MouseArea {
        id: debugMouseArea
        anchors.fill: world
        onPressed: {
            woodenBox.createObject( world );
        }
    }
}
