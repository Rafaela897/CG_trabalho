// nova estrutura. Tá uma sucatada mas deve funcionar.


/*
* drawUntil: Utilizado para saber quantos ficheiros podemos desenhar de uma vez, caso haja v�rios no mesmo block.
* xx,yy,zz: valores para as transforma��es;
* angle: para o rotate;
* type:
* 	1: Translate;
*	2: Rotate;
*	3: Scale;
*	4: Color;
*   5: Diffuse;
*	6: Specular;
*	7: Emissive;
*	8: Ambient;
*	9: Texture file;
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
	string file;
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
	t->file="";
}








// Substituir o parserXML por estas duas funções.

bool testDraw(XMLElement* Elem){
	if ( (Elem->Attribute("diffR"))!= 0 || (Elem->Attribute("diffG"))!= 0 || (Elem->Attribute("diffB"))!= 0 ||
		 (Elem->Attribute("specR"))!= 0 || (Elem->Attribute("specG"))!= 0 || (Elem->Attribute("specB"))!= 0	||
		 (Elem->Attribute("emisR"))!= 0 || (Elem->Attribute("emisG"))!= 0 || (Elem->Attribute("emisB"))!= 0 ||
		 (Elem->Attribute("ambiR"))!= 0 || (Elem->Attribute("ambiG"))!= 0 || (Elem->Attribute("ambiB"))!= 0 ||
		 (Elem->Attribute("texture"))!= 0 )
			return false;
	
	else
		return true;
}


void parserXML(XMLElement* pGroup,int numF,int curLevel) {
	XMLElement *pModel,*children, *aux, *elemTempo, *toUseOutside;
	const char *attributeText = nullptr;
	const char *toIgnore = nullptr;
	bool skip=false, ilumination=false;


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


	while( true ){  // (strcmp(aux->Value(),"models")!=0)
		
		if (strcmp(aux->Value(),"translate")==0){
			float tx=0,ty=0,tz=0,tempo=0;

			Transformation transf;
			transformInit(&transf);

// gerir canDraw
			if(strcmp("models",aux->NextSiblingElement()->Value()) != 0){	// i.e. se existem mais transforma��es para este modelo
				transf.canDraw=false;
			}

			if(strcmp("models",aux->NextSiblingElement()->Value()) == 0){
				toUseOutside = pGroup->FirstChildElement("models")->FirstChildElement("model");

				transf.canDraw=testDraw(toUseOutside);
			}
// 

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

//gerir canDraw
			if(strcmp("models",aux->NextSiblingElement()->Value()) != 0){	// i.e. se existem mais transforma��es para este modelo
				transf.canDraw=false;
			}

			if(strcmp("models",aux->NextSiblingElement()->Value()) == 0){
				toUseOutside = pGroup->FirstChildElement("models")->FirstChildElement("model");

				transf.canDraw=testDraw(toUseOutside);	
			}
//

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
			
// gerir canDraw
			if(strcmp("models",aux->NextSiblingElement()->Value()) != 0){	// i.e. se existem mais transforma��es para este modelo
				transf.canDraw=false;
			}
			if(strcmp("models",aux->NextSiblingElement()->Value()) == 0){
				toUseOutside = pGroup->FirstChildElement("models")->FirstChildElement("model");

				transf.canDraw=testDraw(toUseOutside);	
			}
//

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

//gerir canDraw			
			if(strcmp("models",aux->NextSiblingElement()->Value()) != 0){	// i.e. se existem mais transforma��es para este modelo
				transf.canDraw=false;
			}
			if(strcmp("models",aux->NextSiblingElement()->Value()) == 0){
				toUseOutside = pGroup->FirstChildElement("models")->FirstChildElement("model");

				transf.canDraw=testDraw(toUseOutside);	
			}
//


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


		else if( (strcmp(aux->Value(),"models")==0) ){
			pModel = pGroup->FirstChildElement("models")->FirstChildElement("model");
			float r=0, g=0, b=0;

			Transformation transf;
			transformInit(&transf);

			pModel->QueryStringAttribute("file", &toIgnore);
			pModel->QueryStringAttribute("texture", &attributeText);


			// Diffuse
			pModel->QueryFloatAttribute("diffR", &r);
			pModel->QueryFloatAttribute("diffG", &g);
			pModel->QueryFloatAttribute("diffB", &b);
			if (r>0 || g>0 || b>0){
				transf.drawUntil=numF;
				transf.type=5;
				transf.level=curLevel;
				transf.xx=r;
				transf.yy=g;
				transf.zz=b;
				vecTransform.push_back(transf);
				ilumination=true;
			}
			r=g=b=0;


			// Specular
			pModel->QueryFloatAttribute("specR", &r);
			pModel->QueryFloatAttribute("specG", &g);
			pModel->QueryFloatAttribute("specB", &b);
			if (r>0 || g>0 || b>0){
				transf.drawUntil=numF;
				transf.type=6;
				transf.level=curLevel;
				transf.xx=r;
				transf.yy=g;
				transf.zz=b;
				vecTransform.push_back(transf);
				ilumination=true;
			}
			r=g=b=0;


			// Emissive
			pModel->QueryFloatAttribute("emisR", &r);
			pModel->QueryFloatAttribute("emisG", &g);
			pModel->QueryFloatAttribute("emisB", &b);
			if (r>0 || g>0 || b>0){
				transf.drawUntil=numF;
				transf.type=7;
				transf.level=curLevel;
				transf.xx=r;
				transf.yy=g;
				transf.zz=b;
				vecTransform.push_back(transf);
				ilumination=true;
			}
			r=g=b=0;


			// Ambient
			pModel->QueryFloatAttribute("ambiR", &r);
			pModel->QueryFloatAttribute("ambiG", &g);
			pModel->QueryFloatAttribute("ambiB", &b);
			if (r>0 || g>0 || b>0){
				transf.drawUntil=numF;
				transf.type=8;
				transf.level=curLevel;
				transf.xx=r;
				transf.yy=g;
				transf.zz=b;
				vecTransform.push_back(transf);
				ilumination=true;
			}
			r=g=b=0;
			

			if ( (strcmp(toIgnore,attributeText))==0 ){ // se entrar aqui não tem texturas
				attributeText="";
/*				
				if (!ilumination){ // grupo sem qualquer transformação/textura/iluminação
					transf.drawUntil=numF;
					transf.type=0;
					transf.level=curLevel;
					transf.xx=r;
					transf.yy=g;
					transf.zz=b;
					vecTransform.push_back(transf);
				}
*/
				ilumination=false;
				skip=true;
			}
			else{ // Há textura
				transf.drawUntil=numF;
				transf.type=9;
				transf.level=curLevel;
				transf.file=attributeText;
				vecTransform.push_back(transf);
				skip=true;
			}
		}

		if (skip){
			skip=false;
			break;
		}
		else {
			aux=aux->NextSiblingElement();
		}
	}

// Chamada recursiva
	children = pGroup->FirstChildElement("group");
	while(children!=nullptr){
		parserXML(children,numFigura,curLevel+1);
		children = children->NextSiblingElement();
	}
}
