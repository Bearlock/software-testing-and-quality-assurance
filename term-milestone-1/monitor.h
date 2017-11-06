#ifndef MONITOR_H
#define MONITOR_H

#include <map>
#include <string>

class Monitor {
  const int PULSE_MIN = 0;
  const int PULSE_MAX = 210;
  const std::map<int, std::string> PULSE_ALARM_VALS = {{20, "highest"}, {170, "highest"}, {40, "medium"}, {130, "medium"}, {110, "low"}};

  const double OXYGEN_MIN = 0;
  const double OXYGEN_MAX = 99.9;
  const std::map<double, std::string> OXYGEN_ALARM_VALS = {{50.0, "highest"}, {80.0, "medium"}, {85.0, "low"}};

  const double BP_MAX = 230/150;
  const std::map<double, std::string> BP_ALARM_VALS = {{(50/33), "highest"}, {(200/120), "medium"}, {(70/40), "medium"}, {(150/90), "low"}};
private:
  int pulse;
  int systolic;
  int diastolic;
  double oxygen;
};

#endif
