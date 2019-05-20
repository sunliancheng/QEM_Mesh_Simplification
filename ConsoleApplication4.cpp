//#include <iostream>  
//#include "stdafx.h"
//#include <OpenMesh/Core/IO/MeshIO.hh>  
//#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>  
//#include "GL\glut.h"
//#include <math.h>
//#include <Windows.h>
//#include <string>
//
//using namespace std;
//typedef OpenMesh::TriMesh_ArrayKernelT<> MyMesh;
//
////��꽻���йص�
//int mousetate = 0; //��굱ǰ��״̬
//GLfloat Oldx = 0.0; // ���֮ǰ��λ��
//GLfloat Oldy = 0.0;
////��ʵ�ֽǶȴ�С��صĲ�����ֻ��Ҫ�����Ϳ������
//float xRotate = 0.0f;
//float yRotate = 0.0f;
//float ty = 0.0f;
//float scale = 1;
//
////�ļ���ȡ�йص�
//MyMesh mesh;
//const string file_1 = "obj/����׶.obj";
//const string file_2 = "cactus.ply";
//const string file_3 = "Armadillo.off";
//int currentfile = 1;
//
//GLuint showFaceList, showWireList;
//int showstate = 1;
//bool showFace = true;
//bool showWire = false;
//bool showFlatlines = false;
//
////��ʼ���������   
//void initGL()
//{
//	glClearColor(0.0, 0.0, 0.0, 0.0);
//	glClearDepth(2.0);
//	glShadeModel(GL_SMOOTH);
//	glEnable(GL_DEPTH_TEST); //����������Ȼ������Ĺ��ܣ�������OPengl�Ϳ��Ը���Z���ϵ����أ���ô��ֻ����ǰ��û�ж���������²Ż����������أ��ڻ���3dʱ��������ã��Ӿ�Ч����Ƚ���ʵ
//							 // ------------------- Lighting  
//	glEnable(GL_LIGHTING); // ���enbale��ô��ʹ�õ�ǰ�Ĺ��ղ���ȥ�Ƶ��������ɫ
//	glEnable(GL_LIGHT0); //��һ����Դ����GL_LIGHT1��ʾ�ڶ�����Դ
//						 // ------------------- Display List  
//	showFaceList = glGenLists(1);
//	showWireList = glGenLists(1);
//	int temp = mesh.n_edges();
//
//	// ���� wire 
//	glNewList(showWireList, GL_COMPILE);
//	glDisable(GL_LIGHTING);
//	glLineWidth(1.0f);
//	glColor3f(0.5f, 0.5f, 0.5f);
//	glBegin(GL_LINES);
//	for (MyMesh::HalfedgeIter he_it = mesh.halfedges_begin(); he_it != mesh.halfedges_end(); ++he_it) {
//		//�����������ߵ������յ�
//		glVertex3fv(mesh.point(mesh.from_vertex_handle(*he_it)).data());
//		glVertex3fv(mesh.point(mesh.to_vertex_handle(*he_it)).data());
//	}
//	glEnd();
//	glEnable(GL_LIGHTING);
//	glEndList();
//
//	// ����flat
//	glNewList(showFaceList, GL_COMPILE);
//	for (MyMesh::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it) {
//		glBegin(GL_TRIANGLES);
//		for (MyMesh::FaceVertexIter fv_it = mesh.fv_iter(*f_it); fv_it.is_valid(); ++fv_it) {
//			glNormal3fv(mesh.normal(*fv_it).data());
//			glVertex3fv(mesh.point(*fv_it).data());
//		}
//		glEnd();
//	}
//	glEndList();
//}
//
//// ������ı��С��ʱ��
//void myReshape(GLint w, GLint h)
//{
//	glViewport(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	if (w > h)
//		glOrtho(-static_cast<GLdouble>(w) / h, static_cast<GLdouble>(w) / h, -1.0, 1.0, -100.0, 100.0);
//	else
//		glOrtho(-1.0, 1.0, -static_cast<GLdouble>(h) / w, static_cast<GLdouble>(h) / w, -100.0, 100.0);
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//}
//
//
//// ��ȡ�ļ��ĺ���
//void readfile(string file) {
//	// ���󶥵㷨�� vertex normals
//	mesh.request_vertex_normals();
//	//��������ڶ��㷨�ߣ��򱨴� 
//	if (!mesh.has_vertex_normals())
//	{
//		cout << "���󣺱�׼�������� �����ߡ�������" << endl;
//		return;
//	}
//	// ����ж��㷢�����ȡ�ļ� 
//	OpenMesh::IO::Options opt;
//	if (!OpenMesh::IO::read_mesh(mesh, file, opt))
//	{
//		cout << "�޷���ȡ�ļ�:" << file << endl;
//		return;
//	}
//	else cout << "�ɹ���ȡ�ļ�:" << file << endl;
//	cout << endl; // Ϊ��ui��ʾ�ÿ�һЩ
//				  //��������ڶ��㷨�ߣ�������
//	if (!opt.check(OpenMesh::IO::Options::VertexNormal))
//	{
//		// ͨ���淨�߼��㶥�㷨��
//		mesh.request_face_normals();
//		// mesh��������㷨��
//		mesh.update_normals();
//		// �ͷ��淨��
//		mesh.release_face_normals();
//	}
//}
//
////  ���̽��� 1. �л��ļ� 2.�л���ʾ
//void mySpecial(int key, int x, int y) {
//	switch (key) {
//	case GLUT_KEY_F1:
//		cout << "��ȡ�ļ�:" << file_1 << " ��......" << endl;
//		readfile(file_1);
//		scale = 1.0;
//		currentfile = 1;
//		initGL();
//		break;
//	case GLUT_KEY_F2:
//		cout << "��ȡ�ļ�:" << file_2 << " ��......" << endl;
//		readfile(file_2);
//		scale = 1.2;
//		currentfile = 2;
//		initGL();
//		break;
//	case GLUT_KEY_F3:
//		cout << "��ȡ�ļ�:" << file_3 << " ��......" << endl;
//		readfile(file_3);
//		scale = 0.01;
//		currentfile = 3;
//		initGL();
//		break;
//	case GLUT_KEY_F4:
//		if (showFace == true) {
//			showFace = false;
//			showWire = true;
//			cout << "�л���ʾģʽΪ��WireFrame" << endl;
//		}
//		else if (showWire == true)
//		{
//			showWire = false;
//			showFlatlines = true;
//			cout << "�л���ʾģʽΪ��Flatlines" << endl;
//		}
//		else if (showFlatlines == true) {
//			showFlatlines = false;
//			showFace = true;
//			cout << "�л���ʾģʽΪ��Flat" << endl;
//		}
//		break;
//	case GLUT_KEY_UP:
//		ty += 0.01;
//		break;
//	case GLUT_KEY_DOWN:
//		ty -= 0.01;
//		break;
//	default:
//		break;
//	}
//	glutPostRedisplay();
//}
//
//// ��꽻��
//void myMouse(int button, int state, int x, int y)
//{
//	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
//		mousetate = 1;
//		Oldx = x;
//		Oldy = y;
//	}
//	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
//		mousetate = 0;
//	//�����¼�
//	if (state == GLUT_UP && button == 3) {
//		if (currentfile == 3)
//			scale -= 0.002;
//		else
//			scale -= 0.1;
//	}
//	if (state == GLUT_UP && button == 4) {
//		if (currentfile == 3)
//			scale += 0.002;
//		else
//			scale += 0.1;
//	}
//	glutPostRedisplay();
//}
//
//// ����˶�ʱ
//void onMouseMove(int x, int y) {
//	if (mousetate) {
//		//x��Ӧy����Ϊ��Ӧ���Ƿ�����
//		yRotate += x - Oldx;
//		glutPostRedisplay();
//		Oldx = x;
//		xRotate += y - Oldy;
//		glutPostRedisplay();
//		Oldy = y;
//	}
//}
//
//void myDisplay()
//{
//	//Ҫ���֮ǰ����Ȼ���
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glLoadIdentity();
//	//����ʾ��صĺ���
//	glRotatef(xRotate, 1.0f, 0.0f, 0.0f); // ��������ת�ĺ��� ��һ�������ǽǶȴ�С������Ĳ�������ת�ķ�����
//	glRotatef(yRotate, 0.0f, 1.0f, 0.0f);
//	glTranslatef(0.0f, 0.0f, ty);
//	glScalef(scale, scale, scale); // ����
//
//								   //ÿ��display��Ҫʹ��glcalllist�ص�������ʾ����ʾ�Ķ����б�
//	if (showFace)
//		glCallList(showFaceList);
//	if (showFlatlines) {
//		glCallList(showFaceList);
//		glCallList(showWireList);
//	}
//	if (showWire)
//		glCallList(showWireList);
//
//	glutSwapBuffers(); //����Opengl������ʵ��˫���漼����һ����Ҫ����
//}
//
//int main(int argc, char** argv)
//{
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); // GLUT_Double ��ʾʹ��˫������ǵ�����
//	glutInitWindowPosition(100, 100);
//	glutInitWindowSize(800, 500);
//	glutCreateWindow("Mesh Viewer");
//	//һ��ʼĬ�϶�ȡ�ļ�1
//	readfile(file_1);
//	initGL();
//	glutMouseFunc(myMouse);
//	glutMotionFunc(onMouseMove); // ����ƶ���ʱ��ĺ���
//	glutSpecialFunc(&mySpecial);
//	glutReshapeFunc(&myReshape);
//	glutDisplayFunc(&myDisplay);
//
//	glutMainLoop();
//	return 0;
//}

