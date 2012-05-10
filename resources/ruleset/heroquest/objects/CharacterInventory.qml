import QtQuick 1.1

import "CharacterInventory.js" as Code

Item {
    id: inventory
    objectName: "inventory"

    property int gold: 10000;
    property int cachedAttackBonus: 0
    property int cachedDefendBonus: 0
    property int cachedBodyBonus: 0
    property int cachedMindBonus: 0
    property int cachedMoveBonus: 0

    // fixme
    property string activeWeapon
    property string activeArmor

    signal inventoryUpdated
    onInventoryUpdated: { Code.inventoryDirty() }

    function buyItem( itemName ) { Code.buyItem( itemName ) }
    function giveItem( itemName ) { Code.giveItem( itemName ) }

    Component.onCompleted: {
        //console.log( "onCompleted " + objectName );
    }
}
