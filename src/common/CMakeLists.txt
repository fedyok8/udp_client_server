project(common)

file(GLOB COMMON_SOURCES *.cpp *.hpp)

add_library(${PROJECT_NAME} STATIC ${COMMON_SOURCES})
