import QtQuick 1.1
import Box2D

// Hero options
// Movement - roll move d 6
// Actions
//  - Attack an adjacent monster
//  - Cast a spell on monster or hero that can be seen
//  - Search for treasure in the current room
//  - Search for secret doors in the room or corridor
//  - Search for traps in the room or corridor
//  - Disarm a trap on the square he is on
// caught by trap, drinking potion, picking things up, opening doors, looking through doors/corridors dont count as actions, can be used mid movement

// DM options
// Movement - fixed move squares per monster type, allowed to not move
// Actions
//  - Attack an adjacent hero
//  - Cast a spell on hero that can be seen by monster

World {
    id: physworld


}
