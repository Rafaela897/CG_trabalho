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
	int j = 0;
	float texst = 1.0/stack;
	float texsl = 1.0/slices;


	for (int i = 0; i<  slices; i++){

/*
		//primeiro ponto
		myfile << 0 <<  " " << radius << " " << 0 << endl;
			
		myfile << 0 << " " << 1 <<" " << 0<< endl;
			
		myfile << (i)*texsl << " " << 1.0 << endl;
		*/	 





		//ponto
			myfile << radius*cos((stack / 2 - (j+1))* b)*sin((i+1)*a) <<  " " << radius*sin((stack / 2 - (j+1))* b) << " " << radius*cos((stack / 2 - (j+1))* b)*cos((i+1)*a) << endl;
			
			myfile << cos((stack / 2 - (j+1))* b)*sin((i+1)*a) << " " << sin((stack / 2 - (j+1))* b) <<" " << cos((stack / 2 - (j+1))* b)*cos((i+1)*a)<< endl;
			
			myfile << (i+1.0)*texsl << " " << 1.0- texst << endl;
			
			//ponto
			myfile << radius*cos((stack / 2 - (j+1))* b)*sin(i*a) <<  " " << radius*sin((stack / 2 - (j+1))* b) << " " << radius*cos((stack / 2 - (j+1))* b)*cos(i*a) << endl;

			myfile << cos((stack / 2 - (j+1))* b)*sin(i*a) << " " << sin((stack / 2 - (j+1))* b)  <<" "<<cos((stack / 2 - (j+1))* b)*cos(i*a) << endl;

			myfile << (i)*texsl  << " " << 1.0-texst << endl;

			//ponto
			myfile << radius*cos((stack / 2 - j)* b)*sin(i*a) <<  " " << radius*sin((stack / 2 - j)* b) << " " << radius*cos((stack / 2 - j)* b)*cos(i*a) << endl;
			
			myfile << cos((stack / 2 - j)* b)*sin(i*a) << " " << sin((stack / 2 - j)* b) << " " << cos((stack / 2 - j)* b)*cos(i*a)<< endl;
 
			myfile << i*texsl << " " << 1-texst << endl;

			


		for(int j=1; j <= stack-1; j++ ){


			//ponto
			myfile << radius*cos((stack / 2 - j)* b)*sin((i+1)*a) <<  " " << radius*sin((stack / 2 - j)* b) << " " << radius*cos((stack / 2 - j)* b)*cos((i+1)*a) << endl;
			
			//normal
			myfile << cos((stack / 2 - j)* b)*sin((i+1)*a) <<" " << sin((stack / 2 - j)* b)  << cos((stack / 2 - j)* b)*cos((i+1)*a) << endl;

			//coordenadas de textura

			myfile << (i+1)*texsl <<" " << (stack -(j+1))*texst  << endl;

			
			//ponto
			myfile << radius*cos((stack / 2 - (j+1))* b)*sin((i+1)*a) <<  " " << radius*sin((stack / 2 - (j+1))* b) << " " << radius*cos((stack / 2 - (j+1))* b)*cos((i+1)*a) << endl;

			//normal
			myfile << cos((stack / 2 - (j+1))* b)*sin((i+1)*a) <<" " << sin((stack / 2 - (j+1))* b) << cos((stack / 2 - (j+1))* b)*cos((i+1)*a)<< endl;

			// coordenadas de textura

			myfile << (i+1)*texsl <<" " << (stack -(j+1+1))*texst  << endl;



			//ponto
			myfile << radius*cos((stack / 2 - j)* b)*sin(i*a) <<  " " << radius*sin((stack / 2 - j)* b) << " " << radius*cos((stack / 2 - j)* b)*cos(i*a) << endl;
			
			// normal
			myfile << cos((stack / 2 - j)* b)*sin(i*a)<<" " << sin((stack / 2 - j)* b)  << cos((stack / 2 - j)* b)*cos(i*a) << endl;
			
			//coordenadas de textura
			myfile << (i)*texsl <<" " << (stack -(j+1))*texst  << endl;


			


			

			//ponto
			myfile << radius*cos((stack / 2 - (j+1))* b)*sin((i+1)*a) <<  " " << radius*sin((stack / 2 - (j+1))* b) << " " << radius*cos((stack / 2 - (j+1))* b)*cos((i+1)*a) << endl;
			
			myfile << cos((stack / 2 - (j+1))* b)*sin((i+1)*a) << " " << sin((stack / 2 - (j+1))* b) <<" " << cos((stack / 2 - (j+1))* b)*cos((i+1)*a)<< endl;
			
			myfile << (i+1)*texsl <<" " << (stack -(j+1+1))*texst  << endl;

			//ponto
			myfile << radius*cos((stack / 2 - (j+1))* b)*sin(i*a) <<  " " << radius*sin((stack / 2 - (j+1))* b) << " " << radius*cos((stack / 2 - (j+1))* b)*cos(i*a) << endl;

			myfile << cos((stack / 2 - (j+1))* b)*sin(i*a) << " " << sin((stack / 2 - (j+1))* b)  <<" "<<cos((stack / 2 - (j+1))* b)*cos(i*a) << endl;

			myfile << (i)*texsl <<" " << (stack -(j+1+1))*texst  << endl;


			//ponto
			myfile << radius*cos((stack / 2 - j)* b)*sin(i*a) <<  " " << radius*sin((stack / 2 - j)* b) << " " << radius*cos((stack / 2 - j)* b)*cos(i*a) << endl;
			
			myfile << cos((stack / 2 - j)* b)*sin(i*a) << " " << sin((stack / 2 - j)* b) << " " << cos((stack / 2 - j)* b)*cos(i*a)<< endl;
 
			myfile << (i)*texsl <<" " << (stack -(j+1))*texst  << endl;

		}

			//ponto
			myfile << radius*cos((stack / 2 - j)* b)*sin((i+1)*a) <<  " " << radius*sin((stack / 2 - j)* b) << " " << radius*cos((stack / 2 - j)* b)*cos((i+1)*a) << endl;
			
			//normal
			myfile << cos((stack / 2 - j)* b)*sin((i+1)*a) <<" " << sin((stack / 2 - j)* b)  << cos((stack / 2 - j)* b)*cos((i+1)*a) << endl;

			//coordenadas de textura

			myfile << (i+1)*texsl <<" " <<0  << endl;

			
			//ponto
			myfile << radius*cos((stack / 2 - (j+1))* b)*sin((i)*a) <<  " " << radius*sin((stack / 2 - (j+1))* b) << " " << radius*cos((stack / 2 - (j+1))* b)*cos((i)*a) << endl;

			//normal
			myfile << cos((stack / 2 - (j+1))* b)*sin((i)*a) <<" " << sin((stack / 2 - (j+1))* b) << cos((stack / 2 - (j+1))* b)*cos((i)*a)<< endl;

			// coordenadas de textura

			myfile << (i)*texsl <<" " <<0  << endl;



			//ponto
			myfile << radius*cos((stack / 2 - j)* b)*sin(i*a) <<  " " << radius*sin((stack / 2 - j)* b) << " " << radius*cos((stack / 2 - j)* b)*cos(i*a) << endl;
			
			// normal
			myfile << cos((stack / 2 - j)* b)*sin(i*a)<<" " << sin((stack / 2 - j)* b)  << cos((stack / 2 - j)* b)*cos(i*a) << endl;
			
			//coordenadas de textura
			myfile << (i)*texsl <<" " <<0  << endl;

		/*	//ponto
			myfile << 0 <<  " " << -radius << " " << 0 << endl;
			
			//normal
			myfile << 0 <<" " << -1  << 0 << endl;

			//coordenadas de textura

			myfile << (i)*texsl <<" " <<0  << endl;

		*/



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
    float tx ,ty , tx2 , ty2 ,txk, tyk, tx2k, ty2k;

    for (int i = 0; i < rings; i++) {
        for (int j = 0; j <= slices; j++) {
           // -----CALCULAR------
            // PRIMEIRO
           s = i % rings ;
           t = j % (slices + 1);

           x = (raioE + raioI * cos(s * angulo1)) * cos(t * angulo2);
           y = (raioE + raioI * cos(s * angulo1)) * sin(t * angulo2);
           z = raioI * sin(s * angulo1);
           
           tx = (j)/(float)(slices + 1);
           ty = (i)/(float)rings;

            // PRIMEIROK
           s = (i + 1) % rings ;
           t = j % (slices + 1);

           xk = (raioE + raioI * cos(s * angulo1)) * cos(t * angulo2);
           yk = (raioE + raioI * cos(s * angulo1)) * sin(t * angulo2);
           zk = raioI * sin(s * angulo1);


           txk = (j)/(float)(slices + 1);
           tyk = (i + 1)/(float)rings;


                // SEGUNDO
           s = i % rings ;
           t = (j + 1) % (slices + 1);

           x2 = (raioE + raioI * cos(s * angulo1)) * cos(t * angulo2);
           y2 = (raioE + raioI * cos(s * angulo1)) * sin(t * angulo2);
           z2 = raioI * sin(s * angulo1);
           

           tx2 = (j + 1)/(float)(slices+1);
           ty2 = (i)/(float)rings;

               // SEGUNDOk
           s = (i + 1) % rings ;
           t = (j+1) % (slices + 1);

           x2k = (raioE + raioI * cos(s * angulo1)) * cos(t * angulo2);
           y2k = (raioE + raioI * cos(s * angulo1)) * sin(t * angulo2);
           z2k = raioI * sin(s * angulo1);


           tx2k = (j + 1)/(float)(slices + 1);
           ty2k = (i + 1)/(float)rings;



            // ----DESENHAR-----
			myfile << x <<  " " << y << " " << z << endl;
			myfile << cos(j % (slices + 1) * angulo2)*cos(i % rings * angulo1) <<  " " <<  sin(j % (slices + 1) * angulo2)* cos(i % rings * angulo1) << " " << sin(i % rings * angulo1) << endl;
			myfile << tx <<  " " << ty << endl;


			myfile << x2 <<  " " << y2 << " " << z2 << endl;
			myfile << cos((j + 1) % (slices + 1) * angulo2)*cos(i % rings * angulo1) <<  " " <<  sin((j + 1) % (slices + 1) * angulo2)* cos(i % rings * angulo1) << " " << sin(i % rings * angulo1) << endl;
			myfile << tx2 <<  " " << tx2 << endl;

			myfile << xk <<  " " << yk << " " << zk << endl;
			myfile << cos(j % (slices + 1) * angulo2)*cos((i + 1) % rings  * angulo1) <<  " " << sin(j % (slices + 1) * angulo2)* cos((i + 1) % rings  * angulo1) << " " << sin((i + 1) % rings  * angulo1) << endl;
			myfile << txk <<  " " << tyk << endl;


			myfile << xk <<  " " << yk << " " << zk << endl;
			myfile << cos(j % (slices + 1) * angulo2)*cos((i + 1) % rings  * angulo1) <<  " " <<  sin(j % (slices + 1) * angulo2)* cos((i + 1) % rings  * angulo1) << " " << sin((i + 1) % rings  * angulo1) << endl;
			myfile << txk <<  " " << tyk << endl;

			myfile << x2 <<  " " << y2 << " " << z2 << endl;
			myfile << cos((j + 1) % (slices + 1) * angulo2)*cos(i % rings * angulo1) <<  " " <<  sin((j + 1) % (slices + 1) * angulo2)* cos(i % rings * angulo1) << " " << sin(i % rings * angulo1) << endl;
			myfile << tx2 <<  " " << ty2 << endl;

			myfile << x2k <<  " " << y2k << " " << z2k << endl;
			myfile << cos((j+1) % (slices + 1)* angulo2)*cos((i + 1) % rings * angulo1) <<  " " <<  sin((j+1) % (slices + 1) * angulo2)* cos((i + 1) % rings * angulo1) << " " << sin((i + 1) % rings * angulo1) << endl;
			myfile << tx2k <<  " " << ty2k << endl;



        }
    }
    myfile.close();
}

