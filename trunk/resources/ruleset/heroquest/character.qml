// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import TileTools 1.0

GameCharacter {
    id: character
    objectName: "character"
    width: 32
    height: 32
    Image {
        width: parent.width
        height: parent.height
        source: "../../images/tokens/pog17.png"
    }

}
