#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS

// To Draw RGB and D img.

#include <iostream>
#include <fstream>
#include <string>
#include <glut.h>
#include <math.h>
#include <OpenMesh/Core/IO/MeshIO.hh>  
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include "glScreenShot.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define VIEWPOINT_FARZ -200
#define VIEWPOINT_NEARZ 500
#define DEPTH_FARZ -100
#define DEPTH_NEARZ 200

using namespace std;

typedef OpenMesh::PolyMesh_ArrayKernelT<> MyMesh;
MyMesh mesh;
string plyFilePath;
string plyFileName;

int drawWhat = 0;
int drawIndex = 0;

float rotateAngle[10][2] = { 
	{ 0, 0 },
	{ 0, 30 },
	{ 0, -30 },
	{ 0, 90 },
	{ 0, -90},
	{ 30, 0 },
	{ 30, 45 },
	{ 30, -45 },
	{ -30, 30 },
	{ -30, -30 },
};

void rotateMesh(MyMesh *thisMesh, double xAngle, double yAngle, double zAngle);

void drawFunc(void) {
	// rotateMesh
	MyMesh mesh1 = MyMesh(mesh);
	rotateMesh(&mesh1, rotateAngle[drawIndex][0], rotateAngle[drawIndex][1], 0);

	// start to draw
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPushMatrix();
	glBegin(GL_TRIANGLES);

	if (drawWhat == 0) {
		// Draw RGB
		for (MyMesh::FaceIter f_it = mesh1.faces_begin(); f_it != mesh1.faces_end(); ++f_it)
		{
			for (MyMesh::FVIter fv_it = mesh1.fv_begin(*f_it); fv_it != mesh1.fv_end(*f_it); ++fv_it)
			{
				glColor3ubv(mesh1.color(*fv_it).data());
				glVertex3fv(mesh1.point(*fv_it).data());
			}
		}
	}
	else if (drawWhat == 1) {
		// Draw D
		for (MyMesh::FaceIter f_it = mesh1.faces_begin(); f_it != mesh1.faces_end(); ++f_it)
		{
			for (MyMesh::FVIter fv_it = mesh1.fv_begin(*f_it); fv_it != mesh1.fv_end(*f_it); ++fv_it)
			{
				// our side is positive
				double depthZ = mesh1.point(*fv_it).data()[2];
				double val = (depthZ - DEPTH_FARZ) / (DEPTH_NEARZ - DEPTH_FARZ);

				glColor3d(val, val, val);
				glVertex3fv(mesh1.point(*fv_it).data());
			}
		}
	}
	glEnd();
	glPopMatrix();
	glFlush();

	// save
	string str;
	if (drawWhat == 0) {
		str = plyFileName + "_" + to_string(drawIndex) + "_rgb.bmp";
		drawWhat = 1;
	}
	else if (drawWhat == 1) {
		str = plyFileName + "_" + to_string(drawIndex) + "_d.bmp";
		drawWhat = 0;
		drawIndex++;
		if (drawIndex >= 10)
			exit(0);
	}

	char *name = new char[str.length() + 1];
	strcpy(name, str.c_str());
	SaveScreenShot(name, WINDOW_WIDTH, WINDOW_HEIGHT);
	delete[] name;
}

void rotateMesh(MyMesh *thisMesh, double xAngle, double yAngle, double zAngle) {
	for (MyMesh::VertexIter v_it = thisMesh->vertices_begin(); v_it != thisMesh->vertices_end(); ++v_it)
	{
		MyMesh::VertexHandle vh = *v_it;
		MyMesh::Point p = thisMesh->point(vh);
		MyMesh::Point temp = MyMesh::Point();

		// rotate X
		double xRadian = (xAngle * M_PI) / 180.0;
		temp[0] = p[0];
		temp[1] = p[1] * cos(xRadian) + p[2] * sin(xRadian);
		temp[2] = -p[1] * sin(xRadian) + p[2] * cos(xRadian);
		p = temp;

		// rotate Y
		double yRadian = (yAngle * M_PI) / 180.0;
		temp[0] = p[0] * cos(yRadian) - p[2] * sin(yRadian);
		temp[1] = p[1];
		temp[2] = p[0] * sin(yRadian) + p[2] * cos(yRadian);
		p = temp;

		// rotate Z
		double zRadian = (zAngle * M_PI) / 180.0;
		temp[0] = p[0] * cos(zRadian) + p[1] * sin(zRadian);
		temp[1] = -p[0] * sin(zRadian) + p[1] * cos(zRadian);
		temp[2] = p[2];
		p = temp;

		thisMesh->set_point(vh, p);
	}
}

string SplitFilename(const std::string& str) {
	unsigned found = str.find_last_of("/\\");
	return str.substr(found + 1);
}

int main(int argc, char** argv)
{
	// argv[0]: this program name
	// argv[1]: file_name

	plyFilePath = "D:\\AllProjects\\Cpp\\OpenGLProject\\face.ply";
	if (argc >= 2)
		plyFilePath = argv[1];

	plyFileName = SplitFilename(plyFilePath);

	// read mesh
	mesh.request_vertex_colors();
	OpenMesh::IO::Options opt;
	opt += OpenMesh::IO::Options::VertexColor;
	cout << OpenMesh::IO::read_mesh(mesh, plyFilePath, opt);
	
	// openGL init
	glutInit(&argc, argv);

	// window setting
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	int id = glutCreateWindow("first");

	// pre process
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// view
	gluPerspective(80, (double)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1, VIEWPOINT_NEARZ - VIEWPOINT_FARZ);
	gluLookAt(0, 0, VIEWPOINT_NEARZ,	// eye
		0, 0 , 0,					// center
		0, 1, 0);					// Top

	// show
	glutDisplayFunc(drawFunc);
	glutIdleFunc(drawFunc);
	glutMainLoop();

	system("pause");
	return 0;
}