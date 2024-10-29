# CMakeList.txt : CMake project for FloatVarContainer, include source and define
# project specific logic here.
#

# Collect source and header files
file(GLOB_RECURSE INCLUDES include/*.h)
file(GLOB_RECURSE SOURCES src/*.cpp)


# Create executable
add_library(FloatVarContainer ${SOURCES})

# Add include directories (derived from header file locations)
foreach(dir ${INCLUDES})
    get_filename_component(HEADER_DIR ${dir} DIRECTORY)
    list(APPEND INCLUDE_DIRS ${HEADER_DIR})
endforeach()

list(REMOVE_DUPLICATES INCLUDE_DIRS)
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})

# Check for advanced versions
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET FloatVarContainer PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
