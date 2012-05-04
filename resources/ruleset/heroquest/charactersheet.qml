// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {
    id: character
    width: 167
    height: 185

    property int statAttack: 3
    property int statDefend: 2
    property int statBody: 8
    property int statMind: 2

    property int statCurrentHp: 0
    property int statCurrentMind: 0

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
            text: character.statAttack
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 12
        }

        Text {
            id: defend
            x: 47
            y: 1
            width: 0
            height: 14
            text: character.statDefend
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 12
        }

        Text {
            id: body
            x: 82
            y: 0
            text: character.statBody
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 12
        }

        Text {
            id: mind
            x: 114
            y: 0
            text: character.statMind
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 12
        }
    }
    states: [
        State {
            name: "hero_barbarian"

            PropertyChanges {
                target: character
                statAttack: 3
                statDefend: 2
                statBody: 8
                statMind: 2
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
                statAttack: 2
                statDefend: 2
                statBody: 7
                statMind: 3
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
                statAttack: 2
                statDefend: 2
                statBody: 6
                statMind: 4
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
                statAttack: 1
                statDefend: 2
                statBody: 4
                statMind: 6
            }
            PropertyChanges {
                target: image
                source: "hero_wizard.jpg"
            }
        }
    ]

}
