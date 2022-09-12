cmake_minimum_required(VERSION 3.3)

set(UI_TYPES stdio curses)
set(UI_TYPE stdio CACHE STRING "User Interface Type")
set_property(CACHE UI_TYPE PROPERTY STRINGS ${UI_TYPES})
if (NOT (UI_TYPE IN_LIST UI_TYPES))
   message(FATAL_ERROR "UI_TYPE must be one of ${UI_TYPES}")
endif()

set (SRC_FILES
   "src/ai.c"
   "src/coord.c"
   "src/game.c"
   "src/grid.c"
   "src/input.c"
   "src/player.c"
   "src/rng.c"
   "src/scoreboard.c"
   "src/ship.c"
   "src/ui.c"
   "src/util.c"
)

if (WIN32)
   set(UI_TYPE stdio)
   set (PLATFORM_SRC_FILES
      "src/win32/window.c"
   )
endif (WIN32)

if (UNIX)
   set(UI_TYPE curses)
   set (PLATFORM_SRC_FILES
      "src/linux/window.c"
   )
endif (UNIX)

if (UI_TYPE STREQUAL "stdio")
   set (UI_SRC_FILES
      "src/form_stdio.c"
      "src/menu_stdio.c"
      "src/ui_stdio.c"
   )
elseif (UI_TYPE STREQUAL "curses")
   set (UI_SRC_FILES
      "src/form_curses.c"
      "src/menu_curses.c"
      "src/ui_curses.c"
   )
endif (UI_TYPE)

add_library (battleship STATIC
   ${SRC_FILES} ${UI_SRC_FILES} ${PLATFORM_SRC_FILES})

add_definitions(-Wall -Wextra -Wconversion)

if (WIN32)
   target_link_libraries (battleship conio)
endif (WIN32)

if (UNIX)
   target_link_libraries(battleship form menu ncurses)
endif (UNIX)
