import QtQuick 1.1
//import TileTools 1.0
//import QtDesktop 0.1

 import "Character.js" as Code

//GameCharacter {
Item {
    id: character
    width: avatar.width
    height: avatar.height

//    Window {
//        id: popup
//        width: sheet.width
//        height: sheet.height
//        minimumHeight: sheet.height
//        maximumHeight: sheet.height
//        minimumWidth: sheet.width
//        maximumWidth: sheet.width
//        windowDecoration: true
//        modal: false
//        deleteOnClose: false
//        CharacterSheet {
//            id: sheet
//        }
//    }
//    property alias sheet: sheet
//    MouseArea {
//        anchors.fill: avatar
//        hoverEnabled: true
//        onClicked: popup.visible = true
//    }

    Image {
        id: avatar
        width: 32
        height: 32
        source: "../images/tokens/herodwarf.png"
    }

    property string name: "NoName";
    property variant classKeywords: [ "" ]
    property string className: "Character Class"
    property string classDesc: "Class Description"
    property int statBaseAttack: 0
    property int statBaseDefend: 0
    property int statBaseBody: 0
    property int statBaseMind: 0
    property int statBaseMovement: 0
    property int statCurrentAttack: statBaseAttack + inventory.cachedAttackBonus
    property int statCurrentDefend: statBaseAttack + inventory.cachedDefendBonus
    property int statCurrentBody: statBaseBody + inventory.cachedBodyBonus
    property int statCurrentMind: statBaseMind + inventory.cachedMindBonus
    property int statCurrentMovement: statBaseMovement + inventory.cachedMoveBonus
    property alias avatarImage: avatar.source
    property string profileImage: ""
    property bool isMonster: false;
    property int initiative: 0
    CharacterInventory { id: inventory }
    property alias inventory: inventory

    function calculateMovementSquares() {
        if ( isMonster ) {
            return statCurrentMovement
        }
        return statCurrentMovement * Math.floor( Math.random() * 6 )
    }

    // Gui Elements
    IndicatorRange {
         id: moveRangeIndicator
         range: calculateMovementSquares()
         visible: false
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            console.log( "clicked " + name );
        }
        onDoubleClicked: {
            console.log( "dbl clicked " + name );
        }
        onEntered: {
            console.log( "entered " + name );
            moveRangeIndicator.visible = true
        }
        onExited: {
            console.log( "exited " + name );
            moveRangeIndicator.visible = false
        }
        onPositionChanged: {
            console.log( "moved " + name );
        }
        onPressAndHold: {
            console.log( "hold " + name );
        }
        onPressed: {
            console.log( "pressed " + name );
        }
        onReleased: {
            console.log( "released " + name );
        }
    }

    Component.onCompleted: {
        for ( var i = 0; i < classKeywords.length; i++ ) {
            if ( classKeywords[ i ] == "monster" ) {
               isMonster = true;
            }
        }
    }
}
