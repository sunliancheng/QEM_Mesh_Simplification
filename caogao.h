#include "stdafx.h"
#include "Mesh.h" 
#include "Mesh.cpp"
#include "list.h"
#include "stdlib.h"
#include "index.h"
#include <iostream>


using namespace std;



void modifyOutedgesSecond(HE_vert *vertlist, Node_int<int> *facelist, HE_vert *vert, HE_vert *vert1, HE_vert *vert2, myindex<HE_vert> *indexlist) {


	for (Node_int<int> *face = vert->list_allface; face != NULL; face = face->next) {
		if (face->isEmpty == false) {

			for (HE_edge *edge = vert1->outEdges; edge != NULL; edge = edge->next) {
				int i1, i2;
				if ( edge->vert->index != vert2->index  && edge->nextEdge->vert->index != vert2->index) {
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


}



/*
3.facelist中同时包含两个点的面置isEmpty为空，并且修改包含一个点的面
4.1-3是面向内在点的收缩
*/
//for (Node_int<int>*face = facelist; face != NULL; face = face->next) {
//	int x = vert1->index; int y = vert2->index;
//	if (x == face->x) {
//		if (y == face->y) {
//			face->isEmpty = true;
//		}
//		else if (y == face->z) {
//			face->isEmpty = true;
//		}
//		else {
//			face->x = vert->index;
//		}
//	}
//	else if (x == face->y) {
//		if (y == face->x) {
//			face->isEmpty = true;
//		}
//		else if (y == face->z) {
//			face->isEmpty = true;
//		}
//		else {
//			face->y = vert->index;
//		}
//	}
//	else if (x == face->z) {
//		if (y == face->y) {
//			face->isEmpty = true;
//		}
//		else if (y == face->x) {
//			face->isEmpty = true;
//		}
//		else {
//			face->z = vert->index;
//		}
//	}
//	/////
//	if (y == face->x) {
//		if (x == face->y) {
//			//face->isEmpty = true;
//		}
//		else if (x == face->z) {
//			//face->isEmpty = true;
//		}
//		else {
//			face->x = vert->index;
//		}
//	}
//	else if (y == face->y) {
//		if (x == face->x) {
//			//face->isEmpty = true;
//		}
//		else if (x == face->z) {
//			//face->isEmpty = true;
//		}
//		else {
//			face->y = vert->index;
//		}
//	}
//	else if (y == face->z) {
//		if (x == face->y) {
//			//face->isEmpty = true;
//		}
//		else if (x == face->x) {
//			//face->isEmpty = true;
//		}
//		else {
//			face->z = vert->index;
//		}
//	}
//}

//修改nextEdge属性

//for (Node_int<int>*face = facelist; face != NULL; face = face->next) {
//	
//	if (face->isEmpty == false && (vert->index == face->x || vert->index == face->y || vert->index == face->z )  ) {
//		//cout << vert->index << face->x << face->y << face->z << endl;
//		for (Node_int<int>* f = facelist; f != NULL; f = f->next) {
//			int flag = 0;
//			int i1 = 0, i2 = 0;
//			//先从空点中 查找表的顺序
//			//从点集中寻找 所有空点的出边对应三角形5
//			for (HE_vert *vertt = vertlist; vertt != NULL; vertt = vertt->next) {
//				if (vertt->isEmpty) {
//					//cout << "index:" << vert->index << endl;
//					//cout << vertt->index << " " <<vert1->index << vert2->index<< "face"<<f->x<<f->y<<f->z<<endl;
//					if ( (vertt->index == vert1->index || vertt->index == vert2->index) && (vertt->index == f->x || vertt->index == f->y || vertt->index == f->z) ) {
//						//cout << "tssttststst" << endl;
//						for (HE_edge *edge = vertt->outEdges; edge != NULL; edge = edge->next) {
//							//cout << "tssttststst" << endl;
//							if (edge->vert->index != vert1->index && edge->vert->index != vert2->index && edge->nextEdge->vert->index != vert1->index && edge->nextEdge->vert->index != vert2->index ) {
//								i1 = edge->vert->index; i2 = edge->nextEdge->vert->index;
//								//cout << "tssttststst" << endl;
//								if ((i1 == face->x || i1 == face->y || i1 == face->z) && (i2 == face->x || i2 == face->y || i2 == face->z)) {
//									//cout << "tssttststst"<<endl;
//									for (HE_edge*p = vert->outEdges; p != NULL; p = p->next) {
//										if (p->vert->index == i1 && getPairEdge(vertlist,vert->index,i2,indexlist)->isEmpty == true) {
//											//cout << " " << i1 << " " << i2 << "  " << vertt->index << " " << edge->vert->index << endl;
//											//cout << face->x << face->y << face->z << endl;
//											p->nextEdge = returnnext_Edge(vertlist, face, vert->index, i1, indexlist);
//											creat_edge(vertlist, i2, vert->index, 1,indexlist);
//											p->nextEdge->nextEdge = returnnext_Edge(vertlist, face, i1, i2, indexlist);
//											p->nextEdge->nextEdge->nextEdge = p;
//										}
//									}
//								}
//							}
//						}
//					}
//				}
//			}
//		}
//	}
//}



//findAllEdges(vertlist);
//检查vert的出边集中 是否有第三条边的终点不指向vert
//for (HE_edge *e = vert->outEdges; e != NULL; e = e->next) {
//	if (e->nextEdge->nextEdge->vert != vert) {
//		cout << vert->index << endl;
//		e->nextEdge->nextEdge = getPairEdge(vertlist, vert->index, e->nextEdge->vert->index);
//		e->nextEdge->nextEdge->nextEdge = e;
//		e->nextEdge->nextEdge->vert = vert;
//		//cout << e->vert->index << " " << e->nextEdge->vert->index << e->nextEdge->nextEdge->vert->index << endl;
//		//e->nextEdge->nextEdge->nextEdge = e;
//	}
//}



//新代码
//for (Node_int<int>*face = facelist; face != NULL; face = face->next) {
//	int x = vert1->index; int y = vert2->index;
//	if (x == face->x) {
//		if (y == face->y) {
//			face->isEmpty = true;
//		}
//		else if (y == face->z) {
//			face->isEmpty = true;
//		}
//		else {
//			face->x = vert->index;
//		}
//	}
//	else if (x == face->y) {
//		if (y == face->x) {
//			face->isEmpty = true;
//		}
//		else if (y == face->z) {
//			face->isEmpty = true;
//		}
//		else {
//			face->y = vert->index;
//		}
//	}
//	else if (x == face->z) {
//		if (y == face->y) {
//			face->isEmpty = true;
//		}
//		else if (y == face->x) {
//			face->isEmpty = true;
//		}
//		else {
//			face->z = vert->index;
//		}
//	}
//	/////
//	if (y == face->x) {
//		if (x == face->y) {
//			//face->isEmpty = true;
//		}
//		else if (x == face->z) {
//			//face->isEmpty = true;
//		}
//		else {
//			face->x = vert->index;
//		}
//	}
//	else if (y == face->y) {
//		if (x == face->x) {
//			//face->isEmpty = true;
//		}
//		else if (x == face->z) {
//			//face->isEmpty = true;
//		}
//		else {
//			face->y = vert->index;
//		}
//	}
//	else if (y == face->z) {
//		if (x == face->y) {
//			//face->isEmpty = true;
//		}
//		else if (x == face->x) {
//			//face->isEmpty = true;
//		}
//		else {
//			face->z = vert->index;
//		}
//	}
//}
//修改
//修改