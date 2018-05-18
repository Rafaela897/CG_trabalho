#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>
#include <string.h>
#include <vector>
#include <map>
#include <tuple>
#include <sstream>
#include <math.h>
#include <iostream>
#include <fstream>
#include "tinyxml2.h"
#include "tinyxml2.cpp"

int timebase = 0, frame = 0;

using namespace tinyxml2;
using namespace std;

/*
* drawUntil: Utilizado para saber quantos ficheiros podemos desenhar de uma vez, caso haja v�rios no mesmo block.
* xx,yy,zz: valores para as transforma��es;
* angle: para o rotate;
* type:
* 	1: Translate;
*	2: Rotate;
*	3: Scale;
* level: n�vel do XML em que estamos.
* canDraw:
*	. Se true ent�o n�o h� mais transforma��es neste objeto e podemos desenhar;
*	. Se false ent�o ainda h� mais transforma��es e n�o podemos desenhar.
*/

typedef struct sTransformation
{
	int drawUntil;
	float xx;
	float yy;
	float zz;
	float angle;
	int type;
	int level;
	bool canDraw;
}Transformation, *pTransformation;


void transformInit(pTransformation t)
{
	t->drawUntil=0;
	t->xx = 0.0f;
	t->yy = 0.0f;
	t->zz = 0.0f;
	t->angle = 0.0f;
	t->type = 0;
	t->level = 0;
	t->canDraw=true;
}


/*
* numFigura: var auxiliar para preencher os 'drawUntil' na estrutura. Esse �ndice ser� depois usado para saber quais as figuras a desenhar;
* globalLevel: var auxiliar para preencher os 'level' na estrutura;
* map<numFigura, vector<tuple<coordX, coordY, coordZ> >>
*/

int numFigura=0, globalLevel=0;
map<int, vector<tuple<float,float,float>>> coords;
vector<Transformation> vecTransform;


/*
* L� as coordenadas do ficheiro passado e adiciona aos vetores das coordenadas.
*/
void readCoord(string fileName,int num)
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

		coords[num].push_back(make_tuple(x,y,z));
	}
}


