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

// Triangles have to have side lengths >= 1 and 3 sides.
const int MIN_LENGTH = 1;
const int REQ_SIDES = 3;

// Comments start with '#'
const char COMMENT_CHAR = '#';

// Checks each side to verify minimum length >= 1
bool isValidLength(vector<int> sides) {
  return (sides[SIDEA] >= MIN_LENGTH || sides[SIDEB] >= MIN_LENGTH || sides[HYPOTENUSE] >= MIN_LENGTH);
}

// Checks if the passed in triangle has 3 sides
bool hasValidSides(vector<int> sides) {
  return sides.size() == REQ_SIDES;
}

// Returns true if the 2 sides are longer than longest sides
bool fufillsTriangleInequality(vector<int> sides) {
  return (sides[SIDEA] + sides[SIDEB]) > sides[HYPOTENUSE];
}

// Returns true if all conditions above are fulfilled
bool isTriangle(vector<int> sides) {
  return hasValidSides(sides) && isValidLength(sides) && fufillsTriangleInequality(sides);
}

// Returns true if 2 sides are the same
// Expects integer vector with three sides
bool isIsosceles(vector<int> sides) {
  return(sides[SIDEA] == sides[SIDEB] || sides[SIDEA] == sides[HYPOTENUSE] || sides[SIDEB] == sides[HYPOTENUSE]);
}

// Returns true if all sides are the same
// Expects integer vector with three sides
bool isEquilateral(vector<int> sides) {
  return (sides[SIDEA] == sides[SIDEB] && sides[SIDEA] == sides[HYPOTENUSE]);
}

// Returns true if a string starts with '#'
bool isComment(string line) {
  return (line[0] == COMMENT_CHAR);
}

// Gets input until a valid filename is provided
// Returns filename
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

// Sorts a vector representation of triangle side lengths in ascending order
// Returns the sorted triangle vector
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

// Little helper function to convert vector values to string
string triangleString(vector<int> triangle) {
  string tString = to_string(triangle[SIDEA]) + " " + to_string(triangle[SIDEB]) + " " + to_string(triangle[HYPOTENUSE]);
  return tString;
}

// Another helper function to combine triangleString (above) and the calculated type
string printTriangle(string triangleString, string type) {
  return "Triangle " + triangleString + " is " + type;
}

// Wrapper function for some of the boolean functions above; determines triangle type
// Returns a string with triangle type
string calculateTriangleType(vector<int> triangle) {
  if(isTriangle(triangle)) {
    if(isEquilateral(triangle)) return printTriangle(triangleString(triangle), "equilateral!");
    if(isIsosceles(triangle)) return printTriangle(triangleString(triangle), "isosceles!");
    return printTriangle(triangleString(triangle), "scalene!");
  }

  return printTriangle("on this line", "not a triangle");
}

// Processes the passed in file line by line; determines if a line is a comment or triangle
// If comment, prints it out. Otherwise it will sort/push values into a vector and calculate
// The corresponding triangle type; prints out the string.
void processTriangle(ifstream& triangleFile) {
  string fileText;

  while(getline(triangleFile, fileText)) {
    vector<int> triangle;

    if(isComment(fileText)) {
      cout << fileText << endl;
    }
    else {
      triangle = createSortedTriangle(fileText);
      cout << calculateTriangleType(triangle) << endl;
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

  cout << "\n\n" << endl;
  processTriangle(triangleFile);
  triangleFile.close();

  return 0;
}
