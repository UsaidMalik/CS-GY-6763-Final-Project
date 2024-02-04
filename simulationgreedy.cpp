#include <iostream>
#include <vector>
#include <cstdlib>
#include <climits>
#include <time.h>
using namespace std; 

/* Question
 With a one D line that extends L in one direction and -L the other direction
# and with treasure that spawns in at every time step
# whats the max treasure that can be acquired in T seconds
# treasure may not spawn in the same position twice (makes the problem easier)
# given the player starts at the origin*/


pair<int, int> closestTreasureFinder(const vector<int>& treasureArray, const int currPos);
void visualizer(vector<int>& treasureArray, const int& playerStartingPosition) ;
int greedySimulation(vector<int>& treasureArray, const int& playerStartingPosition, int Time);
void spawnTreasure(vector<int>& treasureArray);
vector<pair<int, int>> treasureOracle(const vector<int>& treasureArray);
pair<int, int> closestTreasureFinder(const vector<int>& treasureArray, const int currPos);
int beelineSimulation(vector<int>& treasureArray, const int& playerStartingPosition, int Time, int deterministicFlag, int oppositeFlag);
int randomWalkSimulation(vector<int>& treasureArray, const int& playerStartingPosition, int Time, int maxStep);


void visualizer(vector<int>& treasureArray, const int& playerStartingPosition, int Time, int totalTreasure) {
    for (int treasure : treasureArray){
        cout << treasure << " ";
    }
    cout << endl;
    for (size_t i = 0; i < playerStartingPosition; ++i){
        cout << "  "; // two spaces
    }
    cout << "*"; // outputting the player
    cout << endl; 
    cout << "Player at position " << playerStartingPosition << " Time is: " << Time << " Total Treasure Secured is " << totalTreasure;
    cout << endl;
    cout << endl;
}

int greedySimulation(vector<int>& treasureArray, const int& playerStartingPosition, int time) {
    int currPos = playerStartingPosition;
    int totalTreasure = 0;
    for (size_t i = 0; i < time; ++i){
        // spawn in the treasure first
        spawnTreasure(treasureArray);
       // visualizer(treasureArray, currPos, i, totalTreasure);
        // call the closest treasure finder to know where to go next (and how much acquired)
        pair<int, int> closestTreasure = closestTreasureFinder(treasureArray, currPos); 
        // now figure out where to go next
        if (closestTreasure.first < currPos){
            // if treasure to the left
            --currPos; // move to the left
        } else if (closestTreasure.first > currPos){
            ++currPos; // move to the right
        } 
        // check if we've arrived at the treasure already then take the treasure
        if (treasureArray[currPos] >= 1){
            totalTreasure += treasureArray[currPos];
            treasureArray[currPos] = 0;
        }
    }
    return totalTreasure;
}

void spawnTreasure(vector<int>& treasureArray) {
    // generating random number between 0 and len(array)
    while (true) {
        int treasureSpawn = rand() % treasureArray.size();

        if (treasureArray[treasureSpawn] == 0){
            treasureArray[treasureSpawn] = 1;
            break;
        }
    }
}

vector<pair<int, int>> treasureOracle(const vector<int>& treasureArray) {
    
    vector<pair<int, int>> treasurePositionsAndValue;
    for (size_t i = 0; i <  treasureArray.size(); ++i){
        if (treasureArray[i] >= 1){
            treasurePositionsAndValue.push_back(make_pair(i, treasureArray[i]));
        }
    }
    return treasurePositionsAndValue;
}

pair<int, int> closestTreasureFinder(const vector<int>& treasureArray, const int currPos) {
    vector<pair<int, int>> treasurePositionsAndValue = treasureOracle(treasureArray);
    pair<int, int> smallestIdxValue = make_pair(INT_MAX, INT_MAX);
    int smallestDist = INT_MAX;
    for (const pair<int, int>& items : treasurePositionsAndValue){
        if (abs(items.first - currPos) < smallestDist){
            smallestIdxValue.first = items.first;
            smallestIdxValue.second = items.second;
        }
    }
    return smallestIdxValue;
}

