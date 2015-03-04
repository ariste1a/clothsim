#pragma once
#include "../token.h"
#include "../vector3.h"
#include <vector>
#include "../matrix34.h"
#include "Skeleton.h"
#include "skinweight.h"
#include "BMPImage.h"
class skin
{
public:
	skin();
	bool load(const char *file);
	bool load(Tokenizer &t);
	std::vector<Vector3*> positions; 
	std::vector<Vector3*> posPrime;
	std::vector<Vector3*> normals;
	std::vector<Vector3*> normalsPrime; 
	std::vector<skinweight*> skinWeights;
	std::vector<Vector3*> triangles;
	std::vector<Matrix34*> bindings;	
	std::vector<Vector3*> texcoords; 
	Skeleton *skel; 
	void bind(Skeleton* skel);
	void skin::draw();
	void skin::update(Skeleton* skel);
	BMPImage textureLoader = *new BMPImage(); 
	bool skin::loadMorph(const char * file);
	std::string texName;
	~skin();
};

