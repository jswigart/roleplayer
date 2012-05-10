import QtQuick 1.1

Item {
    InventoryItem { id: stats }
    property alias stats: stats

    Component.onCompleted: {
        stats.name = "Dagger"
        stats.desc = "None"
        stats.slotsUsed = [ 'weapon' ]
        stats.bonusAttack = 1
        stats.attackThrowable = true
        stats.costBuy = 25
    }
}
