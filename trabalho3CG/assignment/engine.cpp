#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
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
	float time;
	vector<float*> points;
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
	t->time=0;
}


/* 
CATMULL
--------------------------------------------------------------------------------------------------
*/
int POINT_COUNT= 4;
#define TOTAL_POINTS 100


void buildRotMatrix(float *x, float *y, float *z, float *m) {

	m[0] = x[0]; m[1] = x[1]; m[2] = x[2] ; m[3] = 0 ;
	m[4] = y[0]; m[5] = y[1]; m[6] = y[2] ; m[7] = 0 ;
	m[8] = z[0]; m[9] = z[1]; m[10] = z[2]; m[11] = 0;
	m[12] = 0  ; m[13] = 0  ; m[14] = 0   ; m[15] = 1;
}


void cross(float *a, float *b, float *res) {

	res[0] = a[1]*b[2] - a[2]*b[1];
	res[1] = a[2]*b[0] - a[0]*b[2];
	res[2] = a[0]*b[1] - a[1]*b[0];
    res[3] = 0;
}


void normalize(float *a) {

	float l = sqrt(a[0]*a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] = a[0]/l;
	a[1] = a[1]/l;
	a[2] = a[2]/l;
    a[3] = 1;
}


float length(float *v) {

	float res = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
	return res;

}

void multVectorMatrix(float *v, float *m, float *res) {

	for (int j = 0; j < 4; ++j) {
		res[j] = 0;
		for (int k = 0; k < 4; ++k) {
			res[j] += v[k] * m[k * 4 + j];
		}
	}

}

void multMatrixMatrix(float* m1, float* m2, float *res){
    for(int i = 0; i<4; i++){
        multVectorMatrix(m1 + i*4, m2, res + i*4);
    }
}


void getCatmullRomPoint(float t,
                        float *p0, float *p1, float *p2, float *p3,
                        float *pos, float *deriv) {

	// catmull-rom matrix
	float m[16] =  {
        -0.5f,  1.5f, -1.5f,  0.5f,
	    1.0f , -2.5f,  2.0f, -0.5f,
		-0.5f,  0.0f,  0.5f,  0.0f,
		0.0f ,  1.0f,  0.0f,  0.0f
    };

	// Compute A = M * P
	float P[16] = {
        p0[0], p0[1], p0[2], 1,
        p1[0], p1[1], p1[2], 1,
        p2[0], p2[1], p2[2], 1,
        p3[0], p3[1], p3[2], 1
    };

    float A[16];
    multMatrixMatrix(m,P,A);

	// Compute pos = T * A
    float T[4] = {t*t*t, t*t, t, 1};
    multVectorMatrix(T, A, pos);

	// compute deriv = T' * A
    float T_[4] = {3*t*t, 2*t, 1, 0};
    multVectorMatrix(T_, A, deriv);
}


void getGlobalCatmullRomPoint(std::vector<float*> p, float gt, float *pos, float *deriv) {
	
	float t = gt * POINT_COUNT; 
	int index = floor(t);  
	t = t - index; 

	
	int indices[4];
	indices[0] = (index + POINT_COUNT-1)%POINT_COUNT;
	indices[1] = (indices[0]+1)%POINT_COUNT;
	indices[2] = (indices[1]+1)%POINT_COUNT;
	indices[3] = (indices[2]+1)%POINT_COUNT;

	getCatmullRomPoint(t, p[indices[0]], p[indices[1]], p[indices[2]], p[indices[3]], pos, deriv);
}

void renderCatmullRomCurve(vector<float*> p) {
  	glBegin(GL_LINE_LOOP);
    float t;
    float pos[3];
    float deriv[3];
    for(int i= 0; i<TOTAL_POINTS; i++){
        t = ((float)i)/TOTAL_POINTS;
        getGlobalCatmullRomPoint(p, t, pos, deriv);
	glVertex3f(pos[0],pos[1],pos[2]);
    }

    glEnd();
// desenhar a curva usando segmentos de reta - GL_LINE_LOOP
}


