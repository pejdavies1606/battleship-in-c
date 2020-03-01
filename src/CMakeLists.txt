cmake_minimum_required (VERSION 3.1)
project (BattleShip)

# version number
set (BattleShip_VERSION_MAJOR 0)
set (BattleShip_VERSION_MINOR 0)

# preprocessor flags
option(FOR_MSDOS "Use Conio to clear the screen" OFF)

if (FOR_MSDOS)
	add_definitions(-DMSDOS=1)
else ()
	add_definitions(-DMSDOS=0)
endif ()

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
	BattleShipConfig.h.in
	BattleShipConfig.h
	)

include_directories (battleship conio21)
link_directories (battleship conio21)
add_subdirectory (battleship)
add_subdirectory (conio21)

add_executable (BattleShip main.c)
target_link_libraries (BattleShip battleship)
