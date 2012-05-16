import QtQuick 1.1

Item {
    id: stats

    property string name: ""
    property string desc: ""

    function selected() {

    }

    function deSelected() {

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

    function rollDefendDice( numDie, isMonster ) {
        var defend = 0
        for ( var i = 0; i < numDie; i++ ) {
            var result = ( Math.random() * 6 )
            if ( isMonster ) {
                if ( result == 4 ) {
                  ++defend;
                }
            } else {
                if ( result > 4 ) {
                    ++defend;
                }
            }
        }
        return defend;
    }

    function useAbility( user, target ) {
         console.log( "useAbility not implemented" )
    }

    Component.onCompleted: {
    }
    Component.onDestruction: {
    }
}
