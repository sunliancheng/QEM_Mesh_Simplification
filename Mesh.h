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
#include "index.h"
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
	//点所有涉及到的边
	struct Node_int<int>* list_allface;
	Matrix4f Q;

	//记录出边点的索引,采用图论的方式
	//struct Node_int<int>outEdges;
	struct HE_vert *next;

	HE_vert() {
		outEdges = new HE_edge();
		outEdges->isEmpty = true;
		outEdges->vert = NULL;
		list_allface =  (struct Node_int<int>*)malloc(sizeof(struct Node_int<int>));
		list_allface->isEmpty = true;
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
	//此处face_verts指向facelist某一点
	Node_int<int> *face_verts;
	HE_face() {
		edge = NULL;
		a = b = c = d = 0;
	}
};
HE_edge creat_edge(struct HE_vert *vertlist, int x, int y, int z, myindex<HE_vert> *indexlist);
HE_vert* getVertByindex(struct HE_vert *vertlist, int index, myindex<HE_vert> *indexlist);
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
void getFile(string file, struct HE_vert *vertlist, struct Node_int<int> *facelist, myindex<HE_vert> *indexlist) {
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
					insert_index(1, p, indexlist);
					//cout << "in : other" << p->y << endl;
				}
				else {
					p1 = new HE_vert();
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
					if (p->index % 10 == 0) {
						insert_index(p->index, p, indexlist);
					}
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
			/*getVertByindex(vertlist, stringToNum<int>(v[1]),indexlist)->isEmpty = false;
			getVertByindex(vertlist, stringToNum<int>(v[2]), indexlist)->isEmpty = false;
			getVertByindex(vertlist, stringToNum<int>(v[3]), indexlist)->isEmpty = false;*/

			Node_int<int>* face = (struct Node_int<int>*)malloc(sizeof(struct Node_int<int>));
			
			face = insert2<int>( stringToNum<int>(v[1]), stringToNum<int>(v[2]), stringToNum<int>(v[3]),facelist);
			
			
			insert_face<int>(face, getVertByindex(vertlist, stringToNum<int>(v[1]), indexlist)->list_allface);
			insert_face<int>(face, getVertByindex(vertlist, stringToNum<int>(v[2]), indexlist)->list_allface);
			insert_face<int>(face, getVertByindex(vertlist, stringToNum<int>(v[3]), indexlist)->list_allface);
		
			
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
int getSize(HE_vert *vertlist, myindex<HE_vert> *indexlist) {
	HE_vert *vert;
	int j = 0;
	for (vert = find_end(indexlist); vert != NULL; vert = vert->next) {
		j++;
	}
	return j+find_end(indexlist)->index-1;
}
int getsize_vertlist(HE_vert *outEdges) {
	int i = 0;
	HE_vert *p;
	//p = (struct HE_vert*)malloc(sizeof(struct HE_vert));
	for (p = outEdges; p != NULL; p = p->next) {
		i++;
	}
	return i;
}
//根据index，找出点的指针
HE_vert* getVertByindex(struct HE_vert *vertlist,int index, myindex<HE_vert> *indexlist) {
	//HE_vert *new_vert = new HE_vert();
	HE_vert *new_vert;
	//new_vert->outEdges = new HE_edge();
	/*for (new_vert = vertlist; new_vert != NULL; new_vert = new_vert->next) {
	   
		if (index == new_vert->index) {  return new_vert; }
	}
	return new_vert;*/

	//新方法
	//cout << index << " " << find_index(index, indexlist)->index << " " << getSize(vertlist,indexlist) << endl;
	int count = 0;
	for (new_vert = find_index(index, indexlist); new_vert != NULL; new_vert = new_vert->next) {
		//cout << "sdsd" << new_vert->index <<endl;
		if (count >= 10) break;
		count++;
		if (index == new_vert->index) {
			//cout << index << " " << endl;
			return new_vert;
		}
	}
	return new_vert;

}

//找出对边,y为入点，x为出点,找出的边为y->x;如果isEmpty为空则说明没找到
HE_edge* getPairEdge(struct HE_vert *vertlist, int x, int y,myindex<HE_vert>* indexlist) {
	HE_vert *vert;
	HE_edge *edge;
	HE_edge *edge2 = new HE_edge();

	

	//新代码
	for (edge = getVertByindex(vertlist,y,indexlist)->outEdges; edge != NULL; edge = edge->next) {
		//cout << edge->vert->index << endl;
		if (edge->vert != NULL)
		{
			//cout << "d" << endl;
			if (edge->vert->index == x) { edge->isEmpty = false; return edge; }
		}
	}
	edge2->isEmpty = true;


	 //delete(vert); delete(edge2); vert = NULL; edge = NULL;
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


		//p1 = new HE_edge();
		//p1->face = new HE_face();
		//p1->next = new HE_edge();
		//p1->nextEdge = new HE_edge();
		//p1->pair = new HE_edge();
		//p1->vert = new HE_vert();

		tem = new HE_edge();
		//tem->face = new HE_face();
		//tem->next = new HE_edge();
		//tem->nextEdge = new HE_edge();
		//tem->pair = new HE_edge();
		//tem->vert = new HE_vert();





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
		//此处 修改face的点集属性，可以以先不修改，影响不大
		
		//生成面的abcd属性
		getFaceInfo(edge->face, edge->vert, edge->nextEdge->vert, edge->nextEdge->nextEdge->vert);
		//cout << "face:" << edge->face->a << edge->face->b << edge->face->c << edge->face->d << endl;
	}
	
	
}

