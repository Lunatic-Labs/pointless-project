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
We use Boost C++ for the majority of our streams functionality and plan to use it for Regex and many other tasks in the future
Documentation: https://www.boost.org/doc/libs/1_80_0/
**Linux**

*Using aptitude*
In order to get the Boost libraries on linux, run the command `sudo apt-get install libboost-all-dev`

*Using pacman(arch based distros*
Run the command `sudo pacman -S boost`

*Manual Installation (unrecommended)*
First, acquire the Boost Library from https://www.boost.org/doc/libs/1_63_0/more/getting_started/unix-variants.html
Unzip the library and navigate to the Boost_<version>/ folder and run the commands `./bootstrap.sh ` and `./b2 install` to install.

## Mac Setup
To run this project using a M1 Mac please follow these steps: 

*Using Brew and Rosetta Intel emulator*
Before installing Homebrew you will need to install Rosetta2 emulator for the new ARM silicon (M1 chip). Install Rosetta2 via terminal using:
`/usr/sbin/softwareupdate --install-rosetta --agree-to-license`
This will install rosetta2 with no extra button clicks.

After installing Rosetta2 above you can then use the Homebrew cmd and install Homebrew for ARM M1 chip.
`arch -x86_64 /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install.sh)"`

Once Homebrew for M1 ARM is installed use this Homebrew command to install packages: 

Boost library:
`arch -x86_64 brew install boost`

Zlib: 
`arch -x86_64 brew install zlib`

Libzip: 
`arch -x86_64 brew install libzip`

To run this project make sure that `/usr/local/` contains the library installed above. 

Open your text editor (VScode preferably) and open a new terminal. 
Run `arch -x86_64 zsh` to open a Rosetta shell first. Then run make test or make build to proceed. 


## Libzip
This project uses Libzip as a method for generating ZIP files

Documentation https://libzip.org/documentation/ 

**Linux**
Run `sudo apt-get install libzip-dev`. Add `-l zip` to your compile command

## Testing
To build a test, run `make test` from the project root. Note, obj should exist in your project root. From this, run `./obj/<TestModuleName>.test` to run the test.

***Adding an Automated Test Module as a Github Action***: Whenever you write tests, you should automate them so the Github Project runs them whenever code is committed. To do this, edit `.github/workflows` and add `run-<test-module>.yaml` as a file. Note, these files use a different naming convention than cpp files. 

`name` should be set to the name of your test module, capitalize every word and separate by spaces.

`on` decides when the tests should be run. This should probably be `[push]`

`jobs` defines what you want to do here. Add a sublisting with `Run-<Test-Module>`. `runs-on` should be `ubuntu-latest`.

`steps` will 99% look something like this:
```yaml
steps:
  - uses: actions/checkout@v2
  - run: sudo apt install libboost-all-dev -y; sudo apt install libzip-dev -y;
  - run: |
      cd ${{github.workspace}}
      mkdir obj
      make obj/<TestModule>.test
  - run: ${{github.workspace}}/obj/<TestModule>.test
```

If there are any problems with this, send an email to jwroberts1@mail.lipscomb.edu or message Justice Roberts on slack.
