#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

// After the vector is sorted, these are the corresponding indexes
const int SIDEA = 0;
const int SIDEB = 1;
const int HYPOTENUSE = 2;

// Triangles have to have side lenghts >= 2 and 3 sides.
const int MIN_LENGTH = 2;
const int REQ_SIDES = 3;

// Comments start with '#'
const char COMMENT_CHAR = '#';

// isInvalidLength()
// Checks each side to verify minimum length >= 2
bool isValidLength(vector<int> sides) {
  return (sides[SIDEA] >= MIN_LENGTH || sides[SIDEB] >= MIN_LENGTH || sides[HYPOTENUSE] >= MIN_LENGTH);
}

// hasValidSides()
// Checks if the passed in triangle has 3 sides
bool hasValidSides(vector<int> sides) {
  return sides.size() == REQ_SIDES;
}

// fulfillsTriangleInequality()
// Returns true if the 2 sides are longer than longest sides
bool fufillsTriangleInequality(vector<int> sides) {
  return (sides[SIDEA] + sides[SIDEB]) > sides[HYPOTENUSE];
}

// isTriangle()
// Returns true if all conditions above are fulfilled
bool isTriangle(vector<int> sides) {
  return isValidLength(sides) && hasValidSides(sides) && fufillsTriangleInequality(sides);
}

// isIsoceles()
// Returns true if 2 sides are the same
// Expects integer vector with three sides
bool isIsosceles(vector<int> sides) {
  return(sides[SIDEA] == sides[SIDEB] || sides[SIDEA] == sides[HYPOTENUSE] || sides[SIDEB] == sides[HYPOTENUSE]);
}

// isEquilateral()
// Returns true if all sides are the same
// Expects integer vector with three sides
bool isEquilateral(vector<int> sides) {
  return (sides[SIDEA] == sides[SIDEB] && sides[SIDEA] == sides[HYPOTENUSE]);
}

// isComment()
// Returns true if a string starts with '#'
bool isComment(string line) {
  return (line[0] == COMMENT_CHAR);
}

string getFileName() {
  string fileName;
  do {
    cin.clear();
    cin >> fileName;

    if (cin.fail()) {
      cout << "Please enter a valid file name" << endl;
      cin.ignore(500, '\n');
    }
  } while(cin.fail());

  return fileName;
}

vector<int> createSortedTriangle(string fileText) {
  int side;
  vector<int> sides;
  istringstream fileTextStream(fileText);

  while(fileTextStream >> side) {
    sides.push_back(side);
  }

  sort(sides.begin(), sides.end());
  return sides;
}

string triangleString(vector<int> triangle) {
  string tString = to_string(triangle[SIDEA]) + " " + to_string(triangle[SIDEB]) + " " + to_string(triangle[HYPOTENUSE]);
  return tString;
}

string printTriangle(string triangleString, string type) {
  return "Triangle " + triangleString + " is " + type;
}

string calculateTriangleType(vector<int> triangle) {
  if(isTriangle(triangle)) {
    if(isEquilateral(triangle)) return printTriangle(triangleString(triangle), "equilateral!");
    if(isIsosceles(triangle)) return printTriangle(triangleString(triangle), "isosceles!");
    return printTriangle(triangleString(triangle), "scalene!");
  }

  return printTriangle(triangleString(triangle), "not a triangle");
}

void processTriangle(ifstream& triangleFile) {
  string fileText;

  while(getline(triangleFile, fileText)) {
    vector<int> triangle;

    if(isComment(fileText)) {
      cout << fileText << endl;
    }
    else {
      triangle = createSortedTriangle(fileText);
      cout << calculateTriangleType(triangle) << '\n' << endl;
    }
  }
}

int main(int argc, char* argv[]) {
  ifstream triangleFile;
  string fileName;

  cout << "Welcome to triangle identifier. Please input file name: ";
  fileName = getFileName();
  triangleFile.open(fileName);

  while(!triangleFile.is_open()) {
    cout << "That doesn't seem to be a valid file name; try again: ";
    fileName = getFileName();
    triangleFile.open(fileName);
  }

  processTriangle(triangleFile);
  triangleFile.close();

  return 0;
}
