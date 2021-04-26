#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    // calculate hrs, mins, secs
    long hours = seconds / 60 / 60;
    int minutes = ( seconds - ( hours * 60 * 60 ) ) / 60;
    int secs = ( seconds - ( hours * 60 * 60) - ( minutes * 60 ) );

    //handle leading 0 if required
    string hoursS = hours < 10 ? "0" + std::to_string(hours) : std::to_string(hours);
    string minutesS = minutes < 10 ? "0" + std::to_string(minutes) : std::to_string(minutes);
    string secsS = secs < 10 ? "0" + std::to_string(secs) : std::to_string(secs);

    return hoursS + ":" + minutesS + ":" + secsS;
    
}