import QtQuick 1.1

Item {
    HeroBarbarian {
        id: barb
    }

    HeroDwarf {
        id: dwarf
    }

    MouseArea {
        id: clickArea
        x: 0
        y: 0
        width: 100
        height: 100

        onClicked: {
            resolveAttack( barb, dwarf )
        }
    }

    function rollAttackDice( num ) {
        var attack = 0
        for ( var i = 0; i < num; i++ ) {
            var result = ( Math.random() * 6 )
            if ( result < 3 ) {
                ++attack;
            }
        }
        return attack;
    }
    function rollDefendDice( num ) {
        var defend = 0
        for ( var i = 0; i < num; i++ ) {
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

//        resolveAttack( barb, dwarf )
//        resolveAttack( barb, dwarf )
//        resolveAttack( barb, dwarf )
//        resolveAttack( barb, dwarf )
    }
}
