import QtQuick 1.1

Item {
    InventoryItem { id: stats }
    property alias stats: stats

    Component.onCompleted: {
        stats.name = "Short Sword"
        stats.slotsUsed = [ 'weapon' ]
        stats.notUsableByClassKeywords = [ 'wizard' ]
        stats.bonusAttack = 2
        stats.costBuy = 150
    }
}