/*void drawBox(float X, float Y, float Z, int D,string fileName) {

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

	float tx = (1/4) / D;
	float ty = (1/3) / D;
	int r = x, t = y, u = z;

	for (int l = 0; l < D; l++) {
		for (int c = 0; c < D; c++) {
			// frente
			myfile << x - (dx*c) <<  " " << y - (dy*l) << " " << z << endl;
			myfile << 0 << " " << 0 << " " << 1 << endl;
			myfile << (3/4) - (tx*c) <<  " " << 2/3 - (ty*l) << endl;

			myfile << x - (dx*(c + 1)) <<  " " <<  y - (dy*(l + 1)) << " " << z << endl;
			myfile << 0 << " " << 0 << " " << 1 << endl;
			myfile << (3/4) - (tx*(c + 1)) <<  " " <<  2/3  - (ty*(l+1)) << endl;

			myfile << x - (dx*c) <<  " " << y - (dy*(l + 1)) << " " << z << endl;
			myfile << 0 << " " << 0 << " " << 1 << endl;
			myfile << (3/4) - (tx*c) <<  " " << 2/3  - (ty*(l+1)) << endl;

			

			myfile << x - (dx*c) <<  " " << y - (dy*l) << " " << z << endl;
			myfile << 0 << " " << 0 << " " << 1 << endl;
			myfile << (3/4) - (tx*c) <<  " " <<  2/3 - (ty*l) << endl;

			myfile << x - (dx*(c + 1)) <<  " " << y - (dy*l) << " " << z << endl;
			myfile << 0 << " " << 0 << " " << 1 << endl;
			myfile << (3/4) - (tx*(c + 1)) <<  " " << 2/3 -(ty*l) << endl;

			myfile << x - (dx*(c + 1)) <<  " " << y - (dy*(l + 1)) << " " << z << endl;
			myfile << 0 << " " << 0 << " " << 1 << endl;
			myfile << (3/4) - (tx*(c + 1)) <<  " " << 2/3  - (dy*(l+1)) << " " << z << endl;

			


			//lado direito
			myfile << x <<  " " << y - (dy*l) << " " << z - (dz*(c + 1)) << endl;
			myfile << 1 << " " << 0 << " " << 0 << endl;
			myfile  << 3/4 + (tx*(c+1))<< " " << 2/3- (ty*l) << endl;

			myfile << x <<  " " << y - (dy*(l+1)) << " " << z - (dz*c) << endl;
			myfile << 1 << " " << 0 << " " << 0 << endl;
			myfile  << 3/4 + (tx*(c))<< " " << 2/3 - (ty*(l+1)) << endl;

			myfile << x <<  " " << y - (dy*(l+1)) << " " << z - (dz*(c + 1)) << endl;
			myfile << 1 << " " << 0 << " " << 0 << endl;
			myfile  << 3/4 + (tx*(c+1))<< " " << 2/3 - (ty*(l+1)) << endl;


			myfile << x <<  " " << y - (dy*(l + 1)) << " " << z - (dz*c) << endl;
			myfile << 1 << " " << 0 << " " << 0 << endl;
			myfile  << 3/4 + (tx*(c))<< " " << 2/3 - (ty*(l+1)) << endl;

			myfile << x <<  " " << y - (dy*l) << " " << z - (dz*(c + 1)) << endl;
			myfile << 1 << " " << 0 << " " << 0 << endl;
			myfile  << 3/4 + (tx*(c+1))<< " " << 2/3 - (ty*(l)) << endl;

			myfile << x <<  " " << y - (dy*l) << " " << z - (dz*c) << endl;
			myfile << 1 << " " << 0 << " " << 0 << endl;
			myfile  << 3/4 + (tx*(c))<< " " << 2/3 - (ty*(l)) << endl;


			//lado esquerdo
			myfile << -x <<  " " << y - (dy*l) << " " << z - (dz*c) << endl;
			myfile << -1 << " " << 0 << " " << 0 << endl;
			myfile  << 2/4 + (tx*(c))<< " " << 2/3 - (ty*(l)) << endl;

			myfile << -x <<  " " << y - (dy*(l + 1)) << " " << z - (dz*(c + 1)) << endl;
			myfile << -1 << " " << 0 << " " << 0 << endl;
			myfile  << 2/4 + (tx*(c+1))<< " " << 2/3 - (ty*(l+1)) << endl;

			myfile << -x <<  " " << y - (dy*(l + 1)) << " " << z - (dz*c) << endl;
			myfile << -1 << " " << 0 << " " << 0 << endl;
			myfile  << 2/4 + (tx*(c))<< " " << 2/3 - (ty*(l+1)) << endl;

			

			myfile << -x <<  " " << y - (dy*(l + 1)) << " " << z - (dz*(c + 1)) << endl;
			myfile << -1 << " " << 0 << " " << 0 << endl;
			myfile  << 2/4 + (tx*(c+1))<< " " << 2/3 - (ty*(l+1)) << endl;

			myfile << -x <<  " " << y - (dy*l) << " " << z - (dz*c) << endl;
			myfile << -1 << " " << 0 << " " << 0 << endl;
			myfile  << 2/4 + (tx*(c))<< " " << 2/3 - (ty*(l)) << endl;

			myfile << -x <<  " " << y - (dy*l) << " " << z - (dz*(c + 1)) << endl;
			myfile << -1 << " " << 0 << " " << 0 << endl;
			myfile  << 2/4 + (tx*(c+1))<< " " << 2/3 - (ty*(l)) << endl;


			// lado de tras
			myfile << x - (dx*(c + 1)) <<  " " << y - (dy*l) << " " << -z << endl;
			myfile << 0 << " " << 0 << " " << -1 << endl;
			myfile << (tx*(c + 1)) <<  " " << 2/3  - (ty*l) << endl;

			myfile << x - (dx*c) <<  " " << y - (dy*(l + 1)) << " " << -z << endl;
			myfile << 0 << " " << 0 << " " << -1 << endl;
			myfile << (tx*(c)) <<  " " << 2/3  - (ty*(l+1)) << endl;

			myfile << x - (dx*(c + 1)) <<  " " << y - (dy*(l + 1)) << " " << -z << endl;
			myfile << 0 << " " << 0 << " " << -1 << endl;
			myfile << (tx*(c+1)) <<  " " << 2/3  - (ty*(l+1)) << endl;


			myfile << x - (dx*c) <<  " " << y - (dy*(l + 1)) << " " << -z << endl;
			myfile << 0 << " " << 0 << " " << -1 << endl;
			myfile << (tx*(c)) <<  " " << 2/3  - (ty*(l+1)) << endl;

			myfile << x - (dx*(c + 1)) <<  " " << y - (dy*l) << " " << -z << endl;
			myfile << 0 << " " << 0 << " " << -1 << endl;
			myfile << (tx*(c+1)) <<  " " << 2/3  - (ty*(l)) << endl;

			myfile << x - (dx*c) <<  " " << y - (dy*l) << " " << -z<< endl;
			myfile << 0 << " " << 0 << " " << -1 << endl;
			myfile << (tx*(c)) <<  " " << 2/3  - (ty*(l) )<< endl;

			

			//topo
			myfile << x - (dx*c) <<  " " << y << " " << z - (dz*(l + 1)) << endl;
			myfile << 0 << " " << 1 << " " << 0 << endl;
			myfile << (3/4) - (tx*c) <<  " " << 2/3  + (ty*(l + 1)) << endl;
			
			myfile << x - (dx*(c + 1)) <<  " " << y << " " <<  z - (dz*l) << endl;
			myfile << 0 << " " << 1 << " " << 0 << endl;
			myfile << (3/4) - (tx*(c+1)) <<  " " << 2/3  + (ty*(l)) << endl;

			myfile << x - (dx*c) <<  " " << y << " " << z - (dz*l) << endl;
			myfile << 0 << " " << 1 << " " << 0 << endl;
			myfile << (3/4) - (tx*(c)) <<  " " << 2/3  + (ty*(l)) << endl;


			

			myfile << x - (dx*(c + 1)) <<  " " << y << " " << z - (dz*l) << endl;
			myfile << 0 << " " << 1 << " " << 0 << endl;
			myfile << (3/4) - (tx*(c+1)) <<  " " << 2/3  + (ty*(l)) << endl;

			myfile << x - (dx*c) <<  " " << y << " " << z - (dz*(l + 1)) << endl;
			myfile << 0 << " " << 1 << " " << 0 << endl;
			myfile << (3/4) - (tx*(c)) <<  " " << 2/3  + (ty*(l+1)) << endl;

			myfile << x - (dx*(c + 1)) <<  " " << y << " " << z - (dz*(l + 1)) << endl;
			myfile << 0 << " " << 1 << " " << 0 << endl;
			myfile << (3/4) - (tx*(c+1)) <<  " " << 2/3  + (ty*(l+1)) << endl;


			//base
			myfile << x - (dx*c) <<  " " << -y << " " << z - (dz*l) << endl;
			myfile << 0 << " " << -1 << " " << 0 << endl;
			myfile << (3/4) - (tx*(c)) <<  " " << 2/3  - (ty*(l)) << endl;

			myfile << x - (dx*(c + 1)) <<  " " << -y << " " << z - (dz*(l + 1)) << endl;
			myfile << 0 << " " << -1 << " " << 0 << endl;
			myfile << (3/4) - (tx*(c+1)) <<  " " << 2/3  - (ty*(l+1)) << endl;

			myfile << x - (dx*c) <<  " " << -y << " " << z - (dz*(l + 1)) << endl;
			myfile << 0 << " " << -1 << " " << 0 << endl;
			myfile << (3/4) - (tx*(c)) <<  " " << 2/3  - (ty*(l+1)) << endl;


			

			myfile << x - (dx*(c + 1)) <<  " " << -y << " " << z - (dz*(l + 1)) << endl;
			myfile << 0 << " " << -1 << " " << 0 << endl;
			myfile << (3/4) - (tx*(c+1)) <<  " " << 2/3  - (ty*(l+1)) << endl;

			myfile << x - (dx*c) <<  " " << -y << " " << z - (dz*l) << endl;
			myfile << 0 << " " << -1 << " " << 0 << endl;
			myfile << (3/4) - (tx*(c)) <<  " " << 2/3  - (ty*(l)) << endl;

			myfile << x - (dx*(c + 1)) <<  " " << -y << " " << z - (dz*l) << endl;
			myfile << 0 << " " << -1 << " " << 0 << endl;
			myfile << (3/4) - (tx*(c+1)) <<  " " << 2/3  - (ty*(l)) << endl;

		}
	}
	myfile.close();
}*/

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

	float tx = (1/4) / D;
	float ty = (1/3) / D;
	int r = x, t = y, u = z;

	for (int l = 0; l < D; l++) {
		for (int c = 0; c < D; c++) {
			// frente
			myfile << x - (dx*c) <<  " " << y - (dy*l) << " " << z << endl;
			myfile << 0 << " " << 0 << " " << 1 << endl;
			myfile << (3/4) - (tx*c) <<  " " << 2/3 - (ty*l) << endl;

			myfile << x - (dx*(c + 1)) <<  " " <<  y - (dy*(l + 1)) << " " << z << endl;
			myfile << 0 << " " << 0 << " " << 1 << endl;
			myfile << (3/4) - (tx*(c + 1)) <<  " " <<  2/3  - (ty*(l+1)) << endl;

			myfile << x - (dx*c) <<  " " << y - (dy*(l + 1)) << " " << z << endl;
			myfile << 0 << " " << 0 << " " << 1 << endl;
			myfile << (3/4) - (tx*c) <<  " " << 2/3  - (ty*(l+1)) << endl;

			

			myfile << x - (dx*c) <<  " " << y - (dy*l) << " " << z << endl;
			myfile << 0 << " " << 0 << " " << 1 << endl;
			myfile << (3/4) - (tx*c) <<  " " <<  2/3 - (ty*l) << endl;

			myfile << x - (dx*(c + 1)) <<  " " << y - (dy*l) << " " << z << endl;
			myfile << 0 << " " << 0 << " " << 1 << endl;
			myfile << (3/4) - (tx*(c + 1)) <<  " " << 2/3 -(ty*l) << endl;

			myfile << x - (dx*(c + 1)) <<  " " << y - (dy*(l + 1)) << " " << z << endl;
			myfile << 0 << " " << 0 << " " << 1 << endl;
			myfile << (3/4) - (tx*(c + 1)) <<  " " << 2/3  - (dy*(l+1)) << " " << z << endl;

			


			//lado direito
			myfile << x <<  " " << y - (dy*l) << " " << z - (dz*(c + 1)) << endl;
			myfile << 1 << " " << 0 << " " << 0 << endl;
			myfile  << 3/4 + (tx*(c+1))<< " " << 2/3- (ty*l) << endl;

			myfile << x <<  " " << y - (dy*(l+1)) << " " << z - (dz*c) << endl;
			myfile << 1 << " " << 0 << " " << 0 << endl;
			myfile  << 3/4 + (tx*(c))<< " " << 2/3 - (ty*(l+1)) << endl;

			myfile << x <<  " " << y - (dy*(l+1)) << " " << z - (dz*(c + 1)) << endl;
			myfile << 1 << " " << 0 << " " << 0 << endl;
			myfile  << 3/4 + (tx*(c+1))<< " " << 2/3 - (ty*(l+1)) << endl;


			myfile << x <<  " " << y - (dy*(l + 1)) << " " << z - (dz*c) << endl;
			myfile << 1 << " " << 0 << " " << 0 << endl;
			myfile  << 3/4 + (tx*(c))<< " " << 2/3 - (ty*(l+1)) << endl;

			myfile << x <<  " " << y - (dy*l) << " " << z - (dz*(c + 1)) << endl;
			myfile << 1 << " " << 0 << " " << 0 << endl;
			myfile  << 3/4 + (tx*(c+1))<< " " << 2/3 - (ty*(l)) << endl;

			myfile << x <<  " " << y - (dy*l) << " " << z - (dz*c) << endl;
			myfile << 1 << " " << 0 << " " << 0 << endl;
			myfile  << 3/4 + (tx*(c))<< " " << 2/3 - (ty*(l)) << endl;


			//lado esquerdo
			myfile << -x <<  " " << y - (dy*l) << " " << z - (dz*c) << endl;
			myfile << -1 << " " << 0 << " " << 0 << endl;
			myfile  << 2/4 - (tx*(c))<< " " << 2/3 - (ty*(l)) << endl;

			myfile << -x <<  " " << y - (dy*(l + 1)) << " " << z - (dz*(c + 1)) << endl;
			myfile << -1 << " " << 0 << " " << 0 << endl;
			myfile  << 2/4 - (tx*(c+1))<< " " << 2/3 - (ty*(l+1)) << endl;

			myfile << -x <<  " " << y - (dy*(l + 1)) << " " << z - (dz*c) << endl;
			myfile << -1 << " " << 0 << " " << 0 << endl;
			myfile  << 2/4 - (tx*(c))<< " " << 2/3 - (ty*(l+1)) << endl;

			

			myfile << -x <<  " " << y - (dy*(l + 1)) << " " << z - (dz*(c + 1)) << endl;
			myfile << -1 << " " << 0 << " " << 0 << endl;
			myfile  << 2/4 - (tx*(c+1))<< " " << 2/3 - (ty*(l+1)) << endl;

			myfile << -x <<  " " << y - (dy*l) << " " << z - (dz*c) << endl;
			myfile << -1 << " " << 0 << " " << 0 << endl;
			myfile  << 2/4 - (tx*(c))<< " " << 2/3 - (ty*(l)) << endl;

			myfile << -x <<  " " << y - (dy*l) << " " << z - (dz*(c + 1)) << endl;
			myfile << -1 << " " << 0 << " " << 0 << endl;
			myfile  << 2/4 - (tx*(c+1))<< " " << 2/3 - (ty*(l)) << endl;


			// lado de tras
			myfile << x - (dx*(c + 1)) <<  " " << y - (dy*l) << " " << -z << endl;
			myfile << 0 << " " << 0 << " " << -1 << endl;
			myfile << (1/4)-(tx*(c + 1)) <<  " " << 2/3  - (ty*l) << endl;

			myfile << x - (dx*c) <<  " " << y - (dy*(l + 1)) << " " << -z << endl;
			myfile << 0 << " " << 0 << " " << -1 << endl;
			myfile << (1/4)-(tx*(c)) <<  " " << 2/3  - (ty*(l+1)) << endl;

			myfile << x - (dx*(c + 1)) <<  " " << y - (dy*(l + 1)) << " " << -z << endl;
			myfile << 0 << " " << 0 << " " << -1 << endl;
			myfile << (1/4)-(tx*(c+1)) <<  " " << 2/3  - (ty*(l+1)) << endl;


			myfile << x - (dx*c) <<  " " << y - (dy*(l + 1)) << " " << -z << endl;
			myfile << 0 << " " << 0 << " " << -1 << endl;
			myfile << (1/4)-(tx*(c)) <<  " " << 2/3  - (ty*(l+1)) << endl;

			myfile << x - (dx*(c + 1)) <<  " " << y - (dy*l) << " " << -z << endl;
			myfile << 0 << " " << 0 << " " << -1 << endl;
			myfile << (1/4)-(tx*(c+1)) <<  " " << 2/3  - (ty*(l)) << endl;

			myfile << x - (dx*c) <<  " " << y - (dy*l) << " " << -z<< endl;
			myfile << 0 << " " << 0 << " " << -1 << endl;
			myfile << (1/4)-(tx*(c)) <<  " " << 2/3  - (ty*(l) )<< endl;

			

			//topo
			myfile << x - (dx*c) <<  " " << y << " " << z - (dz*(l + 1)) << endl;
			myfile << 0 << " " << 1 << " " << 0 << endl;
			myfile << (3/4) - (tx*c) <<  " " << 2/3  + (ty*(l + 1)) << endl;
			
			myfile << x - (dx*(c + 1)) <<  " " << y << " " <<  z - (dz*l) << endl;
			myfile << 0 << " " << 1 << " " << 0 << endl;
			myfile << (3/4) - (tx*(c+1)) <<  " " << 2/3  + (ty*(l)) << endl;

			myfile << x - (dx*c) <<  " " << y << " " << z - (dz*l) << endl;
			myfile << 0 << " " << 1 << " " << 0 << endl;
			myfile << (3/4) - (tx*(c)) <<  " " << 2/3  + (ty*(l)) << endl;


			

			myfile << x - (dx*(c + 1)) <<  " " << y << " " << z - (dz*l) << endl;
			myfile << 0 << " " << 1 << " " << 0 << endl;
			myfile << (3/4) - (tx*(c+1)) <<  " " << 2/3  + (ty*(l)) << endl;

			myfile << x - (dx*c) <<  " " << y << " " << z - (dz*(l + 1)) << endl;
			myfile << 0 << " " << 1 << " " << 0 << endl;
			myfile << (3/4) - (tx*(c)) <<  " " << 2/3  + (ty*(l+1)) << endl;

			myfile << x - (dx*(c + 1)) <<  " " << y << " " << z - (dz*(l + 1)) << endl;
			myfile << 0 << " " << 1 << " " << 0 << endl;
			myfile << (3/4) - (tx*(c+1)) <<  " " << 2/3  + (ty*(l+1)) << endl;


			//base
			myfile << x - (dx*c) <<  " " << -y << " " << z - (dz*l) << endl;
			myfile << 0 << " " << -1 << " " << 0 << endl;
			myfile << (3/4) - (tx*(c)) <<  " " << 1/3  - (ty*(l)) << endl;

			myfile << x - (dx*(c + 1)) <<  " " << -y << " " << z - (dz*(l + 1)) << endl;
			myfile << 0 << " " << -1 << " " << 0 << endl;
			myfile << (3/4) - (tx*(c+1)) <<  " " << 1/3  - (ty*(l+1)) << endl;

			myfile << x - (dx*c) <<  " " << -y << " " << z - (dz*(l + 1)) << endl;
			myfile << 0 << " " << -1 << " " << 0 << endl;
			myfile << (3/4) - (tx*(c)) <<  " " << 1/3  - (ty*(l+1)) << endl;


			

			myfile << x - (dx*(c + 1)) <<  " " << -y << " " << z - (dz*(l + 1)) << endl;
			myfile << 0 << " " << -1 << " " << 0 << endl;
			myfile << (3/4) - (tx*(c+1)) <<  " " << 1/3  - (ty*(l+1)) << endl;

			myfile << x - (dx*c) <<  " " << -y << " " << z - (dz*l) << endl;
			myfile << 0 << " " << -1 << " " << 0 << endl;
			myfile << (3/4) - (tx*(c)) <<  " " << 1/3  - (ty*(l)) << endl;

			myfile << x - (dx*(c + 1)) <<  " " << -y << " " << z - (dz*l) << endl;
			myfile << 0 << " " << -1 << " " << 0 << endl;
			myfile << (3/4) - (tx*(c+1)) <<  " " << 1/3  - (ty*(l)) << endl;

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
	float texst=1.0f/stacks;
	float texsl=1.0f/slices;

    for(int x = 0 ; x< slices ; x++ ){

    	myfile << 0 << " " << 0 << " " << 0 << endl;
		myfile <<  0  << " " << -1 << " " << 0 << endl;
		myfile <<  x*texsl  << " " <<1 << endl;


		for (int j = stacks-1; j >= 0; j--)
		{
			myfile << ((radius*j)/stacks)*sin(x*angulo) << " " << h - (h/stacks)*j << " " << ((radius*j) / stacks)*cos(x*angulo) << endl;
			myfile <<  0  << " " << -1 << " " << 0 << endl;
			myfile <<  x*texsl  << " " << 1-j*texst << endl;

			
			myfile << ((radius*(j+1))/stacks)*sin(x*angulo) << " " << h - (h/stacks)*(j+1) << " " << ((radius*(j+1)) / stacks)*cos(x*angulo) << endl;
			myfile <<  0  << " " << -1 << " " << 0 << endl;
			myfile <<  x*texsl  << " " << 1-(j+1)*texst << endl;
			

			myfile << ((radius*j) / stacks)*sin(x*angulo + angulo) << " " << h - (h/ stacks)*j << " " << ((radius*j) / stacks)*cos(x*angulo + angulo) << endl;
			myfile <<  0  << " " << -1 << " " << 0 << endl;
			myfile <<  x*texsl  << " " << 1-j*texst << endl;

			myfile << ((radius*j) / stacks)*sin(x*angulo + angulo) << " " << h - (h/ stacks)*j << " " << ((radius*j) / stacks)*cos(x*angulo + angulo) << endl;
			myfile <<  0  << " " << -1 << " " << 0 << endl;
			myfile <<  x*texsl  << " " << 1-j*texst << endl;
			

			myfile << ((radius*(j+1))/stacks)*sin(x*angulo) << " " << h - (h/stacks)*(j+1) << " " << ((radius*(j+1)) / stacks)*cos(x*angulo) << endl;
			myfile <<  0  << " " << -1 << " " << 0 << endl;
			myfile <<  x*texsl  << " " << 1-(j+1)*texst << endl;
			
			myfile << ((radius*(j+1)) / stacks)*sin(x*angulo + angulo) << " " << h - (h/ stacks)*(j+1) << " " << ((radius*(j+1)) / stacks)*cos(x*angulo + angulo) << endl;
			myfile <<  0  << " " << -1 << " " << 0 << endl;
			myfile <<  x*texsl  << " " << 1-(j+1)*texst << endl;
	
		}

		myfile << radius*sin(x*angulo) << " " << "0.0" << " " << radius*cos(x*angulo) << endl;
		myfile <<  0  << " " << -1 << " " << 0 << endl;
		myfile <<  x*texsl  << " " << 1-texst << endl;
		

		myfile << radius*sin(x*angulo + angulo) << " " << "0.0" << " " << radius*cos(x*angulo + angulo) << endl;
		myfile <<  0  << " " << -1 << " " << 0 << endl;
		myfile <<  x*texsl  << " " << 1-texst << endl;


		myfile << "0.0" << " " << "0.0" << " " << "0.0" << endl;
		myfile <<  0  << " " << -1 << " " << 0 << endl;
		myfile <<  x*texsl  << " " << 1-texst << endl;




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
	myfile << 0 << " " << 1 << " " << 0 << endl;
	myfile << 1 << " " << 1 << endl;

	myfile << -x << " " << "0.0" << " " << z << endl;
	myfile << 0 << " " << 1 << " " << 0 << endl;
	myfile << 0 << " " << 0 << endl;

	myfile << x << " " << "0.0" << " " << z << endl;
	myfile << 0 << " " << 1 << " " << 0 << endl;
	myfile << 1<< " " << 0 << endl;



	myfile << -x << " " << "0.0" << " " << z << endl;
	myfile << 0 << " " << 1 << " " << 0 << endl;
	myfile << 0 << " " << 0 << endl;


	myfile << x << " " << "0.0" << " " << -z << endl;
	myfile << 0 << " " << 1 << " " << 0 << endl;
	myfile << 1 << " " << 1 << endl;


	myfile << -x << " " << "0.0" << " " << -z << endl;
	myfile << 0 << " " << 1 << " " << 0 << endl;
	myfile << 0 << " " << 1 << endl;



	
	myfile << -x << " " << "0.0" << " " << z << endl;
	myfile << 0 << " " << -1 << " " << 0 << endl;
	myfile << 0 << " " << 0 << endl;


	myfile << x << " " << "0.0" << " " << -z << endl;
	myfile << 0 << " " << -1 << " " << 0 << endl;
	myfile << 1 << " " << 1 << endl;


	myfile << x << " " << "0.0" << " " << z << endl;
	myfile << 0 << " " << -1 << " " << 0 << endl;
	myfile << 1<< " " << 0 << endl;



	myfile << x << " " << "0.0" << " " << -z << endl;
	myfile << 0 << " " << -1 << " " << 0 << endl;
	myfile << 1 << " " << 1 << endl;

	myfile << -x << " " << "0.0" << " " << z << endl;
	myfile << 0 << " " << -1 << " " << 0 << endl;
	myfile << 0 << " " << 0 << endl;


	myfile << -x << " " << "0.0" << " " << -z << endl;
	myfile << 0 << " " << -1 << " " << 0 << endl;
	myfile << 0 << " " << 1 << endl;



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
