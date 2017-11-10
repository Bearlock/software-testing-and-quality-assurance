#include "monitor.h"
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

Monitor::Monitor() {
  pulse = 0;
  bp = std::make_pair(0,0);
  oxygen = 0;
  minutes = 0;
  seconds = 0;
  totalTime = 0;
  pulseAlarm = false;
  oxygenAlarm = false;
  bpAlarm = false;
}

void Monitor::incrementTime(int incrementBy) {
  totalTime += incrementBy;
  seconds = totalTime % MAX_TIME;
  minutes = (totalTime / MAX_TIME) % MAX_TIME;
}

void Monitor::printStatus() {
  std::cout << std::setw(2) << std::setfill('0') << minutes << ':' << std::setw(2) << std::setfill('0') << seconds <<  '\t' << alertStatus << '\t' << description << std::endl;
}

void Monitor::invalidateOxygen() {
  missedOxygen = 0;
  oxygenReads.clear();
  setStatus("low", "Equipment malfunction", "oxygen");
}

double Monitor::calculateAverage(std::deque<double> someQueue) {
  double tempVal = 0;
  for(int i = 0; i < someQueue.size(); i++) {
    tempVal += someQueue[i];
  }
  return (tempVal / someQueue.size());
}

std::pair<int, int> Monitor::parseBp(std::string parsedString, char delim) {
  std::istringstream potentialBp(parsedString);
  std::string sys;
  std::string dia;

  std::getline(potentialBp, sys, delim);
  std::getline(potentialBp, dia, delim);

  return std::make_pair(stoi(sys), stoi(dia));
}

void Monitor::setStatus(std::string alertLevel, std::string desc, std::string source) {
  if(source == "pulse") {
    if(alertLevel != "none") {
      pulseAlarm = true;
      warnings[0] = std::make_pair(alertLevel, desc);
    }
    else {
      pulseAlarm = false;
      warnings[0] = std::make_pair("", "");
    }
  }
  else if(source == "oxygen") {
    if(alertLevel != "none") {
      oxygenAlarm = true;
      warnings[1] = std::make_pair(alertLevel, desc);
    }
    else {
      oxygenAlarm = false;
      warnings[1] = std::make_pair("", "");
    }
  }
  else if (source == "bp") {
    if(alertLevel != "none") {
      bpAlarm = true;
      warnings[2] = std::make_pair(alertLevel, desc);
    }
    else {
      bpAlarm = false;
      warnings[2] = std::make_pair("", "");
    }
  }

  if(pulseAlarm || oxygenAlarm || bpAlarm) {
    for(int i = 0; i < warnings.size(); i++) {
      if(warnings[i].first == "high") {
        alertStatus = warnings[i].first;
        description = warnings[i].second;
      }
      else if(warnings[i].first == "medium" && alertStatus != "high") {
        alertStatus = warnings[i].first;
        description = warnings[i].second;
      }
      else if (warnings[i].first == "low" && (alertStatus != "high" || alertStatus != "medium")) {
        alertStatus = warnings[i].first;
        description = warnings[i].second;
      }
    }
  }
  else {
    alertStatus = "none";
    description = "Everything is normal";
  }
  // //make this a pair queue
  // if(alertLevel == "high") {
  //   alertStatus = alertLevel;
  //   description = desc;
  // }
  // else if (alertLevel == "medium" && (alertStatus == "medium" || alertStatus == "low" || alertStatus == "none")) {
  //   alertStatus = alertLevel;
  //   description = desc;
  // }
  // else if (alertLevel == "low" && (alertStatus == "low" || alertStatus == "none")) {
  //   alertStatus = alertLevel;
  //   description = desc;
  // }
  // else if (alertLevel == "none" && alertStatus != "low" && alertStatus != "medium" && alertStatus != "high") {
  //   alertStatus = alertLevel;
  //   description = desc;
  // }
}

void Monitor::checkPulse(int readPulse) {
  if(readPulse < PULSE_ALARM_VALS[0].first && readPulse > PULSE_MIN) {
    setStatus(PULSE_ALARM_VALS[0].second, "Pulse is life threatingly low!", "pulse");
  }
  else if(readPulse > PULSE_ALARM_VALS[1].first && readPulse < PULSE_MIN) {
    setStatus(PULSE_ALARM_VALS[1].second, "Pulse is life threatingly high!", "pulse");
  }
  else if (readPulse < PULSE_ALARM_VALS[2].first && readPulse > PULSE_MIN) {
    setStatus(PULSE_ALARM_VALS[2].second, "Pulse is dangerously low!", "pulse");
  }
  else if(readPulse > PULSE_ALARM_VALS[3].first && readPulse < PULSE_MIN) {
    setStatus(PULSE_ALARM_VALS[3].second, "Pulse is dangerously high!", "pulse");
  }
  else if (readPulse > PULSE_ALARM_VALS[4].first && readPulse > PULSE_MIN) {
    setStatus(PULSE_ALARM_VALS[4].second, "Pulse is potentially too high", "pulse");
  }
  else {
    // pulse is either fine or there is an equipment malfunction
    if(readPulse < PULSE_MIN || readPulse > PULSE_MAX) {
      setStatus("low", "Equipment malfunction", "pulse");
    }
    else {
      setStatus("none", "Everything is normal", "pulse");
    }
  }
}

