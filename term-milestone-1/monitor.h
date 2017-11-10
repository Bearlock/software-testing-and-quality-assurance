#ifndef MONITOR_H
#define MONITOR_H

#include <utility>
#include <string>
#include <array>
#include <deque>

class Monitor {
  public:
    Monitor();

    static const int PULSE_MIN;
    static const int PULSE_MAX;
    static const std::array<std::pair<int, std::string>, 5> PULSE_ALARM_VALS;

    static const double OXYGEN_MIN;
    static const double OXYGEN_MAX;
    static const std::array<std::pair<double, std::string>, 3> OXYGEN_ALARM_VALS;

    static const std::pair<int, int> BP_MAX;
    static const std::array<std::pair<std::pair<int, int>, std::string>, 4> BP_ALARM_VALS;

    static const int MAX_TIME;
    static const int TIME_INCREMENT;

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
