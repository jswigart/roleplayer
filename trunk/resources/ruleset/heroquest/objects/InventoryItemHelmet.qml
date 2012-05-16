import QtQuick 1.1

Item {
    InventoryItem { id: stats }
    property alias stats: stats

    Component.onCompleted: {
        stats.name = "Helmet"
        stats.category = [ 'armor' ]
        stats.slotsUsed = [ 'head' ]
        stats.notUsableByClassKeywords = [ 'wizard' ]
        stats.bonusDefend = 1
        stats.costBuy = 125
    }
}
