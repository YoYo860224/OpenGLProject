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
#define POINT_FARZ -200
#define POINT_NEARZ 500

using namespace std;

typedef OpenMesh::PolyMesh_ArrayKernelT<> MyMesh;
MyMesh mesh;

int drawWhat = 0;

void drawFunc(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPushMatrix();
	glBegin(GL_TRIANGLES);

	if (drawWhat == 0) {
		// Draw RGB
		for (MyMesh::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it)
		{
			for (MyMesh::FVIter fv_it = mesh.fv_begin(*f_it); fv_it != mesh.fv_end(*f_it); ++fv_it)
			{
				glColor3ubv(mesh.color(*fv_it).data());
				glVertex3fv(mesh.point(*fv_it).data());
			}
		}
	}
	else if (drawWhat == 1) {
		// Draw D
		for (MyMesh::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it)
		{
			for (MyMesh::FVIter fv_it = mesh.fv_begin(*f_it); fv_it != mesh.fv_end(*f_it); ++fv_it)
			{
				// our side is positive
				double depthZ = mesh.point(*fv_it).data()[2];
				double val = (depthZ - POINT_FARZ) / (POINT_NEARZ - POINT_FARZ);

				glColor3d(val, val, val);
				glVertex3fv(mesh.point(*fv_it).data());
			}
		}
	}
	glEnd();
	glPopMatrix();
	glFlush();

	if (drawWhat == 0) {
		char name[10] = "rgb.bmp";
		SaveScreenShot(name, WINDOW_WIDTH, WINDOW_HEIGHT);
		drawWhat++;
	}
	else if (drawWhat == 1) {
		char name[10] = "d.bmp";
		SaveScreenShot(name, WINDOW_WIDTH, WINDOW_HEIGHT);
		exit(0);
	}
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
		temp[1] = p[1] * cos(xRadian) - p[2] * sin(xRadian);
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

int main(int argc, char** argv)
{
	string plyFilePath = "D:\\AllProjects\\Cpp\\OpenGLProject\\face.ply";
	
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
	gluPerspective(80, (double)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1, POINT_NEARZ - POINT_FARZ);
	gluLookAt(0, 0, POINT_NEARZ,	// eye
		0, 0 , 0,					// center
		0, 1, 0);					// Top

	// show
	glutDisplayFunc(drawFunc);
	glutIdleFunc(drawFunc);
	glutMainLoop();

	system("pause");
	return 0;
}