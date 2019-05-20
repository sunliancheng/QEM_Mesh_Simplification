#pragma once
//! 包含OpenMesh库的头文件
//#include <OpenMesh\Core\IO\MeshIO.hh>
//#include <OpenMesh\Core\Mesh\PolyMesh_ArrayKernelT.hh>
#include <fstream>
#include <cstdlib> //qsort在此头文件中声明
#include <string>
#include <sstream>  
#include "list.h"
#include <vector>
#include <assert.h>
#include "matrix.h"
#include <Core>
#include <Dense>
#include <iostream>

using Eigen::MatrixXf;
using Eigen::MatrixXd;
using Eigen::Matrix3f;
using Eigen::Matrix4f;
using namespace std;

//! 常用类的简短定义
//typedef OpenMesh::Vec3f vec3;


//大部分信息都可以直接获取
//HE_vert* vert1 = edge->vert;
//HE_vert* vert2 = edge->pair->vert;
//HE_face* face1 = edge->face;
//HE_face* face2 = edge->pair->face;

struct HE_vert;

struct HE_edge {
	struct HE_vert* vert;   // vertex at the end of the half-edge
	struct HE_edge* pair;   // oppositely oriented adjacent half-edge 
	struct HE_face* face;   // face the half-edge borders
	struct HE_edge* next;   // linklist next
	bool isEmpty = true;
	struct HE_edge* nextEdge;  //next edge
	HE_edge() {
		next = NULL;
		nextEdge = NULL;
		isEmpty = true;
		vert = NULL;
	}
};
struct HE_vert
{ 
	//xyz index isEmpty *next在读取文件的时候已经写入，
	//生成边的时候 生成*edge
	float x;
	float y;
	float z;
	struct HE_edge* outEdges;  // 建立出边的索引集

	int index;
	bool isEmpty = true;

	Matrix4f Q;

	//记录出边点的索引,采用图论的方式
	//struct Node_int<int>outEdges;
	struct HE_vert *next;