void Monitor::missedOxygenRead() {
  missedOxygen++;
  if (missedOxygen >= 3) {
    invalidateOxygen();
  }
  else {
    checkOxygen(oxygenReads.back());
  }
}

void Monitor::checkOxygen(double readOxygen) {
  if(oxygenReads.size() < 6) {
    if(readOxygen > OXYGEN_MIN && readOxygen < OXYGEN_MAX) {
      oxygenReads.push_back(readOxygen);
    }
  }
  else {
    double oxygenAverage = calculateAverage(oxygenReads);
    oxygenReads.pop_front();

    if(oxygenAverage < OXYGEN_ALARM_VALS[0].first && oxygenAverage > OXYGEN_MIN) {
      setStatus(OXYGEN_ALARM_VALS[0].second, "Oxygen is life threatingly low!", "oxygen");
    }
    else if(oxygenAverage < OXYGEN_ALARM_VALS[1].first) {
      setStatus(OXYGEN_ALARM_VALS[1].second, "Oxygen is dangerously low!", "oxygen");
    }
    else if (oxygenAverage < PULSE_ALARM_VALS[2].first) {
      setStatus(OXYGEN_ALARM_VALS[2].second, "Oxygen is potentially too low", "oxygen");
    }
    else {
      // oxygen is either fine or there is an equipment malfunction
      if(oxygenAverage < OXYGEN_MIN || oxygenAverage > OXYGEN_MAX) {
        setStatus("low", "Equipment malfunction", "oxygen");
      }
      else {
        setStatus("none", "Everything is normal", "oxygen");
      }
    }
  }
}

void Monitor::checkBp(std::pair<int, int> readBp) {
  if (readBp.first < BP_ALARM_VALS[0].first.first || readBp.second < BP_ALARM_VALS[0].first.second) {
    setStatus(BP_ALARM_VALS[0].second, "BP is life threatingly low!", "bp");
    bpAlarm = true;
  }
  else if ((readBp.first > BP_ALARM_VALS[1].first.first || readBp.second > BP_ALARM_VALS[1].first.second) && (readBp.first < BP_MAX.first || readBp.second < BP_MAX.second)) {
    setStatus(BP_ALARM_VALS[1].second, "BP is dangerously high!", "bp");
    bpAlarm = true;
  }
  else if (readBp.first < BP_ALARM_VALS[2].first.first || readBp.second < BP_ALARM_VALS[2].first.second) {
    setStatus(BP_ALARM_VALS[2].second, "BP is dangerously low!", "bp");
    bpAlarm = true;
  }
  else if (readBp.first > BP_ALARM_VALS[3].first.first || readBp.second > BP_ALARM_VALS[3].first.second) {
    setStatus(BP_ALARM_VALS[3].second, "BP is potentially too high", "bp");
    bpAlarm = true;
  }
  else {
    if(readBp.first > BP_MAX.first || readBp.second > BP_MAX.second) {
      setStatus("low", "Equipment malfunction", "bp");
      bpAlarm = true;
    }
    else {
      setStatus("none", "Everything is normal", "bp");
    }
  }
}

void Monitor::processLine(std::string input) {
  incrementTime(TIME_INCREMENT);

  std::istringstream inputStream(input);
  std::vector<std::string> parsedInput;
  std::string tempString;

  while(inputStream >> tempString) {
    parsedInput.push_back(tempString);
  }

  pulse = stoi(parsedInput[0]);
  checkPulse(pulse);

  if(parsedInput.size() == 3) {
    oxygen = stod(parsedInput[1]);
    bp = parseBp(parsedInput[2], '/');
    missedOxygen = 0;

    checkOxygen(oxygen);
    checkBp(bp);
  }
  else if (parsedInput.size() == 2){
    // we're either getting oxygen or bp
    char delimiter = '/';
    std::size_t found = parsedInput[1].find(delimiter);

    if(found != std::string::npos) {
      // we got bp
      bp = parseBp(parsedInput[1], delimiter);
      checkBp(bp);
      missedOxygen++;
    }
    else {
      // we got oxygen
      oxygen = stod(parsedInput[1]);
      missedOxygen = 0;
      checkOxygen(oxygen);
    }
  }
  else if (parsedInput.size() == 1){
    // we're only getting a pulse
    missedOxygen++;
  }
  else {
    // If parsedInput.size() isn't between 1 and 3, there is an error
    setStatus("low", "Equipment malfunction", "pulse");
  }

  if(missedOxygen == 3) {
    invalidateOxygen();
  }

  printStatus();
}
