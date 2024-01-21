#include <iostream>

#include "../../include/puzzle.h"
#include "../../include/html-generator.h"

class HelloWorld : public Puzzle
{

protected:
  void set_answer() override
  {
    answer = "4";
  }

  virtual std::vector<std::string> generate_files(std::string contained_zip_path) override
  {
    try {
      std::string file = generate_html(name, "What is 2+2?", get_seed());
      return {contained_zip_path, file};
    }
    catch(std::exception e) {
      std::cerr << e.what();
      return {};
    }
  }
};

