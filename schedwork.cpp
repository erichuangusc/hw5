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
bool scheduleHelper(const AvailabilityMatrix& avail, size_t dailyNeed, size_t maxShifts, 
                 DailySchedule& sched, vector<int>& shiftsWorking, int dayIndex, int shiftIndex);

bool isValid(const AvailabilityMatrix& avail, size_t maxShifts, const DailySchedule& sched, 
             const vector<int>& shiftsWorking, int dayIndex, int shiftIndex);

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0){
        return false;
    }
    sched.clear();
    // Add your code below
    for (size_t i = 0; i < avail.size(); ++i) {
        vector<Worker_T> day(dailyNeed);
        sched.push_back(day);
        for (size_t j = 0; j < dailyNeed; ++j) {
				sched[i][j] = INVALID_ID;
		}
	}
    vector<int> shifts((int)avail[0].size(), 0);
    return scheduleHelper(avail, dailyNeed, maxShifts, sched, shifts, 0, 0);
}

bool scheduleHelper(const AvailabilityMatrix& avail, size_t dailyNeed, size_t maxShifts, 
                 DailySchedule& sched, vector<int>& shiftsWorking, int dayIndex, int shiftIndex) {
    if (dayIndex == avail.size()) {
        return true;
    }

    if (shiftIndex == dailyNeed) {
        return scheduleHelper(avail, dailyNeed, maxShifts, sched, shiftsWorking, dayIndex + 1, 0);
    }

    for (Worker_T worker = 0; worker < avail[dayIndex].size(); ++worker) {
        sched[dayIndex][shiftIndex] = worker;
        shiftsWorking[worker]++;

        if (isValid(avail, maxShifts, sched, shiftsWorking, dayIndex, shiftIndex)) {
            if (scheduleHelper(avail, dailyNeed, maxShifts, sched, shiftsWorking, dayIndex, shiftIndex + 1)) {
                return true;
            }
        }

        sched[dayIndex][shiftIndex] = INVALID_ID;
        shiftsWorking[worker]--;
    }

    return false;
}

bool isValid(const AvailabilityMatrix& avail, size_t maxShifts, const DailySchedule& sched, 
             const vector<int>& shiftsWorking, int dayIndex, int shiftIndex) {
    Worker_T worker = sched[dayIndex][shiftIndex];

    for (int i = 0; i < shiftIndex; ++i) {
        if (sched[dayIndex][i] == worker) {
            return false;
        }
    }

    if (!avail[dayIndex][worker] || shiftsWorking[worker] > maxShifts) return false;
    return true;
}
