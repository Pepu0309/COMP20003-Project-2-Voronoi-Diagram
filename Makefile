voronoi1: voronoi1.o watchtowers_io.o geometry.o split.o memory_funcs.o # Add other .o files
	gcc -Wall -o voronoi1 voronoi1.o -g watchtowers_io.o geometry.o split.o memory_funcs.o # Add other .o files and libraries

voronoi1-gui: voronoi1-gui.o gtk.o 
	# Add other .o files between voronoi1-gui.o and gtk.o and any libraries.
	gcc `pkg-config --cflags gtk+-3.0` -o voronoi1-gui voronoi1-gui.o watchtowers_io.o geometry.o split.o memory_funcs.o gtk.o `pkg-config --libs gtk+-3.0` -Wall -lm -g `pkg-config --static --libs --cflags igraph`

# Non-GUI object
voronoi1.o: voronoi1.c voronoi1.h
	gcc -Wall -o voronoi1.o -c voronoi1.c -g

# GUI object, runs all ifdef USE_GUI lines.
voronoi1-gui.o: voronoi1.c  geometry.h gtk.h 
	gcc -Wall -o voronoi1-gui.o -c voronoi1.c -g -DUSE_GUI

# Add other targets.
watchtowers_io.o: watchtowers_io.c watchtowers_io.h
	gcc -Wall -o watchtowers_io.o watchtowers_io.c -c -g

geometry.o: geometry.c geometry.h
	gcc -Wall -o geometry.o geometry.c -c -g

split.o: split.c split.h
	gcc -Wall -o split.o split.c -c -g

memory_funcs.o: memory_funcs.c memory_funcs.h
	gcc -Wall -o memory_funcs.o memory_funcs.c -c -g


# GTK helpers, you don't need to change anything in this object.   
gtk.o: gtk.h gtk.c
	gcc -g `pkg-config --cflags gtk+-3.0` -o gtk.o -Wall -c gtk.c `pkg-config --cflags igraph`    
