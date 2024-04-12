/**  @file csma.cpp
 *
 *  @brief CSMA simulator
 *
 *  @author Ana Bandari (abandari)
 *  @author Dajeong Kim (dkim2)
 * 
 *  @bug None known at this time.  
 * 
 */


/*   Includes   */
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string.h>
#include <algorithm>
#include <map>
#include <stdio.h>
#include <vector>

using namespace std;

typedef struct{
    int number, r_status, backoff;
} node_info; 

typedef struct{
    int N, L, M, T; 
    unsigned int total_time, current_time;
    vector<node_info> node;
    vector<int> R; // should we use a set or a vector...?
} values;

static values value;


// Implement file input 
void system_input(string filename){
    ifstream input;
	input.open(filename); 
	if (!input) {
        cerr << "Error: Unable to open file!\n";
		exit(0);
    }
}

// write a function to calculate a backoff?? 
    // its just the modulus but sureee
int backoff(int nodeID, int ticks, int R){
    int backoff; 
    backoff = (nodeID+ticks)% R;  //backoff = mod(nodeID + ticks, R);
    return(backoff);
}


/*
    Some pseudo-code:
    if backoff == 0  
        current time = current_clock_tick + L
        Assign new random value 

    Now is the issue of multilple nodes! 
    if backoff_node_count != 1 
        for each node that is 0 assign random values for each of them. Increase random nodes based on the R vector tracker. 


*/
int csma_calc(){
    vector<int> ready;
    for(int i; i < value.N; i++){
        if(value.node[i].backoff == 0){
            ready.push_back(i); // would a set be better here????
        }
    }

    if(ready.size() > 1){
        // collison occurs increase the R value!!
    }

    if(ready.size() == 1){
        // no collison change the backoff for this node onlyyyy
        // keep track of these number of slots!
        // increase the total time by the link utililization or whateva

    }

    if(ready.size() == 0){
        // clock ticks once more and all backoffs are decremented
    }

    return(transmit_no_collide);
}




int main(int argc, char** argv){
    if(argc != 2){
        printf("Wrong Input: ./csma input.txt\n");
        exit(0);
    }

    // Write the output of the simulator to a file called output.txt
}