void catmullRom (vector<float*> p, float time){
	
	int gt = glutGet(GLUT_ELAPSED_TIME);
    int tms = (int)(1000*time);
    int intv = gt % tms;
    float t = ((float)intv)/tms; // t in [0-1[

    float pos[3];
    float deriv[3];
    float up[3] = {0,1,0};
    float Z[3];

    getGlobalCatmullRomPoint(p, t, pos, deriv);
    normalize(deriv);

    cross(deriv, up, Z);
    normalize(Z);

    cross(Z, deriv, up);
    normalize(up);

    float m[16] = {deriv[0], deriv[1], deriv[2], 0 ,
                   up[0]   , up[1]   , up[2]   , 0 ,
                   Z[0]    , Z[1]    , Z[2]    , 0 ,
                   pos[0]  , pos[1]  , pos[2]  , 1 };

    glMultMatrixf(m);

}


/* 
CATMULL
--------------------------------------------------------------------------------------------------
*/

/*
* numFigura: var auxiliar para preencher os 'drawUntil' na estrutura. Esse �ndice ser� depois usado para saber quais as figuras a desenhar;
* globalLevel: var auxiliar para preencher os 'level' na estrutura;
* map<numFigura, vector<tuple<coordX, coordY, coordZ> >>
*/

int numFigura=0, globalLevel=0;
vector<Transformation> vecTransform;

//
GLuint buffers[1];
int timebase = 0, frame = 0;
map<int,vector<float>> coords;
//

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

		coords[num].push_back(x);
		coords[num].push_back(y);
		coords[num].push_back(z);

	}
}