#include "stdafx.h"
#include "Mesh.h" 
#include "Mesh.cpp"
#include "list.h"
#include "stdlib.h"
#include <iostream>


using namespace std;
//void init(HE_vert *vertlist) {
//	vertlist->edge = (struct HE_edge*)malloc(sizeof(struct HE_edge));
//	vertlist->next = (struct HE_vert*)malloc(sizeof(struct HE_vert));
//	vertlist->edge = NULL; vertlist->next = NULL;
//	vertlist->x = 0;
//	vertlist->y = 0;
//	vertlist->z = 0;
//}




int main()
{
	//CMesh mesh("obj/juxing.obj");
	//cout << "����ģ����Ϣ:" << endl;
	//cout << "  ������:  " << mesh.n_vertices() << endl;
	//cout << "  ��  ��:  " << mesh.n_edges() << endl;
	//cout << "  ��Ƭ��:  " << mesh.n_faces() << endl;
	//cout << "ƽ������:" << endl;

	/*struct HE_vert *vertlist; 
	vertlist = (struct HE_vert*)malloc(sizeof(struct HE_vert));*/
	//struct Node_int<int> *outEdges;
	/*outEdges = (struct Node_int<int>*)malloc(sizeof(struct Node_int<int>));
	outEdges->isEmpty = true;*/
	struct Node_int<int> *facelist;
	struct Node_int<double> *dislist;
	facelist = (struct Node_int<int>*)malloc(sizeof(struct Node_int<int>));
	dislist = (struct Node_int<double>*)malloc(sizeof(struct Node_int<double>));//�ӵڶ����㿪ʼ����
	//�ж��Ƿ�Ϊ��
	/*struct HE_edge *edgelist;
	edgelist = (struct HE_edge*)malloc(sizeof(struct HE_edge)); edgelist->isEmpty = true;*/
	
	

	HE_vert *vertlist = new HE_vert();
	getFile("obj/one_face.obj",vertlist,facelist);

	

	//���������ǹ�����ֱ��������������������������߽�����
	creat_allHE(vertlist, facelist,dislist,0.3);

	for (int i = 1; i <= 8; i++) {
		for (HE_edge*edge = getVertByindex(vertlist, i)->outEdges; edge != NULL; edge = edge->next) {
			cout << i << "->" << edge->vert->index << endl;
		}
	}


	//������
	fact_jointPoint(vertlist, getVertByindex(vertlist, 1), getVertByindex(vertlist, 2), facelist);


	 //����һ��������û������
	 //���ȼ�� vertlist and facelist
	 cout << "second" << endl;
	 findAllEdges(vertlist);
	 //���棺 vertlist��facelist��ʱû���⣬ת����ı�û�٣����ڼ�� nextEdge�Ƿ������Լ�pair
	 //���Ѿ�������ɣ�nextEdge������ȷ
	 for (HE_vert *vert = vertlist; vert != NULL; vert = vert->next) {
		 for (HE_edge *edge = vert->outEdges; edge != NULL; edge = edge->next) {
			 if (vert->isEmpty == false && edge->vert->isEmpty == false)
				 cout << vert->index << "->" << edge->vert->index << "nextEdge:" << edge->vert->index << "->" << edge->nextEdge->vert->index << " " << edge->nextEdge->vert->index << "->" << edge->nextEdge->nextEdge->vert->index << " " << edge->nextEdge->nextEdge->vert->index << "->" << edge->nextEdge->nextEdge->nextEdge->vert->index << endl;
		 }

	 }

	 


	//�������edges��pair,��Ҫ�����ߣ��ǿյ㲻�ܱ������ߵ��յ�Ҳ�����ǿյ�
	 for (HE_vert *vert = vertlist; vert != NULL; vert = vert->next) {
		 if (vert->isEmpty == false) {
			 for (HE_edge *edge = vert->outEdges; edge != NULL; edge = edge->next) {
				 if (edge->vert->isEmpty == false) {
					 cout << vert->index << "->" << edge->vert->index << " ";
					 if (edge->pair != NULL) cout << edge->pair->vert->index;
					 cout << endl;
				 }
			 }
		}

	 }



    //�������face





	 //�鿴�½��� �����Ƿ�������
	 for (HE_edge *edge = getVertByindex(vertlist, 1)->outEdges; edge != NULL; edge = edge->next) {
		 if (edge->vert == getVertByindex(vertlist, 9)) {
			 cout << edge->isEmpty << " " << edge->nextEdge->vert->index << edge->nextEdge->nextEdge->vert->index << edge->nextEdge->nextEdge->nextEdge->vert->index << endl;
		 }
	 }
	 for (HE_edge *edge = getVertByindex(vertlist, 2)->outEdges; edge != NULL; edge = edge->next) {
		 if (edge->vert == getVertByindex(vertlist, 9)) {
			 cout << edge->isEmpty << " " << edge->nextEdge->vert->index << edge->nextEdge->nextEdge->vert->index << edge->nextEdge->nextEdge->nextEdge->vert->index << endl;
		 }
	 }
	 for (HE_edge *edge = getVertByindex(vertlist, 3)->outEdges; edge != NULL; edge = edge->next) {
		 if (edge->vert == getVertByindex(vertlist, 9)) {
			 cout << edge->isEmpty << " " << edge->nextEdge->vert->index<< edge->nextEdge->nextEdge->vert->index << edge->nextEdge->nextEdge->nextEdge->vert->index << endl;
		 }
	 }
	 for (HE_edge *edge = getVertByindex(vertlist, 6)->outEdges; edge != NULL; edge = edge->next) {
		 if (edge->vert == getVertByindex(vertlist, 9)) {
			 cout << edge->isEmpty << " " << edge->nextEdge->vert->index<< edge->nextEdge->nextEdge->vert->index << edge->nextEdge->nextEdge->nextEdge->vert->index << endl;
		 }
	 }
	 for (HE_edge *edge = getVertByindex(vertlist, 7)->outEdges; edge != NULL; edge = edge->next) {
		 if (edge->vert == getVertByindex(vertlist, 9)) {
			 cout << edge->isEmpty << " " << edge->nextEdge->vert->index<< edge->nextEdge->nextEdge->vert->index << edge->nextEdge->nextEdge->nextEdge->vert->index << endl;
		 }
	 }
	 for (HE_edge *edge = getVertByindex(vertlist, 8)->outEdges; edge != NULL; edge = edge->next) {
		 if (edge->vert == getVertByindex(vertlist, 9)) {
			 cout << edge->isEmpty << " " << edge->nextEdge->vert->index<< edge->nextEdge->nextEdge->vert->index << edge->nextEdge->nextEdge->nextEdge->vert->index << endl;
		 }
	 }



	 //�����������










	//cout << (2.12132)*(0+0.86603) + (-1.22475)*(0+0.5) + (-0.86603)*(1.41421) << "�������" << endl;


	//�ص�˼·******************************************************************************
	//�߽���ҷǱ߽�ߣ����������� �߽����Ϊ�߽�߿���������������������
	//�����㶼���ڵ㣬���Ǳ߽��       ����ʱ��ɾ�������棨��������������棩���޸�ʣ�µ��档
	//һ���Ǳ߽�㣬һ�����Ǳ߽��     ����ʱ��ɾ�������棨��������������棩���޸�ʣ�µ��档
	//�������Ǳ߽�㣬�߲��Ǳ߽�ߣ�����������
	//�������Ǳ߽�㣬���Ǳ߽��       ����ʱ��ɾ��һ���棨��������������棩���޸�ʣ�µ��档

	//�ϲ���˼·
	/*
	1.�ҳ�Ҫ�ϲ��������㣬��������߼���ֵ���µ�ĳ��߼���
	  Ȼ���µ���ӵ�vertlistβ�ͣ����������ÿգ�isEmpty��
	  �µ��Q��x,y,z,isEmpty��outedges�����Զ������
	2.outEdges������ÿ���߶���Ӻã�����ÿ���±ߵ�pair��������ڣ�.vert����ָ���µ�
	  outEdges�ıߵ�nextEdge���ԣ�pair�����Զ�Ҫ�޸ģ���pair��nextEdge���漰�޸ı����޸������ԣ�
	  face���Ե�abcd�ȱ߽������¼��㣬����face.edge����Ҳ�޸�
	3.facelist��ͬʱ���������������isEmptyΪ�գ������޸İ���һ�������

	4.1-3���������ڵ������
	*/


	
	
	
	

	
	
	//ǰ��������ҵ�������棬��������޷��ҵ�ǰ�����
	
	

	
	













	system("pause");
	return EXIT_SUCCESS;
}

