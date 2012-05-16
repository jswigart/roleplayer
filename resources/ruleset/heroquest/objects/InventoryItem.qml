import QtQuick 1.1

QtObject {
    id: stats

    property string name: ""
    property string desc: ""

    // armor, weapon, tool, potion, etc
    property variant category: [ '' ]

    // valid slots: head, chest, boots, shield, weapon, offhand. can occupy multiple slots
    property variant slotsUsed: [ '' ]

    // restrictions
    property variant notUsableByClassKeywords: [ '' ]
    property variant notUsableWithSlotKeywords: [ '' ]

    // stat modifiers
    property int bonusAttack: 0
    property int bonusDefend: 0
    property int bonusBody: 0
    property int bonusMind: 0
    property int bonusMove: 0

    property int attackRangeMin: 0
    property int attackRangeMax: 1

    property bool rangedWeapon: false
    property bool attackDiagonally: false
    property bool attackThrowable: false

    // cost to buy/sell
    property int costBuy: 0
    property int costSell: 0

    function usesSlot( slotName ) {
        for ( var i = 0; i < slotsUsed.length; i++ ) {
            if ( slotsUsed[ i ] == slotName ) {
                return true
            }
        }
        return false
    }

    Component.onCompleted: {
    }
    Component.onDestruction: {
    }
}
