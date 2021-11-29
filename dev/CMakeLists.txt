cmake_minimum_required (VERSION 3.1)
project (BattleShip)

# version number
set (BattleShip_VERSION_MAJOR 0)
set (BattleShip_VERSION_MINOR 0)

# preprocessor flags
option(USE_CONIO "Use Conio to clear the screen" OFF)

if (USE_CONIO)
	add_definitions(-DCONIO=1)
endif ()

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
	BattleShipConfig.h.in
	BattleShipConfig.h
	)

include_directories (battleship/include)
link_directories (battleship)
add_subdirectory (battleship)

if (WIN32)
	include_directories (conio21)
	link_directories (conio21)
	add_subdirectory (conio21)
   #add_subdirectory (pdcurses)
endif (WIN32)

add_executable (BattleShip main.c)
target_link_libraries (BattleShip battleship)
