#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool recursiveSchedFind(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    int n,
    int d,
    std::vector<int>& workDays
);

bool validDay(
    std::vector<Worker_T> daySched, 
    const AvailabilityMatrix& avail,
    int n, int d,
    const size_t maxShifts,
    Worker_T worker,
    std::vector<int> workerDays);

bool availConstraint(
    std::vector<Worker_T> daySched, 
    const AvailabilityMatrix& avail,
    int n, int d,
    Worker_T worker);

bool maxConstraint(
    const size_t maxShifts,
    Worker_T worker,
    std::vector<int> workerDays);

// Add your implementation of schedule() and other helper functions here

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

    int N = avail.size();
    int K = avail[0].size();

    //initialize an array that stores the amount of days each worker is working 
    std::vector<int> workerDays(K);
    for (int i = 0; i < K; i++) {
        workerDays[i] = 0;
    }

    for (int n = 0; n < N; n++) {
        std::vector<Worker_T> daySched;
        sched.push_back(daySched);
    }

    return recursiveSchedFind(avail, dailyNeed, maxShifts, sched, 0, 0, workerDays);
}

bool recursiveSchedFind(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    int n,
    int d,
    std::vector<int>& workerDays
    )
{

    // base case: current day is past totalDays
    if (n >= avail.size()) {
        return true;
    }

    // recursive case
    std::vector<Worker_T> daySched(dailyNeed);

    // check if there exists a solution for current slot
    for (Worker_T i = 0; i < avail[0].size(); i++) {

        //generate a daySched
        sched[n].push_back(i);
        workerDays[i]++;

        //then check if that combo of workers satisfy the constraints by the day
        if (validDay(sched[n], avail, n, d, maxShifts, i, workerDays) == true) {
            // then iterate to the next position

            //either check for the next day (if day is complete)
            if (d + 1 >= dailyNeed) {
                if (recursiveSchedFind(avail, dailyNeed, maxShifts, sched, n + 1, 0, workerDays)) {
                    return true;
                } 
            }

            //else move on to the next slot d for the current day 
            else {
                if (recursiveSchedFind(avail, dailyNeed, maxShifts, sched, n, d + 1, workerDays)) {
                    return true;
                }
            }
            
        }
        sched[n].pop_back();
        workerDays[i]--;
    }

    // if it gets here that means no solution was found
    return false;

}



// validDay() takes a daySched and see if that daySched is valid
bool validDay(
    std::vector<Worker_T> daySched, 
    const AvailabilityMatrix& avail,
    int n, int d,
    const size_t maxShifts,
    Worker_T worker,
    std::vector<int> workerDays)
{
    // are all workers assigned that day viable?
    for (Worker_T w = 0; w < daySched.size(); w++) {
        if (availConstraint(daySched, avail, n, d, worker) == true &&
            maxConstraint(maxShifts, worker, workerDays) == true) 
        {
            return true;
        }
    }
    return false;
}


bool availConstraint(
    std::vector<Worker_T> daySched, 
    const AvailabilityMatrix& avail,
    int n, int d,
    Worker_T worker) 
{

    // if worker was already scheduled for the day
    int count = std::count(daySched.begin(), daySched.end(), worker);
    if (count > 1) {
        return false;
    }

    // if the worker is in avail[day]
    if (avail[n][worker] == 0) {
        return false;
    }

    return true;
}

bool maxConstraint(
    const size_t maxShifts,
    Worker_T worker,
    std::vector<int> workerDays)
{

    if (workerDays[worker] <= maxShifts) {
        return true;
    }
    return false;
}