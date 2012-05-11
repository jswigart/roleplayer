// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {
    id: sheet
    x: 0
    y : 0
    width: bg.width * scale
    height: bg.height * scale
    transformOrigin: Item.TopLeft

    scale: 0.5

    property string name : "Default Character Name"
    property string statProfileImage: "elf.jpg"
    property int statLevel: 1
    property int stateTotalXP: 0
    property int statInitiativeScore: 0
    property int statHitPointsMax: 20
    property int statHitPointsCurrent: 20
    property int statHitPointsBloodied: Math.floor( sheet.statHitPointsMax * 0.5 )
    property int stateHitPointsTemp: 0
    property int stateHealSurgeValue: 0
    property int statHealSurgePerDay: 0
    property int stateHealSurgeRemaining: 0

    property int statStrength: 15
    property int statStrengthMod: Math.floor( -5 + sheet.statStrength / 2 )
    property int statStrengthModHalfLevel: Math.floor( sheet.statStrengthMod + statLevel / 2 )
    property int statConstitution: 15
    property int statConstitutionMod: Math.floor( -5 + sheet.statConstitution / 2 )
    property int statConstitutionModHalfLevel: Math.floor( sheet.statConstitutionMod + sheet.statLevel / 2 )
    property int statDexterity: 15
    property int statDexterityMod: Math.floor( -5 + sheet.statDexterity / 2 )
    property int statDexterityModHalfLevel: Math.floor( sheet.statDexterityMod + sheet.statLevel / 2 )
    property int statIntelligence: 15
    property int statIntelligenceMod: Math.floor( -5 + sheet.statIntelligence / 2 )
    property int statIntelligenceModHalfLevel: Math.floor( sheet.statIntelligenceMod + sheet.statLevel / 2 )
    property int statWisdom: 15
    property int statWisdomMod: Math.floor( -5 + sheet.statWisdom / 2 )
    property int statWisdomModHalfLevel: Math.floor( sheet.statWisdomMod + sheet.statLevel / 2 )
    property int statCharisma: 15
    property int statCharismaMod: Math.floor( -5 + sheet.statCharisma / 2 )
    property int statCharismaModHalfLevel: Math.floor( sheet.statCharismaMod + sheet.statLevel / 2 )

    Component.onCompleted: {
    }

    Image {
        id: bg
        x: 0
        y: 0
        width: sourceSize.width
        height: sourceSize.height
        transformOrigin: Item.Center
        visible: true

        source: "charactersheet.jpg"
        fillMode: Image.Stretch

        Item {
            id: skills
            anchors.fill: parent

            Text {
                id: diplomacy
                x: 578
                y: 364
                text: "0"
                font.pixelSize: 28
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                id: bluff
                x: 578
                y: 327
                text: "0"
                font.pixelSize: 28
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                id: athletics
                x: 578
                y: 293
                text: "0"
                font.pixelSize: 28
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                id: arcana
                x: 578
                y: 259
                text: "0"
                font.pixelSize: 28
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                id: acrobatics
                x: 578
                y: 225
                text: "0"
                font.pixelSize: 28
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                id: dungeoneering
                x: 578
                y: 398
                text: "0"
                font.pixelSize: 28
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                id: endurance
                x: 578
                y: 432
                text: "0"
                font.pixelSize: 28
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                id: heal
                x: 578
                y: 466
                text: "0"
                font.pixelSize: 28
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                id: history
                x: 578
                y: 505
                text: "0"
                font.pixelSize: 28
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                id: insight
                x: 578
                y: 539
                text: "0"
                font.pixelSize: 28
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                id: intimidate
                x: 578
                y: 573
                text: "0"
                font.pixelSize: 28
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                id: nature
                x: 578
                y: 607
                text: "0"
                font.pixelSize: 28
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                id: perception
                x: 578
                y: 641
                text: "0"
                font.pixelSize: 28
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                id: religion
                x: 578
                y: 680
                text: "0"
                font.pixelSize: 28
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                id: thievery
                x: 578
                y: 782
                text: "0"
                font.pixelSize: 28
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                id: streetwise
                x: 578
                y: 748
                text: "0"
                font.pixelSize: 28
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                id: stealth
                x: 578
                y: 714
                text: "0"
                font.pixelSize: 28
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }






        }

        Item {
            id: stats
            anchors.fill: parent

            Text {
                id: charismaModHalfLevel
                x: 418
                y: 524
                text: sheet.statCharismaModHalfLevel
                font.pixelSize: 28
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                    id: charismaMod
                    x: 296
                    y: 524
                    text: sheet.statCharismaMod
                    font.pixelSize: 28
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
            }

            Text {
                id: charisma
                x: 53
                y: 524
                text: sheet.statCharisma
                font.pixelSize: 28
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                id: wisdomModHalfLevel
                x: 418
                    y: 478
                    text: sheet.statWisdomModHalfLevel
                    font.pixelSize: 28
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
            }

            Text {
                id: wisdomMod
                x: 296
                y: 478
                text: sheet.statWisdomMod
                font.pixelSize: 28
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                id: wisdom
                x: 53
                y: 478
                text: sheet.statWisdom
                font.pixelSize: 28
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
            }

            Text {
                id: intelligenceModHalfLevel
                x: 418
                y: 413
                text: sheet.statIntelligenceModHalfLevel
                font.pixelSize: 28
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
            }

            Text {
                id: intelligenceMod
                x: 296
                y: 413
                text: sheet.statIntelligenceMod
                font.pixelSize: 28
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                id: intelligence
                x: 53
                y: 413
                text: sheet.statIntelligence
                font.pixelSize: 28
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                id: dexterityModHalfLevel
                x: 418
                y: 367
                text: sheet.statDexterityModHalfLevel
                font.pixelSize: 28
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                id: dexterityMod
                    x: 296
                    y: 367
                    text: sheet.statDexterityMod
                    font.pixelSize: 28
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
            }

            Text {
                id: dexterity
                x: 53
                y: 367
                text: sheet.statDexterity
                font.pixelSize: 28
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                id: constitutionModHalfLevel
                x: 418
                y: 299
                text: sheet.statConstitutionModHalfLevel
                font.pixelSize: 28
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                id: constitutionMod
                x: 296
                y: 299
                text: sheet.statConstitutionMod
                font.pixelSize: 28
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                id: constitution
                x: 53
                y: 299
                text: sheet.statConstitution
                font.pixelSize: 28
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                }

            Text {
                id: strengthModHalfLevel
                x: 418
                y: 253
                text: sheet.statStrengthModHalfLevel
                font.pixelSize: 28
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                id: strengthMod
                x: 296
                y: 253
                text: sheet.statStrengthMod
                font.pixelSize: 28
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                id: strength
                x: 53
                y: 253
                text: "15"
                textFormat: Text.RichText
                verticalAlignment: Text.AlignTop
                font.pixelSize: 28
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                id: level
                x: 1048
                y: 74
                text: sheet.statLevel
                font.pixelSize: 24
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                id: characterName
                x: 722
                y: 32
                text: sheet.name
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 28
            }

            Text {
                id: initiativeScore
                x: 58
                y: 118
                text: sheet.statInitiativeScore
                font.pixelSize: 28
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                id: totalXP
                x: 1479
                y: 35
                text: sheet.stateTotalXP
                font.pixelSize: 24
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                id: hitPointsMax
                x: 1073
                y: 847
                text: sheet.statHitPointsMax
                font.pixelSize: 24
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }
            Text {
                id: hitPointsCurrent
                x: 1293
                y: 1029
                text: sheet.statHitPointsCurrent
                font.pixelSize: 30
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }
            Text {
                id: hitPointsBloodied
                x: 1497
                y: 847
                text: sheet.statHitPointsBloodied
                font.pixelSize: 24
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                id: healSurgeValue
                x: 1497
                y: 895
                text: sheet.stateHealSurgeValue
                font.pixelSize: 24
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                id: healSurgePerDay
                x: 1497
                y: 933
                text: sheet.statHealSurgePerDay
                font.pixelSize: 24
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }



            Text {
                id: hitPointsTemp
                x: 1155
                y: 1124
                text: sheet.stateHitPointsTemp
                font.pixelSize: 24
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                id: healSurgeRemaining
                x: 1464
                y: 1124
                text: sheet.stateHealSurgeRemaining
                font.pixelSize: 24
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            Image {
                id: profilePic
                x: 1112
                y: 168
                width: 382
                height: 344
                sourceSize.width: 304
                fillMode: Image.PreserveAspectFit
                source: sheet.statProfileImage
            }
        }
    }
}
