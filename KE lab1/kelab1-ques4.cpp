#include<iostream>
#include<algorithm>
#include<string>
#include<fstream>
#include<map>
#include<vector>
#include<iterator>

using namespace std ;
int main()
{
	ifstream in("kelab1INPUT.txt") ;
	ofstream out("kelab1OUTPUT4.txt") ;
	string s ;
    map<char,vector<char> > mp ;

	while(getline(in,s))
	{
		char a ;
		for(int i=0;i<s.length();i++)
		{
			if((!isalpha(s[i]) ) && !isspace(s[i])) 
			{
				a = s[i] ;
			}
			else if(isalpha(s[i]))
			{
				mp[s[i]].push_back(a) ;
			}
		}
		
	}
	map<char,vector<char> > :: iterator it=mp.begin() ;
	while(it!=mp.end())
	{
		out<<it->first<<" " ;
		for(int i=0;i<(it->second).size();i++)
		{
			out<<(it->second)[i] <<" ";
		}
		out<<"\n" ;
		it++ ;
	}
	in.close() ;
	out.close() ;
	return 0;
}


