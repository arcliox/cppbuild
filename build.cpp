#include <iostream>
#include <string>
#include <filesystem>
#include <cstdlib>
#include <vector>

#define EXIT_ERROR -1
#define GCC_FLAG 1 
#define CLANG_FLAG 2

int detect_compiler() { 
  int flags = 0;

  if( std::system("g++ -v > /dev/null 2>&1") == 0 ) {
    flags |= GCC_FLAG;
  }

  if( std::system("clang -v > /dev/null 2>&1") == 0 ) {
    flags |= CLANG_FLAG;
  }

  /* TODO: Add code to get more compilers. */
  /* TODO: Add code to get the compiler versions */
  return flags;
}


int main(int argc, char** argv) {
  std::vector<std::string> src = {"main.cpp"};
  std::string 
    project("hello"),
    flags("-std=c++23 "), 
    cxx_compiler
    ;

  std::filesystem::path CURRENT_SRC_DIR = std::filesystem::absolute(__FILE__);
  CURRENT_SRC_DIR.remove_filename();
  CURRENT_SRC_DIR = std::filesystem::weakly_canonical(CURRENT_SRC_DIR);

  std::cout << "CURRENT_BUILD_DIR: " << CURRENT_SRC_DIR << '\n';

  int compiler = detect_compiler();

  if(compiler == 0) {
    std::cerr << "ERROR: No compatible compilers detected! Exiting.\n";
    return EXIT_ERROR;
  }

  //prefer GCC for the moment.
  if((compiler & GCC_FLAG) != 0) {
    cxx_compiler.append("g++ ");
  }
  else if((compiler & CLANG_FLAG)) {
    cxx_compiler.append("clang ");
  }

  std::string cmd = cxx_compiler + flags;

  for( int i = 0; i < src.size(); ++i ) {
     cmd += " " + static_cast<std::string>(CURRENT_SRC_DIR) + "/" + src[i];
  }
  cmd += " -o " + project + " > /dev/null 2>&1";

  std::cout << "Building: " + project << "\n";
  std::system(cmd.c_str());
  std::cout << "Finished.\n";
}