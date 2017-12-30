#include "ArrayGenerator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <stdlib.h>
#include <sstream>

#include "common.h"
#include "md5support.h"
#include "md5.h"
#include "md5Ex.h"

namespace bin2cpp
{
  namespace ArrayGeneratorUtils
  {
    std::string getGetterFunctionName(const char * iFunctionIdentifier)
    {
      std::string getter;
      getter.append("get");
      getter.append(iFunctionIdentifier);
      getter.append("File");
      return getter;
    }

    bool isPrintableCharacter(char c)
    {
      if (c == 39) // character ' must be escaped with \' which is not supported right now
        return false;
      if (c == 92) // character \ must be escaped with \\ which is not supported right now
        return false;
      if (c >= 32 && c<= 126)
        return true;
      return false;
    }

    std::string toCppString(const unsigned char * iBuffer, size_t iSize)
    {
      std::ostringstream oss;

      for(size_t i=0; i<iSize; i++)
      {
        unsigned char c = iBuffer[i];

        if (isPrintableCharacter((char)c))
          oss << '\'' << (char)c << '\'';
        else
          oss << (int)c; //print as decimal value

        size_t lastByteIndex = iSize-1;

        if (i != lastByteIndex)
          oss << ",";
      }

      return oss.str();
    }

    bin2cpp::ErrorCodes createHeaderEmbededFile(const char * iOutputHeaderFilename, const char * iFunctionIdentifier)
    {
      FILE * header = fopen(iOutputHeaderFilename, "w");
      if (!header)
        return bin2cpp::ErrorCodes::UnableToCreateOutputFiles;

      fprintf(header, "/**\n");
      fprintf(header, " * This file was generated by bin2cpp v%s.\n", getVersionString() );
      fprintf(header, " * Copyright (C) 2013-%d end2endzone.com. All rights reserved.\n", bin2cpp::getCopyrightYear());
      fprintf(header, " * Do not modify this file.\n");
      fprintf(header, " */\n");
      fprintf(header, "#pragma once\n");
      fprintf(header, "#include <stddef.h>\n");
      fprintf(header, "namespace bin2cpp\n");
      fprintf(header, "{\n");
      fprintf(header, "  #ifndef BIN2CPP_EMBEDDEDFILE_CLASS\n");
      fprintf(header, "  #define BIN2CPP_EMBEDDEDFILE_CLASS\n");
      fprintf(header, "  class File\n");
      fprintf(header, "  {\n");
      fprintf(header, "  public:\n");
      fprintf(header, "    virtual size_t getSize() const = 0;\n");
      fprintf(header, "    virtual const char * getFilename() const = 0;\n");
      fprintf(header, "    virtual const char * getBuffer() const = 0;\n");
      fprintf(header, "    virtual const char * getMd5() const = 0;\n");
      fprintf(header, "    virtual bool save(const char * iFilename) const = 0;\n");
      fprintf(header, "  };\n");
      fprintf(header, "  #endif\n");
      fprintf(header, "  const File & %s();\n", getGetterFunctionName(iFunctionIdentifier).c_str());
      fprintf(header, "}; //bin2cpp\n");

      fclose(header);

      return bin2cpp::ErrorCodes::Success;
    }

  }; //ArrayGeneratorUtils

  ArrayGenerator::ArrayGenerator()
  {
  }

  ArrayGenerator::~ArrayGenerator()
  {
  }

