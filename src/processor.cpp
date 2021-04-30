#include "processor.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    vector<string> utilizationValues = LinuxParser::CpuUtilization();


    
    float user = std::stof(utilizationValues[0]);
    float kernel = std::stof(utilizationValues[2]);
    float idle = std::stof(utilizationValues[3]);
    
    float currentTime = (user + kernel);
    float prevTime = (prevUser_ + prevKernel_);
    float currentTotal = (user + kernel + idle);
    float prevTotal = (prevUser_ + prevKernel_ + prevIdle_);

    float utilization = (currentTime - prevTime) / (currentTotal - prevTotal);
    
    //assign new values to previous values for next calc.
    prevUtilization_ = utilization;
    prevUser_ = user;
    prevKernel_ = kernel;
    prevIdle_ = idle;
    return utilization;
 }