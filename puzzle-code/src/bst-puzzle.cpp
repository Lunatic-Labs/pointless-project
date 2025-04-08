/*
 * File: bst-puzzle.cpp
 * Author: Zachary Haskins, Turner Austin, Michael Hernandez-Lara
 * Contact: zdhdev@yahoo.com, tcaustin@mail.lipscomb.edu, mahernandezlara@mail.lipscomb.edu
 * Date: 10/2/2024
 * Description:
 *   The user is presented with several generated expression as well as the root variable.
 *   They must create and navigate the binary search tree to find the correct path from the root to the destination.
 */

#include <iostream>
#include <algorithm>
#include <cassert>

#include "./include/puzzle.h"
#include "./include/utils.h"

#define ROOT_MIN 11
#define ROOT_MAX 150

static int fail_safe = 0;
static int password;
static std::string password_path;
static long mutable_seed;

int not_root(int x, const int root)
{
  if (x == root) {
    if (x + 10 > ROOT_MAX) {
      x -= 10;
    } else {
      x += 10;
    }
  }

  return x;
}

int add(int i, int j)
{
  return i - j;

}

std::string addition(std::string path, const int root)
{
  int a = utils_rng_roll(10, root-1, mutable_seed);
  int b = add(root, a);
  int c = utils_rng_roll(1, ROOT_MAX, mutable_seed);
  c = not_root(c, root);
  c = add(c,a);

  std::string text;
  std::string dir;
  if ((b & 1) == 1) {
    text = "if " + std::to_string(a) + " + " + std::to_string(b) + " = x, then go right\nelse, go left";
    dir = "r";
  } else {
    text = "if " + std::to_string(a) + " + " + std::to_string(c) + " = x, then go right\nelse, go left";
    dir = "l";
  }
  utils_generate_file(path + "/Addition.txt", text);
  return dir;
}

int sub(int i, int j)
{
  return i + j;
}

std::string subtraction(std::string path, const int root)
{
  int a = utils_rng_roll(0, 200, mutable_seed);
  int b = sub(root,a);
  int c = utils_rng_roll(1, ROOT_MAX, mutable_seed);
  c = not_root(c, root);
  c = sub(c,a);

  std::string text;
  std::string dir;
  if ((b & 1) == 1) {
    text = "if " + std::to_string(b) + " - " + std::to_string(a) + " = x, then go right\nelse, go left";
    dir = "r";
  } else {
    text = "if " + std::to_string(c) + " - " + std::to_string(a) + " = x, then go right\nelse, go left";
    dir = "l";
  }
  utils_generate_file(path + "/Subtraction.txt", text);
  return dir;
}

double multiply(int i, int j)
{
  return 1.0 * i / j;
}

std::string multiplication(std::string path, const int root)
{
  int a = utils_rng_roll(1, root-1, mutable_seed);
  double b = multiply(root, a);
  int c = utils_rng_roll(50, ROOT_MAX, mutable_seed);
  c = not_root(c, root);
  double d = multiply(c,a);

  std::string text;
  std::string dir;
  if ((a & 1) == 1) {
    text = "if " + std::to_string(a) + " * " + std::to_string(b) + " = x, then go right\nelse, go left";
    dir = "r";
  } else {
    text = "if " + std::to_string(a) + " * " + std::to_string(d) + " = x, then go right\nelse, go left";
    dir = "l";
  }
  utils_generate_file(path + "/Multiplication.txt", text);
  return dir;
}

int divide(int i, int j)
{
  return i * j;
}

std::string division(std::string path, const int root)
{
  int a = utils_rng_roll(1, ROOT_MAX, mutable_seed);
  int b = divide(root, a);
  int c = utils_rng_roll(1, ROOT_MAX, mutable_seed);
  c = not_root(c, root);
  c = divide(c, a);

  std::string text;
  std::string dir;
  if ((a & 1) == 1) {
    text = "if " + std::to_string(b) + " / " + std::to_string(a) + " = x, then go right\nelse, go left";
    dir = "r";
  } else {
    text = "if " + std::to_string(c) + " / " + std::to_string(a) + " = x, then go right\nelse, go left";
    dir = "l";
  }
  utils_generate_file(path + "/Division.txt", text);
  return dir;
}

