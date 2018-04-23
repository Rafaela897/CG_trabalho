#include <stdlib.h>
#include <stdio.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <math.h>

#define _PI_ 3.14159

float alfa = 0.0f, beta = 0.0f, radius = 5.0f;
float camX, camY, camZ;

GLuint vertexCount, vertices;

int timebase = 0, frame = 0;


void sphericalToCartesian() {

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

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set the correct perspective
	gluPerspective(45,ratio,1,1000);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}



void prepareSphere(float radius, float slices, int stack) {

	float *v;

	v = (float *)malloc(sizeof(float) * 2 * 3 * 3 * slices*stack);

	int vertex = 0;
	float a = 2.0f * _PI_ / slices;
  float b = (_PI_) / stack;

	for (int i = 0; i < slices; ++i) {
    for (int j=0; j <= stack-1; j++){

  		v[vertex*3 + 0] = radius*cos((stack/2 - j)*b)*sin(i*a);
  		v[vertex*3 + 1] = radius*sin((stack / 2 - j)* b);
  		v[vertex*3 + 2] = radius*cos((stack / 2 - j)* b)*cos(i*a);

  		vertex++;
  		v[vertex*3 + 0] = radius*cos((stack / 2 - j)* b)*sin((i+1)*a);
  		v[vertex*3 + 1] = radius*sin((stack / 2 - j)* b);
  		v[vertex*3 + 2] = radius*cos((stack / 2 - j)* b)*cos((i+1)*a);

  		vertex++;
  		v[vertex*3 + 0] = radius*cos((stack / 2 - (j+1))* b)*sin((i+1)*a);
  		v[vertex*3 + 1] = radius*sin((stack / 2 - (j+1))* b);
  		v[vertex*3 + 2] = radius*cos((stack / 2 - (j+1))* b)*cos((i+1)*a);

  		// body
  		// tri�ngulo 1
  		vertex++;
  		v[vertex*3 + 0] = radius*cos((stack / 2 - j)* b)*sin(i*a);
  		v[vertex*3 + 1] = radius*sin((stack / 2 - j)* b);
  		v[vertex*3 + 2] = radius*cos((stack / 2 - j)* b)*cos(i*a);

  		vertex++;
  		v[vertex*3 + 0] = radius*cos((stack / 2 - (j+1))* b)*sin((i+1)*a);
  		v[vertex*3 + 1] = radius*sin((stack / 2 - (j+1))* b);
  		v[vertex*3 + 2] = radius*cos((stack / 2 - (j+1))* b)*cos((i+1)*a) ;

  		vertex++;
  		v[vertex*3 + 0] = radius*cos((stack / 2 - (j+1))* b)*sin(i*a);
  		v[vertex*3 + 1] = radius*sin((stack / 2 - (j+1))* b);
  		v[vertex*3 + 2] = radius*cos((stack / 2 - (j+1))* b)*cos(i*a);


  		vertex++;
    }
  }

	vertexCount = vertex;

	glGenBuffers(1, &vertices);
	glBindBuffer(GL_ARRAY_BUFFER,vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount * 3, v,     GL_STATIC_DRAW);

	free(v);

}