void parserXML(XMLElement* pGroup,int numF,int curLevel) {
	XMLElement *pModel,*children, *aux;
	const char *attributeText = nullptr;


	// para saber quantos ficheiros est�o neste bloco e ler as suas coordenadas.
	pModel = pGroup->FirstChildElement("models")->FirstChildElement("model");
	while(pModel !=nullptr ){
		attributeText = pModel->Attribute("file");
		readCoord(attributeText,numF);
		numF++;
		numFigura=numF;

		pModel = pModel->NextSiblingElement();
	}

	aux=pGroup->FirstChildElement();
	// caso no bloco em que estamos n�o haja transforma��es, apenas "model files"
	if ( (strcmp(aux->Value(),"models"))==0 ){
		Transformation transf;
		transformInit(&transf);
		transf.drawUntil=numF;
		transf.level=curLevel;
		vecTransform.push_back(transf);
	}

	while( (strcmp(aux->Value(),"models")!=0)){

		if (strcmp(aux->Value(),"translate")==0){
			float tx=0,ty=0,tz=0;

			Transformation transf;
			transformInit(&transf);

			if(strcmp("models",aux->NextSiblingElement()->Value()) != 0){	// i.e. se existem mais transforma��es para este modelo
				transf.canDraw=false;
			}

			aux->QueryFloatAttribute("X", &tx);
			aux->QueryFloatAttribute("Y", &ty);
			aux->QueryFloatAttribute("Z", &tz);

			transf.drawUntil=numF;
			transf.type=1;
			transf.level=curLevel;
			transf.xx=tx;
			transf.yy=ty;
			transf.zz=tz;

			vecTransform.push_back(transf);
		}

		else if(strcmp(aux->Value(),"scale")==0){
			float sx=0,sy=0,sz=0;

			Transformation transf;
			transformInit(&transf);
			if(strcmp("models",aux->NextSiblingElement()->Value()) != 0){	// i.e. se existem mais transforma��es para este modelo
				transf.canDraw=false;
			}

			aux->QueryFloatAttribute("X", &sx);
			aux->QueryFloatAttribute("Y", &sy);
			aux->QueryFloatAttribute("Z", &sz);

			transf.drawUntil=numF;
			transf.type=3;
			transf.level=curLevel;
			transf.xx=sx;
			transf.yy=sy;
			transf.zz=sz;

			vecTransform.push_back(transf);
		}
		else if(strcmp(aux->Value(),"color")==0){
			float sx=0,sy=0,sz=0;

			Transformation transf;
			transformInit(&transf);
			if(strcmp("models",aux->NextSiblingElement()->Value()) != 0){	// i.e. se existem mais transforma��es para este modelo
				transf.canDraw=false;
			}

			aux->QueryFloatAttribute("R", &sx);
			aux->QueryFloatAttribute("G", &sy);
			aux->QueryFloatAttribute("B", &sz);

			transf.drawUntil=numF;
			transf.type=4;
			transf.level=curLevel;
			transf.xx=sx;
			transf.yy=sy;
			transf.zz=sz;

			vecTransform.push_back(transf);
		}

		else if(strcmp(aux->Value(),"rotate")==0){
			float rx=0,ry=0,rz=0,ang=0;

			Transformation transf;
			transformInit(&transf);
			if(strcmp("models",aux->NextSiblingElement()->Value()) != 0){	// i.e. se existem mais transforma��es para este modelo
				transf.canDraw=false;
			}

			aux->QueryFloatAttribute("X", &rx);
			aux->QueryFloatAttribute("Y", &ry);
			aux->QueryFloatAttribute("Z", &rz);
			aux->QueryFloatAttribute("angle", &ang);

			transf.drawUntil=numF;
			transf.type=2;
			transf.level=curLevel;
			transf.xx=rx;
			transf.yy=ry;
			transf.zz=rz;
			transf.angle=ang;

			vecTransform.push_back(transf);
		}

		aux=aux->NextSiblingElement();
	}

// Chamada recursiva
	children = pGroup->FirstChildElement("group");
	while(children!=nullptr){
		parserXML(children,numFigura,curLevel+1);
		children = children->NextSiblingElement();
	}
}


void execTransform(pTransformation t, bool flag){
	if(flag){
//		cout << "PUSH MATRIX " << endl;
		glPushMatrix();
	}

	switch(t->type){
		case 1 :
		{
//			cout << "TRANSLATE " << endl;
			glTranslatef(t->xx,t->yy,t->zz);
			break;
		}
		case 2:
		{
//			cout << "ROTATE " << endl;
			glRotatef(t->angle,t->xx,t->yy,t->zz);
			break;
		}
		case 3:
		{
//			cout << "SCALE" << endl;
			glScalef(t->xx,t->yy,t->zz);
			break;
		}
		case 4:
		{
			glColor3f(t->xx,t->yy,t->zz);
			break;
		}
		default: // caso type=0 (i.e. bloco sem transforma��es )
		{
//			cout << "NO TRANSFORMATIONS" << endl;
			break;
		}
	}
}

float alfa = 0.0f, beta = 0.0f, radius = 5.0f;
float camX, camY, camZ;

