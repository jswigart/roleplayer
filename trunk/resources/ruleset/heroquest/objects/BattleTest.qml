import QtQuick 1.1

Item {
    HeroBarbarian {
        id: barb
        x: 0
        y: 0
    }

    HeroDwarf {
        id: dwarf
        x: 100
        y: 100
    }

    MonsterGoblin {
        id: goblin
        x: 50
        y: 50
    }

    function rollAttackDice( numDie ) {
        var attack = 0
        for ( var i = 0; i < numDie; i++ ) {
            var result = ( Math.random() * 6 )
            if ( result < 3 ) {
                ++attack;
            }
        }
        return attack;
    }

    function rollDefendDice( numDie ) {
        var defend = 0
        for ( var i = 0; i < numDie; i++ ) {
            var result = ( Math.random() * 6 )
            if ( result > 4 ) {
                ++defend;
            }

            // for monsters
            //if ( result == 4 ) {
            //  ++defend;
            //}
        }
        return defend;
    }

    function resolveAttack( attacker, defender ) {
        console.log( attacker.character.name + " can move " + attacker.character.calculateMovementSquares() + " squares" )
        console.log( defender.character.name + " can move " + defender.character.calculateMovementSquares() + " squares" )

        var attackDie = attacker.character.statCurrentAttack
        var defendDie = defender.character.statCurrentDefend

        var attack = rollAttackDice( attackDie )
        var defend = rollDefendDice( defendDie )

        console.log( attacker.character.name + " rolled " + attackDie + " attack die for " + attack + " hits"  )
        console.log( defender.character.name + " rolled " + defendDie + " defend die for " + defend + " blocks"  )

        var damage = attack - defend;
        if ( damage > 0 ) {
            console.log( defender.character.name + " took " + damage + " damage points " )
        } else {
            console.log( defender.character.name + " blocked all damage " )
        }
    }

    Component.onCompleted: {
        barb.character.name = "Conan"
        dwarf.character.name = "Gimli"

        barb.character.inventory.giveItem( "InventoryItemPlateMail.qml" )
        dwarf.character.inventory.giveItem( "InventoryItemChainMail.qml" )

        scenario.startBattle()
    }
}
