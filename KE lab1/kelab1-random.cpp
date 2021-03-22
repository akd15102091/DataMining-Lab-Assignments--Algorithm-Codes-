#include<iostream>
#include<algorithm>
#include<string>
#include<ctime>
#include<fstream>
#include<map>
#include<vector>
#include<iterator>

using namespace std ;
int main()
{
	ofstream temp("KELab1RandomInput.txt") ;
	
	int n ; // number of transactions

	srand(time(0)) ;
	n = rand() % 50 ;
	n = n+10 ; // because n should not be zero
	for(int i=1;i<=n;i++)
	{
		temp<<i<<" " ;
		int x = rand() % 20 ;
		for(int j=0;j<x;j++)
		{
			int a = rand() % 200 ;
			char ch = char('a'+(a%26)) ;
			temp<<ch<<" " ;	
		} 
		temp<<"\n" ;
	}
	
	// OCCURANCES OF EACH ITEM
	ifstream input("KELab1RandomInput.txt") ;
	ofstream out("KELab1RandomOutput.txt") ;
	string s ;
	map<string,int> mp ;
	while(input >> s)
	{
		if((!isdigit(s[0]) ) && s!="" )
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
	
	temp.close() ;
	input.close() ;
	out.close() ;
	
	return 0 ;
}
