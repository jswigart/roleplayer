import QtQuick 1.1
//import TileTools 1.0

//GameCharacter {
Item {
    id: self
    objectName: "Barbarian"
    width: character.width
    height: character.height

    property bool placeable: true

    Character { id: character }
    property alias character: character

    Component.onCompleted: {
        character.className = "Barbarian"
        character.classDesc = "You are the Barbarian. You know how to use all weapon types, except those which may only be used by the wizard."
        character.classKeywords = [ "barbarian" ]
        character.avatarImage = "../images/tokens/herobarbarian.png"
        character.profileImage = "../images/sheets/profile_barbarian.jpg"
        character.statBaseAttack = 3
        character.statBaseDefend = 2
        character.statBaseBody = 8
        character.statBaseMind = 2
        character.statBaseMovement = 2
        character.inventory.giveItem( "InventoryItemBroadSword.qml" );
        //console.log( "onCompleted " + character.className );

        scenario.addGameObject( self );
    }
}
