all: comp build run clean

comp:
	

build:
	gcc src/*.c -o bin/cgame -lm -lpthread -lrt -lGL -lglfw -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -I vendor/sokol -I vendor/cimgui

run:
	./bin/cgame

clean:
	rm bin/cgame
