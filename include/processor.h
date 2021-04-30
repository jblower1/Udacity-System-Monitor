#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "linux_parser.h"
#include <string>

using std::string;
using std::vector;

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
 float prevUtilization_{0}, prevUser_{0}, prevKernel_{0},prevIdle_{0};
};

#endif