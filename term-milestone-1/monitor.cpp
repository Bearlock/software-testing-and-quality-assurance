#include "monitor.h"
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

const int Monitor::PULSE_MIN = 0;
const int Monitor::PULSE_MAX = 210;
const std::array<std::pair<int, std::string>, 5> Monitor::PULSE_ALARM_VALS = {{std::make_pair(20, "high"), std::make_pair(170, "high"), std::make_pair(40, "medium"), std::make_pair(130, "medium"), std::make_pair(110, "low")}};

const double Monitor::OXYGEN_MIN = 0;
const double Monitor::OXYGEN_MAX = 99.9;
const std::array<std::pair<double, std::string>, 3>Monitor::OXYGEN_ALARM_VALS = {{std::make_pair(50.0, "high"), std::make_pair(80.0, "medium"), std::make_pair(85.0, "low")}};

const std::pair<int, int> Monitor::BP_MAX = std::make_pair(230,150);
const std::array<std::pair<std::pair<int, int>, std::string>, 4> Monitor::BP_ALARM_VALS = {std::make_pair(std::make_pair(50, 33), "high"), std::make_pair(std::make_pair(200, 120), "medium"), std::make_pair(std::make_pair(70, 40), "medium"), std::make_pair(std::make_pair(150, 90), "low")};

const int Monitor::MAX_TIME = 60;
const int Monitor::TIME_INCREMENT = 10;

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
  std::cout << std::setw(2) << std::setfill('0') << minutes << ':' << std::setw(2) << std::setfill('0') << seconds <<  '\t' << currentStatus << '\t' << description << '\n' << std::endl;
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
  try {
    return std::make_pair(stoi(sys), stoi(dia));
  }
  catch(std::invalid_argument&) {
    throw;
  }
}

void Monitor::setStatus(std::string alertLevel, std::string desc, std::string source) {
  if(alertLevel == "none") {
    if (source == "pulse") {
      pulseAlarm = false;
      warnings[0] = std::make_pair("", "");
    }
    else if(source == "oxygen") {
      oxygenAlarm = false;
      warnings[1] = std::make_pair("", "");
    }
    else {
      bpAlarm = false;
      warnings[2] = std::make_pair("", "");
    }
  }
  else {
    if (source == "pulse") {
        pulseAlarm = true;
        warnings[0] = std::make_pair(alertLevel, desc);
    }
    else if (source == "oxygen") {
        oxygenAlarm = true;
        warnings[1] = std::make_pair(alertLevel, desc);
    }
    else {
        bpAlarm = true;
        warnings[2] = std::make_pair(alertLevel, desc);
    }
  }

  if(pulseAlarm || oxygenAlarm || bpAlarm) {
    for(int i = 0; i < warnings.size(); i++) {
      if(warnings[i].first == "high") {
        currentStatus = warnings[i].first;
        description = warnings[i].second;
      }
      else if(warnings[i].first == "medium" && currentStatus != "high") {
        currentStatus = warnings[i].first;
        description = warnings[i].second;
      }
      else if (warnings[i].first == "low" && (currentStatus != "high" || currentStatus != "medium")) {
        currentStatus = warnings[i].first;
        description = warnings[i].second;
      }
    }
    previousSource = source;
  }
  else {
    currentStatus = "none";
    description = "Everything is normal";
  }
}

void Monitor::checkPulse(int readPulse) {
  if(readPulse >= PULSE_MIN && readPulse <= PULSE_MAX) {
    if(readPulse < PULSE_ALARM_VALS[0].first) {
      setStatus(PULSE_ALARM_VALS[0].second, "Pulse is life threateningly low: " + std::to_string(readPulse), "pulse");
    }
    else if(readPulse > PULSE_ALARM_VALS[1].first) {
      setStatus(PULSE_ALARM_VALS[1].second, "Pulse is life threateningly high: " + std::to_string(readPulse), "pulse");
    }
    else if (readPulse < PULSE_ALARM_VALS[2].first) {
      setStatus(PULSE_ALARM_VALS[2].second, "Pulse is dangerously low: " + std::to_string(readPulse), "pulse");
    }
    else if(readPulse > PULSE_ALARM_VALS[3].first) {
      setStatus(PULSE_ALARM_VALS[3].second, "Pulse is dangerously high: " + std::to_string(readPulse), "pulse");
    }
    else if (readPulse > PULSE_ALARM_VALS[4].first) {
      setStatus(PULSE_ALARM_VALS[4].second, "Pulse is potentially too high: " + std::to_string(readPulse), "pulse");
    }
    else {
      setStatus("none", "Everything is normal", "pulse");
    }
  }
  else {
    // pulse is either fine or there is an equipment malfunction
    setStatus("low", "Equipment malfunction", "pulse");
  }
}

void Monitor::missedOxygenRead() {
  missedOxygen++;
  if (missedOxygen > 2) {
    invalidateOxygen();
  }
  else {
    if (oxygenReads.size() < 6 && oxygenReads.size() > 0) {
      oxygenReads.push_back(oxygenReads.back());
    }
    else {
      oxygenReads.pop_front();
      oxygenReads.push_back(oxygenReads.back());
      setOxygenLevel(calculateAverage(oxygenReads));
    }
  }
}

std::string oxygenToString(double oxygen) {
  std::stringstream converter;
  converter << std::fixed << std::setprecision(1) << oxygen;
  return converter.str();
}

