class City {
private:
	string name;
	int x;
	int y;
public:
	// Constructor
	City( string inname, int inx;int iny){
		name = inname;
		x=inx;
		y=iny;
	} 
	~City() {}// Destructor
	// Local data member access functions
	string getname(){ return name; }
	void printCity(){
		cout<<"�������ƣ�"<< name << "��" << "(" <<x<<","<<y<<")"<<endl; 
	}
};


