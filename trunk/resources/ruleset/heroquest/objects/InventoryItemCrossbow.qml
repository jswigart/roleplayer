import QtQuick 1.1

Item {
    InventoryItem { id: stats }
    property alias stats: stats

    AbilityAttack {
    }

    Component.onCompleted: {
        stats.name = "Crossbow"
        stats.desc = "None"
        stats.category = [ 'weapon', 'ranged' ]
        stats.slotsUsed = [ 'weapon' ]
        stats.bonusAttack = 3
        stats.rangedWeapon = true
        stats.attackRangeMin = 2
        stats.costBuy = 350
    }
}
