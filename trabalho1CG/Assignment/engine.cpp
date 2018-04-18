#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>
#include <string.h>
#include <vector>
#include <sstream>
#include <math.h>
#include <iostream>
#include <fstream>
#include "tinyxml2.h"
#include "tinyxml2.cpp"

using namespace tinyxml2;
using namespace std;


vector<float> xs,ys,zs;

float alfa = 0.0f, beta = 0.0f, radius = 5.0f;
float camX, camY, camZ;

void spherical2Cartesian() {

	camX = radius * cos(beta) * sin(alfa);
	camY = radius * sin(beta);
	camZ = radius * cos(beta) * cos(alfa);
}

/*
* Lê as coordenadas do ficheiro passado e adiciona aos vetores das coordenadas.
*/ 
void readCoord(string fileName) 
{
	string buffer;
	ifstream myfile;
	myfile.open(fileName);
	if(!myfile.is_open())
	{
		cerr << "Couldn't open file.";
		exit(EXIT_FAILURE);
	}

	while(getline(myfile,buffer))
	{
		float x,y,z;
		istringstream iss(buffer);
		
		iss >> x >> y >> z;
		xs.push_back(x);
		ys.push_back(y);
		zs.push_back(z);
	}
}


void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


void renderScene(void) {
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();

	gluLookAt(camX, camY, camZ,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);

	// DRAW
	int tam=xs.size();
	glBegin(GL_TRIANGLES);
		glColor3f(1, 1, 1);
		for(int i=0; i<tam; i++)
			glVertex3f(xs[i],ys[i],zs[i]);
	glEnd();

	glutSwapBuffers();
}


void Regularkey(unsigned char key, int a, int b)
{
	switch (key) {		
		case 'i':
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		}
		case 'k':
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		}
		case 'm':
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			break;
		}
	}

	glutPostRedisplay();
}


void processSpecialKeys(int key, int xx, int yy) {

	switch (key) {

	case GLUT_KEY_RIGHT:
		alfa -= 0.1; break;

	case GLUT_KEY_LEFT:
		alfa += 0.1; break;

	case GLUT_KEY_UP:
		beta += 0.1f;
		if (beta > 1.5f)
			beta = 1.5f;
		break;

	case GLUT_KEY_DOWN:
		beta -= 0.1f;
		if (beta < -1.5f)
			beta = -1.5f;
		break;

	case GLUT_KEY_PAGE_DOWN: radius -= 0.1f;
		if (radius < 0.1f)
			radius = 0.1f;
		break;

	case GLUT_KEY_PAGE_UP: radius += 0.1f; break;
	}
	spherical2Cartesian();
	glutPostRedisplay();
}



int main(int argc, char **argv)
{
	string fileName;
	int i=0;

  	// TINYXML
	const char * attributeText = nullptr;
	XMLDocument xmlDoc;
	xmlDoc.LoadFile(argv[1]);

	// apontador para a raíz
	// Se não encontrar raíz, FirstChild() retorna nullptr
	XMLNode * pRoot = xmlDoc.FirstChild();
	if (pRoot == nullptr)
	{
		cout << "No Root found.";
		return XML_ERROR_FILE_READ_ERROR;
	}
	
	// procura o elemento "model"
	XMLElement * pElement = pRoot->FirstChildElement("model");
	if (pElement == nullptr)
	{
		cout << "Element error.";
		return XML_ERROR_PARSING_ELEMENT;
	}

	// Vai buscar texto do atributo "file"
	while(pElement != nullptr)
	{
		attributeText = pElement->Attribute("file");
		if (attributeText == nullptr)
		{
			cout << "Atribute error.";
			return XML_ERROR_PARSING_ATTRIBUTE;
		}
		fileName = attributeText;
		pElement = pElement->NextSiblingElement(); // proximo "file". Estão à mesma altura na árvore, logo são "irmãos".

		readCoord(fileName);
	}

	// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(600,500);
	glutCreateWindow("Assignment");

// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

// Callback registration for keyboard processing
	glutKeyboardFunc(Regularkey);
	glutSpecialFunc(processSpecialKeys);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();

	return 1;
}
