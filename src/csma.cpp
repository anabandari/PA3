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
    int nodeID, r_status, backoff;
} node_info; 

typedef struct{
    int N, L, M; 
    int total_time, current_time;
    vector<node_info> node;
    vector<int> R; // should we use a set or a vector...?
} values;

static values value;

// write a function to calculate a backoff?? 
    // its just the modulus but sureee
int backoff(int nodeID, int ticks, int R){
    int backoff; 
    backoff = (nodeID+ticks)% R;  //backoff = mod(nodeID + ticks, R);
    return(backoff);
}

// Implement file input 
void system_inputs(string filename){
    ifstream input;
	input.open(filename); 
	if (!input) {
        cerr << "Error: Unable to open file!\n";
		exit(0);
    }

    string token;
    while (input >> token) {
        // Number of Nodes
        if(token == "N"){
            int value_N;
            input >> value_N; // Read the value of N
            value.N = value_N; // Store it in the values struct
            printf("value of n %d\n", value.N);
        }
        else if(token == "L"){
            int value_L;
            input >> value_L; // Read the value of N
            value.L = value_L; // Store it in the values struct
            printf("value of l %d\n", value.L);
        }
        else if(token == "M"){
            int value_M;
            input >> value_M; // Read the value of N
            value.M = value_M; // Store it in the values struct
            printf("value of m %d\n", value.M);
        }
        else if(token == "T"){
            printf("hello world\n");
            int value_T;
            input >> value_T; // Read the value of N
            value.total_time = value_T; // Store it in the values struct
            printf("total time %d\n", value.total_time);
        }
        // Random Seed Value
        else if (token == "R") {
            int value_R;
            while (input >> value_R) {
                value.R.push_back(value_R);
            }
            printf("size of whatevaa %zu\n", value.R.size());
        }
    }

    input.close();
}

void instantialize_nodevec(){
    for(int i = 0; i < value.N; i++){
        node_info node;
        printf("hello ready! \n");
        node.nodeID = i;
        node.r_status = 0; 
        node.backoff = backoff(node.nodeID, 0, value.R[node.r_status]);
        value.node.push_back(node);
        printf("%d\n", value.node[i].backoff);
    }
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
    int transmit_no_collide = 0; 

    while(value.current_time != value.total_time){
        // Adding all nodes that have a backoff value of 0 to a vector
        for(int i = 0; i < value.N; i++){
            if(value.node[i].backoff == 0){
                ready.push_back(i); // would a set be better here????
            }
        }

        if(ready.size() > 1){
            // collison occurs increase the R value!!
            value.current_time++; 
            while(ready.size() != 0){
                value.node[ready[ready.size()-1]].r_status++;
                value.node[ready[ready.size()-1]].backoff = backoff(ready[ready.size()-1], value.current_time, value.R[value.node[ready[ready.size()-1]].r_status]);
                ready.pop_back();
            }
        }

        if(ready.size() == 1){
            // no collison change the backoff for this node onlyyyy
            // keep track of these number of slots!
            // increase the total time by the link utililization or whateva
            // small bug for when the time exceeds the total simulation time!!!!!!!!!!
            transmit_no_collide++;
            value.current_time = value.current_time + value.L;
            value.node[ready[0]].r_status = 0;
            value.node[ready[0]].backoff = backoff(ready[0], value.current_time, value.R[0]);
            ready.pop_back();
        }

        if(ready.size() == 0){
            // clock ticks once more and all backoffs are decremented
            for(int i = 0; i < value.N; i++){
                value.node[i].backoff--;
                value.current_time++; 
            }
        }

    }

    return(transmit_no_collide);
}


int main(int argc, char** argv){
    if(argc != 2){
        printf("Wrong Input: ./csma input.txt\n");
        exit(0);
    }

    int num_successes;
    double calculation;
    // Sets up all the variables we need to run our simullaaaatioooonnn!!!!
    system_inputs(argv[1]);
    instantialize_nodevec();
    num_successes = csma_calc();

    calculation = ((double)num_successes)/((double)value.total_time);

    // Write the output of the simulator to a file called output.txt
    ofstream outfile("output.txt");
    outfile << calculation;
    outfile.close(); 
 
}


