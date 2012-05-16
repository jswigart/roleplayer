import QtQuick 1.1

Item {
    id: self
    objectName: "Wizard"
    width: character.width
    height: character.height

    property variant classAbilities: [ "AbilityAttack.qml", "AbilityCastSpell.qml" ]

    property bool placeable: true

    Character { id: character }
    property alias character: character

    Component.onCompleted: {
        character.className = "Wizard"
        character.classDesc = "You are the Wizard. You possess a great knowledge of magic and may cast spells from 3 Spell Colleges. Unfortunately you are untrained in combat and may not use heavy weapons."
        character.classKeywords = [ "wizard" ]
        character.avatarImage = "../images/tokens/herowizard.png"
        character.profileImage = "../images/sheets/profile_wizard.jpg"
        character.statBaseAttack = 1
        character.statBaseDefend = 2
        character.statBaseBody = 4
        character.statBaseMind = 6
        character.statBaseMovement = 2
        //character.giveItem( "InventoryItemDagger.qml" );
        //character.spells = [ "" ]
        character.addAbilities( [ "AbilityAttack.qml", "AbilityCastSpell.qml" ] )

        //console.log( "onCompleted " + character.className );
    }
}
