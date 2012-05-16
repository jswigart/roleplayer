import QtQuick 1.1

Item {
    InventoryItem { id: stats }
    property alias stats: stats

    AbilityAttack {
    }

    Component.onCompleted: {
        stats.name = "Staff"
        stats.category = [ 'weapon' ]
        stats.slotsUsed = [ 'weapon', 'offhand' ]
        stats.bonusAttack = 1
        stats.attackDiagonally = true
        stats.costBuy = 100
    }
}
