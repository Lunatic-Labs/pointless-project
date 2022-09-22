# pointless-project
A website to generate and distribute puzzles to prospective Lipscomb recruits

# Compiling
## g++
This project assumes the use of g++ to compile the code into an executable. On linux, the command `sudo apt install g++` will suffice to install this.

After installation, copy and paste the following code into a file called `hello.cpp`:
```cpp
#include <iostream>

using namespace std

int main () {
    cout << "hello, world!" << endl;
}
```
After this file has been saved, in a terminal in the same directory as `hello.cpp`, run `g++ hello.cpp -o hello` and attempt to run `./hello`
Your terminal output should look like this:
```
$> g++ hello.cpp -o hello
$> ./hello
hello, world!
$>
```
If this works, then g++ has probably been installed correctly.

## Boost
**Linux**
*Using aptitude*
In order to get the Boost libraries on linux, run the command `sudo apt-get install libboost-all-dev`

After this is done, `-l boost_iostreams` should be added to the command used to compile the code.

*Manual Installation (unrecommended)*
First, acquire the Boost Library from https://www.boost.org/doc/libs/1_63_0/more/getting_started/unix-variants.html
Unzip the library and navigate to the Boost_<version>/ folder and run the commands `./bootstrap.sh ` and `./b2 install` to install.

**Mac (untested)**
*Using Brew*
`brew install boost` should install the boost libraries needed for this project.
