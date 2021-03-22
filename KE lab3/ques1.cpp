#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<fstream>
#include<iterator>
#include<algorithm>
#include <sstream> 
using namespace std ;

int main()
{
    ifstream in("missing_input.txt") ;
    string str ;
    map<int,vector<string> > mp ;
    int i=0 ;
    int c=1 ;
    while(in >> str)
    {
        mp[i/4].push_back(str) ;
        i++ ;
    }    
    ofstream out("missing_output.txt") ;
    out<<"Actual Database  :   {Name  , Roll No , Age , Marks } " ;
    out<<"\n\n" ;
    map<int,vector<string> > :: iterator itr ;
    for(itr=mp.begin();itr!=mp.end();itr++ )
    {
        for(int j=0;j<itr->second.size();j++)
        {
            out<<itr->second[j] <<"  ";
        }
        out<<"\n" ;
    }

    out<<"----------------------------------------------------- \n" ;
    out<<"\n\n" ;
    out<<"1) Ignoring the touples : " ;
    out<<"\n\n" ;
    for(itr=mp.begin();itr!=mp.end();itr++ )
    {
        bool b = true ;
        for(int j=0;j<itr->second.size();j++)
        {
            if(itr->second[j] == "-1")
            {
                b = false ;
                break ;
            }
        }
        if(b==true)
        {
            for(int j=0;j<itr->second.size();j++)
            {
                out<<itr->second[j] <<"  ";
            }
            out<<"\n" ;
        }
        
    }
    out<<"\n" ;
    out<<"----------------------------------------------------- \n" ;
    out<<"\n\n" ;
    out<<"2) Assigning globel  constant : " ;
    out<<"\n\n" ;

    for(itr=mp.begin();itr!=mp.end();itr++ )
    {
        for(int j=0;j<itr->second.size();j++)
        {
            if(itr->second[j] == "-1") out<<"NULL  ";
            else out<<itr->second[j] <<"  ";
        }
        out<<"\n" ;
    }
    out<<"\n" ;
    out<<"----------------------------------------------------- \n" ;
    out<<"\n\n" ;
    out<<"3) Assigning attribute mean : " ;
    out<<"\n\n" ;

    map<int,int> value ;
    map<int,int> index ;
    for(itr=mp.begin();itr!=mp.end();itr++ )
    {
        for(int j=0;j<itr->second.size();j++)
        {
            if(itr->second[j] !="-1")
            {
               string s = itr->second[j] ;
               stringstream geek(s);
               int x = 0; 
               geek >> x;
               value[j] += x ;
               index[j]++ ;
                //value[j] += stoi(s) ; 
            }
        }
    }

    for(itr=mp.begin();itr!=mp.end();itr++ )
    {
        for(int j=0;j<itr->second.size();j++)
        {
            if(itr->second[j] =="-1")
            {
                int y = (value[j])/(index[j]) ;
                out<<to_string(y)<<"  " ;
            }
            else
            {
                out<<itr->second[j] <<"  ";
            }
            
        }
        out<<"\n" ;
    }


}