int i = 0;
//创建边，x->y,edgelist 为 vertlist.outedges
HE_edge creat_edge(struct HE_vert *vertlist,  int x, int y, int z, myindex<HE_vert> *indexlist) {

	//findAllEdges(vertlist);

	HE_edge *edge = new HE_edge();
//	edge->vert = NULL;
	edge->face = new HE_face();
	//edge->next = new HE_edge();
	//edge->nextEdge = new HE_edge();
	//edge->pair = new HE_edge();


	   
		//找出终点，指向终点
		//edge->vert = new HE_vert();
		edge->vert = getVertByindex(vertlist, y,indexlist);
		

		//isEmpty 设置为空
		edge->isEmpty = false;
		
		
		insertEdgelist(getVertByindex(vertlist,x,indexlist)->outEdges,edge,vertlist);
		//cout << "创建边" << x << "->" << y << endl;
		
		//cout << "already insert" << endl;
		return *edge;
		//nextEdge
		
		


}
HE_edge* findEdgeInlist(HE_edge *edgelist, int end) {
	HE_edge *edge;
	HE_edge *edge2 = new HE_edge();
	for (edge = edgelist; edge != NULL; edge = edge->next) {
		if (edge->vert->index == end ) {
			return edge;
		}
	}
	return edge2;
}
//创建一个面的三条边
void creat_3edges(struct HE_vert *vertlist, int x, int y, int z,myindex<HE_vert> *indexlist) {
	//cout << "创建面:" << x << " " << y << " " << z << endl;
	creat_edge(vertlist, x, y, z,indexlist);
	creat_edge(vertlist, y, z, x,indexlist);
	creat_edge(vertlist, z, x, y,indexlist);
	HE_edge *edge ;
	//edge->nextEdge = new HE_edge();
	//edge->nextEdge->vert = new HE_vert();
	//edge->vert = new HE_vert();
	for (edge = getVertByindex(vertlist,x,indexlist)->outEdges; edge != NULL; edge = edge->next) {
		

		if (edge->vert->index == y) {
			edge->nextEdge =  findEdgeInlist(getVertByindex(vertlist,y,indexlist)->outEdges, z);
		}
	}
	for (edge = getVertByindex(vertlist, y,indexlist)->outEdges; edge != NULL; edge = edge->next) {


		if (edge->vert->index == z) {
			edge->nextEdge = findEdgeInlist(getVertByindex(vertlist, z,indexlist)->outEdges, x);
		}
	}
	for (edge = getVertByindex(vertlist, z,indexlist)->outEdges; edge != NULL; edge = edge->next) {


		if (edge->vert->index == x) {
			edge->nextEdge = findEdgeInlist(getVertByindex(vertlist, x,indexlist)->outEdges, y);
		}
	}


	

}
//通过两个点查找面，返回另一个面，若返回-1则没找到,还得判断这个面存不存在
int isInFacelist(struct Node_int<int> *facelist, int x, int y,int z, struct HE_vert *vertlist, myindex<HE_vert> *indexlist) {
	//struct Node_int<int> *face = (struct Node_int<int>*)malloc(sizeof(struct Node_int<int>));
	/*for (face = facelist; face != NULL; face = face->next) {
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
	}*/

	//return -1;

	//新代码
	for (Node_int<int> *face = getVertByindex(vertlist, x, indexlist)->list_allface; face != NULL; face = face->next) {
		if (face->isEmpty == false) {
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
	}
	return -1;

}

void find(struct HE_vert *vertlist, struct Node_int<int> *facelist, int x, int y, int z, myindex<HE_vert> *indexlist) {
	int another_z = isInFacelist(facelist, x, y, z,vertlist,indexlist); 
	int another_x = isInFacelist(facelist, y, z, x, vertlist, indexlist);
	int another_y = isInFacelist(facelist, z, x, y, vertlist, indexlist);
	//cout << another_x << another_y << another_z << endl;
	//没找到另一个点，意味着这条是边界
	if (another_z != -1  && getPairEdge(vertlist,x,y, indexlist)->isEmpty)  {
		//cout << "创建面1:"<< y<<" "<<x<<" "<<another_z << endl;
		creat_3edges(vertlist, y, x, another_z,indexlist);
		find(vertlist, facelist, y, x, another_z,indexlist);
	}
	if (another_x != -1 && getPairEdge(vertlist, y, z, indexlist)->isEmpty) {
		//cout << "创建面2:" << z << " " << y << " " << another_x << endl;
		creat_3edges(vertlist, z, y, another_x,indexlist);
		find(vertlist, facelist, z, y, another_x,indexlist);
	}
	if (another_y != -1 && getPairEdge(vertlist, z, x, indexlist)->isEmpty) {
		//cout << "创建面3:" << x << " " << z << " " << another_y << endl;
		creat_3edges(vertlist, x, z, another_y,indexlist);
		find(vertlist, facelist, x, z, another_y,indexlist);
	}
	

}
//通过点集链表找出所有边
void findAllEdges(struct HE_vert *vertlist) {
	HE_vert *vert ;
	HE_edge *edge ;
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
	HE_edge *edge;
	for (edge = edges; edge != NULL; edge = edge->next) {
		if (!edge->isEmpty && !edge->vert->isEmpty) {
			//cout << "找到边：" << edge->pair->vert->index << "->" << edge->vert->index << endl;
			cout << "找到边："  << "->" << edge->vert->index << endl;
		}
	}
}
//给所有边添加上pair, 修改过内存分配
void addEdgePair(struct HE_vert *vertlist,myindex<HE_vert>* indexlist) {
	HE_vert *vert;
	HE_edge *edge;
	HE_edge *edgepair;
	for (vert = vertlist; vert != NULL; vert = vert->next) {
		if (!vert->isEmpty && !vert->outEdges->isEmpty) {
			for (edge = vert->outEdges; edge != NULL; edge = edge->next) {
				if (!edge->isEmpty && !edge->vert->isEmpty) {
					edgepair = getPairEdge(vertlist,vert->index,edge->vert->index, indexlist);
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
//计算所有点的Q     修改过内存分配
void CalVertAllKp(struct HE_vert *vertlist) {

	struct HE_vert *vert;
	struct HE_edge *edge;
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
	HE_vert *vert ;
	HE_edge *edge ;
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
//链表尾插     修改过内存分配
void insertListEnd(HE_vert *vertlist,HE_vert *vert, myindex<HE_vert> *indexlist) {
	HE_vert *p;
	for (p = find_end<HE_vert>(indexlist); p->next != NULL; p = p->next) {
		
	}
	vert->index = p->index + 1;
	p->next = vert;
	vert->next = NULL;
	if (vert->index % 10 == 0) {
		insert_index(vert->index, vert, indexlist);
	}
}
//把两个点的所有outEdges插入到第一个点中,并且把两个点所有list_allfaces插入到新点中
void insert2Outedgesto1(HE_vert*vertlist,HE_vert *vert,HE_vert *vert1, HE_vert *vert2, myindex<HE_vert> *indexlist) {
	//源代码
	//HE_edge *edge = new HE_edge(); edge->vert = new HE_vert();
	//HE_edge *edge2 = new HE_edge();vert->outEdges = new HE_edge();
	//新代码
	HE_edge *edge;
	HE_edge *edge2;
	//vert->outEdges = new HE_edge();

	for (edge = vert1->outEdges; edge != NULL ; edge = edge->next) {
		//cout << edge->vert->index;
		if ( edge->vert->isEmpty == false && edge->vert->index != vert2->index && edge->nextEdge->vert->index != vert2->index) {
			
			creat_edge(vertlist, vert->index, edge->vert->index,1,indexlist);

		}
	}
	
	//findAlledgeByedge(getVertByindex(vertlist,5)->outEdges);
	//此处插入函数有问题，没有设置最后一个的next为null
	for (edge = vert2->outEdges; edge != NULL; edge = edge->next) {
		int flag = 0;
		for (edge2 = vert->outEdges; edge2 != NULL ; edge2 = edge2->next) {
			//cout<< "edge2.isempty:" << edge2->isEmpty << endl;
			//如果2的边不在1中，则尾插，在则不插
			if (edge2->isEmpty == true) {
				if (edge->vert->isEmpty == true || edge->vert->index == vert1->index || edge->nextEdge->vert->index == vert1->index) flag = 1;
			}
			else if ( edge->vert->isEmpty == true || edge->vert->index == edge2->vert->index || edge->vert->index == vert1->index || edge->nextEdge->vert->index == vert1->index) {
				flag = 1;	
			}
		}
		if (flag == 0) {
			creat_edge(vertlist, vert->index, edge->vert->index,1,indexlist);
			//insertEdgelist(vert->outEdges, edge, vert);
		}
		
	}
	vert1->isEmpty = vert2->isEmpty = true;
	//cout << "finish insert20o" << endl;


	//把点12的面 添加到 收缩点 中，并且把删除面 删除
	for (Node_int<int> *face1 = vert1->list_allface; face1 != NULL; face1 = face1->next) {
		if (face1->isEmpty == false && vert2->index != face1->x && vert2->index!=face1->y && vert2->index != face1->z) {
			Node_int<int> *face = new Node_int<int>();
			face->isEmpty = false; face->x = face1->x; face->y = face1->y; face->z = face1->z;
			insert_face(face,vert->list_allface);
			//face1->isEmpty = true;
		}
		else face1->isEmpty = true;
	}
	for (Node_int<int>*face2 = vert2->list_allface; face2 != NULL; face2 = face2->next) {
		if (face2->isEmpty == false && vert1->index != face2->x && vert1->index != face2->y && vert1->index != face2->z) {
			Node_int<int> *face = new Node_int<int>();
			face->isEmpty = false; face->x = face2->x; face->y = face2->y; face->z = face2->z;
			insert_face(face, vert->list_allface);
			//face2->isEmpty = true;
		}
		else face2->isEmpty = true;
	}


}
//根据一个面的三个点，以及一个边 返回下一个边
HE_edge* returnnext_Edge(HE_vert* vertlist,Node_int<int>*facelist, int edge_v1, int edge_v2,myindex<HE_vert> *indexlist) {
	int x = facelist->x; int y = facelist->y; int z = facelist->z;
	if (edge_v1 == x) {
		if (edge_v2 == y)return getPairEdge(vertlist,z,y,indexlist);
		else if (edge_v2 == z)return getPairEdge(vertlist,y,z, indexlist);
	}
	else if (edge_v1 == y) {
		if (edge_v2 == x)return getPairEdge(vertlist, z, x, indexlist);
		else if (edge_v2 == z)return getPairEdge(vertlist, x, z, indexlist);
	}
	else if (edge_v1 == z) {
		if (edge_v2 == x)return getPairEdge(vertlist, y, x, indexlist);
		else if (edge_v2 == y)return getPairEdge(vertlist, x, y, indexlist);
	}

}

//修改outedge和facelist属性,这种方法针对有边界点
void modifyOutedgesSecond(HE_vert *vertlist, Node_int<int> *facelist, HE_vert *vert, HE_vert *vert1, HE_vert *vert2, myindex<HE_vert> *indexlist) {

	
    //修改vert点属性
	for (Node_int<int> *face = vert->list_allface; face != NULL; face = face->next) {
		
		if (face->x == vert1->index || face->x == vert2->index) {
			face->x = vert->index;
		}
		else if (face->y == vert1->index || face->y == vert2->index) {
			face->y = vert->index;
		}
		else if (face->z == vert1->index || face->z == vert2->index) {
			face->z = vert->index;
		}
		//cout << vert->index << " " << face->x << " " << face->y << " " << face->z << endl;
	}
	//把vert周围点相应点的face置为空
	for (HE_edge *edge = vert->outEdges; edge != NULL; edge = edge->next) {
		if (edge->vert->isEmpty == false) {
			for (Node_int<int>*face = edge->vert->list_allface; face != NULL; face = face->next) {
				if (face->isEmpty == false) {
					if ((vert1->index == face->x || vert1->index == face->y || vert1->index == face->z) && (vert2->index == face->x || vert2->index == face->y || vert2->index == face->z)) {
						face->isEmpty = true;
					}
					else if ((vert1->index == face->x || vert1->index == face->y || vert1->index == face->z)) {
						if (vert1->index == face->x) face->x = vert->index;
						else if (vert1->index == face->y) face->y = vert->index;
						else if (vert1->index == face->z) face->z = vert->index;
					}
					else if ((vert2->index == face->x || vert2->index == face->y || vert2->index == face->z)) {
						if (vert2->index == face->x) face->x = vert->index;
						else if (vert2->index == face->y) face->y = vert->index;
						else if (vert2->index == face->z) face->z = vert->index;
					}
				}
			}
		}
	}


	//修改nextEdge属性

	for (Node_int<int> *face = vert->list_allface; face != NULL; face = face->next) {
		if (face->isEmpty == false) {

			for (HE_edge *edge = vert1->outEdges; edge != NULL; edge = edge->next) {
				int i1, i2;
				if (edge->vert->index != vert2->index  && edge->nextEdge->vert->index != vert2->index) {
					//边的顺序为 vert-i1-i2
					i1 = edge->vert->index; i2 = edge->nextEdge->vert->index;
					if ((i1 == face->x || i1 == face->y || i1 == face->z) && (i2 == face->x || i2 == face->y || i2 == face->z)) {
						//cout << "tssttststst"<<endl;
						for (HE_edge*p = vert->outEdges; p != NULL; p = p->next) {
							if (p->vert->index == i1 && getPairEdge(vertlist, vert->index, i2, indexlist)->isEmpty == true) {
								//cout << " " << i1 << " " << i2 << "  " << vertt->index << " " << edge->vert->index << endl;
								//cout << face->x << face->y << face->z << endl;
								p->nextEdge = returnnext_Edge(vertlist, face, vert->index, i1, indexlist);
								creat_edge(vertlist, i2, vert->index, 1, indexlist);
								p->nextEdge->nextEdge = returnnext_Edge(vertlist, face, i1, i2, indexlist);
								p->nextEdge->nextEdge->nextEdge = p;
							}
						}
					}
				}

			}



			for (HE_edge *edge = vert2->outEdges; edge != NULL; edge = edge->next) {

				int i1, i2;
				if (edge->vert->index != vert1->index  && edge->nextEdge->vert->index != vert1->index) {
					//边的顺序为 vert-i1-i2
					i1 = edge->vert->index; i2 = edge->nextEdge->vert->index;
					if ((i1 == face->x || i1 == face->y || i1 == face->z) && (i2 == face->x || i2 == face->y || i2 == face->z)) {
						//cout << "tssttststst"<<endl;
						for (HE_edge*p = vert->outEdges; p != NULL; p = p->next) {
							if (p->vert->index == i1 && getPairEdge(vertlist, vert->index, i2, indexlist)->isEmpty == true) {
								//cout << " " << i1 << " " << i2 << "  " << vertt->index << " " << edge->vert->index << endl;
								//cout << face->x << face->y << face->z << endl;
								p->nextEdge = returnnext_Edge(vertlist, face, vert->index, i1, indexlist);
								creat_edge(vertlist, i2, vert->index, 1, indexlist);
								p->nextEdge->nextEdge = returnnext_Edge(vertlist, face, i1, i2, indexlist);
								p->nextEdge->nextEdge->nextEdge = p;
							}
						}
					}
				}

			}



		}
	}

	/*
	2.outEdges属性中每条边都添加好，并且每条新边的pair（如果存在）.vert属性指向新点
	outEdges的边的nextEdge属性，pair等属性都要修改（若pair和nextEdge都涉及修改边则修改其属性）
	face属性的abcd等边建完重新计算，并且face.edge属性也修改*/
	for (HE_edge*edge = vert->outEdges; edge != NULL; edge = edge->next) {



		//cout << "shousuo" << vert->index << edge->nextEdge->vert->index << endl;

		HE_edge *tem = getPairEdge(vertlist, vert->index, edge->vert->index,indexlist);
		if (tem->isEmpty) {
			edge->pair = NULL;
		}
		else {
			edge->pair = tem;
			tem->pair = edge;
		}
		HE_edge *tem2 = new HE_edge();
		if (edge->nextEdge->isEmpty == false) {
			HE_edge *tem2 = getPairEdge(vertlist, edge->vert->index, edge->nextEdge->vert->index,indexlist);
		}
		if (tem2->isEmpty) {
			edge->nextEdge->pair = NULL;
		}
		else {
			tem2->pair = edge->nextEdge;
			edge->nextEdge->pair = tem2;
		}
		HE_edge *tem3 = new HE_edge();
		if (edge->nextEdge->isEmpty == false) {
			HE_edge *tem3 = getPairEdge(vertlist, edge->vert->index, edge->nextEdge->vert->index,indexlist);
		}
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
		
	}


	//face后需修改

}
//合并点的递归,输入需要合并的两个点,这种方法只适合两个点都不是边界点
void fact_jointPoint(HE_vert *vertlist,HE_vert *vert1,HE_vert *vert2,Node_int<int> *facelist, myindex<HE_vert> *indexlist) {

	if (vert1->isEmpty == false && vert2->isEmpty == false) {
		//cout << "收缩边:" << vert1->index << "->" << vert2->index <<"  ";

		Matrix4f mat;
		Matrix4f m;
		mat = vert1->Q + vert2->Q;
		HE_vert *vert = new HE_vert();
		vert->Q = mat;
		m << mat(0), mat(1), mat(2), mat(3), mat(1), mat(5), mat(6), mat(7), mat(2), mat(6), mat(10), mat(11), 0, 0, 0, 1;
		getVertByindex(vertlist, vert1->index,indexlist)->isEmpty = getVertByindex(vertlist, vert2->index,indexlist)->isEmpty = true;
		//cout << "empty" << vert1->index << " " << vert1->isEmpty << vert2->isEmpty << endl;
		//若行列式不等于0，则表示可逆
		if ((m).determinant()) {
			//cout << "可逆矩阵" << endl;
			Vector4f u(0, 0, 0, 1);
			//cout << u << endl;
			u = m.inverse()*u;

			//cout << m.inverse() << endl;
			//被合并的点 并不去掉，设置为空，新点尾插链表
			vert->x = u[0]; vert->y = u[1]; vert->z = u[2]; vert->isEmpty = false;
			if (Dis2Point(vert, vert1) > 2*Dis2Point(vert1,vert2)) {
				//cout << "ds" << endl;
				vert->x = (vert1->x+vert2->x)/2; vert->y = (vert1->y + vert2->y) / 2; vert->z = (vert1->z + vert2->z) / 2;
			}
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
					end_cost = cost; end_k = k;
				}

			}

			vert->x = u[0]; vert->y = u[1]; vert->z = u[2]; vert->isEmpty = false;

		}

		//cout << vert->x << " " << vert->y << " " << vert->z << " " << endl;


		insertListEnd(vertlist, vert,indexlist);
		//只剩下vert.outEdges属性没有添加了,insert20outedges检测过了
		insert2Outedgesto1(vertlist, vert, vert1, vert2, indexlist);
		//尾插入vertlist
		//insertListEnd(vertlist, vert);



		//设置outEdges的属性，并且修改facelist,两个点都是内点
		//modifyOutedges(vertlist,facelist,vert,vert1,vert2);

		//针对有边界点
		modifyOutedgesSecond(vertlist, facelist, vert, vert1, vert2,indexlist);
	}



}
//找出符合合并的两个点,可能有问题      修改过内存分配
Node_int<HE_vert>* find2V(struct HE_vert *vertlist, float dis, myindex<HE_vert> *indexlist) {
	HE_vert *vert;
	HE_edge *edge;
	Node_int<HE_vert> *result = (struct Node_int<HE_vert>*)malloc(sizeof(struct Node_int<HE_vert>));
	result->isEmpty = true;
	//非空的点 遍历，找出距离小的两个点，需要合并的两个点
	for (vert = vertlist; vert != NULL; vert = vert->next) {
		if (vert->isEmpty == false && vert->index <= getSize(vertlist,indexlist)) {
			

			for (edge = vert->outEdges; edge != NULL; edge = edge->next) {
				if (Dis2Point(vert, edge->vert) < dis && edge->vert->isEmpty ==false) {
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

//打印vertlist和facelist 写入obj文件
void PrintObjFile(HE_vert *vertlist, myindex<HE_vert> *indexlist,string filename) {

	ofstream out;
	out.open(filename);

	for (HE_vert *v = vertlist; v != NULL; v = v->next) {
		if (1) {
			out << "v " << v->x << " " << v->y << " " << v->z << endl;
		}
	}

	for (HE_vert *v = vertlist; v != NULL; v = v->next) {
		if (v->isEmpty == false) {
			for (Node_int<int>*face = v->list_allface; face != NULL; face = face->next) {

				if ( face->isEmpty == false) {
					//cout<<"f " << face->x << " " << face->y << " " << face->z << endl;
					out << "f " << face->x << " " << face->y << " " << face->z << endl;
					for (Node_int<int>*f = getVertByindex(vertlist, face->x, indexlist)->list_allface; f != NULL; f = f->next) {
						if ((face->y == f->x || face->y == f->y || face->y == f->z) && (face->z == f->x || face->z == f->y || face->z == f->z)) {
							f->isEmpty = true;
							
						}
					}
					for (Node_int<int>*f = getVertByindex(vertlist, face->y, indexlist)->list_allface; f != NULL; f = f->next) {
						if ((face->x == f->x || face->x == f->y || face->x == f->z) && (face->z == f->x || face->z == f->y || face->z == f->z)) {
							f->isEmpty = true;
							
						}
					}
					for (Node_int<int>*f = getVertByindex(vertlist, face->z, indexlist)->list_allface; f != NULL; f = f->next) {
						if ((face->y == f->x || face->y == f->y || face->y == f->z) && (face->x == f->x || face->x == f->y || face->x == f->z)) {
							f->isEmpty = true;
							
						}
					}

				}

			}
		}
	}

	out.close();


}
//重新排顺序 vertlist
HE_vert* orderNewVertlist(HE_vert* vertlist, myindex<HE_vert>* indexlist) {
	HE_vert* new_vertlist = new HE_vert();
	int i = 0;
	for (HE_vert *v = vertlist; v != NULL; v = v->next) {
		if (v->isEmpty == false) {
			i++;
			cout << i << endl;
			HE_vert *vert = new HE_vert();
			vert->isEmpty = false;
			vert->index = v->index; vert->list_allface = v->list_allface; vert->x = v->x; vert->y = v->y; vert->z = v->z;
			insertListEnd(new_vertlist, vert, indexlist);
		}
	}
	return new_vertlist;
}
//创建面
void creat_allHE(struct HE_vert *vertlist, struct Node_int<int> *facelist, Node_int<double>*dislist, float percent, myindex<HE_vert> *indexlist) {
	
	cout << "读取文件完毕，正在初始化..." << endl;
	//创建根据face1 创建第一个面
	creat_3edges(vertlist, facelist->x, facelist->y, facelist->z,indexlist);

	//根据第一个面的三条边向外递归，拓展到整个面
	find(vertlist, facelist, facelist->x, facelist->y, facelist->z,indexlist);

	//添加边中的pair和face信息，并且会计算每个面的Kp矩阵
	addEdgePair(vertlist, indexlist);
	
	//计算每个顶点的Kp求和得Q
	CalVertAllKp(vertlist);
	
	//找出阈值距离
    float distance = getAllDis(vertlist, dislist, percent);
	cout << "dis:" << distance << endl;
	//distance = 3.56318;
	
	//通过阈值距离，把所有合并点合并。
	cout << "开始收缩" << endl;
	int count = 7500;
	while (count) {
		if (count % 100 == 0) {
			cout << count << endl;
		}
		count--;
		Node_int<HE_vert> *obj = find2V(vertlist, distance,indexlist);
		//cout << "obj:" << obj->x.index<<" "<<obj->y.index << endl;
		int v_number = getSize(vertlist,indexlist);
		//cout << "v_number" << v_number << endl;
		if (obj->x.index > v_number) break;
		if (!obj->isEmpty && obj->x.index<=v_number) {
		    //cout << obj->x.index << " " << obj->y.index << endl;
		    fact_jointPoint(vertlist, &(obj->x), &obj->y,facelist,indexlist);
	}
	}

	cout << "开始写入文件" << endl;
	PrintObjFile(vertlist,indexlist, "new_obj/test999.obj");


	return;
}

