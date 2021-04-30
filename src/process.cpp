#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid) {}

int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    vector<string> processUtilization = LinuxParser::CpuUtilizationPid(pid_);
    float processTime = stof(processUtilization[0]) + stof(processUtilization[1]) + stof(processUtilization[2]) + stof(processUtilization[3]);
    float seconds = (float)LinuxParser::UpTime(pid_);
    previousUtilization_ = currentUtilization_;
    currentUtilization_ = ( processTime / (float)sysconf(_SC_CLK_TCK) ) / seconds;
    return currentUtilization_;
 }

 float Process::getCpuUtilization() const{
     return currentUtilization_;
 }

string Process::Command() { return LinuxParser::Command(pid_); }

string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const{ 
    return ( this->Process::getCpuUtilization() < a.getCpuUtilization() );
}