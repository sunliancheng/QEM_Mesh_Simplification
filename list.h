#ifndef LIST_H

#define LIST_H


using namespace std;




template<class T> 
struct Node_int{
	T x;
	T y;
	T z;
	bool isEmpty = true;
    Node_int<T>*next=NULL;
	Node_int<T>*pre;
};

template<class T> 
void insert(T x,T y, T z,Node_int<T> *outEdges) {

	

		if (outEdges->isEmpty) {
			outEdges->isEmpty = false;
			outEdges->pre = NULL;
			outEdges->next = NULL;
			outEdges->x = x;
			outEdges->y = y;
			outEdges->z = z;
			
		}
		else {
			
			
				Node_int<T> *p1, *p;
				p = (struct Node_int<T>*)malloc(sizeof(struct Node_int<T>));
				p1 = (struct Node_int<T>*)malloc(sizeof(struct Node_int<T>));
				for (p = outEdges; p->next != NULL; p = p->next) {
				}
				p1->isEmpty = false;
				p1->next = NULL;
				p1->pre = p;
				p1->x = x;
				p1->y = y;
				p1->z = z;
				p->next = p1;
				p = p1;
			

	}

}
//三个点 是否有两个在其中,outedges = face集合
template<class T>
int isInList(T x,T y, Node_int<T> *outEdges) {
	Node_int<T>  *p;
	p = (struct Node_int<T>*)malloc(sizeof(struct Node_int<T>));
	int count = 0;
	int is = 0;//用来判断是否存在，存在返回正数
	int h = 0;
	for (p = outEdges; p != NULL; p = p->next) {
		count = 0;
		is++;

		if (p->x == x || p->y == x || p->z == x) {
			count += 1;
		}
		if (p->x == y || p->y == y || p->z == y) {
			count += 1;
		}
		if (count == 2) {

			
			if ((p->x == x || p->x == y) && (p->y == y || p->y == x)) return p->z;
			if ((p->z == x || p->z == y) && (p->y == y || p->y == x) )return p->x;
			if ((p->x == x || p->x == y) && (p->z == y || p->z == x)) return p->y;

		}

	}
	return -1;
}

//查找第二个
template<class T>
int isInList_second(T x, T y, Node_int<T> *outEdges) {
	Node_int<T>  *p;
	p = (struct Node_int<T>*)malloc(sizeof(struct Node_int<T>));
	int count = 0;
	int is = 0;//用来判断是否存在，存在返回正数
	int h = 0;
	for (p = outEdges; p != NULL; p = p->next) {
		
		count = 0;
		is++;

		if (p->x == x || p->y == x || p->z == x) {
			count += 1;
		}
		if (p->x == y || p->y == y || p->z == y) {
			count += 1;
		}
		if (count == 2) {
			h++;
			if (h == 2) {
				if ((p->x == x || p->x == y) && (p->y == y || p->y == x)) return p->z;
				if ((p->z == x || p->z == y) && (p->y == y || p->y == x))return p->x;
				if ((p->x == x || p->x == y) && (p->z == y || p->z == x)) return p->y;
			}
		}

	}
	return -1;
}

template<class T>
void printLinklist(Node_int<T> *outEdges) {

	Node_int<T> *p;
	p = (struct Node_int<T>*)malloc(sizeof(struct Node_int<T>));

	for (p = outEdges; p != NULL; p = p->next) {
		cout << "first:" << p->x << " second:" << p->y << " third:" << p->z << endl;
	}

}

template<class T>
int getsize(Node_int<T> *outEdges) {
		int i = 0;
		Node_int<T> *p;
		p = (struct Node_int<T>*)malloc(sizeof(struct Node_int<T>));
		for (p = outEdges; p != NULL; p = p->next) {
			i++;
		}
		return i;

}


#endif