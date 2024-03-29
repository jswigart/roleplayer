import QtQuick 1.1
import RolePlayerTools 1.0
//import QtDesktop 0.1

 import "Character.js" as Code

Item {
    id: character
    width: avatar.width
    height: avatar.height

    Behavior on rotation {
        RotationAnimation {
            duration: 500
            direction: RotationAnimation.Shortest
        }
    }

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
    property int statBaseMovement: 2
    property int statCurrentAttack: statBaseAttack + equipmentAttackBonus
    property int statCurrentDefend: statBaseAttack + equipmentDefendBonus
    property int statCurrentBody: statBaseBody + equipmentBodyBonus
    property int statCurrentMind: statBaseMind + equipmentMindBonus
    property int statCurrentMovement: statBaseMovement + equipmentMoveBonus
    property alias avatarImage: avatar.source
    property string profileImage: ""
    property bool isMonster: false;

    property int initiative: 0
    property bool turnEnabled: true

    ///////////////////////////////////////
    // equipment
    property int gold: 0;
    property int equipmentAttackBonus: 0
    property int equipmentDefendBonus: 0
    property int equipmentBodyBonus: 0
    property int equipmentMindBonus: 0
    property int equipmentMoveBonus: 0

    property string activeWeapon: "None"
    property string activeArmor: "None"

    signal inventoryUpdated
    onInventoryUpdated: { Code.inventoryDirty() }

    ///////////////////////////////////////
    function buyItem( itemName ) { Code.buyItem( itemName ) }
    function giveItem( itemName ) { Code.giveItem( itemName ) }
    ///////////////////////////////////////
    function addAbilities( abilityNames ) { Code.addAbilities( abilityNames ) }
    function addAbility( abilityName ) { Code.addAbility( abilityName ) }
    ///////////////////////////////////////
    function calculateMovementSquares() { return Code.calculateMovementSquares() }
    ///////////////////////////////////////

    // Gui Elements
    RangeIndicator {
        id: moveRangeIndicator
        opacity: 0
        range: calculateMovementSquares()
        rangeStep: avatar.width
        Behavior on opacity { NumberAnimation { duration: 500 } }
    }

    RangeIndicator {
        id: attackRangeIndicator
        opacity: 0
        range: 1
        rangeStep: avatar.width
        rangeColor: "red"
        includeDiagonal: true
        Behavior on opacity { NumberAnimation { duration: 500 } }
    }

    MouseArea {
        anchors.fill: parent
        visible: turnEnabled
        hoverEnabled: true
        onClicked:          { Code.mouseClicked() }
        onDoubleClicked:    { Code.mouseDoubleClicked() }
        onEntered:          { Code.mouseEntered() }
        onExited:           { Code.mouseExited() }
        onPositionChanged:  { Code.mousePositionChanged() }
        onPressAndHold:     { Code.mousePressAndHold() }
        onPressed:          { Code.mousePressed() }
        onReleased:         { Code.mouseReleased() }
    }

    function startBattle() {
        console.log( "startBattle " + name );

        // monsters always go last
        if ( isMonster ) {
            initiative = 2;
        } else {
            initiative = 1
        }
    }

    Component.onCompleted: {
        isMonster = false;
        for ( var i = 0; i < classKeywords.length; i++ ) {
            if ( classKeywords[ i ] == "monster" ) {
               isMonster = true;
            }
        }

        addAbilities( [  "AbilityAttack.qml",
                         "AbilitySearchForSecretDoors.qml",
                         "AbilitySearchForTraps.qml",
                         "AbilitySearchForTreasures.qml" ] )

        //scenario.addGameObject( self );
    }
}
