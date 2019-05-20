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
////鼠标交互有关的
//int mousetate = 0; //鼠标当前的状态
//GLfloat Oldx = 0.0; // 点击之前的位置
//GLfloat Oldy = 0.0;
////与实现角度大小相关的参数，只需要两个就可以完成
//float xRotate = 0.0f;
//float yRotate = 0.0f;
//float ty = 0.0f;
//float scale = 1;
//
////文件读取有关的
//MyMesh mesh;
//const string file_1 = "obj/三棱锥.obj";
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
////初始化顶点和面   
//void initGL()
//{
//	glClearColor(0.0, 0.0, 0.0, 0.0);
//	glClearDepth(2.0);
//	glShadeModel(GL_SMOOTH);
//	glEnable(GL_DEPTH_TEST); //用来开启深度缓冲区的功能，启动后OPengl就可以跟踪Z轴上的像素，那么它只有在前面没有东西的情况下才会绘制这个像素，在绘制3d时，最好启用，视觉效果会比较真实
//							 // ------------------- Lighting  
//	glEnable(GL_LIGHTING); // 如果enbale那么就使用当前的光照参数去推导顶点的颜色
//	glEnable(GL_LIGHT0); //第一个光源，而GL_LIGHT1表示第二个光源
//						 // ------------------- Display List  
//	showFaceList = glGenLists(1);
//	showWireList = glGenLists(1);
//	int temp = mesh.n_edges();
//
//	// 绘制 wire 
//	glNewList(showWireList, GL_COMPILE);
//	glDisable(GL_LIGHTING);
//	glLineWidth(1.0f);
//	glColor3f(0.5f, 0.5f, 0.5f);
//	glBegin(GL_LINES);
//	for (MyMesh::HalfedgeIter he_it = mesh.halfedges_begin(); he_it != mesh.halfedges_end(); ++he_it) {
//		//链接这个有向边的起点和终点
//		glVertex3fv(mesh.point(mesh.from_vertex_handle(*he_it)).data());
//		glVertex3fv(mesh.point(mesh.to_vertex_handle(*he_it)).data());
//	}
//	glEnd();
//	glEnable(GL_LIGHTING);
//	glEndList();
//
//	// 绘制flat
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
//// 当窗体改变大小的时候
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
//// 读取文件的函数
//void readfile(string file) {
//	// 请求顶点法线 vertex normals
//	mesh.request_vertex_normals();
//	//如果不存在顶点法线，则报错 
//	if (!mesh.has_vertex_normals())
//	{
//		cout << "错误：标准定点属性 “法线”不存在" << endl;
//		return;
//	}
//	// 如果有顶点发现则读取文件 
//	OpenMesh::IO::Options opt;
//	if (!OpenMesh::IO::read_mesh(mesh, file, opt))
//	{
//		cout << "无法读取文件:" << file << endl;
//		return;
//	}
//	else cout << "成功读取文件:" << file << endl;
//	cout << endl; // 为了ui显示好看一些
//				  //如果不存在顶点法线，则计算出
//	if (!opt.check(OpenMesh::IO::Options::VertexNormal))
//	{
//		// 通过面法线计算顶点法线
//		mesh.request_face_normals();
//		// mesh计算出顶点法线
//		mesh.update_normals();
//		// 释放面法线
//		mesh.release_face_normals();
//	}
//}
//
////  键盘交互 1. 切换文件 2.切换显示
//void mySpecial(int key, int x, int y) {
//	switch (key) {
//	case GLUT_KEY_F1:
//		cout << "读取文件:" << file_1 << " 中......" << endl;
//		readfile(file_1);
//		scale = 1.0;
//		currentfile = 1;
//		initGL();
//		break;
//	case GLUT_KEY_F2:
//		cout << "读取文件:" << file_2 << " 中......" << endl;
//		readfile(file_2);
//		scale = 1.2;
//		currentfile = 2;
//		initGL();
//		break;
//	case GLUT_KEY_F3:
//		cout << "读取文件:" << file_3 << " 中......" << endl;
//		readfile(file_3);
//		scale = 0.01;
//		currentfile = 3;
//		initGL();
//		break;
//	case GLUT_KEY_F4:
//		if (showFace == true) {
//			showFace = false;
//			showWire = true;
//			cout << "切换显示模式为：WireFrame" << endl;
//		}
//		else if (showWire == true)
//		{
//			showWire = false;
//			showFlatlines = true;
//			cout << "切换显示模式为：Flatlines" << endl;
//		}
//		else if (showFlatlines == true) {
//			showFlatlines = false;
//			showFace = true;
//			cout << "切换显示模式为：Flat" << endl;
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
//// 鼠标交互
//void myMouse(int button, int state, int x, int y)
//{
//	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
//		mousetate = 1;
//		Oldx = x;
//		Oldy = y;
//	}
//	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
//		mousetate = 0;
//	//滚轮事件
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
//// 鼠标运动时
//void onMouseMove(int x, int y) {
//	if (mousetate) {
//		//x对应y是因为对应的是法向量
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
//	//要清除之前的深度缓存
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glLoadIdentity();
//	//与显示相关的函数
//	glRotatef(xRotate, 1.0f, 0.0f, 0.0f); // 让物体旋转的函数 第一个参数是角度大小，后面的参数是旋转的法向量
//	glRotatef(yRotate, 0.0f, 1.0f, 0.0f);
//	glTranslatef(0.0f, 0.0f, ty);
//	glScalef(scale, scale, scale); // 缩放
//
//								   //每次display都要使用glcalllist回调函数显示想显示的顶点列表
//	if (showFace)
//		glCallList(showFaceList);
//	if (showFlatlines) {
//		glCallList(showFaceList);
//		glCallList(showWireList);
//	}
//	if (showWire)
//		glCallList(showWireList);
//
//	glutSwapBuffers(); //这是Opengl中用于实现双缓存技术的一个重要函数
//}
//
//int main(int argc, char** argv)
//{
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); // GLUT_Double 表示使用双缓存而非单缓存
//	glutInitWindowPosition(100, 100);
//	glutInitWindowSize(800, 500);
//	glutCreateWindow("Mesh Viewer");
//	//一开始默认读取文件1
//	readfile(file_1);
//	initGL();
//	glutMouseFunc(myMouse);
//	glutMotionFunc(onMouseMove); // 鼠标移动的时候的函数
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
	//cout << "读入模型信息:" << endl;
	//cout << "  顶点数:  " << mesh.n_vertices() << endl;
	//cout << "  边  数:  " << mesh.n_edges() << endl;
	//cout << "  面片数:  " << mesh.n_faces() << endl;
	//cout << "平滑操作:" << endl;

	/*struct HE_vert *vertlist; 
	vertlist = (struct HE_vert*)malloc(sizeof(struct HE_vert));*/
	//struct Node_int<int> *outEdges;
	/*outEdges = (struct Node_int<int>*)malloc(sizeof(struct Node_int<int>));
	outEdges->isEmpty = true;*/
	struct Node_int<int> *facelist;
	struct Node_int<double> *dislist;
	facelist = (struct Node_int<int>*)malloc(sizeof(struct Node_int<int>));
	dislist = (struct Node_int<double>*)malloc(sizeof(struct Node_int<double>));//从第二个点开始插入
	//判断是否为空
	/*struct HE_edge *edgelist;
	edgelist = (struct HE_edge*)malloc(sizeof(struct HE_edge)); edgelist->isEmpty = true;*/
	
	

	HE_vert *vertlist = new HE_vert();
	getFile("obj/one_face.obj",vertlist,facelist);

	

	//接下来就是构造各种遍历函数，根据数据量来看，边建少了
	creat_allHE(vertlist, facelist,dislist,0.3);

	for (int i = 1; i <= 8; i++) {
		for (HE_edge*edge = getVertByindex(vertlist, i)->outEdges; edge != NULL; edge = edge->next) {
			cout << i << "->" << edge->vert->index << endl;
		}
	}


	//收缩边
	fact_jointPoint(vertlist, getVertByindex(vertlist, 1), getVertByindex(vertlist, 2), facelist);


	 //检测第一次收缩有没有问题
	 //首先检测 vertlist and facelist
	 cout << "second" << endl;
	 findAllEdges(vertlist);
	 //报告： vertlist和facelist暂时没问题，转化后的边没少，现在检查 nextEdge是否有误以及pair
	 //边已经建立完成，nextEdge基本正确
	 for (HE_vert *vert = vertlist; vert != NULL; vert = vert->next) {
		 for (HE_edge *edge = vert->outEdges; edge != NULL; edge = edge->next) {
			 if (vert->isEmpty == false && edge->vert->isEmpty == false)
				 cout << vert->index << "->" << edge->vert->index << "nextEdge:" << edge->vert->index << "->" << edge->nextEdge->vert->index << " " << edge->nextEdge->vert->index << "->" << edge->nextEdge->nextEdge->vert->index << " " << edge->nextEdge->nextEdge->vert->index << "->" << edge->nextEdge->nextEdge->nextEdge->vert->index << endl;
		 }

	 }

	 


	//检测所有edges，pair,若要遍历边，那空点不能遍历，边的终点也不能是空点
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



    //检测所有face





	 //查看新建边 数据是否有问题
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



	 //加入收缩外边










	//cout << (2.12132)*(0+0.86603) + (-1.22475)*(0+0.5) + (-0.86603)*(1.41421) << "向量点击" << endl;


	//重点思路******************************************************************************
	//边界点且非边界边（不可收缩） 边界点且为边界边可收缩，其他都可以收缩
	//两个点都是内点，不是边界点       收缩时，删除两个面（包涵这两个点的面），修改剩下的面。
	//一个是边界点，一个不是边界点     收缩时，删除两个面（包涵这两个点的面），修改剩下的面。
	//两个都是边界点，边不是边界边（不能收缩）
	//两个都是边界点，边是边界边       收缩时，删除一个面（包含着两个点的面），修改剩下的面。

	//合并点思路
	/*
	1.找出要合并的两个点，两个点出边集赋值给新点的出边集，
	  然后新点添加到vertlist尾巴，把两个点置空（isEmpty）
	  新点的Q，x,y,z,isEmpty，outedges等属性都已添加
	2.outEdges属性中每条边都添加好，并且每条新边的pair（如果存在）.vert属性指向新点
	  outEdges的边的nextEdge属性，pair等属性都要修改（若pair和nextEdge都涉及修改边则修改其属性）
	  face属性的abcd等边建完重新计算，并且face.edge属性也修改
	3.facelist中同时包含两个点的面置isEmpty为空，并且修改包含一个点的面

	4.1-3是面向内在点的收缩
	*/


	
	
	
	

	
	
	//前面的面能找到后面的面，后面的面无法找到前面的面
	
	

	
	













	system("pause");
	return EXIT_SUCCESS;
}

