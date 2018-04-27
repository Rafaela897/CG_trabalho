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

float camX = 0, camY, camZ = 5;
int startX, startY, tracking = 0;

int alfa = 0, beta = 0, radius = 5;
GLuint vertexCount, vertices;

int timebase = 0, frame = 0;


void sphericalToCartesian() {

	camX = radius * cos(beta) * sin(alfa);
	camY = radius * sin(beta);
	camZ = radius * cos(beta) * cos(alfa);
}


#define POINT_COUNT 5
#define TOTAL_POINTS 100
// Points that make up the loop for catmull-rom interpolation
float p[POINT_COUNT][4] = {{-1,0,-1,1},{-1,0,1,1},{1,0,1,1},{0,0,0,1},{1,0,-1,1}};

void buildRotMatrix(float *x, float *y, float *z, float *m) {

	m[0] = x[0]; m[1] = x[1]; m[2] = x[2] ; m[3] = 0 ;
	m[4] = y[0]; m[5] = y[1]; m[6] = y[2] ; m[7] = 0 ;
	m[8] = z[0]; m[9] = z[1]; m[10] = z[2]; m[11] = 0;
	m[12] = 0  ; m[13] = 0  ; m[14] = 0   ; m[15] = 1;
}

void printMatrix(float* m){
    printf("%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n",
            m[0] , m[1] , m[2] , m[3] ,
            m[4] , m[5] , m[6] , m[7] ,
            m[8] , m[9] , m[10], m[11],
            m[12], m[13], m[14], m[15]);
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
	float m[16] =  {	-0.5f,  1.5f, -1.5f,  0.5f,
						 1.0f, -2.5f,  2.0f, -0.5f,
						-0.5f,  0.0f,  0.5f,  0.0f,
						 0.0f,  1.0f,  0.0f,  0.0f };
			
	// Compute A = M * P
	float P[16] = { p0[0], p0[1], p0[2], p0[3],
                    p1[0], p1[1], p1[2], p1[3], 
                    p2[0], p2[1], p2[2], p2[3], 
                    p3[0], p3[1], p3[2], p3[3]};
    //printf("Pontos:\n");
    //printMatrix(P);

    float A[16];
    multMatrixMatrix(m,P,A);

	// Compute pos = T * A
    
    float T[4] = {t*t*t, t*t, t, 1};
    multVectorMatrix(T, A, pos);
	
	// compute deriv = T' * A

    float T_[4] = {3*t*t, 2*t, 1, 0};
    multVectorMatrix(T_, A, deriv);

	// ...

    


}


// given  global t, returns the point in the curve
void getGlobalCatmullRomPoint(float gt, float *pos, float *deriv) {

	float t = gt * POINT_COUNT; // this is the real global t
	int index = floor(t);  // which segment
	t = t - index; // where within  the segment

	// indices store the points
	int indices[4]; 
	indices[0] = (index + POINT_COUNT-1)%POINT_COUNT;	
	indices[1] = (indices[0]+1)%POINT_COUNT;
	indices[2] = (indices[1]+1)%POINT_COUNT; 
	indices[3] = (indices[2]+1)%POINT_COUNT;

	getCatmullRomPoint(t, p[indices[0]], p[indices[1]], p[indices[2]], p[indices[3]], pos, deriv);
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

void renderCatmullRomCurve() {
    glBegin(GL_LINE_LOOP);
    float t;
    float pos[4];
    float deriv[4];
    for(int i= 0; i<TOTAL_POINTS; i++){
        t = ((float)i)/TOTAL_POINTS;
        getGlobalCatmullRomPoint(t, pos, deriv);
        glVertex3f(pos[0],pos[1],pos[2]);
    }

    glEnd();
// desenhar a curva usando segmentos de reta - GL_LINE_LOOP
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
}

void renderScene(void) {
	static float t = 0;
	float pos[4] = {1.0, 1.0, 1.0, 0.0};
	float deriv[4];
	float fps;
	int time;
	char s[64];
	float up[4] = {0,1,0,0};

	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	gluLookAt(camX,camY,camZ, 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);

	renderCatmullRomCurve();
    
	// apply transformations here
    getGlobalCatmullRomPoint(t, pos, deriv);
    normalize(deriv);
    float Z[4];
    cross(deriv, up, Z);
    normalize(Z);
    cross(Z, deriv, up);
    normalize(up);
    
    /*
    float m[16] = {deriv[0], up[0], Z[0], pos[0],
                   deriv[1], up[1], Z[1], pos[1],
                   deriv[2], up[2], Z[2], pos[2],
                   0       , 0    , 0   , 1     };
    */
    float m[16] = {deriv[0], deriv[1], deriv[2], 0 ,
                   up[0]   , up[1]   , up[2]   , 0 ,
                   Z[0]    , Z[1]    , Z[2]    , 0 ,
                   pos[0]  , pos[1]  , pos[2]  , 1 };
    glMultMatrixf(m);

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
	t+=0.001;
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

void processMouseButtons(int button, int state, int xx, int yy) 
{
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
			alfa += (xx - startX);
			beta += (yy - startY);
		}
		else if (tracking == 2) {
			
			radius -= yy - startY;
			if (radius < 3)
				radius = 3.0;
		}
		tracking = 0;
	}
}


void processMouseMotion(int xx, int yy)
{
	int deltaX, deltaY;
	int alphaAux, betaAux;
	int rAux;

	if (!tracking)
		return;

	deltaX = xx - startX;
	deltaY = yy - startY;

	if (tracking == 1) {

		alphaAux = alfa - deltaX;
		betaAux = beta + deltaY;

		if (betaAux > 85.0)
			betaAux = 85.0;
		else if (betaAux < -85.0)
			betaAux = -85.0;

		rAux = radius;
	}
	else if (tracking == 2) {

		alphaAux = alfa;
		betaAux = beta;
		rAux = radius - deltaY;
		if (rAux < 1)
			rAux = 1;
	}
	camX = rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camZ = rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camY = rAux *							     sin(betaAux * 3.14 / 180.0);
}


void initGL() {

// OpenGL settings 
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

// init
	sphericalToCartesian();
	glEnableClientState(GL_VERTEX_ARRAY);

	prepareSphere(0.1,20,20);

	//prepareTorus(3,28,100,50);
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
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);


#ifndef __APPLE__	
// init GLEW
	glewInit();
#endif	


	initGL();

	glutMainLoop();
	
	return 1;
}
