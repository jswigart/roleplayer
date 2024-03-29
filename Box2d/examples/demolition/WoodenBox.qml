import QtQuick 1.0
import Box2D 1.0

Body {
    id: body

    width: 100;
    height: 100;
    sleepingAllowed: true

    fixtures: Box {
        anchors.fill: parent
        density: 1;
        friction: 0.4;
        restitution: 0.5;
    }

    Image {
        anchors.fill: parent
        source: "images/woodenbox.png"
        anchors.margins: -1
    }

    MouseArea {
        anchors.fill: parent
        onClicked: body.applyTorque(400)
    }
}
