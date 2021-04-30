#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string key, unit, line;
  float totalMemory, freeMemory, value;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value >> unit;
      if(key == "MemTotal:"){
        totalMemory = value;
      }
      if(key == "MemFree:"){
        freeMemory = value;
      }
    }
  }
  return ( ( totalMemory - freeMemory ) / totalMemory); //utilised memory
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  long uptime, idle;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idle;
  }
  return uptime;
 }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

vector<string> LinuxParser::CpuUtilizationPid(int pid){
  string line, value;
  string utime, stime, cutime, cstime, startTime;
  vector<string> utilizationValues;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::stringstream linestream(line);
    for(int i{1}; i<=22; i++){
      linestream >> value;
      if(i == 15 || i == 16 || i == 17 || i == 18 || i == 22){
        utilizationValues.push_back(value);
      }
    }
    return utilizationValues;
  }
  return {"0","0","0","0","0"}  ;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line, value;
  vector<string> utilizationValues;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    if(getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> value; //"cpu"
      for(int i{0}; i<10; i++){
        linestream >> value;
        utilizationValues.push_back(value);
      }
    }
  }
  return utilizationValues;
 }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  const string identifier{"processes"};
  return LinuxParser::ParseStatForProcess(identifier);
} 

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  const string identifier{"procs_running"};
  return LinuxParser::ParseStatForProcess(identifier);
}

int LinuxParser::ParseStatForProcess(const string &identifier){
    string line;
  string key; 
  int value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == identifier){
        return value;
      }
    }     
  }
  return 0;
}

string LinuxParser::Command(int pid) { 
  string command;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if(stream.is_open()){
    std::getline(stream, command);
    //observing null terminator issue that cuts off the string
    //but mentor help suggests to leave this as-is
  }
  return command;
 }

string LinuxParser::Ram(int pid) { 
  string line, key, size;
  long mb;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key >> size;
      if(key == "VmSize:"){
        mb = std::stol(size) / 1024;
        return std::to_string(mb) ;
      }
    }
  }
  return "";
 }

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string line, key, uid;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key >> uid;
      if(key == "Uid:"){
        return uid;
      }
    }
  }
  return "";
 }

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string line, uname, parsedUid, x;
  string uid = LinuxParser::Uid(pid); 
  std::ifstream stream(kPasswordPath);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> uname >> x >> parsedUid;
      if(parsedUid == uid){
        return uname;
      }
    }
  }
  return "";
 }

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string line, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::stringstream linestream(line);
    for(int i{1}; i<=15; i++){
      linestream >> value;
      if(i == 15){
        return  LinuxParser::UpTime() - (std::stol(value) / sysconf(_SC_CLK_TCK));
      }
    }
  }
  return 0;
 }
