file(GLOB_RECURSE internFiles
        "./zoe/src/internal/files/*"
        )

set(targetFile ./zoe/src/zoe/core/VirtualFiles.cpp)

file(WRITE ${targetFile} "#include \"File.h\"\n#include \"VirtualFiles.h\"\nvoid Zoe::initVirtualFiles(){")

foreach(file IN LISTS internFiles)
    file(RELATIVE_PATH fileName ${CMAKE_CURRENT_SOURCE_DIR}/zoe/src/internal/files/ ${file})
    file(READ ${file} content HEX)
    string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1u,"
            regexedContent ${content})
    string(LENGTH ${content} contentLength)
    file(APPEND ${targetFile} "\n    {\n        File f(\"zoe/internal/${fileName}\");\n        const uint8_t data[] {${regexedContent}};\n        f.getIOStream().write((const char*)data, ${contentLength}/2);\n    }")
endforeach()

file(APPEND ${targetFile} "\n}")