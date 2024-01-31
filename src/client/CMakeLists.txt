project(client)

file(GLOB CLIENT_SOURCES *.cpp *.hpp)

add_executable(${PROJECT_NAME} ${CLIENT_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ../)
target_link_libraries(${PROJECT_NAME} common)

install(TARGETS ${PROJECT_NAME})