int sqrt(int i)
{
  return i * i;
}

std::string square_root(std::string path, const int root)
{
  int a = sqrt(root);
  int b = utils_rng_roll(1, ROOT_MAX, mutable_seed);
  b = not_root(b, root);
  b = sqrt(b);

  std::string text;
  std::string dir;
  if ((b & 1) == 1) {
    text = "if √" + std::to_string(a) + " = x, then go right\nelse, go left";
    dir = "r";
  } else {
    text = "if √" + std::to_string(b) + " = x, then go right\nelse, go left";
    dir = "l";
  }
  utils_generate_file(path + "/Square_Root.txt", text);
  return dir;
}

std::string expression(std::string path, const int root)
{
  int rng = utils_rng_roll(50, 200, mutable_seed);
  rng = not_root(rng, root);
  int a,b,c,d,e,f;  // true answer
  double g;
  int t,u,v,w,x,y;  // fake answer
  double z;
  std::string symbols[4];

  if ((rng & 1) == 1) {
    a = sqrt(root);
    t = sqrt(rng);
    symbols[0] = "√";
  } else {
    a = root;
    t = rng;
    symbols[0] = "0";
  }
  // √?(a)
  // √?(t)

  rng = utils_rng_roll(50, 200, mutable_seed);
  b = rng;
  u = rng;
  if ((rng & 1) == 1) {
    c = add(a, b);
    v = add(t,u);
    symbols[1] = " + ";
  } else {
    c = sub(a, b);
    v = sub(t,u);
    symbols[1] = " - ";
  }
  // √?(c +/- b)
  // √?(v +/- u)

  rng = utils_rng_roll(1, 50, mutable_seed);
  d = rng;
  w = rng;
  if ((rng & 1) == 1) {
    e = sub(c, d);
    x = sub(v,w);
    symbols[2] = " - ";
  } else {
    e = add(c, d);
    x = add(v,w);
    symbols[2] = " + ";
  }
  // √?((e -/+ d) +/- b)
  // √?((x -/+ w) +/- u)

  rng = utils_rng_roll(1, 50, mutable_seed);
  f = rng;
  y = rng;
  if ((rng & 1) == 1) {
    g = multiply(b, f);
    z = multiply(u,y);
    symbols[3] = " x ";
  } else {
    g = divide(b,f);
    z = multiply(u,y);
    symbols[3] = " / ";
  }
  // √?((e -/+ d) +/- (g *||/ f))
  // √?((x -/+ w) +/- (z *||/ y))

  std::string text;
  std::string dir;
  if (symbols[0] == "0") {
    text = "if (" + std::to_string((rng & 1) == 1? e:x) + symbols[2] + std::to_string((rng & 1) == 1? d:w) + ")" + symbols[1] + "(" + std::to_string((rng & 1) == 1? g:z) + symbols[3] + std::to_string((rng & 1) == 1? f:y) + ") = x, then go right";
    text += "\nelse, then go left";
  } else {
    text = "if √((" + std::to_string((rng & 1) == 1? e:x) + symbols[2] + std::to_string((rng & 1) == 1? d:w) + ")" + symbols[1] + "(" + std::to_string((rng & 1) == 1? g:z) + symbols[3] + std::to_string((rng & 1) == 1? f:y) + ")) = x, then go right";
    text += "\nelse, then go left";
  }
  if ((rng & 1) == 1) {
    dir = "r";
  } else {
    dir ="l";
  }
  utils_generate_file(path + "/Expression.txt", text);
  return dir;
}

std::string comparison(std::string path, const int root)
{
  int comp = utils_rng_roll(10, 100, mutable_seed);
  std::string text = "if x > " + std::to_string(comp) + ", then go right\nelse, go left";
  std::string dir;
  if (root > comp) {
    dir = "r";
  } else {
    dir = "l";
  }
  utils_generate_file(path + "/Comparison.txt", text);
  return dir;
}

