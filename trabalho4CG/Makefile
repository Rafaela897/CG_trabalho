all:
	g++ generator.cpp -o generator -Wno-deprecated -L/opt/X11/lib -lglut -lGLU -lGL -lGLEW -lIL -std=c++11
	g++ engine.cpp -o engine -Wno-deprecated -L/opt/X11/lib -lglut -lGLU -lGL -lGLEW -lIL -std=c++11

gera:
	./generator Sphere 70 100 100 sun 
	./generator Sphere 1 10 10 mercury
	./generator Sphere 4 15 15 venus
	./generator Sphere 5 20 20 earth
	./generator Sphere 3 15 25 mars
	./generator Sphere 1 10 10 marsmoons
	./generator Sphere 2 10 10 lua
	./generator Sphere 30 70 70 jupiter
	./generator Sphere 1 10 10 jupitermoons
	./generator Sphere 20 60 60 saturn
	./generator Sphere 12 40 40 uranus
	./generator Sphere 10 30 30 neptune
	./generator Torus 3 29 70 50 ring
	./generator Sphere 1 10 10  sphere
	./generator Torus 3 3 3 5 torus
	

engi:
	./engine config.xml

tt:
	./engine teste1.xml