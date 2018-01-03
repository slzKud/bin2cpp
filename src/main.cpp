// main.cpp : Defines the entry point for the console application.
//

#include "targetver.h"

#include "errorcodes.h"
#include "SegmentGenerator.h"
#include "StringGenerator.h"
#include "ArrayGenerator.h"

#include <stdlib.h>
#include <stdio.h>
#include <sstream>

#include "common.h"
#include "argumentparser.h"
#include "logger.h"

//#define ENABLE_BREAKPOINT_DEBUGGING

#ifdef ENABLE_BREAKPOINT_DEBUGGING
#include <windows.h>
#endif

void printHeader()
{
  printf("bin2cpp v%s\n", bin2cpp::getVersionString() );
  printf("Convert binary files into C++ source code.\n");
  printf("Copyright (C) 2013-%d end2endzone.com. All rights reserved.\n", bin2cpp::getCopyrightYear());
  printf("bin2cpp is open source software, see http://github.com/end2endzone/bin2cpp \n");
}

void printUsage()
{
  //usage string in docopt format. See http://docopt.org/
  static const char usage[] = 
    "Usage:\n"
    "  bin2cpp --file=<path> --output=<path> --headerfile=<name> --identifier=<name> [--generator=<name>] [--chunksize=<value>] [--override] [--noheader] [--quiet]\n"
    "  bin2cpp --help\n"
    "  bin2cpp --version\n"
    "\n"
    "Options:\n"
    "  --help               Display this help message.\n"
    "  --version            Display this application version.\n"
    "  --file=<path>        Path of the input file used for embedding as a C++ source code.\n"
    "  --output=<path>      Output folder where to create generated code. ie: .\\generated_files\n"
    "  --headerfile=<name>  File name of the generated C++ Header file. ie: SplashScreen.h\n"
    "  --generator=<name>   Name of the generator to use. Possible values are 'segment', 'string' and 'array'. [default: segment].\n"
    "  --identifier=<name>  Identifier of the function name that is used to get an instance of the file. ie: SplashScreen\n"
    "  --chunksize=<value>  Size in bytes of each string segments (bytes per row). [default: 200].\n"
    "  --override           Tells bin2cpp to overwrite the destination files.\n"
    "  --noheader           Do not print program header to standard output.\n"
    "  --quiet              Do not log any message to standard output.\n"
    "\n";
  printf("%s", usage);
}

