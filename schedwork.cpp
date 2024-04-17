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
                 DailySchedule& sched, vector<int>& shifts, int dayIndex, int shiftIndex);

bool isValid(const AvailabilityMatrix& avail, size_t maxShifts, const DailySchedule& sched, 
             const vector<int>& shifts, int row, int col);

void printSched(const DailySchedule& sched);

// Add your implementation of schedule() and other helper functions here

// bool schedule(
//     const AvailabilityMatrix& avail,
//     const size_t dailyNeed,
//     const size_t maxShifts,
//     DailySchedule& sched
// )
// {
//     if(avail.size() == 0U){
//         return false;
//     }
//     sched.clear();
//     // Add your code below
//     for (size_t i = 0; i < avail.size(); ++i) {
//         vector<Worker_T> day(dailyNeed);
//         sched.push_back(day);
//         for (size_t j = 0; j < dailyNeed; ++j) {
// 				sched[i][j] = INVALID_ID;
// 		}
// 	}
//     vector<int> shifts((int)avail[0].size(), 0);
//     return scheduleHelper(avail, dailyNeed, maxShifts, sched, shifts, 0, 0);
// }

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
    sched.resize(avail.size(), vector<Worker_T>(dailyNeed, INVALID_ID));

    vector<int> shifts((int)avail[0].size(), 0);
    return scheduleHelper(avail, dailyNeed, maxShifts, sched, shifts, 0, 0);
}


bool scheduleHelper(const AvailabilityMatrix& avail, size_t dailyNeed, size_t maxShifts, 
                DailySchedule& sched, vector<int>& shifts, int dayIndex, int shiftIndex) {
    if (dayIndex == avail.size()) {
        return true;
    }

    if (shiftIndex == dailyNeed) {
        return scheduleHelper(avail, dailyNeed, maxShifts, sched, shifts, dayIndex + 1, 0);
    }

    for (Worker_T worker = 0; worker < avail[dayIndex].size(); ++worker) {
        sched[dayIndex][shiftIndex] = worker;
        shifts[worker]++;

        if (isValid(avail, maxShifts, sched, shifts, dayIndex, shiftIndex)) {
            if (scheduleHelper(avail, dailyNeed, maxShifts, sched, shifts, dayIndex, shiftIndex + 1)) {
                return true;
            }
        }

        sched[dayIndex][shiftIndex] = INVALID_ID;
        shifts[worker]--;
    }

    return false;
}

// bool scheduleHelper(AvailabilityMatrix avail, size_t dailyNeed, size_t maxShifts, 
//                 DailySchedule& sched, vector<int>& shifts, int row, int col) {
// 		if(row == (int)avail.size()) return true;

// 		int next_row = row, next_col = col+1;
// 		if (next_col == (int)dailyNeed){
// 				next_row = row + 1;
// 				next_col = 0;
// 		}

// 		for (Worker_T worker = 0; worker < avail[0].size(); worker++) {
// 				sched[row][col] = worker;
// 				shifts[worker] += 1;

// 				if (isValid(avail, maxShifts, sched, shifts, row, col)) {
// 						if (scheduleHelper(avail, dailyNeed, maxShifts, sched, shifts, next_row, next_col)) return true;
// 						else {
// 								sched[row][col] = INVALID_ID;
// 								shifts[worker]--;
// 						}
// 				}
// 				else {
// 						sched[row][col] = INVALID_ID;
// 						shifts[worker]--;
// 				}
// 		}

// 		return false;
// }


// bool isValid(AvailabilityMatrix avail, size_t maxShifts, DailySchedule& sched, vector<int>& shifts, int row, int col) {
// 		Worker_T worker = sched[row][col];
// 		vector<Worker_T> day = sched[row];

// 		day.erase(day.begin() + col);
// 		vector<Worker_T>::iterator it = find(day.begin(), day.end(), worker);

// 		if (it != day.end()|| !avail[row][worker] || shifts[worker] > (int)maxShifts) return false;

// 		return true;
// }

bool isValid(const AvailabilityMatrix& avail, size_t maxShifts, const DailySchedule& sched, 
             const vector<int>& shiftsWorking, int row, int col) {
    Worker_T worker = sched[row][col];

    // Check for the same worker assigned twice on the same day
    for (int i = 0; i < col; ++i) {
        if (sched[row][i] == worker) {
            return false;
        }
    }

    if (!avail[row][worker] || shiftsWorking[worker] > maxShifts) return false;
    
    return true;
}
