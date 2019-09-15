add_library (battleship STATIC
	coord.c
	game.c
	grid.c
	menu.c
	player.c
	rng.c
	scoreboard.c
	ship.c
	ui.c
	util.c
	window.c
	)
include_directories (../conio21)
link_directories (../conio21)
target_link_libraries (battleship conio)