	HE_vert() {
		outEdges = new HE_edge();
		outEdges->isEmpty = true;
		outEdges->vert = NULL;
		/*outEdges->face = NULL;
		outEdges->isEmpty = true;
		outEdges->next = NULL;
		outEdges->pair = NULL;
		outEdges->vert = NULL;
		outEdges->nextEdge = NULL;
		cout << "vert_creat" << endl;*/
	}
	
};
struct HE_face
{
	struct HE_edge* edge;  // one of the half-edges bordering the face
	float a, b, c, d;
	Matrix4f m;
	HE_face() {
		edge = NULL;
		a = b = c = d = 0;
	}
};
HE_edge creat_edge(struct HE_vert *vertlist, int x, int y, int z);
void findAllEdges(struct HE_vert *vertlist);
//p指向当前链表最后一个节点，edge指向需要添加进去的边，此处忽略数据集有问题
HE_edge* insertEdge(HE_edge *edgelist,HE_edge *p,HE_edge *edge) {
	
	
	if (edgelist->isEmpty) {
		//对指针内数据进行操作
		edgelist->isEmpty = false;
		edgelist->next = NULL;
		
		return p;

	}
	else {
		struct HE_edge *p1;
		p1 = (struct HE_edge*)malloc(sizeof(struct HE_edge));
		
		//对指针内数据进行操作
		p1->face = false;
		p1->next = NULL;
		//修改指针连接
		p->next = p1;
		p = p1;

		return p;
	}
}
void addface(HE_edge *edgelist, HE_vert *vertlist) {
	//对点进行操作

	//对边进行操作

	//对面进行操作

}
//获取平面abcd，四个参数，也能求法向量
void getFaceInfo(HE_face *face, HE_vert *vert1, HE_vert *vert2, HE_vert *vert3) {

	

	face->a = (vert2->y - vert1->y)*(vert3->z - vert1->z) - (vert2->z - vert1->z)*(vert3->y - vert1->y);
	
	face->b = (vert2->z - vert1->z)*(vert3->x - vert1->x) - (vert2->x - vert1->x)*(vert3->z - vert1->z);
	face->c = (vert2->x - vert1->x)*(vert3->y - vert1->y) - (vert2->y - vert1->y)*(vert3->x- vert1->x);
	
	float n = sqrt (  face->a*face->a + face->b*face->b + face->c*face->c  );
	//cout << face->a << " " << face->b << " " << face->c << "n:" << n << endl;
	face->a = face->a / n; face->b = face->b / n; face->c = face->c / n;
	face->d = 0 - (face->a*vert1->x + face->b * vert1->y + face->c*vert1->z);

	//MyMatrix4f(face->a*face->a, face->a*face->b, face->a*face->c, face->a*face->d, face->a*face->b, face->b*face->b, face->b*face->c, face->b*face->d, face->a*face->c, face->b*face->c, face->c*face->c, face->c*face->d, face->a*face->d, face->b*face->d, face->c*face->d, face->d*face->d);
	face->m << face->a*face->a, face->a*face->b, face->a*face->c, face->a*face->d, face->a*face->b, face->b*face->b, face->b*face->c, face->b*face->d, face->a*face->c, face->b*face->c, face->c*face->c, face->c*face->d, face->a*face->d, face->b*face->d, face->c*face->d, face->d*face->d;
	//cout << face->m << endl;


}
//点到平面距离
float getDistance(HE_face *face, HE_vert *vert) {
	float d;
	d = fabs(face->a*vert->x + face->b*vert->y + face->c*vert->z + face->d) / sqrt(face->a*face->a + face->b*face->b + face->c*face->c);
	return d;
}
//半边循环找面的所有边
//HE_edge* edge = face->edge;
//do {
//	// do something with edge
//	edge = edge->next;
//} while (edge != face->edge);
void SplitString(const string& s, vector<string>& v, const string& c)
{
	string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
}
template <class Type>
Type stringToNum(const string& str)
{
	istringstream iss(str);
	Type num;
	iss >> num;
	return num;
}
//从文件读取 点集链表
void getFile(string file, struct HE_vert *vertlist, struct Node_int<int> *facelist) {
	int i = 1;
	ifstream infile;
	infile.open(file.data());   //将文件流对象与文件连接起来 
	assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 


	struct HE_vert  *p, *p1;
	p = new HE_vert(); p1 = new HE_vert();
	/*p->outEdges = (struct Node_int<int>*)malloc(sizeof(struct Node_int<int>));
	p->outEdges->next = NULL;*/
	p = vertlist;
	
	string s;
	int j = 0;
	while (getline(infile, s))
	{
		
		j = j + 1;
		//cout << s << endl;
		vector<string> v;
		SplitString(s, v, " "); //可按多个字符来分隔;
		if (j % 100 == 0) {
			cout << j << endl;
		}
		//when readin is point
		if (v[0] == "v") {
			
				if (vertlist->isEmpty) {			
					
					p->x = stringToNum<float>(v[1]);
					p->y = stringToNum<float>(v[2]);
					p->z = stringToNum<float>(v[3]);
					p->isEmpty = false;
					p->index = i;
					p->outEdges = new HE_edge();
					p->outEdges->face = new HE_face();
					p->outEdges->isEmpty = true;
					p->outEdges->nextEdge = NULL;
					p->outEdges->pair = new HE_edge();
					p->outEdges->vert = new HE_vert();
					p->next = NULL;
					//cout << "in : other" << p->y << endl;
				}
				else {
					p1 = (struct HE_vert*)malloc(sizeof(struct HE_vert));
					p1->x = stringToNum<float>(v[1]);
					p1->y = stringToNum<float>(v[2]);
					p1->z = stringToNum<float>(v[3]);
					p1->isEmpty = false;
					p1->next = NULL;
					p1->index = i;
					p1->outEdges = new HE_edge();
					p1->outEdges->face = new HE_face();
					p1->outEdges->isEmpty = true;
					p1->outEdges->nextEdge = NULL;
					p1->outEdges->pair = new HE_edge();
					p1->outEdges->vert = new HE_vert();
					p->next = p1;
					p = p1;
					/*p->outEdges = (struct Node_int<int>*)malloc(sizeof(struct Node_int<int>));
					p->outEdges->isEmpty = true;
					p->outEdges->next = NULL;*/
					//cout << "in : other" << p->y << endl;

				}		
		}

		// when readin is face
		//使用方法有问题，可以直接读三个面。直接能生成面
		else if (v[0] == "f") {
			//把点相关点 输入vextor容器中
			insert<int>( stringToNum<int>(v[1]), stringToNum<int>(v[2]), stringToNum<int>(v[3]),facelist);




			/*p = (struct HE_vert*)malloc(sizeof(struct HE_vert));
			
			for (p = vertlist; p != NULL; p = p->next) {
				if (p->index == stringToNum<int>(v[1])) {
					insert(stringToNum<int>(v[2]),p->outEdges);			
					insert(stringToNum<int>(v[3]), p->outEdges);
				}
				else if (p->index == stringToNum<int>(v[2])) {	
					insert(stringToNum<int>(v[1]), p->outEdges);
					insert(stringToNum<int>(v[3]), p->outEdges);
				}
				else if (p->index == stringToNum<int>(v[3])) {
					cout << vertlist->isEmpty << " 2 " << vertlist->outEdges->isEmpty << endl;
					insert(stringToNum<int>(v[1]), p->outEdges);
					insert(stringToNum<int>(v[2]), p->outEdges);
				}
			
			}*/
		/*	for (p = vertlist; p != NULL; p = p->next) {
				if (p->index == stringToNum<int>(v[2])) {
					cout << vertlist->isEmpty << " 2 " << vertlist->outEdges->isEmpty << endl;
					insert(stringToNum<int>(v[1]), p->outEdges);
					insert(stringToNum<int>(v[3]), p->outEdges);

				}

				for (p = vertlist; p != NULL; p = p->next) {
					if (p->index == stringToNum<int>(v[3])) {
						cout << vertlist->isEmpty << " 3 " << vertlist->outEdges->isEmpty << endl;
						insert(stringToNum<int>(v[2]), p->outEdges);
						insert(stringToNum<int>(v[1]), p->outEdges);

					}

				}
			}*/
			
			
		}
		++i;
	}infile.close();             //关闭文件输入流
}
void printVertlist(struct HE_vert *vertlist) {
	struct HE_vert *p;
	p = (struct HE_vert*)malloc(sizeof(struct HE_vert));
	for (p = vertlist; p != NULL; p = p->next) { 
		
		cout <<"index:"<<p->index<< " x:" << p->x << " y:" << p->y << " z:" << p->z << endl;
		/*for (std::vector<int>::iterator it = p->outEdges.begin(); it != p->outEdges.end();++it) {
			cout << " it:" << *it ;
		}*/
	}
}
//得出vertlist长度
int getSize(HE_vert *vertlist) {
	HE_vert *vert = new HE_vert();
	int j = 0;
	for (vert = vertlist; vert != NULL; vert = vert->next) {
		j++;
	}
	return j;
}
int getsize_vertlist(HE_vert *outEdges) {
	int i = 0;
	HE_vert *p;
	p = (struct HE_vert*)malloc(sizeof(struct HE_vert));
	for (p = outEdges; p != NULL; p = p->next) {
		i++;
	}
	return i;
}
//根据index，找出点的指针
HE_vert* getVertByindex(struct HE_vert *vertlist,int index) {
	HE_vert *new_vert = new HE_vert();
	
	//new_vert->outEdges = new HE_edge();
	for (new_vert = vertlist; new_vert != NULL; new_vert = new_vert->next) {
		if (index == new_vert->index) return new_vert;
	}
	return new_vert;
}