int main(int argc, char* argv[])
{
  #ifdef ENABLE_BREAKPOINT_DEBUGGING
  MessageBox(NULL, "", "", MB_OK);
  #endif

  //look for help
  std::string dummy;
  if (bin2cpp::parseArgument("help", dummy, argc, argv))
  {
    printHeader();
    printUsage();
    return bin2cpp::ErrorCodes::Success;
  }

  //look for noheader
  bool noheader = false;
  if (bin2cpp::parseArgument("noheader", dummy, argc, argv))
  {
    noheader = true;
  }

  //look for quiet
  bool quiet = false;
  if (bin2cpp::parseArgument("quiet", dummy, argc, argv))
  {
    quiet = true;
  }

  //force noheader if quiet
  if (quiet)
    noheader = true;

  bin2cpp::setQuietMode(quiet);

  //look for version
  if (bin2cpp::parseArgument("version", dummy, argc, argv))
  {
    if (!noheader)
      printHeader();
    return bin2cpp::ErrorCodes::Success;
  }

  if (!noheader && !quiet)
    printHeader();

  //mandatory arguments
  std::string inputFilename;
  std::string outputFolder;
  std::string headerFilename;
  std::string functionIdentifier;

  if (!bin2cpp::parseArgument("file", inputFilename, argc, argv))
  {
    bin2cpp::ErrorCodes error = bin2cpp::ErrorCodes::MissingArguments;
    bin2cpp::log(bin2cpp::LOG_ERROR, "%s (file)", getErrorCodeDescription(error));
    printUsage();
    return error;
  }

  if (!bin2cpp::parseArgument("output", outputFolder, argc, argv))
  {
    bin2cpp::ErrorCodes error = bin2cpp::ErrorCodes::MissingArguments;
    bin2cpp::log(bin2cpp::LOG_ERROR, "%s (output)", getErrorCodeDescription(error));
    printUsage();
    return error;
  }

  if (!bin2cpp::parseArgument("headerfile", headerFilename, argc, argv))
  {
    bin2cpp::ErrorCodes error = bin2cpp::ErrorCodes::MissingArguments;
    bin2cpp::log(bin2cpp::LOG_ERROR, "%s (headerfile)", getErrorCodeDescription(error));
    printUsage();
    return error;
  }

  if (!bin2cpp::parseArgument("identifier", functionIdentifier, argc, argv))
  {
    bin2cpp::ErrorCodes error = bin2cpp::ErrorCodes::MissingArguments;
    bin2cpp::log(bin2cpp::LOG_ERROR, "%s (identifier)", getErrorCodeDescription(error));
    printUsage();
    return error;
  }

  //optional arguments
  static const size_t DEFAULT_CHUNK_SIZE = 200;
  size_t chunkSize = DEFAULT_CHUNK_SIZE;
  bool overrideExisting = false;

  size_t tmpChunkSize = 0;
  if (bin2cpp::parseArgument("chunksize", tmpChunkSize, argc, argv))
  {
    chunkSize = tmpChunkSize;
  }

  if (bin2cpp::parseArgument("override", dummy, argc, argv))
  {
    overrideExisting = true;
  }

  //select generator
  bin2cpp::SegmentGenerator segmentGenerator;
  bin2cpp::StringGenerator stringGenerator;
  bin2cpp::ArrayGenerator arrayGenerator;
  bin2cpp::IGenerator * selectedGenerator = NULL;

  std::string generatorName;
  if (bin2cpp::parseArgument("generator", generatorName, argc, argv))
  {
    if (generatorName == "segment")
    {
      selectedGenerator = &segmentGenerator;
    }
    else if (generatorName == "string")
    {
      selectedGenerator = &stringGenerator;
    }
    else if (generatorName == "array")
    {
      selectedGenerator = &arrayGenerator;
    }

    //validate generator selection
    if (selectedGenerator == NULL)
    {
      bin2cpp::ErrorCodes error = bin2cpp::ErrorCodes::MissingArguments;
      bin2cpp::log(bin2cpp::LOG_ERROR, "%s, unknown values for 'generator' argument!", getErrorCodeDescription(error));
      printUsage();
      return error;
    }
  }

  //apply default generator
  if (selectedGenerator == NULL)
  {
    selectedGenerator = &segmentGenerator;
  }

  // printing info
  std::string chunkInfo = "";
  if (chunkSize != DEFAULT_CHUNK_SIZE)
  {
    std::stringstream ss;
    ss << " using chunks of ";
    ss << chunkSize;
    ss << " bytes";
    chunkInfo.append(ss.str());
  }
  std::string overrideInfo = "";
  if (overrideExisting)
    overrideInfo = " overriding existing files";
  bin2cpp::log(bin2cpp::LOG_INFO, "Embedding \"%s\" into \"%s\"%s%s...", inputFilename.c_str(), headerFilename.c_str(), chunkInfo.c_str(), overrideInfo.c_str());
  bin2cpp::log(bin2cpp::LOG_INFO, "Using '%s' generator...", selectedGenerator->getName());

  //generate header
  bin2cpp::log(bin2cpp::LOG_INFO, "Generating header file...");
  bin2cpp::ErrorCodes headerResult = selectedGenerator->createHeaderEmbededFile(outputFolder.c_str(), headerFilename.c_str(), functionIdentifier.c_str(), overrideExisting);
  if (headerResult == bin2cpp::ErrorCodes::Success)
  {
    bin2cpp::log(bin2cpp::LOG_INFO, "done");
  }
  else if (headerResult == bin2cpp::ErrorCodes::OutputFilesSkipped)
  {
    bin2cpp::log(bin2cpp::LOG_WARNING, "%s", getErrorCodeDescription(headerResult));
  }
  else
  {
    bin2cpp::log(bin2cpp::LOG_ERROR, "%s", getErrorCodeDescription(headerResult));
    bin2cpp::log(bin2cpp::LOG_ERROR, "Embedding failed!");
    return headerResult;
  }

  //generate cpp
  if (!quiet)
  {
    printf("Generating cpp file...\n");
  }
  bin2cpp::ErrorCodes cppResult = selectedGenerator->createCppEmbeddedFile(inputFilename.c_str(), outputFolder.c_str(), headerFilename.c_str(), functionIdentifier.c_str(), chunkSize, overrideExisting);
  if (cppResult == bin2cpp::ErrorCodes::Success)
  {
    bin2cpp::log(bin2cpp::LOG_INFO, "done");
  }
  else if (cppResult == bin2cpp::ErrorCodes::OutputFilesSkipped)
  {
    bin2cpp::log(bin2cpp::LOG_WARNING, "%s", getErrorCodeDescription(cppResult));
  }
  else
  {
    bin2cpp::log(bin2cpp::LOG_ERROR, "%s", getErrorCodeDescription(cppResult));
    bin2cpp::log(bin2cpp::LOG_ERROR, "Embedding failed!");
    return cppResult;
  }
 
  //success
  return 0;
}
