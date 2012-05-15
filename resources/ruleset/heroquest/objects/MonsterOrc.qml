import QtQuick 1.1

//GameCharacter {
Item {
    id: goblin
    objectName: "Zombie"
    width: character.width
    height: character.height

    property bool placeable: true

    Character { id: character }
    property alias character: character

    Component.onCompleted: {
        character.name = "Orc"
        character.classDesc = ""
        character.classKeywords = [ "monster" ]
        character.avatarImage = "../images/tokens/orc1.png"
        character.profileImage = ""
        character.statBaseAttack = 3
        character.statBaseDefend = 2
        character.statBaseBody = 1
        character.statBaseMind = 2
        character.statBaseMovement = 8
        //console.log( "onCompleted " + character.className );
    }
}
