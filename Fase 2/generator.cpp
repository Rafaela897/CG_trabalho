#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>

float alfa = 0.0;
float beta = 0.0;
float r = 5.0;

using namespace std;


void drawSphere(float radius,int slices,int stack, string fileName)
{
	ofstream myfile;
	myfile.open(fileName);
	if(!myfile.is_open())
	{
		cerr << "Couldn't open file.";
		exit(EXIT_FAILURE);
	}

	float a = (2 * M_PI) / slices;
	float b = (M_PI) / stack;

	for (int i = 0; i<  slices; i++) {
		for(int j=0; j <= stack-1; j++ ){
			myfile << radius*cos((stack / 2 - j)* b)*sin(i*a) <<  " " << radius*sin((stack / 2 - j)* b) << " " << radius*cos((stack / 2 - j)* b)*cos(i*a) << endl;
			myfile << radius*cos((stack / 2 - j)* b)*sin((i+1)*a) <<  " " << radius*sin((stack / 2 - j)* b) << " " << radius*cos((stack / 2 - j)* b)*cos((i+1)*a) << endl;
			myfile << radius*cos((stack / 2 - (j+1))* b)*sin((i+1)*a) <<  " " << radius*sin((stack / 2 - (j+1))* b) << " " << radius*cos((stack / 2 - (j+1))* b)*cos((i+1)*a) << endl;

			myfile << radius*cos((stack / 2 - j)* b)*sin(i*a) <<  " " << radius*sin((stack / 2 - j)* b) << " " << radius*cos((stack / 2 - j)* b)*cos(i*a) << endl;
			myfile << radius*cos((stack / 2 - (j+1))* b)*sin((i+1)*a) <<  " " << radius*sin((stack / 2 - (j+1))* b) << " " << radius*cos((stack / 2 - (j+1))* b)*cos((i+1)*a) << endl;
			myfile << radius*cos((stack / 2 - (j+1))* b)*sin(i*a) <<  " " << radius*sin((stack / 2 - (j+1))* b) << " " << radius*cos((stack / 2 - (j+1))* b)*cos(i*a) << endl;
		}
	}
	myfile.close();
}

void drawTorus(float raioI , float raioE , int rings, int slices, string fileName)
{
	ofstream myfile;
	myfile.open(fileName);
	if(!myfile.is_open())
	{
		cerr << "Couldn't open file.";
		exit(EXIT_FAILURE);
	}
    // CONSTANTES
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



            // ----DESENHAR-----
						myfile << x <<  " " << y << " " << z << endl;
						myfile << x2 <<  " " << y2 << " " << z2 << endl;
						myfile << xk <<  " " << yk << " " << zk << endl;

						myfile << xk <<  " " << yk << " " << zk << endl;
						myfile << x2 <<  " " << y2 << " " << z2 << endl;
						myfile << x2k <<  " " << y2k << " " << z2k << endl;


        }
    }
    myfile.close();
}


