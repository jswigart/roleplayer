import QtQuick 1.1

Item {
    InventoryItem { id: stats }
    property alias stats: stats

    Component.onCompleted: {
        stats.name = "Shield"
        stats.slotsUsed = [ 'offhand' ]
        stats.notUsableByClassKeywords = [ 'wizard' ]
        stats.bonusDefend = 1
        stats.costBuy = 150
    }
}