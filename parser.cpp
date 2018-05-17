
/*
* type:
* 	1: Point;
*	2: Directional;
*	3: Spot;
*/
typedef struct sLight
{
	int type;
	float xx;
	float yy;
	float zz;
}Light,*pLight;



void lightInit(pLight l)
{
	l->type=0;
	l->xx = 0.0f;
	l->yy = 0.0f;
	l->zz = 0.0f;	
} 


void parserLights(XMLElement* pLight){
	XMLElement *aux, *children;
	const char *attributeText = nullptr;
	float lx=0,ly=0,lz=0;

	aux = pLight->FirstChildElement("light");

	while(aux!=nullptr){
		Light light;
		lightInit(&light);

		aux->QueryStringAttribute("type", &attributeText);
		aux->QueryFloatAttribute("X", &lx);
		aux->QueryFloatAttribute("Y", &ly);
		aux->QueryFloatAttribute("Z", &lz);


		if( (strcmp(attributeText,"POINT"))==0)
			light.type=1;
		else if( (strcmp(attributeText,"DIRECTIONAL"))==0)
			light.type=2;
		else if( (strcmp(attributeText,"SPOT"))==0)
			light.type=3;

		light.xx=lx;
		light.yy=ly;
		light.zz=lz;

		vecLight.push_back(light);

		children = pLight->FirstChildElement("light");
		cout << "TIPO-> " << attributeText << endl;

		cout << "child " << children->Value() << endl;

		aux = aux->NextSiblingElement();
	}
}

Na main:


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


	XMLElement* pLights = pRoot->FirstChildElement("lights");
	if (pLights == nullptr)
	{
		cout << "Light element error.";
		return XML_ERROR_PARSING_ELEMENT;
	}

	while (pLights != nullptr)
	{
		parserLights(pLights);
		pLights = pLights->NextSiblingElement();
	}


//	cout << "XML OK" << endl;
// ----------------
/*
	cout << "NUM FIGURA: " << numFigura << endl;

	cout << " Struct: " << endl;
	for(int i=0;i<vecTransform.size();i++){
		cout << endl;
		cout << "drawUntil: " << vecTransform[i].drawUntil << endl;
		cout << "x: " << vecTransform[i].xx << endl;
		cout << "y: " << vecTransform[i].yy << endl;
		cout << "z: " << vecTransform[i].zz << endl;
		cout << "type: " << vecTransform[i].type << endl;
		cout << "level: " << vecTransform[i].level << endl;
		cout << "Texture : " << vecTransform[i].file << endl;
		cout << "canDraw: " << vecTransform[i].canDraw << endl;
		for(int j=0;j< vecTransform[i].points.size();j++)
			cout << "elem " << j <<" : " <<  vecTransform[i].points[j] << endl;
		cout << "time: " << vecTransform[i].time <<  endl;
	}
*/
	cout << endl;
	cout << "Struct Lights: " << endl;
	for(int i=0;i<vecLight.size();i++){
		cout << endl;
		cout << "x: " << vecLight[i].xx << endl;
		cout << "y: " << vecLight[i].yy << endl;
		cout << "z: " << vecLight[i].zz << endl;
		cout << "Type: " << vecLight[i].type << endl;
	}
