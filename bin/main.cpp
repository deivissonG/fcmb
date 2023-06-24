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
#ifndef BOOST_ALL_DYN_LINK
#   define BOOST_ALL_DYN_LINK

int GetFingerprint();
int futronic();

#endif
#include <iostream>
#include <sstream>
#include "Scanner.h"
#include "bozorth.h"
#include "Cwsq.h"
#include "Mindtct.h"
#include "Utils.h"
#include <filesystem>
namespace fs = std::filesystem;
int main(int argc, const char *argv[])
{
  fs::path sysTmpDir = fs::temp_directory_path();
  fs::path tmpDir = sysTmpDir / "fcmb/";
  std::cout << tmpDir << std::endl;

  Fcmb::CreateDirectory(tmpDir);

  if (argc > 1)
  {
    std::string arg = argv[1];
    if (arg == "futronic")
    {
      return futronic();
    }
    if (arg == "get")
    {
      return GetFingerprint();
    }
    if (arg == "setprobe")
    {
      if (argc != 3)
      {
        std::cout << "usage: " << argv[0] << " setprobe <filename>" << std::endl;
        return 1;
      }
      std::string filename = argv[2];
      if (!Fcmb::FileExists(filename))
      {
        std::cout << "File does not exist!" << std::endl;
        return 2;
      }
      std::string content;
      Fcmb::ReadFile(filename, content);
      std::string probe = tmpDir / "probenew.xyt";
      Fcmb::WriteFile(probe, content);
      return 0;
    }
  }

//  if (argc != 3)
//  {
//      std::cout << "usage: " << argv[0] << " <directory> <name> " << std::endl;
//      return 1;
//  }
//
//  std::string directory = argv[1];
//  std::string name = argv[2];
//
//  if (Fcmb::InvalidDir(directory)) {
//    std::cout << "Invalid directory name given!" << std::endl;
//    return 2;
//  }

  if (GetFingerprint() != 0)
  {
    std::cout << "Error getting fingerprint" << std::endl;
    return 3;
  }

  int result = 0;
//  Bozorth bozorth(directory + name, directory + name);
//  result = bozorth.Execute();
  std::cout << "Bozorth result: " << result << std::endl;
  return 0;
}

int GetFingerprint() {
  fs::path directory = fs::temp_directory_path() / "fcmb";
  std::string name = "verify";

  std::ostringstream oss;
  oss << directory / (name + ".bmp");
  std::string bitmap = oss.str();

  LOG(bitmap)

  try
  {
    Scanner scanner(bitmap);
    scanner.ScanImage();
  }
  catch (const ScannerException& e)
  {
    std::cout << e.what() << std::endl;
    return 1;
  }

  Cwsq cwsq(bitmap);
  cwsq.Execute();

  Mindtct mindtct(directory / name);
  mindtct.Execute();

  std::cout << "Fingerprint: " << Fcmb::ConvertPathToChar(directory / name / ".xyt") << std::endl;
  std::ifstream currentFingerPrint(Fcmb::ConvertPathToChar(directory / (name + ".xyt")));
  if (!currentFingerPrint.is_open())
  {
    std::cout << "Error opening file" << std::endl;
    return 1;
  }

  std::string result;
  while ( currentFingerPrint ) {
    std::string line;
    std::getline(currentFingerPrint, line);
    result += line + "\n";
  }
  std::cout << result << std::endl;
  oss.str(""); // clear ostream
  oss << directory << "bmp/" << name << ".bmp";
  Fcmb::MoveBinary(bitmap, oss.str());
//  Fcmb::RemoveBinary(oss.str());
  return 0;
}

int SetProbe(const char *probe, const fs::path& tmpDir)
{
  std::string fileName = "probe";
  std::string probePath = tmpDir.string() + fileName;
  std::cout << probePath << std::endl;
  std::string probe_str(probe);
  Fcmb::WriteFile(probePath, probe_str);
  return 0;
}

int GetProbe(const fs::path& tmpDir, std::string& probe)
{
  std::string fileName = "probe";
  std::string probePath = tmpDir.string() + fileName;
  std::cout << probePath << std::endl;
  std::ifstream probeFile(probePath);
  if (!probeFile.is_open())
  {
    std::cout << "Error opening file" << std::endl;
    return 1;
  }
  while ( probeFile ) {
    std::getline(probeFile, probe);
    std::cout << probe << std::endl;
  }
  return 0;
}

int captureFrame(std::basic_string<char>& bitmap)
{
  LOG(bitmap)
  try
  {
    Scanner scanner(bitmap);
    scanner.ScanImage();
  }
  catch (const ScannerException& e)
  {
    std::cout << e.what() << std::endl;
    return 1;
  }
  return 0;
}

int futronic() {
  fs::path directory = fs::temp_directory_path() / "fcmb";
  std::string name = "oselio";

  std::ostringstream oss;
  oss << directory / (name + ".bmp");
  std::string bitmap = oss.str();

  int ret;

  ret = captureFrame(bitmap);
  if (ret)
  {
    std::cout << "Error capturing frame" << std::endl;
    return 1;
  }

  std::cout << "Fingerprint bitmap: " << bitmap << std::endl;

  return 0;
}

int getFingerprintMinutiae (const fs::path& tmpDir, std::string& minutiae) {
  std::string fileName = "verify";
  std::string minutiaePath = tmpDir.string() + fileName;
  std::cout << minutiaePath << std::endl;
  std::ifstream minutiaeFile(minutiaePath);
  if (!minutiaeFile.is_open())
  {
    std::cout << "Error opening file" << std::endl;
    return 1;
  }
  while ( minutiaeFile ) {
    std::getline(minutiaeFile, minutiae);
    std::cout << minutiae << std::endl;
  }
  return 0;
}