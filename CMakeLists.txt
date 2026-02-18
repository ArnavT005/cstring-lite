cmake_minimum_required(VERSION 3.24)
project(cstring-lite)

if (NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (${TULI_ENV_BUILD_TESTS})
  add_subdirectory(tests)
elseif (${TULI_ENV_BUILD_EXAMPLES})
  add_subdirectory(examples)
endif()