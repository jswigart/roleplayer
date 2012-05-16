var equipment = new Array
var classAbilityNames = new Array
var abilities = new Array

function addAbilities( abilityNames ) {
    for( var i = 0; i < abilityNames.length; i++ ) {
        addAbility( abilityNames[ i ] )
    }
}

function addAbility( abilityName ) {
    if ( classAbilityNames.indexOf( abilityName ) == -1 ) {
        classAbilityNames.push( abilityName )
        classAbilityNames.sort()
    }
}

function buyItem( itemName ) {
    var component = Qt.createComponent( itemName );
    if (component.status == Component.Ready) {
        var item = component.createObject( character );
        if ( item.stats.costBuy > gold ) {
            console.log( character.name + " can't buy " + item.stats.name )
            item.destroy()
        } else {
            equipment.push( item );

            inventoryUpdated()

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
        var item = component.createObject( character );
        if ( item == null ) {
            console.log( "can't give " + itemName )
        } else {
            equipment.push( item );

            inventoryUpdated()
            console.log( "gave " + character.name + " a " + item.stats.name )

            return item
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

    var weapon = "None"
    var armor = "None"

    // destroy old abilities
    for( var i in abilities ) {
        abilities[ i ].destroy()
    }

    // rebuild new ability list
    var abilities = new Array;
    for( var i = 0; i < classAbilityNames.length; i++ ) {
        var component = Qt.createComponent( classAbilityNames[ i ] );
        if (component.status == Component.Ready) {
            var ability = component.createObject( character );
            if ( ability == null ) {
                console.log( "can't allocate ability " + classAbilityNames[ i ] )
            } else {
                abilities.push( ability );
                console.log( "gave " + character.name + " ability " + ability.name )
            }
        } else {
            console.debug( "giveAbility " + component.errorString() );
        }
    }

    // accumulate all stat bonuses for equipment
    for( var i in equipment ) {
        var item = equipment[ i ]
        attackBonus += item.stats.bonusAttack
        defendBonus += item.stats.bonusDefend
        bodyBonus += item.stats.bonusBody
        mindBonus += item.stats.bonusMind
        moveBonus += item.stats.bonusMove
        if ( weapon == "None" && item.stats.usesSlot( "weapon" ) ) {
            weapon = item.stats.name;
        }
        if ( armor == "None" && item.stats.usesSlot( "chest" ) ) {
            armor = item.stats.name;
        }
    }

    // store it, so property bindings can update any dependent data
    equipmentAttackBonus = attackBonus;
    equipmentDefendBonus = defendBonus;
    equipmentBodyBonus = bodyBonus
    equipmentMindBonus = mindBonus
    equipmentMoveBonus = moveBonus

    activeWeapon = weapon
    activeArmor = armor
}


