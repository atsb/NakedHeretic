cmake_minimum_required(VERSION 3.15)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
project("Heretic")

# Any settings that should apply to all targets in this directory and all
# subdirectories should go here.  Use judiciously.
if(MSVC)
    add_definitions("/D_CRT_SECURE_NO_WARNINGS" "/D_CRT_SECURE_NO_DEPRECATE"
                    "/D_CRT_NONSTDC_NO_DEPRECATE")
else()
    add_compile_options("-Wall" "-Wdeclaration-after-statement"
                        "-Wredundant-decls")
endif()

find_package(SDL2 2.0.7)
find_package(SDL2_mixer 2.0.2)
find_package(m)

if( NOT WIN32 OR NOT MSVC )
add_compile_definitions(doom UNIX=1)
else( WIN32 OR MSVC )
add_compile_definitions(doom WINDOWS=1)
endif( NOT WIN32 OR NOT MSVC )

include(CheckSymbolExists)
include(CheckIncludeFile)
check_symbol_exists(strcasecmp "strings.h" HAVE_DECL_STRCASECMP)
check_symbol_exists(strncasecmp "strings.h" HAVE_DECL_STRNCASECMP)
check_include_file("dirent.h" HAVE_DIRENT_H)

# Without a hyphen. This is used for the bash-completion scripts.
string(TOLOWER "heretic" PROGRAM_SPREFIX)

# With a hyphen, used almost everywhere else.
set(PROGRAM_PREFIX "heretic")

foreach(SUBDIR src)
    add_subdirectory("${SUBDIR}")
endforeach()
