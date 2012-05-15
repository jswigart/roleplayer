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
        character.name = "Mummy"
        character.classDesc = ""
        character.classKeywords = [ "monster", "undead" ]
        character.avatarImage = "../images/tokens/mummy.png"
        character.profileImage = ""
        character.statBaseAttack = 3
        character.statBaseDefend = 4
        character.statBaseBody = 2
        character.statBaseMind = 0
        character.statBaseMovement = 4
        //console.log( "onCompleted " + character.className );
    }
}
