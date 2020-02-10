project(something CXX)

cmake_minimum_required(VERSION 2.6)

find_package(Curses REQUIRED)
if (CURSES_FOUND)
	include_directories(${CURSES_INCLUDE_DIRS})
	link_libraries(${CURSES_LIBRARIES})
endif()

find_package(Threads REQUIRED)

add_executable(something
	"src/main.cpp"
	"src/include/functions.cpp"
	"src/include/item.cpp"
	"src/include/chest.cpp"
	"src/include/dungeon-generator.hpp"
	"src/include/player.cpp"
)

target_link_libraries(something ${CMAKE_THREAD_LIBS_INIT})
