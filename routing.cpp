/* Here is a skeleton of a possible solution strategy. */
/* You are NOT required to follow it, but it might help you get started. */
#include <stdio.h>
#include <stdlib.h>

#include <fstream>
#include <iostream>

//ROWS AND COLUMS
#define MAX_ROW 1000
#define MAX_COL 1000
#define MAX_EVENTS 1000

//Event types
#define NULL_EVENT 0
#define CALL_ARRIVAL 1
#define DEPARTURE 2
#define STOP 3

using namespace std;

//Function headers
bool RouteCall(char src, char dst, int callid);
void ReleaseCall(char src,char dst,int callid);

/* Global matrix data structure for network topology/state information */
int propdelay[MAX_ROW][MAX_COL];
int capacity[MAX_ROW][MAX_COL];
int available[MAX_ROW][MAX_COL];
int cost[MAX_ROW][MAX_COL];

/* Event record */
struct Event
{
    float event_time;
    int event_type;
    int callid;
    char source;
    char destination;
    float duration;
  } EventList[MAX_EVENTS];

int main()
  {
   FILE* fp1 = fopen("topology.dat", "rb");
	char src;
	char dst;
	int delay;
	int cap;
	int i;
     while((fscanf(fp1, "%c %c %d %d\n", &src, &dst, &delay, &cap)) == 4 )
       {
	// char row = src - 'A'; 
	// char col = dst - 'A';
	// propdelay[row][col] = delay; 
	// propdelay[col][row] = delay;
	
	// capacity[row][col] = cap; 
	// capacity[col][row] = cap;
	
	// available[row][col] = cap; 
	// available[col][row] = cap;
      }
    // fclose(fp1);

    /* Next read in the calls from "callworkload.dat" and set up events */

    /* Now simulate the call arrivals and departures */
    // while( numevents > 0 )
      // {
	// /* Get information about the current event */
	// printf("Event of type %d at time %8.6f (call %d from %c to %c)\n", type, start, callid, src, dst);
	
	// if( type == CALL_ARRIVAL )
	    // if( RouteCall(src, dst, callid) > 0 )
		// success++;
	    // else blocked++;
	// else ReleaseCall(src, dst, callid);
      // }
    /* Print final report here */
  }
