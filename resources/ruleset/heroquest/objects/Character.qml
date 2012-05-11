import QtQuick 1.1
//import TileTools 1.0
import QtDesktop 0.1

 //import "Character.js" as Code

//GameCharacter {
Item {
    id: character
    width: avatar.width
    height: avatar.height

    CharacterInventory { id: inventory }

    Window {
        id: popup
        width: sheet.width
        height: sheet.height
        minimumHeight: sheet.height
        maximumHeight: sheet.height
        minimumWidth: sheet.width
        maximumWidth: sheet.width
        windowDecoration: true
        modal: false
        deleteOnClose: false

        CharacterSheet {
            id: sheet
        }
    }

    property alias sheet: sheet

    Image {
        id: avatar
        width: 32
        height: 32
        source: "../images/tokens/herodwarf.png"
    }

    MouseArea {
         anchors.fill: avatar
         hoverEnabled: true
//         onEntered: { sheet.visible = true }
//         onExited: { sheet.visible = false }
         onClicked: popup.visible = true

     }

    property string name: "NoName";

    property variant classKeywords: [ "" ]
    property string className: "Character Class"
    property string classDesc: "Class Description"

    property int statBaseAttack: 0
    property int statBaseDefend: 0
    property int statBaseBody: 0
    property int statBaseMind: 0
    property int statBaseMovement: 0

    property int statCurrentAttack: statBaseAttack + inventory.cachedAttackBonus
    property int statCurrentDefend: statBaseAttack + inventory.cachedDefendBonus
    property int statCurrentBody: statBaseBody + inventory.cachedBodyBonus
    property int statCurrentMind: statBaseMind + inventory.cachedMindBonus
    property int statCurrentMovement: statBaseMovement + inventory.cachedMoveBonus

    property alias inventory: inventory

    property alias avatarImage: avatar.source
    property string profileImage: ""

    Component.onCompleted: {
    }
}
