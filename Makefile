all: comp build run clean

comp:
	

build:
	gcc src/*.c -o bin/cgame -lm -lpthread -lrt -lGL -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -I vendor/sokol_gp/thirdparty -I vendor/sokol_gp

run:
	./bin/cgame

clean:
	rm bin/cgame
