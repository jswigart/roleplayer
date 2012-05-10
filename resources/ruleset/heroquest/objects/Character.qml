import QtQuick 1.1
//import TileTools 1.0

 import "Character.js" as Code

//GameCharacter {
Item {
    id: character
    width: sheet.width
    height: sheet.height

    CharacterInventory { id: inventory }

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

    // alias the property sheet so other files can access it through me
    property alias sheet: sheet

    Component.onCompleted: {
    }

    CharacterSheet {
        id :sheet
    }
    Image {
        id: avatar
        width: 32
        height: 32
        source: "../images/tokens/herodwarf.png"
    }
}