void Monitor::setOxygenLevel(double oxygenAverage) {
  if(oxygenAverage < OXYGEN_ALARM_VALS[0].first) {
    setStatus(OXYGEN_ALARM_VALS[0].second, "Oxygen is life threateningly low: " + oxygenToString(oxygenAverage), "oxygen");
  }
  else if(oxygenAverage < OXYGEN_ALARM_VALS[1].first) {
    setStatus(OXYGEN_ALARM_VALS[1].second, "Oxygen is dangerously low: " + oxygenToString(oxygenAverage), "oxygen");
  }
  else if (oxygenAverage < OXYGEN_ALARM_VALS[2].first) {
    setStatus(OXYGEN_ALARM_VALS[2].second, "Oxygen is potentially too low: " + oxygenToString(oxygenAverage), "oxygen");
  }
  else {
    setStatus("none", "Everything is normal", "oxygen");
  }
}

void Monitor::checkOxygen(double readOxygen) {
  if(oxygenReads.size() < 6) {
    if(readOxygen >= OXYGEN_MIN && readOxygen <= OXYGEN_MAX) {
      missedOxygen = 0;
      oxygenReads.push_back(readOxygen);
      setStatus("none", "Everything is normal", "oxygen");
    }
    else {
      invalidateOxygen();
    }
  }
  else if (readOxygen >= OXYGEN_MIN && readOxygen <= OXYGEN_MAX) {
    missedOxygen = 0;
    oxygenReads.pop_front();
    oxygenReads.push_back(readOxygen);
    setOxygenLevel(calculateAverage(oxygenReads));
  }
  else {
    invalidateOxygen();
  }
}

std::string bpToString(std::pair<int, int> bp) {
  return std::to_string(bp.first) + "/" + std::to_string(bp.second);
}

void Monitor::checkBp(std::pair<int, int> readBp) {
  if ((readBp.first < BP_MAX.first && readBp.second < BP_MAX.second) && (readBp.first >= 0 && readBp.second >= 0)) {
    if (readBp.first < BP_ALARM_VALS[0].first.first || readBp.second < BP_ALARM_VALS[0].first.second) {
      setStatus(BP_ALARM_VALS[0].second, "BP is life threateningly low: " + bpToString(readBp),  "bp");
    }
    else if (readBp.first < BP_ALARM_VALS[2].first.first || readBp.second < BP_ALARM_VALS[2].first.second) {
      setStatus(BP_ALARM_VALS[2].second, "BP is dangerously low: " + bpToString(readBp), "bp");
    }
    else if ((readBp.first > BP_ALARM_VALS[1].first.first || readBp.second > BP_ALARM_VALS[1].first.second)) {
      setStatus(BP_ALARM_VALS[1].second, "BP is dangerously high: " + bpToString(readBp), "bp");
    }
    else if (readBp.first > BP_ALARM_VALS[3].first.first || readBp.second > BP_ALARM_VALS[3].first.second) {
      setStatus(BP_ALARM_VALS[3].second, "BP is potentially too high: " + bpToString(readBp), "bp");
    }
    else {
      setStatus("none", "Everything is normal", "bp");
    }
  }
  else {
    setStatus("low", "Equipment malfunction", "bp");
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

  if(parsedInput.size() == 3) {
    try {
      pulse = stoi(parsedInput[0]);
      oxygen = stod(parsedInput[1]);
      bp = parseBp(parsedInput[2], '/');
    }
    catch(std::invalid_argument&) {
      setStatus("low", "Equipment malfunction", "pulse");
      invalidateOxygen();
      printStatus();
      previousStatus = currentStatus;
      currentStatus = "none";
      return;
    }

    checkPulse(pulse);
    checkOxygen(oxygen);
    checkBp(bp);
  }
  else if (parsedInput.size() == 2){
    // we're either getting oxygen or bp
    char delimiter = '/';
    std::size_t found = parsedInput[1].find(delimiter);

    if(found != std::string::npos) {
      // we got bp
      try {
        pulse = stoi(parsedInput[0]);
        bp = parseBp(parsedInput[1], delimiter);
      }
      catch(std::invalid_argument&) {
        setStatus("low", "Equipment malfunction", "pulse");
        invalidateOxygen();
        printStatus();
        previousStatus = currentStatus;
        currentStatus = "none";
        return;
      }

      checkPulse(pulse);
      checkBp(bp);
      missedOxygenRead();
    }
    else {
      // we got oxygen
      try {
        pulse = stoi(parsedInput[0]);
        oxygen = stod(parsedInput[1]);
      }
      catch(std::invalid_argument&) {
        setStatus("low", "Equipment malfunction", "pulse");
        invalidateOxygen();
        printStatus();
        previousStatus = currentStatus;
        currentStatus = "none";
        return;
      }

      checkPulse(pulse);
      checkOxygen(oxygen);
    }
  }
  else if (parsedInput.size() == 1){
    // we're only getting a pulse
    try {
      pulse = stoi(parsedInput[0]);
    }
    catch(std::invalid_argument&) {
      setStatus("low", "Equipment malfunction", "pulse");
      invalidateOxygen();
      printStatus();
      previousStatus = currentStatus;
      currentStatus = "none";
      return;
    }

    missedOxygenRead();
    checkPulse(pulse);
  }
  else {
    // If parsedInput.size() isn't between 1 and 3, there is an error
    setStatus("low", "Equipment malfunction", "pulse");
  }

  printStatus();
  previousStatus = currentStatus;
  currentStatus = "none";
}