void parserXML(XMLElement* pGroup,int numF,int curLevel) {
	XMLElement *pModel,*children, *aux, *elemTempo;
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
			float tx=0,ty=0,tz=0,tempo=0;

			Transformation transf;
			transformInit(&transf);

			if(strcmp("models",aux->NextSiblingElement()->Value()) != 0){	// i.e. se existem mais transforma��es para este modelo
				transf.canDraw=false;
			}

			aux->QueryFloatAttribute("time", &tempo); // caso translate tenha "time"
			if(tempo!=0){
				transf.time=tempo;
				elemTempo=aux->FirstChildElement();
				
				while(elemTempo!=nullptr){
					elemTempo->QueryFloatAttribute("X", &tx);
					elemTempo->QueryFloatAttribute("Y", &ty);
					elemTempo->QueryFloatAttribute("Z", &tz);
					
					float* v = (float*)malloc(3*sizeof(float));
		            v[0] = tx;
		            v[1] = ty;
		            v[2] = tz;


					transf.points.push_back(v);
					
					elemTempo = elemTempo->NextSiblingElement();
				}
			}

			else{ // translate "normal"
				aux->QueryFloatAttribute("X", &tx);
				aux->QueryFloatAttribute("Y", &ty);
				aux->QueryFloatAttribute("Z", &tz);
				transf.xx=tx;
				transf.yy=ty;
				transf.zz=tz;
			}

			transf.drawUntil=numF;
			transf.type=1;
			transf.level=curLevel;


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

// <rotate time=10 axisX=0 axisY=1 axisZ=0 />
		else if(strcmp(aux->Value(),"rotate")==0){
			float rx=0,ry=0,rz=0,ang=0,tempo=0;

			Transformation transf;
			transformInit(&transf);
			if(strcmp("models",aux->NextSiblingElement()->Value()) != 0){	// i.e. se existem mais transforma��es para este modelo
				transf.canDraw=false;
			}

			aux->QueryFloatAttribute("time", &tempo);
			aux->QueryFloatAttribute("axisX", &rx);
			aux->QueryFloatAttribute("axisY", &ry);
			aux->QueryFloatAttribute("axisZ", &rz);
			aux->QueryFloatAttribute("angle", &ang);

			transf.drawUntil=numF;
			transf.type=2;
			transf.level=curLevel;
			transf.xx=rx;
			transf.yy=ry;
			transf.zz=rz;
			transf.angle=ang;				
			transf.time=tempo;

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
int old_t;


void execTransform(pTransformation t, bool flag){
	if(flag){
//		cout << "PUSH MATRIX " << endl;
		glPushMatrix();
	}

	switch(t->type){
		case 1 :
		{
//			cout << "TRANSLATE " << endl;
			if (t->time==0){
				//cout << "TIME ==0" << endl;
				glTranslatef(t->xx,t->yy,t->zz);
			}
			else{
				//cout << "TIME!=0" << endl;
				POINT_COUNT = t->points.size();
				if (POINT_COUNT>=4) {
					renderCatmullRomCurve(t->points);
					catmullRom(t->points,t->time);
				}
			}
			break;
		}
		case 2:
		{
			if (t->time==0.0){
				//cout << "ROTATE " << endl;
				cout << "TIME ==0" << endl;
				glRotatef(t->angle,t->xx,t->yy,t->zz);
			
			}else{
				cout << "TIME!=0" << endl;
				float time=t->time;

				int gt = glutGet(GLUT_ELAPSED_TIME);

			    int tm = (int)(1000*time);
				int intv = gt % tm;

				glRotatef(((float)intv/(float)tm)*360.0 , t->xx, t->yy, t->zz);

			}

//			
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

// Vars para a câmara
//camType: 0-> fps, 1-> mouse

float alpha = 0.0f, beta = 0.0f, r = 5.0f, radius=100.0f;
int alphaAux, betaAux;
float camX, camY, camZ;
int startX, startY, tracking = 0, camType=0; 

float px=300,py=200,pz=-400;



void spherical2Cartesian() {
// alpha -> horizontal, beta -> vertical
// faz sentido ter radius ?
	camX = radius * cos(beta) * sin(alpha);
	camY = radius * sin(beta);
	camZ = radius * cos(beta) * cos(alpha);
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

	frame++;
	time=glutGet(GLUT_ELAPSED_TIME); 
	if (time - timebase > 1000) { 
		fps = frame*1000.0/(time-timebase); 
		timebase = time; 
		frame = 0; 
		sprintf(s, "FPS: %f6.2", fps);
		glutSetWindowTitle(s);
	}


	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();

// C�mara est� num ponto  não fixo.
	gluLookAt(px,py,pz,
		px+camX,py+camY,pz+camZ,
		0.0f, 1.0f, 0.0f);

// vbo	
/*
//vários buffers:
	GLuint buffers[coords.size()];
	glGenBuffers(coords.size(), buffers);
*/


	unsigned int total=0;
	// DRAW
	int localLevel=-1,i=0,drawn=0;
	bool lastCanDraw=true;
	map<int, vector<float>>::iterator it;

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

// desenhar as figuras desde o �ltimo �ndice que j� desenhamos(drawn) at� ao indice que queremos.

		while(drawn<vecTransform[i].drawUntil){
			// Set buffer active
			//glBindBuffer(GL_ARRAY_BUFFER,buffers[drawn]); // (vários buffers)
			glBindBuffer(GL_ARRAY_BUFFER,buffers[0]);
			// Fill buffer
			glBufferData(GL_ARRAY_BUFFER, coords[drawn].size()* sizeof(float) , coords[drawn].data(), GL_STATIC_DRAW); 

			total+=coords[drawn].size(); // total/3 dá o número de vértices das figuras.
    		drawn++;
		}

		glVertexPointer(3,GL_FLOAT,0,0); // define the semantics
    	glDrawArrays(GL_TRIANGLE_STRIP, 0, total/3); // Draw VBOs
	}

// Voltar ao n�vel -1, utilizando o "level" da �ltima transforma��o.
	for(localLevel=vecTransform[i-1].level; localLevel >= 0;localLevel--){
		glPopMatrix();
		total=0;
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

		case 'd': // strafe right
		{
            px+=5*sin(alpha-M_PI_2);
            pz+=5*cos(alpha-M_PI_2);
            break;
		}
        case 'a': // strafe left
        {
            px+=5*sin(alpha+M_PI_2);
            pz+=5*cos(alpha+M_PI_2);
            break;
        }
        case 'w': // forward
        {
            px+=5*sin(alpha);
            pz+=5*cos(alpha);
            py+=5*sin(beta);
            break;
        }
        case 's': // backwards
        {
            px-=5*sin(alpha);
            pz-=5*cos(alpha);
            py-=5*sin(beta);
			break;
		}
		case 'c': //camType: 0-> fps, 1->mouse
		{
			if (camType==0) camType=1;
			else{
				camType=0;
				alpha = alphaAux * 3.14 / 180.0;
				beta = betaAux * 3.14 / 180.0;
				spherical2Cartesian();
			}
			break;
		}
	}
	glutPostRedisplay();
}


void processSpecialKeys(int key, int xx, int yy) {
	if (!camType){ // caso esteja no modo fps
		switch (key) {
			case GLUT_KEY_RIGHT: // look right
				alpha-=0.05;
				break;

			case GLUT_KEY_LEFT: // look left
				alpha+=0.05;
				break;

			case GLUT_KEY_UP: // look up
				if(beta+0.05<(M_PI/2))   //Limite do ângulo vertical
		            beta+=0.05;
				break;

			case GLUT_KEY_DOWN: // look down
				if(beta-0.05>-(M_PI/2))  //Limite do ângulo vertical 
		            beta-=0.05;
		        break;
		}
		
		spherical2Cartesian();
		glutPostRedisplay();
	}
}


void processMouseButtons(int button, int state, int xx, int yy) {
	if (camType){ // camara modo mouse
		if (state == GLUT_DOWN)  {
			startX = xx;
			startY = yy;
			if (button == GLUT_LEFT_BUTTON)
				tracking = 1;
			else if (button == GLUT_RIGHT_BUTTON)
				tracking = 2;
			else
				tracking = 0;
		}
		else if (state == GLUT_UP) {
			if (tracking == 1) {
				alpha += (xx - startX);
				beta += (yy - startY);
			}
			else if (tracking == 2) {

				r -= yy - startY;
				if (r < 3)
					r = 3.0;
			}
			tracking = 0;
		}
	}
}


void processMouseMotion(int xx, int yy) {
	if (camType){ // camara modo mouse
		int deltaX, deltaY;
		int rAux;

		if (!tracking)
			return;

		deltaX = xx - startX;
		deltaY = yy - startY;

		if (tracking == 1) {


			alphaAux = alpha + deltaX;
			betaAux = beta + deltaY;

			if (betaAux > 85.0)
				betaAux = 85.0;
			else if (betaAux < -85.0)
				betaAux = -85.0;

			rAux = r;
		}
		else if (tracking == 2) {

			alphaAux = alpha;
			betaAux = beta;
			rAux = r - deltaY;
			if (rAux < 3)
				rAux = 3;
		}
		camX = rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
		camZ = rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
		camY = rAux * 							     sin(betaAux * 3.14 / 180.0);
	}
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
		for(int j=0;j< vecTransform[i].points.size();j++)
			cout << "elem " << j <<" : " <<  vecTransform[i].points[j] << endl;
		cout << "time: " << vecTransform[i].time <<  endl;
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
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);

#ifndef __APPLE__
    glewInit();
#endif
    
//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	
    //Enable Buffer Functionality
    glGenBuffers(1, buffers);
	glEnableClientState(GL_VERTEX_ARRAY);

// enter GLUT's main cycle
	glutMainLoop();

	return 1;
}
