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
#include <string>    
#include <vector>    
#include <fstream>   
#include <iostream> 
#include <cstdlib>   
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <algorithm>
#include <map>
#include <vector>
#include <iomanip>

using namespace std;

/// Create struct for node information
typedef struct{
    int nodeID, r_status, backoff;
} node_info; 

/// Create struct for nodes and parameters
typedef struct{
    int N, L, M; 
    unsigned int total_time, current_time;
    vector<node_info> node;
    vector<long> R; 
} values;

static values value;

/// Function to calculate backoff
int backoff(int nodeID, int ticks, int R){
    double backoff; 
    /// Backoff is equal to modulus of nodeID + ticks and backoff window
    backoff = (nodeID+ticks)%R; 
    /// Return calculated backoff value
    return backoff;
}

// Implement file input 
void system_inputs(string filename){
    ifstream input;
	input.open(filename); 
	if (!input) {
        /// Check that file is opened correctly
        cerr << "Error: Unable to open file!\n";
		exit(0);
    }

    /// Read input file and save values to parameters in struct "value"
    string token;
    while (getline(input, token)) {
        char* str = const_cast<char *>(token.c_str());
        long val;
        /// Get value of number of nodes
        if (*str == 'N'){
            sscanf(str + 1, "%d", &value.N);
        }
        /// Get value of packet size
        else if (*str == 'L'){
            sscanf(str + 1, "%d", &value.L);
        } 
        /// Get value of maximum retransmission attempt
        else if (*str == 'M'){
            sscanf(str + 1, "%d", &value.M);
        }
        /// Get value of total time of simulation (clock ticks)
        else if (*str == 'T'){
            sscanf(str + 1, "%u", &value.total_time);
        }
        /// Get initial random number range
        else if (*str == 'R') {
            val = strtol(str + 1, &str, 10);
            while (val){
                value.R.push_back(val); 
                val = strtol(str, &str, 10);
            }
        }
    }

    /// Close file after reading and obtaining all required parameters
    input.close();
}

/// Instantiate a vector of node_info structs
void instantialize_nodevec(){
    /// increment up to N number of nodes
    for(int i = 0; i < value.N; i++){
        node_info node;
        /// set value of nodeID to increment index
        node.nodeID = i;
        node.r_status = 0; 
        /// Get value of backoff from backoff function
        node.backoff = backoff(node.nodeID, 0, value.R[node.r_status]);
        value.node.push_back(node);
    }
}


/// Function to implement Carrier Sense Multiple Access algorithm
int csma_calc(){
    vector<int> ready;
    /// Initialize number of successful transmissions without collisions to zero
    int transmit_no_collide = 0; 
    value.current_time = 0;

    /// Repeat until we reach total time T
    while(value.current_time < value.total_time){

        /// Adding all nodes that have a backoff value of 0 to a vector
        for(int i = 0; i < value.N; i++){
            if(value.node[i].backoff == 0){
                ready.push_back(i); 
            }
        }

        /// If there is more than one node ready to transmit
        if(ready.size() > 1){ 
            /// Increment current time
            value.current_time++;          
            /// Collison will happen. Update r_status and backoffs
            while(ready.size() != 0){
                value.node[ready[ready.size()-1]].r_status++;
                value.node[ready[ready.size()-1]].backoff = backoff(ready[ready.size()-1], value.current_time, value.R[value.node[ready[ready.size()-1]].r_status]);
                printf("node: %d  ", ready.size()-1);
                printf("backoff: %d\n", value.node[ready[ready.size()-1]].backoff);
                ready.pop_back();
            }

            
            printf("collision\n");
        }

        /// If there is only one node ready to transmit
        else if(ready.size() == 1){

            /// Check if length of packet is able to be transmitted within available total time
            if(value.current_time + value.L < value.total_time) {
                /// Add amount of time to transmit packet
                value.current_time += value.L;
                /// Increment number of successful transmissions without collisions
                transmit_no_collide += value.L;
            }
            /// If there is not enough time left, increment time to only to total time
            else {       
                /// Increment number of successful transmissions without collisions
                transmit_no_collide += value.L - ((value.current_time + value.L) - value.total_time);
                /// Increment time to total time
                value.current_time = value.total_time;
            }
            
            value.node[ready[0]].r_status = 0;
            value.node[ready[0]].backoff = backoff(ready[0], value.current_time, value.R[0]);
            printf("node: %d  ", ready[0]);
            printf("backoff: %d\n", value.node[ready[0]].backoff);  
            ready.pop_back();
            printf("transmission\n");
        }
        
        /// If there are no transmissions
        else if (ready.size() == 0){
            /// Increment current time
            value.current_time++; 
            // clock ticks once more and all backoffs are decremented
            for(int i = 0; i < value.N; i++){
                value.node[i].backoff--;   
                printf("node: %d  ", i);
                printf("backoff: %d\n", value.node[i].backoff);            
            }

            printf("normal\n");
        }

        printf("time: %d\n", value.current_time);
    }
    
    /// Return number of successful transmissions without collisions
    printf("transmit_no_collide, %d", transmit_no_collide);
    return(transmit_no_collide);
}


int main(int argc, char** argv){
    if(argc != 2){
        /// Check for correct input file called "input"
        printf("Wrong Input. Try: ./csma input\n");
        exit(0);
    }

    int num_successes;
    double calculation;
    /// Sets up all the variables we need to run simulation from input file
    system_inputs(argv[1]);
    /// Function to instantiate a vector of node_info structs
    instantialize_nodevec();
    /// Get number of successes
    num_successes = csma_calc();

    calculation = ((double)num_successes)/((double)value.total_time);

    /// Write the output of the simulator to a file called output.txt
    ofstream outfile("output.txt");
    /// Round output to 2 decimal points
    outfile << std::fixed << std::setprecision(2) << calculation;
    outfile.close(); 
 
}


