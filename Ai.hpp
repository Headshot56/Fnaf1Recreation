#pragma once
#include <string>

//Define the map with rooms and paths between them for ai to navigate. Rooms and paths dont need pixel positions since all positions are relative to eachother.
class Room{
    //Spot an animatronic can be in. The players position is a special room.
    public:
        std::string name;
        Animatronic* occupant = nullptr; //Pointer to animatronic currently in the room. Nullptr if none.

        Room(std::string n){
            name = n;
        }
};

class Hallway{
    //Path between two rooms. Each path has a probability of heading a certain direction.
    private:
        Room* roomA;
        Room* roomB;
        float probAtoB; //Probability of going from A to B. 1-probAtoB is probability of going from B to A.
    public:
        Hallway(Room* a, Room* b, float pAtoB){
            roomA = a;
            roomB = b;
            probAtoB = pAtoB;
        }
};

class Animatronic{
    //Base class for all animatronics.
    //Planned to be inherited for each animatronic though since they all have different behaviors except for basic stuff like position and move timers
    //This class will need to also hold information that can be used in CameraDraw to show the animatronics on the cameras
    //Just like the actual FNAF1 animatronics we will use ai levels 0-20 to determine how aggressive they are
    //Each animatronic has move timers but unlike the original FNAF these will not be set based on frame rate but rather actual time passed to make it FPS independent
    public:
        std::string name;
        unsigned int aiLevel = 0;
        unsigned int moveTimer = 0; //Time in seconds until next movement opportunity
};