void prepareTorus(float raioI , float raioE , int rings, int slices)
{

	float *v;

	v = (float *)malloc(sizeof(float) * 2 * 3 * 3 * rings * slices * slices);

	int vertex = 0;

	const double angulo1 = (2 * M_PI)/rings;
	const double angulo2 = (2 * M_PI)/slices;

	float s , t , x , y , z, x2 , y2 , z2 , xk, yk, zk, x2k, y2k, z2k;

	for (int i = 0; i < rings; i++) {
			for (int j = 0; j <= slices; j++) {
				 // -----CALCULAR------
					// PRIMEIRO
				 s = i % rings ;
				 t = j % (slices + 1);

				 x = (raioE + raioI * cos(s * angulo1)) * cos(t * angulo2);
				 y = (raioE + raioI * cos(s * angulo1)) * sin(t * angulo2);
				 z = raioI * sin(s * angulo1);

					// PRIMEIROK
				 s = (i + 1) % rings ;
				 t = j % (slices + 1);

				 xk = (raioE + raioI * cos(s * angulo1)) * cos(t * angulo2);
				 yk = (raioE + raioI * cos(s * angulo1)) * sin(t * angulo2);
				 zk = raioI * sin(s * angulo1);


							// SEGUNDO
				 s = i % rings ;
				 t = (j + 1) % (slices + 1);

				 x2 = (raioE + raioI * cos(s * angulo1)) * cos(t * angulo2);
				 y2 = (raioE + raioI * cos(s * angulo1)) * sin(t * angulo2);
				 z2 = raioI * sin(s * angulo1);

						 // SEGUNDOk
				 s = (i + 1) % rings ;
				 t = (j+1) % (slices + 1);

				 x2k = (raioE + raioI * cos(s * angulo1)) * cos(t * angulo2);
				 y2k = (raioE + raioI * cos(s * angulo1)) * sin(t * angulo2);
				 z2k = raioI * sin(s * angulo1);

				v[vertex*3 + 0] = x;
	   		v[vertex*3 + 1] = y;
	   		v[vertex*3 + 2] = z;

	   		vertex++;
	   		v[vertex*3 + 0] = x2;
	   		v[vertex*3 + 1] = y2;
	   		v[vertex*3 + 2] = z2;

	   		vertex++;
	   		v[vertex*3 + 0] = xk;
	   		v[vertex*3 + 1] = yk;
	   		v[vertex*3 + 2] = zk;

				//
				vertex++;
				v[vertex*3 + 0] = xk;
				v[vertex*3 + 1] = yk;
				v[vertex*3 + 2] = zk;

				vertex++;
				v[vertex*3 + 0] = x2;
				v[vertex*3 + 1] = y2;
				v[vertex*3 + 2] = z2;

				vertex++;
				v[vertex*3 + 0] = x2k;
				v[vertex*3 + 1] = y2k;
				v[vertex*3 + 2] = z2k;

				vertex++;
	    }
	  }

		vertexCount = vertex;

		glGenBuffers(1, &vertices);
		glBindBuffer(GL_ARRAY_BUFFER,vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount * 3, v, GL_STATIC_DRAW);

		free(v);

	}

	void prepareBox(float X, float Y, float Z, int D) {

		float *v;

		v = (float *)malloc(sizeof(float) * 6* 6 * 3 * D*D);

		int vertex = 0;
		float x = X / 2;
		float y = Y / 2;
		float z = Z / 2;
		if (D < 1)
			D = 1;
		float dx = X / D;
		float dy = Y / D;
		float dz = Z / D;
		int r = x, t = y, u = z;

		for (int l = 0; l < D; l++) {
			for (int c = 0; c < D; c++) {

				//frente
	  		v[vertex*3 + 0] = x - (dx*c);
	  		v[vertex*3 + 1] = y - (dy*l);
	  		v[vertex*3 + 2] = z;

	  		vertex++;
	  		v[vertex*3 + 0] = x - (dx*(c + 1));
	  		v[vertex*3 + 1] = y - (dy*(l + 1));
	  		v[vertex*3 + 2] = z;

	  		vertex++;
	  		v[vertex*3 + 0] = x - (dx*c);
	  		v[vertex*3 + 1] = y - (dy*(l + 1));
	  		v[vertex*3 + 2] = z;

	  		vertex++;
	  		v[vertex*3 + 0] = x - (dx*c);
	  		v[vertex*3 + 1] = y - (dy*l);
	  		v[vertex*3 + 2] = z;

	  		vertex++;
	  		v[vertex*3 + 0] = x - (dx*(c + 1));
	  		v[vertex*3 + 1] = y - (dy*l);
	  		v[vertex*3 + 2] = z;

	  		vertex++;
	  		v[vertex*3 + 0] = x - (dx*(c + 1));
	  		v[vertex*3 + 1] = y - (dy*(l + 1));
	  		v[vertex*3 + 2] = z;

				//lado direito
				vertex++;
	  		v[vertex*3 + 0] = x;
	  		v[vertex*3 + 1] = y - (dy*l);
	  		v[vertex*3 + 2] = z - (dz*(c + 1));

	  		vertex++;
	  		v[vertex*3 + 0] = x;
	  		v[vertex*3 + 1] = y - (dy*(l+1));
	  		v[vertex*3 + 2] = z - (dz*c);

	  		vertex++;
	  		v[vertex*3 + 0] = x;
	  		v[vertex*3 + 1] = y - (dy*(l+1));
	  		v[vertex*3 + 2] = z - (dz*(c + 1));

				vertex++;
	  		v[vertex*3 + 0] = x;
	  		v[vertex*3 + 1] = y - (dy*(l + 1));
	  		v[vertex*3 + 2] = z - (dz*c);

	  		vertex++;
	  		v[vertex*3 + 0] = x;
	  		v[vertex*3 + 1] = y - (dy*l);
	  		v[vertex*3 + 2] = z - (dz*(c + 1));

	  		vertex++;
	  		v[vertex*3 + 0] = x;
	  		v[vertex*3 + 1] = y - (dy*l);
	  		v[vertex*3 + 2] = z - (dz*c);

				//lado esquerdo
				vertex++;
	  		v[vertex*3 + 0] = -x;
	  		v[vertex*3 + 1] = y - (dy*l);
	  		v[vertex*3 + 2] = z - (dz*c);

	  		vertex++;
	  		v[vertex*3 + 0] = -x;
	  		v[vertex*3 + 1] = y - (dy*(l+1));
	  		v[vertex*3 + 2] =z - (dz*(c + 1));

	  		vertex++;
	  		v[vertex*3 + 0] = -x;
	  		v[vertex*3 + 1] = y - (dy*(l+1));
	  		v[vertex*3 + 2] = z - (dz*c);

				vertex++;
	  		v[vertex*3 + 0] = -x;
	  		v[vertex*3 + 1] = y - (dy*(l + 1));
	  		v[vertex*3 + 2] = z - (dz*(c + 1));

	  		vertex++;
	  		v[vertex*3 + 0] = -x;
	  		v[vertex*3 + 1] = y - (dy*l);
	  		v[vertex*3 + 2] = z - (dz*c);

	  		vertex++;
	  		v[vertex*3 + 0] = -x;
	  		v[vertex*3 + 1] = y - (dy*l);
	  		v[vertex*3 + 2] = z - (dz*(c + 1));

				//tras
				vertex++;
	  		v[vertex*3 + 0] = x - (dx*(c + 1));
	  		v[vertex*3 + 1] = y - (dy*l);
	  		v[vertex*3 + 2] = -z;

	  		vertex++;
	  		v[vertex*3 + 0] = x - (dx*c);
	  		v[vertex*3 + 1] = y - (dy*(l + 1));
	  		v[vertex*3 + 2] = -z;

	  		vertex++;
	  		v[vertex*3 + 0] = x - (dx*(c + 1));
	  		v[vertex*3 + 1] = y - (dy*(l + 1));
	  		v[vertex*3 + 2] = -z;

	  		vertex++;
	  		v[vertex*3 + 0] = x - (dx*c);
	  		v[vertex*3 + 1] =y - (dy*(l + 1));
	  		v[vertex*3 + 2] = -z;

	  		vertex++;
	  		v[vertex*3 + 0] = x - (dx*(c + 1));
	  		v[vertex*3 + 1] = y - (dy*l);
	  		v[vertex*3 + 2] = -z;

	  		vertex++;
	  		v[vertex*3 + 0] = x - (dx*c);
	  		v[vertex*3 + 1] = y - (dy*l);
	  		v[vertex*3 + 2] = -z;

				//topo
				vertex++;
	  		v[vertex*3 + 0] = x - (dx*c);
	  		v[vertex*3 + 1] = y ;
	  		v[vertex*3 + 2] = z - (dz*(l + 1));

	  		vertex++;
	  		v[vertex*3 + 0] = x - (dx*(c + 1));
	  		v[vertex*3 + 1] = y ;
	  		v[vertex*3 + 2] = z - (dz*l);

	  		vertex++;
	  		v[vertex*3 + 0] = x - (dx*c);
	  		v[vertex*3 + 1] = y ;
	  		v[vertex*3 + 2] = z - (dz*l);

	  		vertex++;
	  		v[vertex*3 + 0] = x - (dx*(c + 1));
	  		v[vertex*3 + 1] = y ;
	  		v[vertex*3 + 2] = z - (dz*l);

	  		vertex++;
	  		v[vertex*3 + 0] = x - (dx*c);
	  		v[vertex*3 + 1] = y ;
	  		v[vertex*3 + 2] = z - (dz*(l + 1));

	  		vertex++;
	  		v[vertex*3 + 0] = x - (dx*(c + 1));
	  		v[vertex*3 + 1] = y ;
	  		v[vertex*3 + 2] = z - (dz*(l + 1));

				//base
				vertex++;
				v[vertex*3 + 0] = x - (dx*c);
				v[vertex*3 + 1] = -y ;
				v[vertex*3 + 2] = z - (dz*l);

				vertex++;
				v[vertex*3 + 0] = x - (dx*(c + 1));
				v[vertex*3 + 1] = -y ;
				v[vertex*3 + 2] = z - (dz*(l + 1));

				vertex++;
				v[vertex*3 + 0] = x - (dx*c);
				v[vertex*3 + 1] = -y ;
				v[vertex*3 + 2] = z - (dz*(l + 1));

				vertex++;
				v[vertex*3 + 0] = x - (dx*(c + 1));
				v[vertex*3 + 1] = -y ;
				v[vertex*3 + 2] = z - (dz*(l + 1));

				vertex++;
				v[vertex*3 + 0] = x - (dx*c);
				v[vertex*3 + 1] = -y ;
				v[vertex*3 + 2] = z - (dz*l);

				vertex++;
				v[vertex*3 + 0] = x - (dx*(c + 1));
				v[vertex*3 + 1] = -y ;
				v[vertex*3 + 2] = z - (dz*l);

  			vertex++;
	    }
	  }

		vertexCount = vertex;

		glGenBuffers(1, &vertices);
		glBindBuffer(GL_ARRAY_BUFFER,vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount * 3, v,     GL_STATIC_DRAW);

		free(v);

	}


	void prepareCone(float radius, float h, int slices, int stacks) {

		float *v;

		v = (float *)malloc(sizeof(float) * 2 * 3 * 3 * slices*stacks*stacks);

		int vertex = 0;
		float angulo = (2*(M_PI))/ slices;
		int z = 0;


		for(int x = 0 ; x< slices ; x++ ){
		for (int j = stacks; j >= 0; j--)
		{

	  		v[vertex*3 + 0] = ((radius*j)/stacks)*sin(x*angulo);
	  		v[vertex*3 + 1] =  h - (h/stacks)*j;
	  		v[vertex*3 + 2] = ((radius*j) / stacks)*cos(x*angulo);

	  		vertex++;
	  		v[vertex*3 + 0] = ((radius*j) / stacks)*sin(x*angulo + angulo);
	  		v[vertex*3 + 1] = h - (h/ stacks)*j;
	  		v[vertex*3 + 2] = ((radius*j) / stacks)*cos(x*angulo + angulo);

	  		vertex++;
	  		v[vertex*3 + 0] = 0.0;
	  		v[vertex*3 + 1] = h;
	  		v[vertex*3 + 2] = 0.0;


	  		vertex++;
	  		v[vertex*3 + 0] = radius*sin(x*angulo);
	  		v[vertex*3 + 1] = 0.0;
	  		v[vertex*3 + 2] = radius*cos(x*angulo);

	  		vertex++;
	  		v[vertex*3 + 0] = 0.0;
	  		v[vertex*3 + 1] = 0.0;
	  		v[vertex*3 + 2] = 0.0 ;

	  		vertex++;
	  		v[vertex*3 + 0] = radius*sin(x*angulo + angulo);
	  		v[vertex*3 + 1] = 0.0;
	  		v[vertex*3 + 2] = radius*cos(x*angulo + angulo);


	  		vertex++;
	    }
	  }

		vertexCount = vertex;

		glGenBuffers(1, &vertices);
		glBindBuffer(GL_ARRAY_BUFFER,vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount * 3, v,     GL_STATIC_DRAW);

		free(v);

	}

	void preparePlane(float X, float Z) {

		float *v;

		v = (float *)malloc(sizeof(float) * 4 * 3 * 3 );

		int vertex = 0;
		float x = X / 2;
		float z = Z / 2;
		float y = 0.0;

		v[vertex*3 + 0] = x;
		v[vertex*3 + 1] = y;
		v[vertex*3 + 2] = -z;

		vertex++;
		v[vertex*3 + 0] = -x;
		v[vertex*3 + 1] = y;
		v[vertex*3 + 2] = z;

		vertex++;
		v[vertex*3 + 0] = x;
		v[vertex*3 + 1] = y;
		v[vertex*3 + 2] = z;
		//
		vertex++;
		v[vertex*3 + 0] = -x;
		v[vertex*3 + 1] = y;
		v[vertex*3 + 2] = z;

		vertex++;
		v[vertex*3 + 0] = x;
		v[vertex*3 + 1] = y;
		v[vertex*3 + 2] = -z;

		vertex++;
		v[vertex*3 + 0] = -x;
		v[vertex*3 + 1] = y;
		v[vertex*3 + 2] = -z;

		//
		vertex++;
		v[vertex*3 + 0] = -x;
		v[vertex*3 + 1] = y;
		v[vertex*3 + 2] = z;

		vertex++;
		v[vertex*3 + 0] = x;
		v[vertex*3 + 1] = y;
		v[vertex*3 + 2] = -z;

		vertex++;
		v[vertex*3 + 0] = x;
		v[vertex*3 + 1] = y;
		v[vertex*3 + 2] = z;

		//
		vertex++;
		v[vertex*3 + 0] = x;
		v[vertex*3 + 1] = y;
		v[vertex*3 + 2] = -z;

		vertex++;
		v[vertex*3 + 0] = -x;
		v[vertex*3 + 1] = y;
		v[vertex*3 + 2] = z;

		vertex++;
		v[vertex*3 + 0] = -x;
		v[vertex*3 + 1] = y;
		v[vertex*3 + 2] = -z;


		vertex++;

		vertexCount = vertex;

		glGenBuffers(1, &vertices);
		glBindBuffer(GL_ARRAY_BUFFER,vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount * 3, v,     GL_STATIC_DRAW);

		free(v);

	}


