#include<iostream>
#include<fstream>
#include<map>
#include<iterator>
using namespace std ;
int main()
{
	ifstream in("kelab1INPUT.txt") ;
	ofstream out("kelabOUTPUT3.txt") ;
	string s ;
	map<string,int> mp ;
	while(in >> s)
	{
		if(!isdigit(s[0]))
		{
			mp[s]++ ;
		}
	}
	map<string,int> :: iterator itr = mp.begin() ;
	while(itr!=mp.end())
	{
		out<< itr->first <<" "<<itr->second<<" \n" ;
		itr++ ;
	}
	in.close() ;
	out.close() ;
	return 0;
}


