#include "../../include/puzzle.h"
#include "../../include/html-generator.h"
#include <vector>

class ColorPuzzle : public Puzzle {

public:

  void set_answer() override
  {
    answer = "331E54F4AA00";
  }

  virtual std::vector<std::string> generate_files(std::string contained_zip_path) override
  {
    std::ifstream desc_file = std::ifstream("Colors/desc.txt");
    std::stringstream desc_stream;
    desc_stream << desc_file.rdbuf();
    std::string desc = desc_stream.str();

    desc_file.close();
    std::string file = generate_html("Read.Me", desc, 0);
    return {contained_zip_path, file};
  }
};