void spherical2Cartesian() {

	camX = radius * cos(beta) * sin(alfa);
	camY = radius * sin(beta);
	camZ = radius * cos(beta) * cos(alfa);
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


/*
* localLevel: N�vel em que estamos;
* drawn: N�mero de figuras j� desenhadas. Serve de �ndice para percorrer o map das figuras;
* lastCanDraw: Flag para controlar os PopMatrix e PushMatrix. Necess�rio porque os grupos podem ter mais que uma transforma��o.
*/
void renderScene(void) {

	float fps;
	int time;
	char s[64];

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();

// C�mara est� num ponto fixo.
	gluLookAt(300,-200,-400,
		0.0, 0.0,0.0,
		0.0f, 1.0f, 0.0f);


	// DRAW
	int localLevel=-1,i=0,drawn=0;
	bool lastCanDraw=true;
	map<int, vector<tuple<float,float,float>>>::iterator it;


	for(i=0;i<vecTransform.size();i++){

	// verificar o n�vel em que estamos para saber se � preciso fazer PopMatrix.
		while(vecTransform[i].level < localLevel){
			glPopMatrix();
			localLevel--;
		}

/*
(...)
	<group>
		<TRANSFORMA��ES/>
		<models>
			<file = "" />
		</models>
	</group>    <--------
	<group>		<--------
		<TRANSFORMA��ES/>
		<models>
			<file = "" />
		</models>
	</group>
(...)

Este If � para o caso acima.
Ver Teste 2 para exemplo concreto.
*/
		if(vecTransform[i].level==localLevel && lastCanDraw){
			glPopMatrix();
		}

// Caso haja v�rias transforma��es no mesmo objeto temos que as executar todas antes de poder desenhar
		if(!vecTransform[i].canDraw){
			localLevel=vecTransform[i].level;
			execTransform(&vecTransform[i],lastCanDraw);
			lastCanDraw=false;
			continue;
		}

		localLevel=vecTransform[i].level;
		execTransform(&vecTransform[i],lastCanDraw);
		lastCanDraw=true;


		glBegin(GL_TRIANGLES);

// desenhar as figuras desde o �ltimo �ndice que j� desenhamos(drawn) at� ao indice que queremos.
			while(drawn<vecTransform[i].drawUntil){
				it=coords.find(drawn); // vai buscar a key da figura que queremos � struct.
				for(auto vecIt=it->second.begin(); vecIt != it->second.end(); ++vecIt){
					float x,y,z;
					x = std::get<0>(*vecIt);
					y = std::get<1>(*vecIt);
					z = std::get<2>(*vecIt);
					glVertex3f(x,y,z);
				}
				drawn++;
			}
		glEnd();
	}

// Voltar ao n�vel -1, utilizando o "level" da �ltima transforma��o.
	for(localLevel=vecTransform[i-1].level; localLevel >= 0;localLevel--){
		glPopMatrix();
	}

	frame++;
	time=glutGet(GLUT_ELAPSED_TIME); 
	if (time - timebase > 1000) { 
		fps = frame*1000.0/(time-timebase); 
		timebase = time; 
		frame = 0; 
		sprintf(s, "FPS: %f6.2", fps);
		glutSetWindowTitle(s);
	}

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
	int i=0;

// -----------------
  	// TINYXML

	XMLDocument xmlDoc;
	xmlDoc.LoadFile(argv[1]);

	XMLNode *pRoot = xmlDoc.FirstChild();
	if (pRoot == nullptr)
	{
		cout << "No Root found." << endl;
		return XML_ERROR_FILE_READ_ERROR;
	}

	XMLElement* pGroup = pRoot->FirstChildElement("group");
	if (pGroup == nullptr)
	{
		cout << "Group element error.";
		return XML_ERROR_PARSING_ELEMENT;
	}

	while (pGroup != nullptr)
	{
		parserXML(pGroup,numFigura,globalLevel);
		pGroup = pGroup->NextSiblingElement();
	}
//	cout << "XML OK" << endl;
// ----------------

	cout << " Struct: " << endl;
	for(int i=0;i<vecTransform.size();i++){
		cout << endl;
		cout << "drawUntil: " << vecTransform[i].drawUntil << endl;
		cout << "x: " << vecTransform[i].xx << endl;
		cout << "y: " << vecTransform[i].yy << endl;
		cout << "z: " << vecTransform[i].zz << endl;
		cout << "type: " << vecTransform[i].type << endl;
		cout << "level: " << vecTransform[i].level << endl;
		cout << "canDraw: " << vecTransform[i].canDraw << endl;
	}

	// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("Assignment");

// Required callback registry
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
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