void drawBox(float X, float Y, float Z, int D,string fileName) {

	ofstream myfile;
	myfile.open(fileName);
	if(!myfile.is_open())
	{
		cerr << "Couldn't open file.";
		exit(EXIT_FAILURE);
	}

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
			// frente
			myfile << x - (dx*c) <<  " " << y - (dy*l) << " " << z << endl;
			myfile << x - (dx*(c + 1)) <<  " " <<  y - (dy*(l + 1)) << " " << z << endl;
			myfile << x - (dx*c) <<  " " << y - (dy*(l + 1)) << " " << z << endl;

			myfile << x - (dx*c) <<  " " << y - (dy*l) << " " << z << endl;
			myfile << x - (dx*(c + 1)) <<  " " << y - (dy*l) << " " << z << endl;
			myfile << x - (dx*(c + 1)) <<  " " << y - (dy*(l + 1)) << " " << z << endl;

			//lado direito
			myfile << x <<  " " << y - (dy*l) << " " << z - (dz*(c + 1)) << endl;
			myfile << x <<  " " << y - (dy*(l+1)) << " " << z - (dz*c) << endl;
			myfile << x <<  " " << y - (dy*(l+1)) << " " << z - (dz*(c + 1)) << endl;

			myfile << x <<  " " << y - (dy*(l + 1)) << " " << z - (dz*c) << endl;
			myfile << x <<  " " << y - (dy*l) << " " << z - (dz*(c + 1)) << endl;
			myfile << x <<  " " << y - (dy*l) << " " << z - (dz*c) << endl;

			//lado esquerdo
			myfile << -x <<  " " << y - (dy*l) << " " << z - (dz*c) << endl;
			myfile << -x <<  " " << y - (dy*(l + 1)) << " " << z - (dz*(c + 1)) << endl;
			myfile << -x <<  " " << y - (dy*(l + 1)) << " " << z - (dz*c) << endl;

			myfile << -x <<  " " << y - (dy*(l + 1)) << " " << z - (dz*(c + 1)) << endl;
			myfile << -x <<  " " << y - (dy*l) << " " << z - (dz*c) << endl;
			myfile << -x <<  " " << y - (dy*l) << " " << z - (dz*(c + 1)) << endl;

			// lado de tras
			myfile << x - (dx*(c + 1)) <<  " " << y - (dy*l) << " " << -z << endl;
			myfile << x - (dx*c) <<  " " << y - (dy*(l + 1)) << " " << -z << endl;
			myfile << x - (dx*(c + 1)) <<  " " << y - (dy*(l + 1)) << " " << -z << endl;

			myfile << x - (dx*c) <<  " " << y - (dy*(l + 1)) << " " << -z << endl;
			myfile << x - (dx*(c + 1)) <<  " " << y - (dy*l) << " " << -z << endl;
			myfile << x - (dx*c) <<  " " << y - (dy*l) << " " << -z<< endl;

			//base
			myfile << x - (dx*c) <<  " " << y << " " << z - (dz*(l + 1)) << endl;
			myfile << x - (dx*(c + 1)) <<  " " << y << " " <<  z - (dz*l) << endl;
			myfile << x - (dx*c) <<  " " << y << " " << z - (dz*l) << endl;

			myfile << x - (dx*(c + 1)) <<  " " << y << " " << z - (dz*l) << endl;
			myfile << x - (dx*c) <<  " " << y << " " << z - (dz*(l + 1)) << endl;
			myfile << x - (dx*(c + 1)) <<  " " << y << " " << z - (dz*(l + 1)) << endl;

			//topo
			myfile << x - (dx*c) <<  " " << -y << " " << z - (dz*l) << endl;
			myfile << x - (dx*(c + 1)) <<  " " << -y << " " << z - (dz*(l + 1)) << endl;
			myfile << x - (dx*c) <<  " " << -y << " " << z - (dz*(l + 1)) << endl;

			myfile << x - (dx*(c + 1)) <<  " " << -y << " " << z - (dz*(l + 1)) << endl;
			myfile << x - (dx*c) <<  " " << -y << " " << z - (dz*l) << endl;
			myfile << x - (dx*(c + 1)) <<  " " << -y << " " << z - (dz*l) << endl;
		}
	}
	myfile.close();
}

void drawCone(float radius,float h,int slices, int stacks, string fileName)
{
	ofstream myfile;
	myfile.open(fileName);
	if(!myfile.is_open())
	{
		cerr << "Couldn't open file.";
		exit(EXIT_FAILURE);
	}

	float angulo = (2*(M_PI))/ slices;
	int z = 0;

    for(int x = 0 ; x< slices ; x++ ){
		for (int j = stacks; j >= 0; j--)
		{
			myfile << ((radius*j)/stacks)*sin(x*angulo) << " " << h - (h/stacks)*j << " " << ((radius*j) / stacks)*cos(x*angulo) << endl;
			myfile << ((radius*j) / stacks)*sin(x*angulo + angulo) << " " << h - (h/ stacks)*j << " " << ((radius*j) / stacks)*cos(x*angulo + angulo) << endl;
			myfile << "0.0" << " " << h << " " << "0.0" << endl;
		}
		myfile << radius*sin(x*angulo) << " " << "0.0" << " " << radius*cos(x*angulo) << endl;
		myfile << "0.0" << " " << "0.0" << " " << "0.0" << endl;
		myfile << radius*sin(x*angulo + angulo) << " " << "0.0" << " " << radius*cos(x*angulo + angulo) << endl;
    }
	myfile.close();
}


