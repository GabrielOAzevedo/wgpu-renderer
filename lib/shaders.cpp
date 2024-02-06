#include <fstream>
#include <string>

const std::string loadShaderFromFile(const char* path) {
  std::ifstream in(path);
  std::string contents((std::istreambuf_iterator<char>(in)),
                       std::istreambuf_iterator<char>());

  return contents;
}