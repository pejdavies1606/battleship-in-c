cmake_minimum_required (VERSION 3.1)
project (BattleShip)

# version number
set (BattleShip_VERSION_MAJOR 0)
set (BattleShip_VERSION_MINOR 0)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
	"${PROJECT_SOURCE_DIR}/BattleShipConfig.h.in"
	"${PROJECT_BINARY_DIR}/BattleShipConfig.h"
	)

# add the binary tree to the search path for include files
# so that CMake will find the header file
include_directories ("${PROJECT_BINARY_DIR}")

include_directories ("${PROJECT_SOURCE_DIR}/battleship")
add_subdirectory (battleship)
set (LIBS ${LIBS} battleship)

# add the executable
add_executable(BattleShip main.c)
target_link_libraries (BattleShip ${LIBS})
