#include<iostream>
#include<cstring>
#include<cstdlib>

using namespace std;

struct Node {
   float price;
   int num;
   string key;
   Node *child[100];
};

Node *newNode(string key);
Node *createRoot();
Node* createChild(Node *root,int index);
Node* AskFunction(Node *root,int index);
int justDigit(string str);
int isalphabet(char letter);


int justDigit(string detail){

	for(int i=0;detail[i]!='\0';i++){
		if( (detail[i]>='a' && detail[i]<='z') || (detail[i]>='A' && detail[i]<='Z') ) {
			return 0;
		}
	}
	return 1;
}
int isalphabet(char letter){
    if( (letter>='A' && letter<= 'Z') || (letter>='a' && letter<='z') )
        return 1;
    else
        return 0;
}

Node *newNode(string key) {
    Node *temp = new Node;
	temp->num=1;
	temp->price=0;
	temp->key = key;

    for (int i = 0; i <100; i++)
        temp->child[i] = NULL;
    return temp;
}

//Agac gosterimini olusturur..
Node* createChild(Node* root,int index){
	int i=0,k=0,j=0;
	char *object;
	string detail;
	
	object = (char *) malloc(100);

	detail=root->key;

    for(i=0;detail[i]!='\0';i++){
    	if(detail[i]!='+'){
			object[k]=detail[i];
			k++;
		}
        //+'ya kadar olan ilk nesneyi kaydet..
		else if (detail[i]=='+'){
			object[k]='\0';
        	root->child[j] = newNode(object);

        	index=0;
        	j++;
        	k=0;
		}
	}
    //+ ve null arasindaki nesneyi kaydet..
	if(detail[i]=='\0'){
		object[k]='\0';
        root->child[j] = newNode(object);

	}

	int childNum=j;
	
	while(index<=childNum){
		AskFunction(root->child[index],index);
		index++;
	}
	free(object);
	return root;
}
//Nesne hakkinda bilgi toplar..
Node* AskFunction(Node* root,int index){
	string str;
	char *object, *temp;
	object = (char *) malloc(100);
	temp = (char *) malloc(100);

	int j=0,k=0;

	str=root->key;
 	//Sayinin kat sayisini belirler..
	for(int i=0;str[i]!='\0';i++){
		temp[k]=str[i];
		k++;
 	}
	if(isalphabet(str[0])==0)
		root->num=atof(temp);

	//Soruyu sorarken kat sayilari yazdirmaz..
	for(int i=0;str[i]!='\0';i++){
		if(isalphabet(str[i])){
			object[j]=str[i];
			j++;
		}
		else{
			 i++;
		}
	}
	object[j]='\0';

	cout<<"What is "<<object<<"?:\n";
	getline(cin,str);

	//Girilen bilgi(str) para ise para bilgisini girer..
	if(justDigit(str)){
	    int k=0;
		for(int i=0;str[i]!='\0';i++){
			temp[k]=str[i];
			k++;
		}
		root->price=atof(temp);
		index++;
		root=createChild(root,index);
	}
	//Girilen bilgi(str) para degil ise yeni root olarak gonderir..
	else{
		root->child[0] = newNode(str);
		root->child[0]->num=1;
    	root=createChild(root->child[0],index);
	}
	free(object);
	free(temp);
    return root;
}

//Ilk koku belirler ve bilgisini almak icin fonksiyonu cagirir..
Node *createRoot()
{
	string object;
	cout<<"Define object:\n";
	getline(cin,object);

    Node *root = newNode(object);
    root->child[0]=AskFunction(root,0);
    
    return root;
}
//Tum kokleri, kat sayilari ve parasi ile inceleyerek toplam parayi bulur..
float Calculation(Node* root){

	Node *temp =new Node;
	Node *iter=new Node;

	int i=0,index,k=0;
	float price[100],prices=0;
    float total=0;
	temp=root;
	iter=root;
	
    index=0;
	while(root->child[index]!=NULL){

		if(iter->child[i]==NULL){
			temp=root;
			iter=root;
			index++;
			i=index;
            prices=0;
  		}
		if(root->child[index]==NULL){

			for(i=0;i<index;i++){
				total+=price[i];
			}
			return total;
		}
		if(iter->child[i]->child[0]->child[0]!=NULL){
			iter=iter->child[i];
			//ilk kat sayilari da tutar..
			iter->num*=temp->num;
			
			temp=temp->child[i];
   			i=0;
		}
		else{
			prices+= (iter->child[i]->num * iter->num) * iter->child[i]->price;
            price[index]=prices;
			if(iter==root){
				index++;
                prices=0;
			}
			i++;
		}
    }

	for(i=0;i<index;i++){
		total+=price[i];
	}
	return total;
}
int main()
{
	Node *root = createRoot();
	
	float totalPrice;
 	totalPrice=Calculation(root->child[0]);
 	cout<<"Total cost of "<<root->key<<" is "<<totalPrice<<endl;

	return 0;
}

