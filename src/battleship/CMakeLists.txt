file(GLOB SRC_FILES "*.c")
add_library (battleship STATIC ${SRC_FILES})
add_definitions(-Wall -Wextra -Wconversion)
target_link_libraries (battleship conio)