//找出对边,y为入点，x为出点,找出的边为y->x;如果isEmpty为空则说明没找到
HE_edge* getPairEdge(struct HE_vert *vertlist, int x, int y) {
	HE_vert *vert = new HE_vert();
	HE_edge *edge = new HE_edge();
	HE_edge *edge2 = new HE_edge();
	for (vert = vertlist; vert != NULL; vert = vert->next) {
		if (y == vert->index) {
			//cout <<"y"<< y << endl;
			for (edge = vert->outEdges; edge != NULL; edge = edge->next) {
				//cout << edge->vert->index << endl;
				if (edge->vert != NULL) 
				{
					//cout << "d" << endl;
					if (edge->vert->index == x) { edge->isEmpty = false; return edge; }
				}
			}
		}
	}
	edge2->isEmpty = true;
	return edge2;
}
int count_add_edge = 0;
//把边插入到出边链表汇
void insertEdgelist(struct HE_edge *edgelist, struct HE_edge *p,struct HE_vert *vertlist) {
	count_add_edge++;
	//cout << "count_number:" <<count_add_edge << endl;
	if (edgelist->isEmpty) {		
		
		edgelist->isEmpty = false;
		
		edgelist->face = p->face;
		edgelist->nextEdge = p->nextEdge;
		edgelist->pair = p->pair;
		

		edgelist->vert = p->vert;
		//edgelist->vert->index = p->vert->index;
		
		


		edgelist->next = NULL;
		//cout << "该边第一次初始化" << edgelist->vert->index << endl;
	}
	else {
	
		struct HE_edge *p1,*tem;
		//p1 = (struct  HE_edge*)malloc(sizeof(struct HE_edge));
		//tem = (struct  HE_edge*)malloc(sizeof(struct HE_edge));


		p1 = new HE_edge();
		p1->face = new HE_face();
		p1->next = new HE_edge();
		p1->nextEdge = new HE_edge();
		p1->pair = new HE_edge();
		p1->vert = new HE_vert();

		tem = new HE_edge();
		tem->face = new HE_face();
		tem->next = new HE_edge();
		tem->nextEdge = new HE_edge();
		tem->pair = new HE_edge();
		tem->vert = new HE_vert();





		for (p1 = edgelist; p1->next != NULL; p1 = p1->next) {
			if (p1->next == NULL) {
				//cout << "nul" << endl;
			}
		}
	
		p1->next = tem;
		(tem->isEmpty) = false;
		tem->face = p->face;
		tem->nextEdge = p->nextEdge;
		tem->pair = p->pair;
		tem->vert = p->vert;
		tem->next = NULL;
		p1 = tem;
		p1->next = NULL;
		//cout << "第二次往其中插入边" << endl;
	}

}


//创建face
void create_face(struct HE_vert *vertlist,  struct HE_edge *edge) {
	HE_face *face = new HE_face();
	
	if (edge->face->edge == NULL) {
		face->edge = edge;

		edge->face = face;
		edge->nextEdge->face = face;
		edge->nextEdge->nextEdge->face = face;
		//cout << "生成面" << endl;	

		//生成面的abcd属性
		getFaceInfo(edge->face, edge->vert, edge->nextEdge->vert, edge->nextEdge->nextEdge->vert);
		//cout << "face:" << edge->face->a << edge->face->b << edge->face->c << edge->face->d << endl;
	}
	
	
}

