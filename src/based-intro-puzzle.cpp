#include <iostream>
#include <sstream>
#include <climits>
#include <cmath>

#include "./include/puzzle.h"
#include "./include/utils.h"

#define MAX_HEX_ANS_LEN 8

static std::string solve(int num)
{
  std::stringstream ss;
  std::string sString, reverse;
  ss << std::hex << num;
  sString = ss.str();

  for(int i = 0; i < MAX_HEX_ANS_LEN; i++) { //reverse sString
    if(i < (int)sString.size()) { 
      if(sString[(int)sString.size()-i-1] > 96) //convert lower case to capital
        sString[(int)sString.size()-i-1] -= 32;

      reverse += sString[(int)sString.size()-i-1];
    } 
    else reverse += "0"; //add trailing zeros when applicable
  }
  return reverse;
}

//=====================================================================
// std::string create_table() returns an html table with the proper
// styling and values which visually represents a light box as 
// described in desc.txt.
//=====================================================================
static std::string create_table(const short base, short len, int num)
{
  if(num > pow(base, len)-1) { //number cant be stored in specified light box 
    return "ERROR: Number Does not fit light box.";
  }
  std::stringstream lb_table; //light box table
    
  int *base_table = new int[len]; //table for holding each digit of base number
  for(int i = 0; i < len; i++) base_table[i] = 0;

  for(int i = len-1; i >= 0; i--) //populate base_table
    while(num >= pow(base, i)) {
      num -= pow(base, i);
      base_table[i]++;
    }

  lb_table << "    <table class='number'>\n";
  lb_table << "        <tbody>\n";
  for(int i = base-1; i >=0; i--) {//rows
    lb_table << "            <tr>\n";
    for(int j = 0; j < len; j++) {//cols
      if(base_table[j] == i) {//if the current light in this col is on
        if(i == 0) lb_table << "                <td style='color: rgb(255, 0, 0);'>⦿</td>\n";
        else lb_table << "                <td style='color: rgb(141, 255, 141);'>⦿</td>\n";
      }else lb_table << "                <td>⦿</td>\n";
    }
    lb_table << "            </tr>\n";
  }
  lb_table << "        </tbody>\n";
  lb_table << "    </table>";  
  
  delete [] base_table;
  return lb_table.str();
}

Puzzle based_intro_puzzle_create(long seed)
{
  std::vector<std::string> values;
  
  //define arguments
  short base = 16;
  int num = utils_rng_roll((INT_MAX >> 1), INT_MAX, seed);
  int len = 0;
  while(pow(base, len) < num) len++;

  values.push_back(create_table(base, len, num));

  std::string html_content = utils_html_printf("Base Intro Puzzle", "./files-based-intro/.desc.txt", values);
  utils_generate_file("./files-based-intro/instructions.html", html_content);

  return Puzzle{"files-based-intro", solve(num), {}};
}
