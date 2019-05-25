

#include "stdafx.h"
#include "Mesh.h" 
#include "Mesh.cpp"
#include "list.h"
#include "stdlib.h"
#include "index.h"
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
	
	struct Node_int<int> *facelist;
	struct Node_int<double> *dislist;
	struct myindex<HE_vert> *indexlist = new myindex<HE_vert>();
	facelist = (struct Node_int<int>*)malloc(sizeof(struct Node_int<int>));
	dislist = (struct Node_int<double>*)malloc(sizeof(struct Node_int<double>));//从第二个点开始插入

	initial_indexlist(indexlist);


	string filename = "obj/bottle.obj";
	string filename2 = "new_obj/test2.obj";
	HE_vert *vertlist = new HE_vert();
	getFile(filename,vertlist,facelist,indexlist);
	


	//if(getVertByindex(vertlist, 1043, indexlist)->next==NULL)cout<<"sdf"<<endl;
	//接下来就是构造各种遍历函数，根据数据量来看，边建少了
	/*cout << getVertByindex(vertlist, 328, indexlist)->next->index;*/
	creat_allHE(vertlist, facelist,dislist,0.8,indexlist);

	

	

	

	

	system("pause");
	return EXIT_SUCCESS;
}

