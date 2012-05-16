import QtQuick 1.1

Item {
    InventoryItem { id: stats }
    property alias stats: stats

    Component.onCompleted: {
        stats.name = "Tool Kit"
        stats.desc = "None"
        stats.costBuy = 250
        stats.category = [ 'tool' ]

        parent.addAbility( "AbilityDisarmTrap.qml" )
    }
}