int i = 0;
//创建边，x->y,edgelist 为 vertlist.outedges
HE_edge creat_edge(struct HE_vert *vertlist,  int x, int y, int z) {

	//findAllEdges(vertlist);
	
	

	HE_edge *edge = new HE_edge();
	
//	edge->vert = NULL;
	
	edge->face = new HE_face();
	edge->next = new HE_edge();
	edge->nextEdge = new HE_edge();
	edge->pair = new HE_edge();


	    

		//找出终点，指向终点
		edge->vert = new HE_vert();
		edge->vert = getVertByindex(vertlist, y);
		cout << "创建边：" << x << "->" << y << endl;
		//cout << "vert.index:" << edge->vert->index << endl;
		//cout <<"x"<< edge->vert->x << endl;
		
		//创建face
		//edge->face = &create_face(vertlist, x, y, z);

		//设置pair的属性
		/*HE_edge *tem = new HE_edge();
		*tem = getPairEdge(vertlist, x, y);
		if (tem->isEmpty) {
			edge->pair = NULL;
		}
		else edge->pair = tem;*/

		//isEmpty 设置为空
		edge->isEmpty = false;

		//add the edge to vert->outedges
		//cout << vertlist->outEdges->vert->index << endl;
		
		//getVertByindex(vertlist, x).outEdges = new HE_edge();
		//getVertByindex(vertlist, x).outEdges->vert = new HE_vert();
		insertEdgelist(getVertByindex(vertlist,x)->outEdges,edge,vertlist);
		//cout << getVertByindex(vertlist, x).outEdges->vert->index << endl;
		//findAllEdges(vertlist);
		
		//cout << "already insert" << endl;
		return *edge;
		//nextEdge
		
		


}
HE_edge* findEdgeInlist(HE_edge *edgelist, int end) {
	HE_edge *edge = new HE_edge();
	HE_edge *edge2 = new HE_edge();
	for (edge = edgelist; edge != NULL; edge = edge->next) {
		if (edge->vert->index == end ) {
			return edge;
		}
	}
	return edge2;
}
//创建一个面的三条边
void creat_3edges(struct HE_vert *vertlist, int x, int y, int z) {
	cout << "创建面:" << x << " " << y << " " << z << endl;
	creat_edge(vertlist, x, y, z);
	creat_edge(vertlist, y, z, x);
	creat_edge(vertlist, z, x, y);
	HE_edge *edge = new HE_edge();
	edge->nextEdge = new HE_edge();
	edge->nextEdge->vert = new HE_vert();
	edge->vert = new HE_vert();
	for (edge = getVertByindex(vertlist,x)->outEdges; edge != NULL; edge = edge->next) {
		
		
		if (edge->vert->index == y) {
			edge->nextEdge =  findEdgeInlist(getVertByindex(vertlist,y)->outEdges, z);
		}
	}
	for (edge = getVertByindex(vertlist, y)->outEdges; edge != NULL; edge = edge->next) {


		if (edge->vert->index == z) {
			edge->nextEdge = findEdgeInlist(getVertByindex(vertlist, z)->outEdges, x);
		}
	}
	for (edge = getVertByindex(vertlist, z)->outEdges; edge != NULL; edge = edge->next) {


		if (edge->vert->index == x) {
			edge->nextEdge = findEdgeInlist(getVertByindex(vertlist, x)->outEdges, y);
		}
	}



	/*creat_edge(vertlist, 1, 2, 3);
	vertlist->outEdges->nextEdge = new HE_edge();
	vertlist->outEdges->nextEdge = &creat_edge(vertlist, 2, 3, 1);
	vertlist->outEdges->nextEdge->vert = new HE_vert();
	vertlist->outEdges->nextEdge->vert = creat_edge(vertlist, 2, 3, 1).vert;*/
	

}
//通过两个点查找面，返回另一个面，若返回-1则没找到,还得判断这个面存不存在
int isInFacelist(struct Node_int<int> *facelist, int x, int y,int z) {
	struct Node_int<int> *face = (struct Node_int<int>*)malloc(sizeof(struct Node_int<int>));
	for (face = facelist; face != NULL; face = face->next) {
		if (x == face->x) {
			if (y == face->y) {
				if (z == face->z) {
					int result = isInList(x, y, facelist);
					int result1 = isInList_second(x, y, facelist);
					if (result == z) {
						return result1;
					}
					else
						return result;
				}
				else {
					
				}
			}
			else if (y == face->z) {
				if (z == face->y) {
					int result = isInList(x, y, facelist);
					int result1 = isInList_second(x, y, facelist);
					if (result == z) {
						return result1;
					}
					else
						return result;
				}
				
			}
		}
		else if (x == face->y) {
			if (y == face->x) {
				if (z == face->z) {
					int result = isInList(x, y, facelist);
					int result1 = isInList_second(x, y, facelist);
					if (result == z) {
						return result1;
					}
					else
						return result;
				}
				else {
					
				}
			}
			else if (y == face->z) {
				if (z == face->x) {
					int result = isInList(x, y, facelist);
					int result1 = isInList_second(x, y, facelist);
					if (result == z) {
						return result1;
					}
					else
						return result;
				}
				
			}
		}
		else if (x == face->z) {
			if (y == face->y) {
				if (z == face->x) {
					int result = isInList(x, y, facelist);
					int result1 = isInList_second(x, y, facelist);
					if (result == z) {
						return result1;
					}
					else
						return result;
				}
				else {
					
				}
			}
			else if (y == face->x) {
				if (z == face->y) {
					int result = isInList(x, y, facelist);
					int result1 = isInList_second(x, y, facelist);
					if (result == z) {
						return result1;
					}
					else
						return result;
				}
				
			}
		}
	}


	return -1;


}

void find(struct HE_vert *vertlist, struct Node_int<int> *facelist, int x, int y, int z) {
	int another_z = isInFacelist(facelist, x, y, z); 
	int another_x = isInFacelist(facelist, y, z, x);
	int another_y = isInFacelist(facelist, z, x, y);
	//cout << another_x << another_y << another_z << endl;
	//没找到另一个点，意味着这条是边界
	if (another_z != -1  && getPairEdge(vertlist,x,y)->isEmpty)  {
		//cout << "创建面1:"<< y<<" "<<x<<" "<<another_z << endl;
		creat_3edges(vertlist, y, x, another_z);
		find(vertlist, facelist, y, x, another_z);
	}
	if (another_x != -1 && getPairEdge(vertlist, y, z)->isEmpty) {
		//cout << "创建面2:" << z << " " << y << " " << another_x << endl;
		creat_3edges(vertlist, z, y, another_x);
		find(vertlist, facelist, z, y, another_x);
	}
	if (another_y != -1 && getPairEdge(vertlist, z, x)->isEmpty) {
		//cout << "创建面3:" << x << " " << z << " " << another_y << endl;
		creat_3edges(vertlist, x, z, another_y);
		find(vertlist, facelist, x, z, another_y);
	}
	

}
//通过点集链表找出所有边
void findAllEdges(struct HE_vert *vertlist) {
	HE_vert *vert = new HE_vert();
	HE_edge *edge = new HE_edge();
	for (vert = vertlist; vert != NULL; vert = vert->next) {
		if (!vert->isEmpty && !vert->outEdges->isEmpty) {
			for (edge = vert->outEdges; edge != NULL; edge = edge->next) {
				
				if (!edge->isEmpty && !edge->vert->isEmpty) {
					cout << "找到边：" << vert->index << "->" << edge->vert->index << endl;
				}
			}
		}
	
	}


}
//通过边集链表找出所有边
void findAlledgeByedge(struct HE_edge *edges) {
	HE_edge *edge = new HE_edge();
	for (edge = edges; edge != NULL; edge = edge->next) {
		if (!edge->isEmpty && !edge->vert->isEmpty) {
			cout << "找到边：" << edge->pair->vert->index << "->" << edge->vert->index << endl;
		}
	}
}
//给所有边添加上pair
void addEdgePair(struct HE_vert *vertlist) {
	HE_vert *vert = new HE_vert();
	HE_edge *edge = new HE_edge();
	HE_edge *edgepair = new HE_edge();
	for (vert = vertlist; vert != NULL; vert = vert->next) {
		if (!vert->isEmpty && !vert->outEdges->isEmpty) {
			for (edge = vert->outEdges; edge != NULL; edge = edge->next) {
				if (!edge->isEmpty && !edge->vert->isEmpty) {
					edgepair = getPairEdge(vertlist,vert->index,edge->vert->index);
					if (!edgepair->isEmpty) {
						edge->pair = edgepair;
					}
					else edge->pair = NULL;

					//添加面
					create_face(vertlist, edge);

				}
			}
		}

	}


}
//计算所有点的Q
void CalVertAllKp(struct HE_vert *vertlist) {

	struct HE_vert *vert = new HE_vert();
	struct HE_edge *edge = new HE_edge();
	for (vert = vertlist; vert != NULL; vert = vert->next) {
		vert->Q << 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;
		for (edge = vert->outEdges; edge != NULL; edge = edge->next) {
			vert->Q += edge->face->m;
			//cout << "vertQ and faceM" << edge->face->m << endl << vert->Q << endl;
		}
	}

}
//计算2个点的距离
float Dis2Point(HE_vert *p1,HE_vert *p2) {
	return DistanceBetween2point(p1->x, p1->y, p1->z, p2->x, p2->y, p2->z);
}