void drawPlane(string fileName,float X=2,float Z=2)
{
	float x = X / 2;
	float z = Z / 2;

	ofstream myfile;
	myfile.open(fileName);
	if(!myfile.is_open())
	{
		cerr << "Couldn't open file.";
		exit(EXIT_FAILURE);
	}

	myfile << x << " " << "0.0" << " " << -z << endl;
	myfile << -x << " " << "0.0" << " " << z << endl;
	myfile << x << " " << "0.0" << " " << z << endl;

	myfile << -x << " " << "0.0" << " " << z << endl;
	myfile << x << " " << "0.0" << " " << -z << endl;
	myfile << -x << " " << "0.0" << " " << -z << endl;

	myfile << -x << " " << "0.0" << " " << z << endl;
	myfile << x << " " << "0.0" << " " << -z << endl;
	myfile << x << " " << "0.0" << " " << z << endl;

	myfile << x << " " << "0.0" << " " << -z << endl;
	myfile << -x << " " << "0.0" << " " << z << endl;
	myfile << -x << " " << "0.0" << " " << -z << endl;

	myfile.close();
}


int main(int argc,char **argv)
{
	string fileName,extension=".3d";

// generator Plane X Z filename (2,2)
	if(strcmp(argv[1],"Plane")==0)
	{
		if(argc==3)
		{
			fileName = argv[2] + extension;
			drawPlane(fileName);
		}
		else if(argc==5 && stof(argv[2])>0 && stof(argv[3])>0)
		{
			float x,z;
			x=stof(argv[2]);
			z=stof(argv[3]);
			fileName = argv[4] + extension;
			drawPlane(fileName,x,z);
		}
		else
			cout << "Invalid.\n";
	}

// generator Cone radius height slices stacks filename (1,2,20,20)
	else if(strcmp(argv[1],"Cone")==0)
	{
		int slices,stacks;
		float radius,height;

		radius=stof(argv[2]);
		height=stof(argv[3]);
		slices=stoi(argv[4]);
		stacks=stoi(argv[5]);

		if(radius>0 && height>0 && slices>0 && stacks>0 && argc==7 )
		{
			fileName = argv[6] + extension;
			drawCone(radius,height,slices,stacks,fileName);
		}
		else
			cout << "Invalid.\n";
	}

// generator Sphere radius slices stacks filename (1,20,20)
	else if(strcmp(argv[1],"Sphere")==0)
	{
		int slices,stack;
		float radius;

		radius=stof(argv[2]);
		slices=stoi(argv[3]);
		stack=stoi(argv[4]);

		if(radius>0 && stack>0 && slices>0 && argc==6)
		{
			fileName = argv[5] + extension;
			drawSphere(radius,slices,stack,fileName);
		}
		else
			cout << "Invalid.\n";
	}

// generator Box X Y Z D filename (1 1 1 5)
	else if(strcmp(argv[1],"Box")==0)
	{
		float X,Y,Z,D;

		X=stof(argv[2]);
		Y=stof(argv[3]);
		Z=stof(argv[4]);
		D=stof(argv[5]);

		if(X>0 && Y>0 && Z>0 && D>=0 && argc==7)
		{
			fileName = argv[6] + extension;
			drawBox(X,Y,Z,D,fileName);
		}
		else
			cout << "Invalid.\n";
	}
	// generator Torus
	else if(strcmp(argv[1],"Torus")==0)
	{

		float raioI, raioE;
		int rings, slices;

		raioI=stof(argv[2]);
		raioE=stof(argv[3]);
		rings=stoi(argv[4]);
		slices=stoi(argv[5]);

		if(raioI>0 && raioE>0 && rings>0 && slices>=0 && argc==7)
		{
			fileName = argv[6] + extension;
			drawTorus(raioI,raioE,rings,slices,fileName);
		}
		else
			cout << "Invalid.\n";
	}

	else cout << "Invalid.\n";

	return 0;
}
