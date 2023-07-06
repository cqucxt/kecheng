#include<iostream>
#include<string.h>
#include<cmath>
using namespace std;

class city{		//存储城市的xy坐标 
private:
	int x;
	int y;
	//string name;
public:

	city() {}
	city( int b, int c){
	//	name=a;
		x=b;
		y=c;
	} 
		
	city(const city&o){
	//	name=o.name;
		x=o.x;
		y=o.y;
	}
		
	//string Name(){
	//	return name;
	//}	
	
	int X(){
		return x;
	}
		
	int Y(){
		return y;
	}
};


class BSTNode{		//存储一个城市的所有信息并封装一些函数操作 
	private:
		city it;	//xy坐标 
		string name;	//名称 
		BSTNode*lc;
		BSTNode*rc;
	
	public:
		BSTNode() {lc=rc=NULL;}
		BSTNode(string a, city b, BSTNode*l=NULL, BSTNode*r=NULL){
			name=a;
			it=b;
			lc=l;
			rc=r;
		}
		~BSTNode()	{}
		
		city& Element()	{return it;}
		void setElement(const city &a)	{it=a;}
		string Name()	{return name;}
		void setName(string a)	{name=a;}
		inline BSTNode*left()	{return lc;}
		void setLeft(BSTNode*left)	{lc=left;} 
		inline BSTNode*right()	{return rc;}
		void setRight(BSTNode*right)	{rc=right;}
		bool isLeaf()	{return (lc==NULL)&&(rc==NULL);}
		
		 
};


class BST{
	private:	//将...help类型的函数放进私有类进行保护，不对用户开放 
		BSTNode*root;
		int nodecount;
		city findhelp(BSTNode *rt, string k){
		//	cout<<rt->Name()<<endl;
			if(rt==NULL)	{ city temp(10000,-999);	return temp;} 	
			if(k<rt->Name())	findhelp(rt->left(),k);
			else if(k>rt->Name())	findhelp(rt->right(),k);
			else return rt->Element();
		}
		
		BSTNode* inserthelp(BSTNode*root, const string k, const city it){
			if(root==NULL)	return new BSTNode(k,it,NULL,NULL);
			if(k<root->Name())	{root->setLeft(inserthelp(root->left(),k,it));	} 
			else {root->setRight(inserthelp(root->right(),k,it));} 
			return root;
		}
		
		BSTNode* deletemin(BSTNode*rt){
			if(rt->left()==NULL)	return rt->right();
			else{
				rt->setLeft(deletemin(rt->left()));
				return rt;
			}
		}
		
		BSTNode* getmin(BSTNode *rt){
			if(rt->left()==NULL)
				return rt;
			else return getmin(rt->left());
		}
		
		BSTNode* removehelp(BSTNode*rt, const string k){
			if(rt==NULL)	return NULL;
			else if (k<rt->Name())
				rt->setLeft(removehelp(rt->left(),k));
			else if (k>rt->Name())
				rt->setRight(removehelp(rt->right(),k));
			else{
				BSTNode* temp=rt;
				if(rt->left()==NULL){
					rt=rt->right();
					delete temp;
				}
				else if(rt->right()==NULL){
					rt=rt->left();
					delete temp;
				}
				else{
					BSTNode* temp=getmin(rt->right());
					rt->setName(temp->Name());
					rt->setRight(deletemin(rt->right()));
					delete temp;
				} 
			}
			return rt;
		}
		
		void clearhelp(BSTNode*root){
			if(root==NULL)	return;
			clearhelp(root->left());
			clearhelp(root->right());
			delete root;
		}
		
		void printhelp(BSTNode* root, int level){	//直接打印输出有关信息 
			if(root==NULL)	return;
			printhelp(root->left(), level+1);
			cout<<root->Name()<<endl;
			printhelp(root->right(),level+1);
		}
		
		void print2(BSTNode* root, char c){		//打印输出首字母检索后的城市信息 
			if(root==NULL)	return ;
			print2(root->left(),c);
			if(root->Name().at(0)==c)	cout<<root->Name()<<' '<<root->Element().X()<<' '<<root->Element().Y()<<endl;
			print2(root->right(),c);
		}
		void _print(BSTNode*root,city &a,int l){	//打印输出给定范围内的城市坐标 
			if(root==NULL)	return ;
			double len=sqrt(pow(root->Element().X()-a.X(),2)+pow(root->Element().Y()-a.Y(),2));
			_print(root->left(),a,l);
			if(len<l)	cout<<root->Name()<<' '<<root->Element().X()<<' '<<root->Element().Y()<<endl;
			_print(root->right(),a,l);	
			
		}
	public:
		BST()	{ root=NULL; nodecount=0;}
		~BST()	{clearhelp(root);}
		void clear()	{clearhelp(root);root=NULL;nodecount=0;}
		void insert(const string k, const city&e){root=inserthelp(root,k,e);nodecount++;}
		city remove(const string k){
			city temp=findhelp(root,k);
			if(temp.X()!=1000&&temp.Y()!=-999){
				root=removehelp(root,k);
				nodecount--;
			}
			return temp;
		}
		
		city find(const string k){
			city temp=findhelp(root,k);
			if(temp.X()!=3.1415&&temp.Y()!=-3.1415)	return temp;
		}
		
		int size()	{ return nodecount++;}
		void print(){
			printhelp(root,0);
		} 
		void Print(char c){
			print2(root,c);
		} 
		void Pprint(city&a, int len){
			_print(root,a,len);
		}
};
int main(){
	BST test;

	int n;
	cin>>n;
	string s;
	int a,b;
	for(int i=0;i<n;i++){
		cin>>s;
		cin>>a;
		cin>>b;
		city temp(a,b);
		test.insert(s,temp);
	}


	while(1){
	
		int judge;
		int a,b;
		string s,d;
		cin>>judge;
		
		if(judge==2)	break;
		else if(judge==0){
			city shanchu;
			cin>>s;
			test.remove(s);
		}
		else if(judge==1){
			cin>>d;
			cin>>a>>b;
			city temp(a,b);
			test.insert(d,temp);
		}	
		
	}
	
	int len;
	char c;
	cin>>c;

	cin>>a>>b>>len;
	city last(a,b);
	test.print();
	test.Print(c);
	test.Pprint(last,len);
}

