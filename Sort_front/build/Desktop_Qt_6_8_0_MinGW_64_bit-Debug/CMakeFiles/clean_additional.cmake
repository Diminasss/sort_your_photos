# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Sort_front_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Sort_front_autogen.dir\\ParseCache.txt"
  "Sort_front_autogen"
  )
endif()
