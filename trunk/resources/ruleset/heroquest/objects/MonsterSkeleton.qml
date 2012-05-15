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
        character.name = "Skeleton"
        character.classDesc = ""
        character.classKeywords = [ "monster", "undead" ]
        character.avatarImage = "../images/tokens/skeleton1.png"
        character.profileImage = ""
        character.statBaseAttack = 2
        character.statBaseDefend = 2
        character.statBaseBody = 1
        character.statBaseMind = 0
        character.statBaseMovement = 6
        //console.log( "onCompleted " + character.className );
    }
}
