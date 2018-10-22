#pragma once
#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <string>
#include <glut.h>
#include <math.h>
#include <OpenMesh/Core/IO/MeshIO.hh>  
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>

BOOL WriteBitmapFile(char * filename, int width, int height, unsigned char * bitmapData);
void SaveScreenShot(char * filename, int width, int height);