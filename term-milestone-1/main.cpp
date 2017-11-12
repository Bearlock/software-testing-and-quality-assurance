#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "monitor.h"

using namespace std;

int main(int argc, char* argv[]) {
  Monitor healthMonitor;
  string inputText;

  cout << endl;

  if(argc > 1) {
    ifstream monitorFile;
    monitorFile.open(argv[1]);

    if(!monitorFile.is_open()) {
      cout << "That doesn't seem to be a valid file name.\nPlease enter a valid file name." << endl;
      return 1;
    }
    else {
      // Good file read
      while(getline(monitorFile, inputText)) {
        cout << inputText << endl;
        if(inputText[0] != '#') {
          healthMonitor.processLine(inputText);
        }
        else {
          // if the string doesn't start with #, process it,
          // otherwise its a comment
        }
      }
    }
  }
  else {
    // No file, assume CLI input
    cout << "Welcome to a health monitoring program. Please enter vitals: " << endl;
    while(getline(cin, inputText)) {
      healthMonitor.processLine(inputText);
    }
  }
}
