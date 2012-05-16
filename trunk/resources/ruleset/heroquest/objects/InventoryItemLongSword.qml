import QtQuick 1.1

Item {
    InventoryItem { id: stats }
    property alias stats: stats

    AbilityAttack {
    }

    Component.onCompleted: {
        stats.name = "Long Sword"
        stats.category = [ 'weapon' ]
        stats.slotsUsed = [ 'weapon' ]
        stats.notUsableByClassKeywords = [ 'wizard' ]
        stats.bonusAttack = 3
        stats.attackDiagonally = true
        stats.costBuy = 350
    }
}
