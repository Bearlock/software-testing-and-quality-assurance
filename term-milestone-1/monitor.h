#ifndef MONITOR_H
#define MONITOR_H

#include <utility>
#include <string>
#include <array>
#include <deque>

class Monitor {
  public:
    Monitor();

    const int PULSE_MIN = 0;
    const int PULSE_MAX = 210;
    const std::array<std::pair<int, std::string>, 5> PULSE_ALARM_VALS = {{std::make_pair(20, "high"), std::make_pair(170, "high"), std::make_pair(40, "medium"), std::make_pair(130, "medium"), std::make_pair(110, "low")}};

    const double OXYGEN_MIN = 0;
    const double OXYGEN_MAX = 99.9;
    const std::array<std::pair<double, std::string>, 3>OXYGEN_ALARM_VALS = {{std::make_pair(50.0, "high"), std::make_pair(80.0, "medium"), std::make_pair(85.0, "low")}};

    const std::pair<int, int> BP_MAX = std::make_pair(230,150);
    const std::array<std::pair<std::pair<int, int>, std::string>, 4> BP_ALARM_VALS = {std::make_pair(std::make_pair(50, 33), "high"), std::make_pair(std::make_pair(200, 120), "medium"), std::make_pair(std::make_pair(70, 40), "medium"), std::make_pair(std::make_pair(150, 90), "low")};

    const int MAX_TIME = 60;
    const int TIME_INCREMENT = 10;

    int minutes;
    int seconds;
    int totalTime;
    int pulse;
    int missedOxygen;

    bool pulseAlarm;
    bool oxygenAlarm;
    bool bpAlarm;

    double oxygen;
    std::pair<int, int> bp;

    std::string description;
    std::string alertStatus;
    std::deque<double> oxygenReads;
    std::array<std::pair<std::string, std::string>, 3> warnings;

    void incrementTime(int);
    void printStatus();
    void setStatus(std::string, std::string, std::string);
    void processLine(std::string);
    void checkPulse(int);
    void checkOxygen(double);
    void checkBp(std::pair<int, int>);
    void invalidateOxygen();
    void missedOxygenRead();

    double calculateAverage(std::deque<double>);

    std::pair<int, int> parseBp(std::string, char);
};

#endif