//遍历所有点的出边的长度,返回阈值距离
float getAllDis(HE_vert *vertlist, Node_int<double> *dislist, float percent) {
	HE_vert *vert = new HE_vert();
	HE_edge *edge = new HE_edge();
	vector<float> v = vector<float>();
	
	for (vert = vertlist; vert != NULL; vert = vert->next) {
		for (edge = vert->outEdges; edge != NULL; edge = edge->next) {
			
			v.push_back(Dis2Point(vert, edge->vert));
		}
	}
	
	//插入到距离链表中 进行冒泡排序
	sort(v.begin(), v.end(), less<double>());
	
	return v[floor(v.size()*percent)];
		
}
//链表尾插
void insertListEnd(HE_vert *vertlist,HE_vert *vert) {
	HE_vert *p = new HE_vert();
	for (p = vertlist; p->next != NULL;p = p->next) {
		
	}
	vert->index = p->index + 1;
	p->next = vert;
	vert->next = NULL;
}
//把两个点的所有outEdges插入到第一个点中
void insert2Outedgesto1(HE_vert*vertlist,HE_vert *vert,HE_vert *vert1, HE_vert *vert2) {

	for (HE_edge*edge = vert1->outEdges; edge != NULL; edge = edge->next) {
		cout << vert1->index << "->" << edge->vert->index << endl;
	}


	HE_edge *edge = new HE_edge(); edge->vert = new HE_vert();
	HE_edge *edge2 = new HE_edge();
	vert->outEdges = new HE_edge();
	
	for (edge = vert1->outEdges; edge != NULL ; edge = edge->next) {
		cout << edge->vert->index;
		if ( edge->vert != vert2) {
			
			creat_edge(vertlist, vert->index, edge->vert->index,1);

			//insertEdgelist(vert->outEdges, edge, vert);	
			cout << "insert " << vert->index << "->" << edge->vert->index << endl;
		}
	}
	
	

	//此处插入函数有问题，没有设置最后一个的next为null
	for (edge = vert2->outEdges; edge != NULL; edge = edge->next) {
		int flag = 0;
		for (edge2 = vert->outEdges; edge2 != NULL ; edge2 = edge2->next) {
			//如果2的边不在1中，则尾插，在则不插
			if (edge->vert == edge2->vert || edge->vert == vert1) {
				flag = 1;
				
			}
		}
		if (flag == 0) {
			creat_edge(vertlist, vert->index, edge->vert->index,1);
			//insertEdgelist(vert->outEdges, edge, vert);
		}
		
	}
	vert1->isEmpty = vert2->isEmpty = true;
	//cout << "finish insert20o" << endl;
}
//根据一个面的三个点，以及一个边 返回下一个边
HE_edge* returnnext_Edge(HE_vert* vertlist,Node_int<int>*facelist, int edge_v1, int edge_v2) {
	int x = facelist->x; int y = facelist->y; int z = facelist->z;
	if (edge_v1 == x) {
		if (edge_v2 == y)return getPairEdge(vertlist,z,y);
		else if (edge_v2 == z)return getPairEdge(vertlist,y,z);
	}
	else if (edge_v1 == y) {
		if (edge_v2 == x)return getPairEdge(vertlist, z, x);
		else if (edge_v2 == z)return getPairEdge(vertlist, x, z);
	}
	else if (edge_v1 == z) {
		if (edge_v2 == x)return getPairEdge(vertlist, y, x);
		else if (edge_v2 == y)return getPairEdge(vertlist, x, y);
	}

}

