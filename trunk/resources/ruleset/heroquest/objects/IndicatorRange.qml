import QtQuick 1.1

Item {
    id: root
    z: -10
    visible: false

    property real range: 7
    property real step: 32

    ////////////////////////////////////////////////////////
    // TEST
    Timer {
        id: testtoggle
        interval: 2000; running: true; repeat: true
        onTriggered: {
            root.visible = !root.visible
        }
    }
    ////////////////////////////////////////////////////////
//    Timer {
//        id: deleter
//        interval: 10; running: false; repeat: true

//        property int d: 0

//        onTriggered: {
//            for ( var i = 0; i < root.children.length; ++i ) {
//                if ( root.children[ i ].d == d ) {
//                    root.children[ i ].destroy()
//                }
//            }
//            --d;
//        }
//    }
    ////////////////////////////////////////////////////////
    Component {
        id: block

        Rectangle {
            id: b
            property real d: 0

            border.width: 1
            border.color: "black"
            color: Qt.lighter( "blue", 1.0 + d / range )
            radius: 10
            width: step * 0.99
            height: step * 0.99

            Text {
                id: dist
                anchors.fill: parent
                color: "black"
                verticalAlignment: Text.AlignVCenter
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignHCenter
                text: d
            }

            Timer {
                id: expander
                interval: 10; running: true; repeat: false
                onTriggered: {
                    if ( root.visible ) {
                        expand( b )
                    }
                 }
             }

            Component.onCompleted: {
                console.log( "item created at " + x + " x " + y + " y " + d + " distance" );
            }

            onVisibleChanged: {
                if ( visible ) {
                    //deleter.stop()
                    expand( b )
                } else {
                    var highestBlock = 1
                    for ( var i = 0; i < root.children.length; ++i ) {
                        if ( root.children[ i ].d > highestBlock ) {
                            highestBlock = root.children[ i ].d
                        }
                    }
                    deleter.d = highestBlock
                    deleter.start();
                }
            }
        }
    }
    ////////////////////////////////////////////////////////

    Component.onCompleted: {
        block.createObject( root, { "x": 0, "y": 0, "d": 0 } )
    }

    function expand( element ) {
        element.objectName = "obj_" + x + "_" + y

        var expandd = element.d + 1

        console.log( "expand" );

        if ( element.d <= root.range ) {
            for ( var i = -1; i <= 1; i++ ) {
                if ( i == 0 ) {
                    continue;
                }

                var expandx = element.pos.x + step * i
                var expandy = element.pos.y
                var name = "obj_" + expandx + "_" + expandy
                var existingItem = root.childAt( expandx, expandy )
                if ( existingItem == null ) {
                    block.createObject( root, { "d": expandd, "x": expandx, "y": expandy} )
                    //console.log( "item created at " + expandx + "x " + expandy + "y" );
                } else {
                    //console.log( "item at " + expandx + " x " + expandy + " y " + existingItem );
                }
            }

            for ( var i = -1; i <= 1; i++ ) {
                if ( i == 0 ) {
                    continue;
                }

                var expandx = element.pos.x
                var expandy = element.pos.y + step * i
                var name = "obj_" + expandx + "_" + expandy
                var existingItem = root.childAt( expandx, expandy )
                if ( existingItem == null ) {
                    block.createObject( root, { "d": expandd, "x": expandx, "y": expandy} )
                    //console.log( "item created at " + expandx + "x " + expandy + "y" );
                } else {
                    //console.log( "item at " + expandx + " x " + expandy + " y " + existingItem );
                }
            }
        }
    }
}
