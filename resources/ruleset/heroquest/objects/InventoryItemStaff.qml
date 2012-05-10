import QtQuick 1.1

Item {
    InventoryItem { id: stats }
    property alias stats: stats

    Component.onCompleted: {
        stats.name = "Staff"
        stats.slotsUsed = [ 'weapon', 'offhand' ]
        stats.bonusAttack = 1
        stats.attackDiagonally = true
        stats.costBuy = 100
    }
}