  bin2cpp::ErrorCodes ArrayGenerator::createCppEmbeddedFile(const char * iInputFilename, const char * iOutputFolder, const char * iHeaderFilename, const char * iFunctionIdentifier, size_t iChunkSize, bool iOverrideExisting)
  {
    //check if input file exists
    FILE * input = fopen(iInputFilename, "rb");
    if (!input)
      return bin2cpp::ErrorCodes::InputFileNotFound;

    //Uppercase function identifier
    std::string functionIdentifier = iFunctionIdentifier;
    functionIdentifier[0] = toupper(functionIdentifier[0]);

    //Build header file path
    std::string headerPath;
    headerPath.append(iOutputFolder);
    if (headerPath[headerPath.length()-1] != '\\')
      headerPath.append("\\");
    headerPath.append(iHeaderFilename);

    //Build cpp file file
    std::string outputCppFilename = headerPath;
    strReplace(outputCppFilename, ".h", ".cpp");

    //check if output files already exists
    if (fileExists(headerPath.c_str()) || fileExists(outputCppFilename.c_str()))
    {
      //compute input/output file md5
      std::string inputFileHexDigest = getFileHexDigest(iInputFilename);
      std::string sourceFileHexDigest = getGeneratedFileHexDigest(outputCppFilename.c_str());
      if (inputFileHexDigest == sourceFileHexDigest)
      {
        //md5 identical
        return bin2cpp::ErrorCodes::OutputFilesSkipped;
      }

      //md5 not identical
      if (!iOverrideExisting)
        return bin2cpp::ErrorCodes::OutputFilesAlreadyExist;
    }

    //create Header file
    bin2cpp::ErrorCodes headerResult = bin2cpp::ArrayGeneratorUtils::createHeaderEmbededFile(headerPath.c_str(), functionIdentifier.c_str());
    if (headerResult != bin2cpp::ErrorCodes::Success)
    {
      fclose(input);
      return headerResult;
    }

    //create cpp file
    FILE * cpp = fopen(outputCppFilename.c_str(), "w");
    if (!cpp)
    {
      fclose(input);
      return bin2cpp::ErrorCodes::UnableToCreateOutputFiles;
    }

    //determine segment size
    long fileSize = getFileSize(input);
    long lastSegmentSize = fileSize%iChunkSize;
    size_t numSegments = fileSize/iChunkSize + (lastSegmentSize == 0 ? 0 : 1);

    //Build class name
    std::string className;
    className.append(functionIdentifier.c_str());
    className.append("File");

    //Build function 
    std::string getterFunctionName = bin2cpp::ArrayGeneratorUtils::getGetterFunctionName(functionIdentifier.c_str());

    //write cpp file heading
    fprintf(cpp, "/**\n");
    fprintf(cpp, " * This file was generated by bin2cpp v%s.\n", getVersionString() );
    fprintf(cpp, " * Copyright (C) 2013-%d end2endzone.com. All rights reserved.\n", bin2cpp::getCopyrightYear());
    fprintf(cpp, " * Do not modify this file.\n");
    fprintf(cpp, " */\n");
    fprintf(cpp, "#include \"%s\"\n", iHeaderFilename );
    fprintf(cpp, "#include <stdio.h> //for FILE\n");
    fprintf(cpp, "#include <string> //for memcpy\n");
    fprintf(cpp, "namespace bin2cpp\n");
    fprintf(cpp, "{\n");
    fprintf(cpp, "  class %s : public virtual bin2cpp::File\n", className.c_str());
    fprintf(cpp, "  {\n");
    fprintf(cpp, "  public:\n");
    fprintf(cpp, "    %s() {}\n", className.c_str());
    fprintf(cpp, "    ~%s() {}\n", className.c_str());
    fprintf(cpp, "    virtual size_t getSize() const { return %d; }\n", fileSize);
    fprintf(cpp, "    virtual const char * getFilename() const { return \"%s\"; }\n", getFilename(iInputFilename).c_str());
    fprintf(cpp, "    virtual const char * getBuffer() const\n");
    fprintf(cpp, "    {\n");
    fprintf(cpp, "      static const unsigned char buffer[] = {\n");

    //Compute MD5 while generating cpp code
    MD5_CTX context;
    MD5Init(&context);

    //create buffer for each chunks from input buffer
    int numLinePrinted = 0;
    unsigned char * buffer = new unsigned char[iChunkSize];
    while(!feof(input))
    {
      //read a chunk of the file
      size_t readSize = fread(buffer, 1, iChunkSize, input);

      bool isLastChunk = !(readSize == iChunkSize);

      if (readSize > 0)
      {
        if (numLinePrinted > 0)
        {
          //end previous line
          fprintf(cpp, ",\n");
        }

        //send to MD5 for analysist
        MD5Update(&context, buffer, readSize);

        //output
        fprintf(cpp, "        %s", bin2cpp::ArrayGeneratorUtils::toCppString(buffer, readSize).c_str());
        numLinePrinted++;
      }

      //end the array. all the file content is printed
      if (isLastChunk)
      {
        fprintf(cpp, "\n");
        fprintf(cpp, "      };\n");
      }
    }
    delete[] buffer;
    buffer = NULL;

    //compute final digest
    MD5DIGEST digest;
    MD5Final(digest.bytes, &context);
    std::string md5String = toString(digest);

    //write cpp file footer
    fprintf(cpp, "      return (const char *)buffer;\n");
    fprintf(cpp, "    }\n");
    fprintf(cpp, "    virtual const char * getMd5() const { return \"%s\"; }\n", md5String.c_str() );
    fprintf(cpp, "    virtual bool save(const char * iFilename) const\n");
    fprintf(cpp, "    {\n");
    fprintf(cpp, "      FILE * f = fopen(iFilename, \"wb\");\n");
    fprintf(cpp, "      if (!f) return false;\n");
    fprintf(cpp, "      size_t fileSize = getSize();\n");
    fprintf(cpp, "      const char * buffer = getBuffer();\n");
    fprintf(cpp, "      fwrite(buffer, 1, fileSize, f);\n");
    fprintf(cpp, "      fclose(f);\n");
    fprintf(cpp, "      return true;\n");
    fprintf(cpp, "    }\n");
    fprintf(cpp, "  };\n");
    fprintf(cpp, "  const File & %s() { static %s _instance; return _instance; }\n", getterFunctionName.c_str(), className.c_str());
    fprintf(cpp, "}; //bin2cpp\n");

    fclose(input);

    return bin2cpp::ErrorCodes::Success;
  }

}; //bin2cpp