
import QtQuick 1.1
//import TileTools 1.0

//GameCharacter {
Item {
    id: dwarf
    objectName: "Dwarf"
    width: character.width
    height: character.height

    property bool placeable: true

    Character { id: character }
    property alias character: character

    Component.onCompleted: {
        character.className = "Dwarf"
        character.classDesc = "You are the Dwarf. You know how to use all weapon types, except those which may only be used by the wizard. You also have the unique ability to disarm traps."
        character.classKeywords = [ "dwarf" ]
        character.avatarImage = "../images/tokens/herodwarf.png"
        character.profileImage = "../images/sheets/profile_dwarf.jpg"
        character.statBaseAttack = 2
        character.statBaseDefend = 2
        character.statBaseBody = 7
        character.statBaseMind = 3
        character.statBaseMovement = 2
        character.inventory.giveItem( "InventoryItemShortSword.qml" );
        //character.ability = [ "disarmtraps" ]

        //console.log( "onCompleted " + character.className );
    }
}
