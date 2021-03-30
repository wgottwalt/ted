CMAKE_MINIMUM_REQUIRED  (VERSION 3.0)
PROJECT                 (TEd LANGUAGES CXX)

# packages

# includes and paths/directories
SET                     (CMAKE_INCLUDE_CURRENT_DIR on)
SET                     (CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)
INCLUDE_DIRECTORIES     (${PROJECT_SOURCE_DIR}/sources)
SET                     (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
SET                     (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/plugin)
SET                     (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

# compiler settings
IF                      (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    MESSAGE             ("--> building on Linux")
    SET                 (CMAKE_CXX_FLAGS "-std=c++20 -pthread -flto -fconcepts -fcoroutines -fanalyzer -fPIC -pedantic -W -Wall -Wextra -Os -s")
    SET                 (CMAKE_C_FLAGS "-std=c18 -pthread -flto -fanalyzer -fPIC -pedantic -W -Wall -Wextra -Os -s")
ELSE                    ()
    MESSAGE             (FATAL_ERROR "--> unsupported build host: ${CMAKE_SYSTEM_NAME}")
ENDIF                   ()

# sources
ADD_SUBDIRECTORY        (sources)
