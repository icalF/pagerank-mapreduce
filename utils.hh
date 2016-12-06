#ifndef __UTILS__H_
#define __UTILS__H_ 1

#include <cstdio>
#include <string>
#include <vector>

using namespace std;

string vectorToString(vector<int>& v) {
  char buff[v.size() * 10 * sizeof( char )];
  for (unsigned offset = 0, i = 0; i < v.size(); ++i) {
    offset += sprintf (buff + offset, 
                      (i ? ",%d" : "%d"),
                      v[i]);
  }
  return string(buff);
}

string doubleToString(double num) {
  char buff[30];
  sprintf(buff, "%.8lf", num);
  return string(buff);
}

#endif