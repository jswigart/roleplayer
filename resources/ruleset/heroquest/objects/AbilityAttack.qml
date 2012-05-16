import QtQuick 1.1

Item {
    id: stats

    property string name: "Attack"
    property string desc: "Attack a target with your currently equipped weapon"

    function useAbility( user, target ) {
        var attackDie = user.character.statCurrentAttack
        var defendDie = target.character.statCurrentDefend

        var attack = rollAttackDice( attackDie )
        var defend = rollDefendDice( defendDie, target.character.isMonster )

        //console.log( user.character.name + " rolled " + attackDie + " attack die for " + attack + " hits"  )
        //console.log( target.character.name + " rolled " + defendDie + " defend die for " + defend + " blocks"  )

        var msg = user.character.name
        msg += "(" + user.character.activeWeapon + ") "
        msg += target.character.name
        msg += "(" + target.character.activeArmor + ") "

        console.log( msg )
        console.log( "att(" + attackDie + ") vs def(" + defendDie + ") = rolled " + attack + " vs " + defend )

        var damage = attack - defend;
        if ( damage > 0 ) {
            console.log( target.character.name + " took " + damage + " damage points " )
        } else {
            console.log( target.character.name + " blocked all damage " )
        }
    }
    Component.onCompleted: {
    }
}
