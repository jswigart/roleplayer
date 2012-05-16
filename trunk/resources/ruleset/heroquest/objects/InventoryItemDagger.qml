import QtQuick 1.1

Item {
    InventoryItem { id: stats }
    property alias stats: stats

    AbilityAttack {
    }

    Component.onCompleted: {
        stats.name = "Dagger"
        stats.desc = "None"
        stats.category = [ 'weapon' ]
        stats.slotsUsed = [ 'weapon' ]
        stats.bonusAttack = 1
        stats.attackThrowable = true
        stats.costBuy = 25
    }
}
