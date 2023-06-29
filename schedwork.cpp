#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please
#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
//updates the number of shifts a worker has been scheduled for
void updateShifts (vector<Worker_T>& shiftsTaken, Worker_T worker, int change);
//checks if scheduling an employee is valid
bool is_valid (const AvailabilityMatrix& avail, const size_t maxShifts, std::vector<Worker_T> shiftsTaken, int day, Worker_T employee);
//performs recursive function
bool scheduleHelper (DailySchedule& sched, const AvailabilityMatrix& avail, const size_t dailyNeed, const size_t maxShifts, std::vector<Worker_T>& shiftsTaken, int days);

// Add your implementation of schedule() and other helper functions here
//updates the number of shifts a worker has been scheduled for
void updateShifts (vector<Worker_T>& shiftsTaken, Worker_T worker, int change){ //check if i need to specify for negatives and positives or 0s
    shiftsTaken[worker] += change;
}

//checks if scheduling an employee is valid
bool is_valid (const AvailabilityMatrix& avail, const size_t maxShifts, std::vector<Worker_T> shiftsTaken, int day, Worker_T employee){
    //check if employee is available
    if (avail[day][employee]  == 0){
        return false;
    }

    //check if employee has reached maxShifts
    if(shiftsTaken[employee] >= maxShifts){
        return false;
    }

    return true;
}

bool scheduleHelper (DailySchedule& sched, const AvailabilityMatrix& avail, const size_t dailyNeed, const size_t maxShifts, vector<Worker_T>& shiftsTaken, int days) {
    if (days > avail.size()){
        return true;
    }

    int count = 0; //keep track of how many workers still needed
    int temp; //to hold j at end of loop

    for (size_t j=0; j< (avail[days].size()); j++){ //looping over available workers on this day
      if (is_valid(avail, maxShifts, shiftsTaken, days, j)){ //check if they're available and their shifts
        updateShifts(shiftsTaken, j, 1); //place first employee
        sched[days][count] = j; //add first employee to the schedule
        count++; //increase count and move on to next needed worker
      }

      if (count == dailyNeed){ //if already placed all needed workers for this day
        if(scheduleHelper(sched, avail, dailyNeed, maxShifts, shiftsTaken, days+1)){ //move onto next day
          return true;
        } else {
          scheduleHelper(sched, avail, dailyNeed, maxShifts, shiftsTaken, days);
        }
      }
      temp = j;
    }
    
      updateShifts(shiftsTaken, temp, -1); //backtrack
      count--;
      sched[days][count] = INVALID_ID;
      return false;

}

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }

    sched.clear();

    // Add your code below

    size_t n = avail.size(); //to hold number of days
    size_t k = (avail[0]).size(); //to hold number of workers

    //CONSTRAINTS to automatically disqualify
    //if available workers are less than needed
    if (k<dailyNeed){
        cout << "available workers are less than needed" << endl;
        cout << "available: " << k << endl;
        cout << "needed: " << dailyNeed <<endl;
        return false;
    }

    //if workers needed and their max shifts dont cover all the days
    if ((dailyNeed*maxShifts) < n){
        cout << "workers needed and their max shifts don't cover all the days" <<endl;
        cout << "workers needed: " << dailyNeed << endl;
        cout << "max shift: " << maxShifts << endl;
        cout << "number of days: " << n << endl;
        return false;
    }

    //if there's a day where nobody works
    for (int i=0; i<n; i++){
        bool availability = false;
        for (int j=0; j<k; j++){
            if (avail[i][j] == 1){
                availability = true;
            }
        }

        if (!availability){
            cout << "day " << i << " has no available employees" <<endl;
            return false;
        }
    }


    //CODE START
    //to hold number of shifts each worker took on
    vector<Worker_T> shiftsTaken_; //used to be Worker_T but didnt work for accessing 0

    //populating shifts taken vector
    for (int i=0; i<k; i++){ //check that this doesn't just keep adding to the same one if run multiple times
        shiftsTaken_.push_back(0);
    }

    sched.resize(n, vector<Worker_T>(dailyNeed));

    //populate sched with -1s
    for (int i=0; i<n; i++){
        for (int j=0; j< dailyNeed; j++){
            sched[i][j] = INVALID_ID;
        }
    }

    bool status = scheduleHelper (sched, avail, dailyNeed, maxShifts, shiftsTaken_, 0);

    if (status){
        return true;
    } else {
        return false;
    }

}

void printShifts(const vector<Worker_T> shiftsTaken)
{
  for(auto s : shiftsTaken)
    {
      cout << s << ", ";
    }
    cout << endl;
}

