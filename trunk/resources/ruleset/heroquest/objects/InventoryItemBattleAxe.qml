import QtQuick 1.1

Item {
    InventoryItem { id: stats }
    property alias stats: stats

    AbilityAttack {
    }

    Component.onCompleted: {
        stats.name = "Battle Axe"
        stats.category = [ 'weapon' ]
        stats.slotsUsed = [ 'weapon', 'offhand' ]
        stats.notUsableByClassKeywords = [ 'wizard' ]
        stats.bonusAttack = 4
        stats.attackDiagonally = true
        stats.costBuy = 450
    }
}
