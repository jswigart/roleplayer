import QtQuick 1.1

Item {
    HeroBarbarian {
        id: barb
        x: 0
        y: 0

        Component.onCompleted: {
            character.name = "Aragorn"
            character.giveItem( "InventoryItemBroadSword.qml" );
            character.giveItem( "InventoryItemPlateMail.qml" )
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                resolveAttack( barb, goblin )
            }
        }
    }

    HeroDwarf {
        id: dwarf
        x: 32
        y: 0

        Component.onCompleted: {
            character.name = "Gimli"
            character.giveItem( "InventoryItemShortSword.qml" );
            character.giveItem( "InventoryItemChainMail.qml" )
        }
    }

    HeroElf {
        id: elf
        x: 64
        y: 0

        Component.onCompleted: {
            character.name = "Legolas"
            character.giveItem( "InventoryItemShortSword.qml" );
        }
    }

    HeroWizard {
        id: wizard
        x: 96
        y: 0

        Component.onCompleted: {
            character.name = "Gandalf"
            character.giveItem( "InventoryItemDagger.qml" );
        }
    }

    MonsterGoblin {
        id: goblin
        x: 0
        y: 64

        Component.onCompleted: {
            character.name = "Gobez"
        }
    }

    MonsterGargoyle {
        id: garg
        x: 32
        y: 64

        Component.onCompleted: {
            character.name = "Gargoyle"
        }
    }

    Component.onCompleted: {
        //scenario.startBattle()
    }
}
