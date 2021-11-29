set (SRC_FILES
   "src/coord.c"
   "src/game.c"
   "src/grid.c"
   "src/input.c"
   "src/menu.c"
   "src/player.c"
   "src/rng.c"
   "src/scoreboard.c"
   "src/ship.c"
   "src/ui.c"
   "src/ui_stdio.c"
   "src/util.c"
   "src/window.c"
)
add_library (battleship STATIC ${SRC_FILES})
add_definitions(-Wall -Wextra -Wconversion)
if (WIN32)
   target_link_libraries (battleship conio)
endif (WIN32)
if (UNIX)
   target_link_libraries(battleship ncurses)
endif (UNIX)
