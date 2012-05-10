
import QtQuick 1.1
//import TileTools 1.0

//GameCharacter {
Item {
    id: elf
    objectName: "Elf"
    width: character.width
    height: character.height

    Character { id: character }
    property alias character: character

    Component.onCompleted: {
        character.className = "Elf"
        character.classDesc = "You are the Elf. You know how to use all weapon types, except those which may only be used by the wizard. You also have some knowledge of magic, allowing you to cast spells from one Spell College."
        character.classKeywords = [ "elf" ]
        character.statBaseAttack = 2
        character.statBaseDefend = 2
        character.statBaseBody = 5
        character.statBaseMind = 5
        character.statBaseMovement = 2
        character.inventory.giveItem( "InventoryItemShortSword.qml" );
        //character.spells = [ "" ]

        //console.log( "onCompleted " + character.className );
    }
}
