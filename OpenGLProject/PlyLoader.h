#pragma once

#include <glut.h>
#include <gl/glu.h>
#include <gl/gl.h>
#include <vector>
#include <iostream>
using namespace std;

struct SModelData
{
	vector <float> vecFaceTriangles; // = face * 9
	vector <float> vecFaceTriangleColors; // = face * 9
	vector <float> vecNormals; // = face * 9
	int iTotalConnectedTriangles;
};

class CPLYLoader
{
public:
	CPLYLoader();
	int LoadModel(char *filename);
	void Draw();

private:
	float* mp_vertexXYZ;
	float* mp_vertexNorm;
	float* mp_vertexRGB;
	int m_totalConnectedQuads;
	int m_totalConnectedPoints;
	int m_totalFaces;
	SModelData m_ModelData;
};