//修改outedge和facelist属性
void modifyOutedges(HE_vert *vertlist,Node_int<int> *facelist,HE_vert *vert,HE_vert *vert1,HE_vert *vert2) {



	/*
	3.facelist中同时包含两个点的面置isEmpty为空，并且修改包含一个点的面
	4.1-3是面向内在点的收缩
	*/
	for (Node_int<int>*face = facelist; face != NULL; face = face->next) {
		int x = vert1->index; int y = vert2->index;
		if (x == face->x) {
			if (y == face->y) {
				face->isEmpty = true;
			}
			else if (y == face->z) {
				face->isEmpty = true;
			}
			else {
				face->x = vert->index;
			}
		}
		else if (x == face->y) {
			if (y == face->x) {
				face->isEmpty = true;
			}
			else if (y == face->z) {
				face->isEmpty = true;
			}
			else {
				face->y = vert->index;
			}
		}
		else if (x == face->z) {
			if (y == face->y) {
				face->isEmpty = true;
			}
			else if (y == face->x) {
				face->isEmpty = true;
			}
			else {
				face->z = vert->index;
			}
		}
		/////
		if (y == face->x) {
			if (x == face->y) {
				//face->isEmpty = true;
			}
			else if (x == face->z) {
				//face->isEmpty = true;
			}
			else {
				face->x = vert->index;
			}
		}
		else if (y == face->y) {
			if (x == face->x) {
				//face->isEmpty = true;
			}
			else if (x == face->z) {
				//face->isEmpty = true;
			}
			else {
				face->y = vert->index;
			}
		}
		else if (y == face->z) {
			if (x == face->y) {
				//face->isEmpty = true;
			}
			else if (x == face->x) {
				//face->isEmpty = true;
			}
			else {
				face->z = vert->index;
			}
		}
	}
	/*
	1.找出要合并的两个点，两个点出边集赋值给新点的出边集，
	然后新点添加到vertlist尾巴，把两个点置空（isEmpty）
	新点的Q，x,y,z,isEmpty，outedges等属性都已添加*/

	//for (HE_vert *vert = vertlist; vert != NULL; vert = vert->next) {
	//	for (HE_edge *edge = vert->outEdges; edge != NULL; edge = edge->next) {
	//		if (vert->isEmpty == false && edge->vert->isEmpty == false) {
	//		
	//		}
	//			//cout << vert->index << "->" << edge->vert->index << "nextEdge:" << edge->vert->index << "->" << edge->nextEdge->vert->index << " " << edge->nextEdge->vert->index << "->" << edge->nextEdge->nextEdge->vert->index << " " << edge->nextEdge->nextEdge->vert->index << "->" << edge->nextEdge->nextEdge->nextEdge->vert->index << endl;
	//	}
	//}

	

	

	//修改nextEdge属性
	


	cout << "out" << endl;
	for (HE_edge *edge = getVertByindex(vertlist,5)->outEdges; edge != NULL; edge = edge->next) {
		cout << edge->vert->index ;
	}
	cout << "first" << endl;
	for (Node_int<int>* face = facelist; face != NULL; face = face->next) {
		int flag = 0;
		if(face->isEmpty==false)cout << face->x << " " << face->y << " " << face->z << endl;
		if ((face->x == vert->index || face->y == vert->index || face->z == vert->index) && face->isEmpty == false) {
			//cout << face->x << " " << face->y << " " << face->z << endl;
			//对于特定某个面 检查

			if (vert->index == face->x) {

				if (getPairEdge(vertlist, vert->index, face->y)->isEmpty) {
					cout << "创建边：" << face->y << "->" << vert->index << endl;
					creat_edge(vertlist, face->y, vert->index, face->z);

				}
				else if (getPairEdge(vertlist, vert->index, face->z)->isEmpty) {
					cout << "创建边：" << face->z << "->" << vert->index << endl;
					creat_edge(vertlist, face->z, vert->index, face->y);
				}


				for (HE_edge *p = getVertByindex(vertlist, face->y)->outEdges; p != NULL; p = p->next) {
					//cout << "check " << p->vert->index << endl;
					if (p->vert == vert) {
						for (HE_edge *q = getVertByindex(vertlist, face->z)->outEdges; q != NULL; q = q->next) {
							//cout << "p" << p->vert->index << endl;
							if (q->vert->index == face->y) {
								//cout << "11" << endl;
								p->nextEdge = getPairEdge(vertlist, face->z, vert->index);
							}
						}
					}
				}
				for (HE_edge *p = getVertByindex(vertlist, face->z)->outEdges; p != NULL; p = p->next) {
					//cout << "check " << face->z << p->vert->index << endl;
					if (p->vert == vert) {
						for (HE_edge *q = getVertByindex(vertlist, face->y)->outEdges; q != NULL; q = q->next) {
							//cout << "p" << p->vert->index << endl;
							if (q->vert->index == face->z) {
								//cout << "22" << endl;
								p->nextEdge = getPairEdge(vertlist, face->y, vert->index);
							}
						}
					}
				}


			}
			else if (vert->index == face->y) {

				if (getPairEdge(vertlist, vert->index, face->x)->isEmpty) {
					cout << "创建边：" << face->x << "->" << vert->index << endl;
					creat_edge(vertlist, face->x, vert->index, face->z);
				}
				else if (getPairEdge(vertlist, vert->index, face->z)->isEmpty) {
					cout << "创建边：" << face->z << "->" << vert->index << endl;
					creat_edge(vertlist, face->z, vert->index, face->y);
				}


				for (HE_edge *p = getVertByindex(vertlist, face->z)->outEdges; p != NULL; p = p->next) {
					if (p->vert == vert) {
						for (HE_edge *q = getVertByindex(vertlist, face->x)->outEdges; q != NULL; q = q->next) {
							//cout << "p" << p->vert->index << endl;
							if (q->vert->index == face->z) {
								//cout << "33" << endl;
								p->nextEdge = getPairEdge(vertlist, face->x, vert->index);
							}
						}
					}
				}
				for (HE_edge *p = getVertByindex(vertlist, face->x)->outEdges; p != NULL; p = p->next) {
					if (p->vert == vert) {
						for (HE_edge *q = getVertByindex(vertlist, face->z)->outEdges; q != NULL; q = q->next) {
							//cout << "p" << p->vert->index << endl;
							if (q->vert->index == face->x) {
								//cout << "44" << endl;
								p->nextEdge = getPairEdge(vertlist, face->z, vert->index);
							}
						}
					}
				}
			}
			else if (vert->index == face->z) {

				if (getPairEdge(vertlist, vert->index, face->y)->isEmpty) {
					cout << "创建边：" << face->y << "->" << vert->index << endl;
					creat_edge(vertlist, face->y, vert->index, face->z);
				}
				else if (getPairEdge(vertlist, vert->index, face->x)->isEmpty) {
					cout << "创建边：" << face->x << "->" << vert->index << endl;
					creat_edge(vertlist, face->x, vert->index, face->y);
				}


				for (HE_edge *p = getVertByindex(vertlist, face->y)->outEdges; p != NULL; p = p->next) {
					if (p->vert == vert) {
						for (HE_edge *q = getVertByindex(vertlist, face->x)->outEdges; q != NULL; q = q->next) {
							//cout << "p" << p->vert->index << endl;
							if (q->vert->index == face->y) {
								//cout << "55" << endl;
								p->nextEdge = getPairEdge(vertlist, face->x, vert->index);
							}
						}
					}
				}
				for (HE_edge *p = getVertByindex(vertlist, face->x)->outEdges; p != NULL; p = p->next) {
					if (p->vert == vert) {
						for (HE_edge *q = getVertByindex(vertlist, face->y)->outEdges; q != NULL; q = q->next) {
							//cout << "p" << p->vert->index << endl;
							if (q->vert->index == face->x) {
								//cout << "66" << endl;
								p->nextEdge = getPairEdge(vertlist, face->y, vert->index);
							}
						}
					}
				}
			}

		}


	}

	findAllEdges(vertlist);

	for (Node_int<int>* face = facelist; face != NULL; face = face->next) {
		int flag = 0;
		if ((face->x == vert->index || face->y == vert->index || face->z == vert->index) && face->isEmpty == false) {

			//设置nextEdge,c
			for (HE_edge *edge = vert->outEdges; edge != NULL; edge = edge->next) {
				if (edge->vert->index == face->x || edge->vert->index == face->y || edge->vert->index == face->z) {
					//这里能锁定两个面了。

					int another;
					for (HE_edge*p = edge->vert->outEdges; p != NULL; p = p->next) {
						if (p->vert == vert) {
							another = p->nextEdge->vert->index;
							if (face->x == another || face->y == another || face->z == another) {
								HE_edge *ed = new HE_edge();
								ed = returnnext_Edge(vertlist, facelist, vert->index, edge->vert->index);
								p->nextEdge->nextEdge = returnnext_Edge(vertlist, face, vert->index, another);
								edge->nextEdge->nextEdge = new HE_edge();
								HE_edge *ed2 = new HE_edge();
								ed2 = returnnext_Edge(vertlist, facelist, edge->vert->index, another);
								p->nextEdge->nextEdge->nextEdge = returnnext_Edge(vertlist, face, another, edge->vert->index);
								//cout << "sdf" <<ed->vert->index << " " << ed2->vert->index << endl;
							}


						}

					}



				}
				



			}

			

		}


	}

	//检查vert的出边集中 是否有第三条边的终点不指向vert
	for (HE_edge *e = vert->outEdges; e != NULL; e = e->next) {
		
		if (e->nextEdge->nextEdge->vert != vert ) {
			cout << vert->index << endl;
			
			e->nextEdge->nextEdge = getPairEdge(vertlist, vert->index, e->nextEdge->vert->index);
			e->nextEdge->nextEdge->nextEdge = e;
			e->nextEdge->nextEdge->vert = vert;
			
			//cout << e->vert->index << " " << e->nextEdge->vert->index << e->nextEdge->nextEdge->vert->index << endl;
			//e->nextEdge->nextEdge->nextEdge = e;
		}
	}
	
	

		


	





	
	

	/*
	2.outEdges属性中每条边都添加好，并且每条新边的pair（如果存在）.vert属性指向新点
	outEdges的边的nextEdge属性，pair等属性都要修改（若pair和nextEdge都涉及修改边则修改其属性）
	face属性的abcd等边建完重新计算，并且face.edge属性也修改*/
	for (HE_edge*edge = vert->outEdges; edge != NULL; edge = edge->next) {

		HE_edge *tem = getPairEdge(vertlist, vert->index, edge->vert->index);
		if (tem->isEmpty) {
			edge->pair = NULL;
		}
		else {
			edge->pair = tem;
			tem->pair = edge;
		}

		HE_edge *tem2 = getPairEdge(vertlist, edge->vert->index, edge->nextEdge->vert->index);
		if (tem2->isEmpty) {
			edge->nextEdge->pair = NULL;
		}
		else {
			tem2->pair = edge->nextEdge;
			edge->nextEdge->pair = tem2;
		}

		HE_edge *tem3 = getPairEdge(vertlist, edge->nextEdge->vert->index, vert->index);
		if (tem3->isEmpty) {
			edge->nextEdge->nextEdge->pair = NULL;
		}
		else {
			tem3->pair = edge->nextEdge->nextEdge;
			edge->nextEdge->nextEdge->pair = tem3;
		}

		
		edge->face->edge = edge->nextEdge->face->edge = edge->nextEdge->nextEdge->face->edge = edge;
		getFaceInfo(edge->face, vert, edge->vert, edge->nextEdge->vert);
		edge->nextEdge->face = edge->nextEdge->nextEdge->face = edge->face;
		
		/*cout << "face:" << edge->face->a << " " << edge->face->b << " " << edge->face->c << " " << edge->face->d << " " << edge->face->m << " " << edge->face->edge->vert->index << endl;
		cout << edge->nextEdge->face->edge->vert->index << endl;*/

	}


	//face后需修改
	
	

	




}
//合并点的递归,输入需要合并的两个点
void fact_jointPoint(HE_vert *vertlist,HE_vert *vert1,HE_vert *vert2,Node_int<int> *facelist) {
	Matrix4f mat;
	Matrix4f m;
	mat = vert1->Q + vert1->Q;
	HE_vert *vert = new HE_vert();
	vert->Q = mat;
	m << mat(0), mat(1), mat(2), mat(3), mat(1), mat(5), mat(6), mat(7), mat(2), mat(6), mat(10), mat(11), 0, 0, 0, 1;
	getVertByindex(vertlist, vert1->index)->isEmpty = getVertByindex(vertlist, vert2->index)->isEmpty = true;
	//cout << "empty" << vert1->index << " " << vert1->isEmpty << vert2->isEmpty << endl;
	//若行列式不等于0，则表示可逆
	if ( (m).determinant()) {
		cout << "可逆矩阵" << endl;
		Vector4f u(0, 0, 0, 1);
		cout << u << endl;
		u = m.inverse()*u;
		
		cout << m.inverse() << endl;
		//被合并的点 并不去掉，设置为空，新点尾插链表
		vert->x = u[0]; vert->y = u[1]; vert->z = u[2]; vert->isEmpty = false;
		
	}
	else {
		cout << "不可逆矩阵" << endl;
		//以0.1为单位遍历【0,1】
		float end_cost = 999999;
		float end_k = 0;
		Vector4f u(0, 0, 0, 0);

		float cost = 0;
		for (float k = 0; k <= 1; k = k + 0.1) {

			Vector4f v1(vert1->x, vert1->y, vert1->z, 0); Vector4f v2(vert2->x, vert2->y, vert2->z, 0);
			
			u = (1 - k)*v1 + k*v2;
			cost = u.transpose()*(mat)*u;
			//cout << cost << "cost" << endl;
			
			if (end_cost > cost) {
				end_cost = cost; end_k = k;   }

		}

		vert->x = u[0]; vert->y = u[1]; vert->z = u[2]; vert->isEmpty = false;

	}
	insertListEnd(vertlist, vert);
	//只剩下vert.outEdges属性没有添加了,insert20outedges检测过了
	insert2Outedgesto1(vertlist,vert, vert1, vert2);
	//尾插入vertlist
	//insertListEnd(vertlist, vert);



	//设置outEdges的属性，并且修改facelist
	modifyOutedges(vertlist,facelist,vert,vert1,vert2);



}
//找出符合合并的两个点
Node_int<HE_vert>* find2V(struct HE_vert *vertlist, float dis) {
	HE_vert *vert = new HE_vert();
	HE_edge *edge = new HE_edge();
	Node_int<HE_vert> *result = (struct Node_int<HE_vert>*)malloc(sizeof(struct Node_int<HE_vert>));
	result->isEmpty = true;
	//非空的点 遍历，找出距离小的两个点，需要合并的两个点
	for (vert = vertlist; vert != NULL; vert = vert->next) {
		if (vert->isEmpty == false && vert->index <= getSize(vertlist)) {
			

			for (edge = vert->outEdges; edge != NULL; edge = edge->next) {
				if (Dis2Point(vert, edge->vert) < dis) {
					result->isEmpty = false;
					result->x = *vert;
					result->y = *edge->vert;
					return result;
				}
			}
			
		}
		
	}
	
	return result;

}

