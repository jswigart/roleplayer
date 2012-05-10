
import QtQuick 1.1

Rectangle {
    id: character
    width: image.width
    height: image.height

    Image {
        id: image
        x: 0
        y: 0
        width: 620
        height: 691
        source: "../images/sheets/template-usa.jpg"
    }

    Item {
        id: stats
        x: 93
        y: 477
        width: 452
        height: 54

        property int fontSize: 30

        Text {
            id: attack
            x: 39
            y: 7
            width: 40
            height: 40
            text: statCurrentAttack
            font.pixelSize: stats.fontSize
            anchors.verticalCenterOffset: 0
            verticalAlignment: Text.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Text.AlignHCenter
        }

        Text {
            id: defend
            x: 150
            y: 7
            width: 40
            height: 40
            text: statCurrentDefend
            font.pixelSize: stats.fontSize
            anchors.verticalCenterOffset: 0
            verticalAlignment: Text.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Text.AlignHCenter
        }

        Text {
            id: body
            x: 261
            y: 1
            width: 40
            height: 40
            text: statCurrentBody
            font.pixelSize: stats.fontSize
            verticalAlignment: Text.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Text.AlignHCenter
        }

        Text {
            id: mind
            x: 372
            y: 7
            width: 40
            height: 40
            text: statCurrentMind
            font.pixelSize: stats.fontSize
            anchors.verticalCenterOffset: 0
            verticalAlignment: Text.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Text.AlignHCenter
        }

        Text {
            id: movement
            x: 311
            y: 68
            width: 40
            height: 30
            text: statCurrentMovement + " Red Die"
            font.pixelSize: 20
            anchors.verticalCenterOffset: 56
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        Text {
            id: curWeapon
            x: 311
            y: 98
            width: 40
            height: 30
            text: inventory.activeWeapon
            font.pixelSize: 20
            anchors.verticalCenterOffset: 86
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        Text {
            id: curArmor
            x: 311
            y: 128
            width: 40
            height: 30
            text: inventory.activeArmor
            font.pixelSize: 20
            anchors.verticalCenterOffset: 116
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    Text {
        id: characterClass
        x: 152
        y: 67
        text: className
        font.pixelSize: 46
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: 0
        font.family: "Romic Std Light"
        font.bold: true
        horizontalAlignment: Text.AlignHCenter
    }

    Image {
        id: profile
        x: 312
        y: 131
        width: 232
        height: 283
        source: ""
    }

    Text {
        id: characterDesc
        x: 87
        y: 131
        width: 214
        height: 283
        text: classDesc
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 16
        wrapMode: Text.WordWrap
        font.bold: true
        font.family: "Romic Std Light"
        horizontalAlignment: Text.AlignLeft
    }
}
