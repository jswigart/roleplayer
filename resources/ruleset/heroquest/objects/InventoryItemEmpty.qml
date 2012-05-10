import QtQuick 1.1

Item {
    InventoryItem { id: stats }
    property alias stats: stats

    Component.onCompleted: {
        stats.name = "None"
        stats.desc = "None"
    }
}
