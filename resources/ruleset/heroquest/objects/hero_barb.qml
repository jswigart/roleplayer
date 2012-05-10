// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import TileTools 1.0

GameCharacter {
    id: character
    objectName: "Barbarian"
    width: 64
    height: 64
    Image {
        width: parent.width
        height: parent.height
        source: "../images/tokens/herobarbarian.png"
    }
}
