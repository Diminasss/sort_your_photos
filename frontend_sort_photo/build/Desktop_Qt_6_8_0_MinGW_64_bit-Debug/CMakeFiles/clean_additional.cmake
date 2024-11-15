# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\frontend_sort_photo_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\frontend_sort_photo_autogen.dir\\ParseCache.txt"
  "frontend_sort_photo_autogen"
  )
endif()
