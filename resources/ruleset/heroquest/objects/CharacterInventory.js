function buyItem( itemName ) {
    var component = Qt.createComponent( itemName );
    if (component.status == Component.Ready) {
        var item = component.createObject( inventory );
        if ( item.stats.costBuy > gold ) {
            console.log( character.name + " can't buy " + item.stats.name )
            item.destroy()
        } else {
            inventoryUpdated()

            //item.parent = inventory;
            gold = gold - item.stats.costBuy

            console.log( character.name + " bought " + item.stats.name + " for " + item.stats.costBuy )
            console.log( character.name + " has " + gold + " left" )
        }
    } else {
        console.debug( "buyItem " + component.errorString() );
    }
}

function giveItem( itemName ) {
    var component = Qt.createComponent( itemName );
    if (component.status == Component.Ready) {
        var item = component.createObject( inventory );
        if ( item == null ) {
            console.log( "can't give " + itemName )
        } else {
            inventoryUpdated()
            // item.parent = inventory;
            console.log( "gave " + character.name + " a " + item.stats.name )
        }
    } else {
        console.debug( "giveItem " + component.errorString() );
    }
}

function inventoryDirty() {
    var attackBonus = 0
    var defendBonus = 0
    var bodyBonus = 0
    var mindBonus = 0
    var moveBonus = 0

    var weapon = ""
    var armor = ""

    // accumulate all stat bonuses for equipment
    for ( var i = 0; i < inventory.children.length; ++i ) {
        var item = inventory.children[ i ]
        attackBonus += item.stats.bonusAttack
        defendBonus += item.stats.bonusDefend
        bodyBonus += item.stats.bonusBody
        mindBonus += item.stats.bonusMind
        moveBonus += item.stats.bonusMove
        if ( weapon == "" && item.stats.usesSlot( "weapon" ) ) {
            weapon = item.stats.name;
        }
        if ( armor == "" && item.stats.usesSlot( "chest" ) ) {
            armor = item.stats.name;
        }
    }

    // store it, so property bindings can update any dependent data
    cachedAttackBonus = attackBonus;
    cachedDefendBonus = defendBonus;
    cachedBodyBonus = bodyBonus
    cachedMindBonus = mindBonus
    cachedMoveBonus = moveBonus

    activeWeapon = weapon
    activeArmor = armor
}

