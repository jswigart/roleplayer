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
        character.name = "Gargoyle"
        character.classDesc = ""
        character.classKeywords = [ "monster" ]
        character.avatarImage = "../images/tokens/gargoyle.png"
        character.profileImage = ""
        character.statBaseAttack = 4
        character.statBaseDefend = 5
        character.statBaseBody = 3
        character.statBaseMind = 4
        character.statBaseMovement = 6
        //console.log( "onCompleted " + character.className );
    }
}