void creat_allHE(struct HE_vert *vertlist, struct Node_int<int> *facelist, Node_int<double>*dislist, float percent) {
	
	
	//创建根据face1 创建第一个面
	creat_3edges(vertlist, facelist->x, facelist->y, facelist->z);
	//cout << "创建完第一个面" << facelist->x << endl;


	//根据第一个面的三条边向外递归，拓展到整个面
	find(vertlist, facelist, facelist->x, facelist->y, facelist->z);


	//添加边中的pair和face信息，并且会计算每个面的Kp矩阵
	addEdgePair(vertlist);
	
	//计算每个顶点的Kp求和得Q
	CalVertAllKp(vertlist);
	int v_number = getSize(vertlist);

	//找出阈值距离
	float distance = getAllDis(vertlist, dislist, percent);
	
	
	//通过阈值距离，把所有合并点合并。
	/*while (1) {
		Node_int<HE_vert> *obj = find2V(vertlist, distance);
		if (obj->x.index >= v_number) break;;
		if (!obj->isEmpty && obj->x.index<=v_number) {
		    cout << obj->x.index << " " << obj->y.index << endl;
		    fact_jointPoint(vertlist, &(obj->x), &obj->y,facelist);
	}
	}*/

	
	//遍历新插入点
	/*HE_vert *v = new HE_vert();
	for (v = getVertByindex(vertlist,1043); v != NULL; v = v->next) {
		cout << v->index << endl;
	}*/
	//cout << getVertByindex(vertlist, 1044)->x << getVertByindex(vertlist, 1044)->y << getVertByindex(vertlist, 1044)->z << endl;
	
	//搜索点的数量
	/*HE_vert *v = new HE_vert();
	int j = 0;
	for (v = vertlist; v != NULL ; v = v->next) {
		if( ! v->isEmpty)
		j++;
	}
	cout << j << endl;*/

	return;
}