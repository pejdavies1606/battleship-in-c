set (SRC_FILES
   "coord.c"
   "game.c"
   "grid.c"
   "input.c"
   "menu.c"
   "player.c"
   "rng.c"
   "scoreboard.c"
   "ship.c"
   "ui.c"
   "util.c"
   "window.c"
)
add_library (battleship STATIC ${SRC_FILES})
add_definitions(-Wall -Wextra -Wconversion)
if (WIN32)
   target_link_libraries (battleship conio)
endif (WIN32)

