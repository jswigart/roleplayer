import QtQuick 1.1

//GameCharacter {
Item {
    id: goblin
    objectName: "Goblin"
    width: character.width
    height: character.height

    property bool placeable: true

    Character { id: character }
    property alias character: character

    Component.onCompleted: {
        character.name = "Goblin"
        character.classDesc = ""
        character.classKeywords = [ "monster" ]
        character.avatarImage = "../images/tokens/goblin1.png"
        character.profileImage = ""
        character.statBaseAttack = 2
        character.statBaseDefend = 1
        character.statBaseBody = 1
        character.statBaseMind = 1
        character.statBaseMovement = 10
        //console.log( "onCompleted " + character.className );
    }
}
