all:
	g++ generator.cpp -o generator -Wno-deprecated -L/opt/X11/lib -lglut -lGLU -lGL -lGLEW -std=c++11
	g++ engine.cpp -o engine -Wno-deprecated -L/opt/X11/lib -lglut -lGLU -lGL -lGLEW -std=c++11

gera:
	./generator Sphere 70 100 100 sun
	./generator Sphere 1 20 20 mercury
	./generator Sphere 4 20 20 venus
	./generator Sphere 5 20 20 earth
	./generator Sphere 3 20 20 mars
	./generator Sphere 1 20 20 marsmoons
	./generator Sphere 2 20 20 lua
	./generator Sphere 30 100 100 jupiter
	./generator Sphere 1 20 20 jupitermoons
	./generator Sphere 20 100 100 saturn
	./generator Sphere 12 100 100 uranus
	./generator Sphere 10 100 100 neptune
	./generator Torus 3 28 100 50 ring
	./generator Sphere 1 10 10  sphere
	./generator Torus 3 3 3 5 torus
	python3 Patch_Beizier.py 40 teapot.patch teapot.3d

engi:
	./engine config.xml

tt:
	./engine t.xml
