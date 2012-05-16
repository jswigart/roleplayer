import QtQuick 1.1

Item {
    InventoryItem { id: stats }
    property alias stats: stats

    Component.onCompleted: {
        stats.name = "Chain Mail"
        stats.category = [ 'armor' ]
        stats.slotsUsed = [ 'chest' ]
        stats.notUsableByClassKeywords = [ 'wizard' ]
        stats.bonusDefend = 1
        stats.costBuy = 500
    }
}
