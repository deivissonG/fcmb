/*
* MIT License
*
* Copyright (c) 2018 Derick Felix <derickfelix@zoho.com>
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#include "Utils.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string.h>

namespace Fcmb {
  void CopyBinary(const std::string& source, const std::string& target)
  {
    LOG("Copying binary")
    std::ifstream ifs(source, std::ios::binary);
    if (!ifs) {
      std::cout << "Failed to open file " << source << " for reading!" << std::endl;
      return;
    }
    std::ofstream ofs(target, std::ios::binary);
    if (!ofs) {
      std::cout << "Failed to open file " << target << " for writing!" << std::endl;
      return;
    }
    ofs << ifs.rdbuf();
  }

  void MoveBinary(const std::string& source, const std::string& target)
  {
    CopyBinary(source, target);
    remove(source.c_str());
  }

  void RemoveBinary(const std::string& filename)
  {
    remove(filename.c_str());
  }

  void CreateDirectory(const std::string& path)
  {
    if (std::filesystem::exists(path)) {
      LOG("Directory already exists")
      return;
    }
    LOG("Creating directory")
    if (system(("mkdir -p " + path).c_str())) {
      std::cout << "Failed to create directory " << path << "!" << std::endl;
    } else {
      LOG("Done!")
    }
  }

  void WriteFile(const std::string& path, const std::string& content)
  {
    std::ofstream file;
    file.open(path);
    file << content;
    file.close();
    LOG("File successfully written")
  }

  void ReadFile(const std::string& filename, std::string& content) {
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
      content += line + "\n";
    }
    file.close();
  }

  std::string ConvertPathToString(const std::string path)
  {
    std::string path_str = path;
    if (!path_str.empty()) {
      if (path_str[0] == '"')
        path_str.erase(0, 1);

      if (path_str[path_str.size() - 1] == '"')
        path_str.pop_back();
    }
    return path_str;
  }

  char * ConvertStringToChar(const std::string str)
  {
    char *cstr = new char[str.length() + 1];
    strcpy(cstr, str.c_str());
    return cstr;
  }

  char * ConvertPathToChar(const std::filesystem::path path)
  {
    return ConvertStringToChar(ConvertPathToString(path.string()));
  }
}
