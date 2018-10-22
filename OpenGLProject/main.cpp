#define _USE_MATH_DEFINES

#include <iostream>
#include <fstream>
#include <string>
#include <glut.h>
#include <math.h>
#include <OpenMesh/Core/IO/MeshIO.hh>  
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>  

typedef OpenMesh::PolyMesh_ArrayKernelT<> MyMesh;
using namespace std;

MyMesh mesh;

void drawFunc(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glBegin(GL_TRIANGLES);

	for (MyMesh::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it) 
	{
		for (MyMesh::FVIter fv_it = mesh.fv_begin(*f_it); fv_it != mesh.fv_end(*f_it); ++fv_it)
		{
			glColor3ubv(mesh.color(*fv_it).data());
			glVertex3fv(mesh.point(*fv_it).data());		
		}
	}

	glEnd();
	glPopMatrix();

	glFlush();
}

int main(int argc, char** argv)
{
	mesh.request_vertex_colors();
	OpenMesh::IO::Options opt;
	opt += OpenMesh::IO::Options::VertexColor;
	
	cout << OpenMesh::IO::read_mesh(mesh, "D:\\AllProjects\\Cpp\\OpenGLProject\\face.ply", opt);

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(400, 400);
	glutCreateWindow("first");

	// Enable Smooth Shading
	glShadeModel(GL_SMOOTH);
	// Black Background
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	// Depth Buffer Setup
	glClearDepth(1.0f);
	// Enables Depth Testing
	glEnable(GL_DEPTH_TEST);
	// The Type Of Depth Testing To Do	
	glDepthFunc(GL_LEQUAL);

	gluPerspective(80, 1, 0.1, 1000);
	gluLookAt(0, 0, 500,	// eye
		0, 0 , 0,			// center
		0, 1, 0);			// Top

	glutDisplayFunc(drawFunc);
	//glutIdleFunc(drawFunc);
	glutMainLoop();

	system("pause");
	return 0;
}