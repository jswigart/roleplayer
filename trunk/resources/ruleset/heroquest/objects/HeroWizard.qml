
import QtQuick 1.1
//import TileTools 1.0

//GameCharacter {
Item {
    id: elf
    objectName: "Wizard"
    width: character.width
    height: character.height

    Character { id: character }
    property alias character: character

    Component.onCompleted: {
        character.className = "Wizard"
        character.classDesc = "You are the Wizard. You possess a great knowledge of magic and may cast spells from 3 Spell Colleges. Unfortunately you are untrained in combat and may not use heavy weapons."
        character.classKeywords = [ "wizard" ]
        character.statBaseAttack = 1
        character.statBaseDefend = 2
        character.statBaseBody = 4
        character.statBaseMind = 6
        character.statBaseMovement = 2
        character.inventory.giveItem( "InventoryItemDagger.qml" );
        //character.spells = [ "" ]

        //console.log( "onCompleted " + character.className );
    }
}
