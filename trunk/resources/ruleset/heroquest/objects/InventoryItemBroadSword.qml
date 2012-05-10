import QtQuick 1.1

Item {
    InventoryItem { id: stats }
    property alias stats: stats

    Component.onCompleted: {
        stats.name = "Broad Sword"
        stats.slotsUsed = [ 'weapon' ]
        stats.notUsableByClassKeywords = [ 'wizard' ]
        stats.bonusAttack = 3
        stats.costBuy = 250
    }
}
