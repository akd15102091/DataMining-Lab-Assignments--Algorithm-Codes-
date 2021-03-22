#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>
using namespace std ;
int main()
{
	ifstream in("kelab1INPUT.txt") ;
	ofstream out("kelab1OUTPUT2.txt") ;
	string s ;

	while(getline(in,s))
	{
        out<<s ;
        out<<"\n" ;
	}
	in.close() ;
	out.close() ;
	return 0;
}