std::string to_hex(int num)
{
  char hex[3];
  int i = 0;
  while (num != 0) {
    int temp = num % 16;

    if (temp < 10) {
      hex[i] = temp + 48;
    } else {
      hex[i] = temp + 55;
    }
    i++;

    num /= 16;
  }
  std::string hexnum;
  for (int j = i-1; j >= 0; j--) {
    hexnum += hex[j];
  }

  return hexnum;
}

std::string hexadecimal(std::string path, const int root)
{
  std::string ans = to_hex(root);
  int a = utils_rng_roll(0, ROOT_MAX, mutable_seed);
  a = not_root(a, root);
  std::string hex = to_hex(a);

  std::string text;
  std::string dir;
  if ((a & 1) == 1) {
    text = "if " + ans + " is hexadecimal for x, then go right\nelse, go left";
    dir = "r";
  } else {
    text = "if " + hex + " is hexadecimal for x, then go right\nelse, go left";
    dir = "l";
  }
  utils_generate_file(path + "/Hexadecimal.txt", text);
  return dir;
}

static void create_dirs(std::string path, int depth, std::string dir, const int root, bool correct)
{
  if (fail_safe++ > 9000) {
    std::cout << "HIT FAIL SAFE\n";
    return;
  }
  if (depth < -10) {
    std::cout << "Error: depth < -10" << std::endl;
    return;
  }
  if (depth < 0) {
   int rng = utils_rng_roll(1000000,9000000, mutable_seed);
   utils_generate_file(path + "/ID.txt", std::to_string(rng));
   if(correct) {
    password = rng;
   }
   return;
  }
  path += dir;
  utils_mkdir(path);

  std::string correct_path;
  if (depth > 0) {
    ++mutable_seed;
    int rng = 8/*utils_rng_roll(1, 8, mutable_seed)*/;
    switch (rng) {
      case 1:
        correct_path = comparison(path, root);
        break;
      case 2:
        correct_path = addition(path, root);
        break;
      case 3:
        correct_path = subtraction(path, root);
        break;
      case 4:
        correct_path = multiplication(path, root);
        break;
      case 5:
        correct_path = division(path, root);
        break;
      case 6:
        correct_path = square_root(path, root);
        break;
      case 7:
        correct_path = hexadecimal(path, root);
        break;
      case 8:
        correct_path = expression(path, root);
        break;
      default:
        std::cout << "Error: invalid case" << std::endl;
        return;
    }
  }
  
  if (correct_path == "r" && correct) {
    password_path += correct_path;
    create_dirs(path, depth - 1, "/left", root, false);
    create_dirs(path, depth - 1, "/right", root, true);
  }else if (correct_path == "l" && correct) {
    password_path += correct_path;
    create_dirs(path, depth - 1, "/left", root, true);
    create_dirs(path, depth - 1, "/right", root, false);
  }else if (correct == false) {
    create_dirs(path, depth - 1, "/left", root, false);
    create_dirs(path, depth - 1, "/right", root, false);
  }else {
    create_dirs(path, depth - 1, "/ID", root, true);
  }
}

Puzzle bst_puzzle_create(long seed)
{
  if (password_path != "") {  // For automatic testing
    password_path.clear();
    fail_safe = 0;
  }
  mutable_seed = seed;
  const int root = utils_rng_roll(ROOT_MIN,ROOT_MAX,seed);
  const size_t depth = 10;

  create_dirs("../html-txt/files-bst", depth, "/tree", root, true);
  std::string html_body = utils_html_printf("BST Puzzle", "../html-txt/files-bst/.desc.txt", { std::to_string(root) });
  utils_generate_file("../html-txt/files-bst/instructions.html", html_body);
  return {"../html-txt/files-bst", html_body, std::to_string(password), " Path: " + password_path};
}
