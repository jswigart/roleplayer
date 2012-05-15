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
        character.name = "Chaos Warrior"
        character.classDesc = ""
        character.classKeywords = [ "monster" ]
        character.avatarImage = "../images/tokens/chaoswarrior.png"
        character.profileImage = ""
        character.statBaseAttack = 4
        character.statBaseDefend = 4
        character.statBaseBody = 3
        character.statBaseMind = 3
        character.statBaseMovement = 7
        //console.log( "onCompleted " + character.className );
    }
}
