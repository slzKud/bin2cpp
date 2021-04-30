![bin2cpp logo](https://github.com/end2endzone/bin2cpp/raw/master/docs/bin2cpp-splashscreen.png)


# bin2cpp #
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Github Releases](https://img.shields.io/github/release/end2endzone/bin2cpp.svg)](https://github.com/end2endzone/bin2cpp/releases)

bin2cpp is a command line tool for embedding small files (like images, icons or raw data files) into a C++ executable.


## Status ##

Build:

| Service/Platform    | Build                                                                                                                                                                                   | Tests                                                                                                                                                                                                                                                  |
|---------------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| AppVeyor            | [![Build status](https://img.shields.io/appveyor/ci/end2endzone/bin2cpp/master.svg?logo=AppVeyor&logoColor=white)](https://ci.appveyor.com/project/end2endzone/bin2cpp)                 | [![Tests status](https://img.shields.io/appveyor/tests/end2endzone/bin2cpp/master.svg?logo=AppVeyor&logoColor=white)](https://ci.appveyor.com/project/end2endzone/bin2cpp/branch/master/tests)                                                         |
| Travis CI           | [![Build Status](https://img.shields.io/travis/end2endzone/bin2cpp/master.svg?logo=Travis-CI&style=flat&logoColor=white)](https://travis-ci.org/end2endzone/bin2cpp)                    |                                                                                                                                                                                                                                                        |
| Windows Server 2019 | [![Build on Windows](https://github.com/end2endzone/bin2cpp/actions/workflows/build_windows.yml/badge.svg)](https://github.com/end2endzone/bin2cpp/actions/workflows/build_windows.yml) | [![Tests on Windows](https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/end2endzone/58cf6c72c08e706335337d5ef9ca48e8/raw/bin2cpp.master.Windows.json)](https://github.com/end2endzone/bin2cpp/actions/workflows/build_windows.yml) |
| Ubuntu 20.04        | [![Build on Linux](https://github.com/end2endzone/bin2cpp/actions/workflows/build_linux.yml/badge.svg)](https://github.com/end2endzone/bin2cpp/actions/workflows/build_linux.yml)       | [![Tests on Linux](https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/end2endzone/58cf6c72c08e706335337d5ef9ca48e8/raw/bin2cpp.master.Linux.json)](https://github.com/end2endzone/bin2cpp/actions/workflows/build_linux.yml)       |
| macOS 10.15         | [![Build on macOS](https://github.com/end2endzone/bin2cpp/actions/workflows/build_macos.yml/badge.svg)](https://github.com/end2endzone/bin2cpp/actions/workflows/build_macos.yml)       | [![Tests on macOS](https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/end2endzone/58cf6c72c08e706335337d5ef9ca48e8/raw/bin2cpp.master.macOS.json)](https://github.com/end2endzone/bin2cpp/actions/workflows/build_macos.yml)       |

Statistics:

| AppVeyor                                                                                                                                       | Travic CI                                                                                                              | GitHub                                                                                                                     |
|------------------------------------------------------------------------------------------------------------------------------------------------|------------------------------------------------------------------------------------------------------------------------|----------------------------------------------------------------------------------------------------------------------------|
| [![Statistics](https://buildstats.info/appveyor/chart/end2endzone/bin2cpp)](https://ci.appveyor.com/project/end2endzone/bin2cpp/branch/master) | [![Statistics](https://buildstats.info/travisci/chart/end2endzone/bin2cpp)](https://travis-ci.org/end2endzone/bin2cpp) | [![Statistics](https://buildstats.info/github/chart/end2endzone/bin2cpp)](https://github.com/end2endzone/bin2cpp/actions/) |




# Purpose #

bin2cpp is not implemented using [executable resources](http://en.wikipedia.org/wiki/Resource_(Windows)). Instead, bin2cpp creates a single function call for retrieving the content and the properties of a file which makes it harder to steal the executable's content. It also makes it even harder to replace an existing resource of the executable.

It is designed to be easy to use by developers and to provide easy call functions to get access to the data of the embedded files.

The generated functions that reads and extracts the embedded content does not rely on external libraries so you don't need to setup your projects to use any third party library to start using bin2cpp. All your embedded data can be accessed right away.




# Features #

The main features of the project are:

* Easily converts small files as C++ source code for embedding into a C++ executable.
* Access content with a unique function call for each embedded file.
* Supports multiple embedded files at once.
* Makes it harder for resource hacker to modify or steal the embedded files.
* No third party libraries required for retrieving the data of the embedded files.
* Supports different types of code generator: string, segment, array.
* File's originals `size` and `filename` properties available from generated source code.
* Source code control: select a custom file interface and namespace.


## Use cases ##

The following list show situations where bin2cpp is useful:

* Embedding default configuration files if none are provided.
* Embedding GLSL shaders into the executable.
* Embedding text or XML databases (gui layout, names, dictionaries or localization strings)
* A portable alternative to C++11 raw string literals.
* Prevent stealing copyrighted resources.
* Embedding images/icons/sounds for a GUI application. The application executable can be shipped/copied as a single file. 
* Embedding a long PL/SQL code string.
* Allowing an executable to be downloaded from an intranet server as a single file.
* Distributing an application without an installer package. All configurations files and resources can be embedded and extracted at first launch of the application.




# Usage #

The following section shows how to use bin2cpp with code examples:



## Command Line Usage ##

```
bin2cpp --file=<path> --output=<path> --headerfile=<name> --identifier=<name>
        [--generator=<name>] [--encoding=<name>] [--chunksize=<value>] [--namespace=<value>]
        [--baseclass=<value>] [--override] [--noheader] [--quiet]
bin2cpp --help
bin2cpp --version
```

| Argument        | Description                                                                                                                                                                                                               |
|-----------------|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| --help          | Display this help message.                                                                                                                                                                                                |
| --version       | Display this application version.                                                                                                                                                                                         |
| --file=\        | Path of the input file used for embedding as C++ source code.                                                                                                                                                             |
| --dir=\         | Path of the input directory used for embedding all files of the directory as C++ source code. When specified, the parameters 'headerfile' and 'identifier' are automatically calculated and cannot be manually specified. |
| --output=\      | Output folder where to create generated code. ie: .\generated_files                                                                                                                                                       |
| --headerfile=\  | File name of the generated C++ Header file. ie: SplashScreen.h                                                                                                                                                            |
| --generator=\   | Name of the generator to use. Possible values are 'segment', 'string', 'array' and 'win32'. [default: segment].                                                                                                           |
| --encoding=\    | Name of the binary to string literal encoding to use. Possible values are 'oct' and 'hex'. [default: oct].                                                                                                                |
| --identifier=\  | Identifier of the function name that is used to get an instance of the file. ie: SplashScreen                                                                                                                             |
| --chunksize=\   | Size in bytes of each string segments (bytes per row). [default: 200].                                                                                                                                                    |
| --baseclass=\   | The name of the interface for embedded files. [default: File].                                                                                                                                                            |
| --namespace=\   | The namespace of the generated source code [default: bin2cpp].                                                                                                                                                            |
| --managerfile=\ | File name of the generated C++ header file for the FileManager class. ie: FileManager.h.                                                                                                                                  |
| --registerfile  | Register the generated file to the FileManager class. This flags is automatically set when parameter 'managerfile' is specified. [default: false].                                                                        |
| --override      | Tells bin2cpp to overwrite the destination files.                                                                                                                                                                         |
| --noheader      | Do not print program header to standard output.                                                                                                                                                                           |
| --quiet         | Do not log any message to standard output.                                                                                                                                                                                |

## Example ##



### Input File: ###
html5skeleton.html
```html
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=yes">
  <title>Hello World!</title>
</head>
<body>
Hello World!
</body>
</html>
```



### Command: ###
```batchfile
bin2cpp.exe --file=html5skeleton.html --output=.\outdir --headerfile=file_html5skeleton.h
            --identifier=HtmlSample --chunksize=50
```



### Console output ###

```batchfile
bin2cpp v2.2.0 - Convert binary files into C++ source code.
Copyright (C) 2013-2019 end2endzone.com. All rights reserved.
bin2cpp is open source software, see http://github.com/end2endzone/bin2cpp
Embedding "html5skeleton.html" using chunks of 50 bytes...
Writing file ".\outdir\file_html5skeleton.h"...
Writing file ".\outdir\file_html5skeleton.cpp"...
```



### File file_html5skeleton.h ###

```cpp
/**
 * This file was generated by bin2cpp v2.2.0
 * Copyright (C) 2013-2019 end2endzone.com. All rights reserved.
 * bin2cpp is open source software, see http://github.com/end2endzone/bin2cpp
 * Source code for file 'html5skeleton.html', last modified 1513117337.
 * Do not modify this file.
 */
#ifndef FILE_HTML5SKELETON_H
#define FILE_HTML5SKELETON_H

#include <stddef.h>

namespace bin2cpp
{
  #ifndef BIN2CPP_EMBEDDEDFILE_CLASS
  #define BIN2CPP_EMBEDDEDFILE_CLASS
  class File
  {
  public:
    virtual size_t getSize() const = 0;
    virtual const char * getFilename() const = 0;
    virtual const char * getBuffer() const = 0;
    virtual bool save(const char * iFilename) const = 0;
  };
  #endif
  const File & getHtmlSampleFile();
}; //bin2cpp

#endif //FILE_HTML5SKELETON_H
```

### File file_html5skeleton.cpp ###

```cpp
/**
 * This file was generated by bin2cpp v2.2.0
 * Copyright (C) 2013-2019 end2endzone.com. All rights reserved.
 * bin2cpp is open source software, see http://github.com/end2endzone/bin2cpp
 * Source code for file 'html5skeleton.html', last modified 1513117337.
 * Do not modify this file.
 */
#include "file_html5skeleton.h"
#include <stdio.h> //for FILE
#include <string> //for memcpy
namespace bin2cpp
{
  class HtmlSampleFile : public virtual bin2cpp::File
  {
  public:
    HtmlSampleFile() { build(); }
    virtual ~HtmlSampleFile() {}
    virtual size_t getSize() const { return 238; }
    virtual const char * getFilename() const { return "html5skeleton.html"; }
    virtual const char * getBuffer() const { return mBuffer.c_str(); }
    void build()
    {
      mBuffer.clear();
      mBuffer.reserve(getSize()); //allocate all required memory at once to prevent reallocations
      mBuffer.append("<!DOCTYPE html>\r\n<html lang=\"en\">\r\n<head>\r\n  <meta", 50);
      mBuffer.append(" charset=\"utf-8\">\r\n  <meta name=\"viewport\" content", 50);
      mBuffer.append("=\"width=device-width, initial-scale=1, user-scalab", 50);
      mBuffer.append("le=yes\">\r\n  <title>Hello World!</title>\r\n</head>\r\n", 50);
      mBuffer.append("<body>\r\nHello World!\r\n</body>\r\n</html>", 38);
    }
    virtual bool save(const char * iFilename) const
    {
      FILE * f = fopen(iFilename, "wb");
      if (!f) return false;
      size_t fileSize = getSize();
      const char * buffer = getBuffer();
      fwrite(buffer, 1, fileSize, f);
      fclose(f);
      return true;
    }
  private:
    std::string mBuffer;
  };
  const File & getHtmlSampleFile() { static HtmlSampleFile _instance; return _instance; }
}; //bin2cpp
```

### Code sample (using embedded files) ###
```cpp
#include <stdio.h>
#include <string>
#include "file_html5skeleton.h" //a single include file is all you need

int main(int argc, char* argv[])
{
  //get a reference to the embedded file
  const bin2cpp::File & resource = bin2cpp::getHtmlSampleFile();

  //print information about the file.
  printf("Embedded file '%s' is %lu bytes long.\n", resource.getFilename(), resource.getSize());

  //Saving content back to a file.
  printf("Saving embedded file to 'html5skeleton_copy.html'...\n");
  bool saved = resource.save("html5skeleton_copy.html");
  if (saved)
    printf("saved\n");
  else
    printf("failed\n");

  //Get the internal buffer and do something with the binary data
  const char * buffer = resource.getBuffer();
  size_t bufferSize = resource.getSize();
  //...
  
  return 0;
}
```



## Screenshots ##

[![bin2cpp v2.0.0 Sample](https://raw.githubusercontent.com/end2endzone/bin2cpp/master/docs/bin2cpp-v2.0.0-sample.png)](https://raw.githubusercontent.com/end2endzone/bin2cpp/master/docs/bin2cpp-v2.0.0-sample.png)

bin2cpp v2.0.0 Sample




# Build #

Please refer to file [INSTALL.md](INSTALL.md) for details on how installing/building the application.




# Platform #

bin2cpp has been tested with the following platform:

* Linux, 32 and 64 bit
* Windows, 32 and 64 bit
* macOS, 32 and 64 bit



# Versioning #

We use [Semantic Versioning 2.0.0](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/end2endzone/bin2cpp/tags).




# Authors #

* **Antoine Beauchamp** - *Initial work* - [end2endzone](https://github.com/end2endzone)

See also the list of [contributors](https://github.com/end2endzone/bin2cpp/blob/master/AUTHORS) who participated in this project.




# License #

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details

