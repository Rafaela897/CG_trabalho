all:
	g++ generator.cpp -o generator -Wno-deprecated -L/opt/X11/lib -lglut -lGLU -lGL -std=c++11
	g++ engine.cpp -o engine -Wno-deprecated -L/opt/X11/lib -lglut -lGLU -lGL -std=c++11

exe:
	./engine config.XML
