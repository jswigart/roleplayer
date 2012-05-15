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
        character.name = "Fimir"
        character.classDesc = ""
        character.classKeywords = [ "monster" ]
        character.avatarImage = "../images/tokens/fimir1.png"
        character.profileImage = ""
        character.statBaseAttack = 3
        character.statBaseDefend = 3
        character.statBaseBody = 2
        character.statBaseMind = 3
        character.statBaseMovement = 6
        //console.log( "onCompleted " + character.className );
    }
}