int randomWalkSimulation(vector<int>& treasureArray, const int& playerStartingPosition, int Time, int maxStep){

    int currPos = playerStartingPosition;
    int totalTreasure = 0;

    while (Time > 0)
    {
        // figure out how long im going to step for and which direction
        int currStep = rand() % maxStep; // my steps are up so ill make a new stp
        bool directionFlag = rand() % 2 == 0; // randomly setting the flag if true we go right if flase we go left
        // spawn in the treasure first
        // call the closest treasure finder to know where to go next (and how much acquired)
        while (currStep > 0 && Time > 0){ // ensuring that i have enougfh steps and enough time
        {
            spawnTreasure(treasureArray); // spawn in the first treasure.

            visualizer(treasureArray, currPos, Time, totalTreasure);
            if (treasureArray[currPos] >= 1){ // ill check if my current pos gives me any treasure in which case i wont move
                totalTreasure += treasureArray[currPos];
                treasureArray[currPos] = 0;
            } else{
                // if my current pos gave me no treasure ill make my move
                if (directionFlag) // go right
                    currPos++;
                else
                    currPos--; // go to the left
            }
            cout << "Curr step is" << currStep << endl;
            cout << "Direction is " << (directionFlag ? " Right" : "Left") << endl;
            currStep--;
            Time--; // i used up a time step so i have to get rid of it
        }
    }
    }

    return totalTreasure;

}



int beelineSimulation(vector<int>& treasureArray, const int& playerStartingPosition, int Time, int deterministicFlag, int oppositeFlag){
    // if deterministic flag is true the beeline goes in teh opposite direction of the first treasure spawn if the opposite flag is also true
    // otherwwise beeline will go towards teh first treasure

    int currPos = playerStartingPosition;
    int totalTreasure = 0;
    bool directionFlag = rand() % 2 == 0; // randomly setting the flag if true we go right if flase we go left

    // spawn in the treasure once and go to the opposite side. of the spawn if deterministic flag set
    spawnTreasure(treasureArray);    
    if (deterministicFlag){ 
        pair<int, int> spawnPoint = closestTreasureFinder(treasureArray, currPos); // the treasure thats closest
        if (oppositeFlag){  
            bool directionFlag = spawnPoint.first <= playerStartingPosition; // LEQ since either or is fine if it spawns on the head
        } // if the opp osite flag not set we set the direction flag the other wawy
        bool directionFlag = spawnPoint.first >= playerStartingPosition; // we go towards the first one so  to the right
    } 

    for (size_t i = 0; i < Time; ++i){
    {
        
       visualizer(treasureArray, currPos, i, totalTreasure); // calling the visualizer to see how it looks.

        // before moving ill check if i should beeline left, beeline right, or stay put if a treasure spawns on my head. 
        if (treasureArray[currPos] >= 1){ // ill check if my current pos gives me any treasure in which case i wont move
            totalTreasure += treasureArray[currPos];
            treasureArray[currPos] = 0;
        } else{
            // if my current pos gave me no treasure ill make my move
            if (directionFlag) // go right
                currPos++;
            else
                currPos--; // go to the left
        }

        if (currPos == treasureArray.size() - 1)
            directionFlag = false; // checking if ive reached the all the way right side of the board. in which case ill tell  the direction flag explicity to go to teh left
        else if (currPos == 0)
            directionFlag = true ; // otherwise ill set the direction flag to fo to teh rightg
        
        spawnTreasure(treasureArray);    // spawning in the treasure 

    }
    

    }
    return totalTreasure;
}



int main(){

   
    srand(time(0)); // calling srand with seed

    vector<int> iterations = { 10000}; // num simulations
    vector<int> TimeSteps = {5, 10, 20, 40, 60, 100, 200}; // num time steps
    vector<int> lengthSteps = {5, 10, 20, 40, 60, 100, 200}; // length steps
    vector<int> oppositeFlags = {true, false};
    vector<int> deterministicFlags = {true, false};
    // doing it monte carlo
    cout << "Monte Carlo Iterations, Amount of Time Steps, Length (L) of the Array, Average Treasure Acquired, Deterministic Flag Set, Opposite Flag Set " << endl;
    for (int iteration : iterations)
        for(int timeStep : TimeSteps)
            for(int lengthStep : lengthSteps)
                {
                        int avg = 0;
                        for (size_t i = 0; i < iteration; ++i){
                            int playerPosition = lengthStep;
                            vector<int> treasureArray(lengthStep*2 +1, 0); // making thing -L and L
                            avg += greedySimulation(treasureArray, playerPosition, timeStep);    
                        }
                        avg /= iteration;
                        cout << iteration << "," << timeStep << "," << lengthStep << "," << avg << "," << endl; 
            
            }
}