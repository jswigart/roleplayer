import QtQuick 1.1

Item {
    property alias stats: stats

    InventoryItem {
        id: stats
    }

    Component.onCompleted: {
        stats.name = "Battle Axe"
        stats.slotsUsed = [ 'weapon', 'offhand' ]
        stats.notUsableByClassKeywords = [ 'wizard' ]
        stats.bonusAttack = 4
        stats.attackDiagonally = true
        stats.costBuy = 450
    }
}
