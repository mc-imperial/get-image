all:
	g++ -std=c++0x get_image.cpp lodepng.cpp -o get_image -lGL -lglut -lGLEW
