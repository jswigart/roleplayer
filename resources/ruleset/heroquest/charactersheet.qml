// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {
    id: character
    width: 167
    height: 185

    property int statBaseAttack: 3
    property int statBaseDefend: 2
    property int statBaseBody: 8
    property int statBaseMind: 2

    function calcCurrentAttack() {
        return statBaseAttack;
    }
    function calcCurrentDefend() {
        return statBaseDefend;
    }

    property int statCurrentAttack: calcCurrentAttack()
    property int statCurrentDefend: calcCurrentDefend()
    property int statCurrentBody: statBaseBody
    property int statCurrentMind: statBaseMind

    Image {
        id: image
        x: 0
        y: 0
        width: 167
        height: 185
        source: "hero_barbarian.jpg"
    }

    Item {
        id: stats
        x: 17
        y: 134
        width: 135
        height: 15

        Text {
            id: attack
            x: 14
            y: 0
            width: 6
            height: 14
            text: character.statCurrentAttack
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 12
        }

        Text {
            id: defend
            x: 47
            y: 1
            width: 0
            height: 14
            text: character.statCurrentDefend
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 12
        }

        Text {
            id: body
            x: 82
            y: 0
            text: character.statCurrentBody
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 12
        }

        Text {
            id: mind
            x: 114
            y: 0
            text: character.statCurrentMind
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 12
        }
    }
    states: [
        State {
            name: "hero_barbarian"

            PropertyChanges {
                target: character
                statBaseAttack: 3
                statBaseDefend: 2
                statBaseBody: 8
                statBaseMind: 2
            }
            PropertyChanges {
                target: image
                source: "hero_barbarian.jpg"
            }
        },
        State {
            name: "hero_dwarf"

            PropertyChanges {
                target: character
                statBaseAttack: 2
                statBaseDefend: 2
                statBaseBody: 7
                statBaseMind: 3
            }
            PropertyChanges {
                target: image
                source: "hero_dwarf.jpg"
            }
        },
        State {
            name: "hero_elf"
            PropertyChanges {
                target: character
                statBaseAttack: 2
                statBaseDefend: 2
                statBaseBody: 6
                statBaseMind: 4
            }
            PropertyChanges {
                target: image
                source: "hero_elf.jpg"
            }
        },
        State {
            name: "hero_wizard"
            PropertyChanges {
                target: character
                statBaseAttack: 1
                statBaseDefend: 2
                statBaseBody: 4
                statBaseMind: 6
            }
            PropertyChanges {
                target: image
                source: "hero_wizard.jpg"
            }
        }
    ]

}