void drawSphere() {

	glBindBuffer(GL_ARRAY_BUFFER,vertices);
	glVertexPointer(3,GL_FLOAT,0,0);

	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

/*

void drawTorus() {

	glBindBuffer(GL_ARRAY_BUFFER,vertices);
	glVertexPointer(3,GL_FLOAT,0,0);

	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void drawBox() {

	glBindBuffer(GL_ARRAY_BUFFER,vertices);
	glVertexPointer(3,GL_FLOAT,0,0);

	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}


void drawCone() {

	glBindBuffer(GL_ARRAY_BUFFER,vertices);
	glVertexPointer(3,GL_FLOAT,0,0);

	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}


void drawPlane() {

	glBindBuffer(GL_ARRAY_BUFFER,vertices);
	glVertexPointer(3,GL_FLOAT,0,0);

	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}*/

void renderScene(void) {

	float pos[4] = {1.0, 1.0, 1.0, 0.0};
	float fps;
	int time;
	char s[64];

	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	gluLookAt(camX,camY,camZ, 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);

	drawSphere();
	drawTorus();

	frame++;
	time=glutGet(GLUT_ELAPSED_TIME); 
	if (time - timebase > 1000) { 
		fps = frame*1000.0/(time-timebase); 
		timebase = time; 
		frame = 0; 
		sprintf(s, "FPS: %f6.2", fps);
		glutSetWindowTitle(s);
	} 

// End of frame
	glutSwapBuffers();
}



void processKeys(int key, int xx, int yy) 
{
	switch(key) {
	
		case GLUT_KEY_RIGHT: 
						alfa -=0.1; break;

		case GLUT_KEY_LEFT: 
						alfa += 0.1; break;

		case GLUT_KEY_UP : 
						beta += 0.1f;
						if (beta > 1.5f)
							beta = 1.5f;
						break;

		case GLUT_KEY_DOWN: 
						beta -= 0.1f; 
						if (beta < -1.5f)
							beta = -1.5f;
						break;

		case GLUT_KEY_PAGE_DOWN : radius -= 0.1f; 
			if (radius < 0.1f)
				radius = 0.1f;
			break;

		case GLUT_KEY_PAGE_UP: radius += 0.1f; break;

	}
	sphericalToCartesian();

}



void initGL() {

// OpenGL settings 
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

// init
	sphericalToCartesian();
	glEnableClientState(GL_VERTEX_ARRAY);

	prepareSphere(2,20,20);
}


int main(int argc, char **argv) {

// init
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(320,320);
	glutCreateWindow("CG@DI-UM");
		
// callback registration
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);

// keyboard callback registration
	glutSpecialFunc(processKeys);

#ifndef __APPLE__	
// init GLEW
	glewInit();
#endif	


	initGL();

	glutMainLoop();
	
	return 1;
}
