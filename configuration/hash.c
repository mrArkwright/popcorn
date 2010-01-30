#include "hash.h"


Hash eightbitHash(char* string){
	Hash current;
	unsigned int i,tmp=i=0;

	while( (current = string[i++] ) != '\0'){
		current -= 32;
		current *= 2;
		tmp += (unsigned int) current;
	}
	tmp/=i;
	return (Hash) tmp;
}

ListElement *addToHashlist( char* name, void *dataPtr ,ListElement** hashList ){
	ListElement *tmp,*hashElement = malloc(sizeof(ListElement));
	hashElement->hash = eightbitHash(name);
	hashElement->name = name;
	hashElement->next = NULL;
	hashElement->dataPtr = dataPtr;
	if( hashList[hashElement->hash] == NULL )
		hashList[hashElement->hash] =hashElement;
	else{
		tmp = hashList[hashElement->hash];
		if(strcmp(tmp->name,name)==0) return NULL;
		while(tmp->next != NULL){
			if(strcmp(tmp->name,name)==0) return NULL;
			tmp = tmp->next;
		}
		tmp->next = hashElement;
		hashElement->prev = tmp;
	}
	return hashElement;
}

ListElement *getFromHashlist( char* name,ListElement** hashList){
	Hash hash = eightbitHash(name);
	ListElement *tmp = hashList[hash];
	while(tmp != NULL){
		if(strcmp(tmp->name,name)==0)
			return tmp;
		else
			tmp = tmp->next;
	}
	return tmp;
}
/*
int main(){

	int i=0;
	unit *a,*b,*c,*d;
	char *A,*B,*C,*D;
	ListElement *hashList[256];

	for(i;i<256;hashList[i++] = NULL); 
	
	A="eins";
	B="zwei";
	C="drei";
	D="vier";

	a = malloc(sizeof(a));
	b = malloc(sizeof(a));
	c = malloc(sizeof(a));
	d = malloc(sizeof(a));

	a->id=1;
	b->id=2;
	c->id=3;
	d->id=4;

	if (addToHashlist(A,a,hashList)->name == NULL ) return 1;
	if (addToHashlist(B,b,hashList)->name == NULL ) return 2;
	if (addToHashlist(C,c,hashList)->name == NULL ) return 3;
	if (addToHashlist(D,d,hashList)->name == NULL ) return 4;

	printf("eins: %d\n", getFromHashlist("eins",hashList)->dataPtr->id);
	printf("zwei: %d\n", getFromHashlist("zwei",hashList)->dataPtr->id);
	printf("drei: %d\n", getFromHashlist("drei",hashList)->dataPtr->id);
	printf("vier: %d\n", getFromHashlist("vier",hashList)->dataPtr->id);
	freeList(hashList);

	return 0;
}
*/
void freeList(ListElement ** hashList){
	int i;
	ListElement* tmp;
	for(i=0;i<256;i++){
		tmp = hashList[i];
		if(tmp ==NULL) continue;
		while(tmp->next != NULL){
			tmp = tmp->next;
			free(tmp->prev);
		}
		free(tmp);
	}
}

