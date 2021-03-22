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
    ifstream in("binning_input.txt") ;
    string str ;
    int bin_size = 4 ; // size of bins or bucket

    vector<int> binning ;
    while(in>>str)
    {
        stringstream geek(str);
        int x = 0; 
        geek >> x;
        binning.push_back(x) ;
    }
    int n = binning.size() ;
    sort(binning.begin(),binning.end()) ;

    vector<vector<int> >edpArr ; // equal depth partitioning array
    int i=0 ;
    while(i<n)
    {
        vector<int> vec ;
        int j=0 ;
        while(j<bin_size)
        {
            vec.push_back(binning[i+j]) ;
            j++ ;
        }
        edpArr.push_back(vec) ;
        i = i+j ;
    }

    ofstream out("binning_output.txt") ;
    out<<"1) After sorting and partitioning data with respect to equal depth partitioning ,\n equal depth bins are  : ";
    out<<"\n\n" ;
    for(int i=0;i<edpArr.size();i++)
    {
        for(int j=0;j<edpArr[i].size();j++)
        {
            out<<edpArr[i][j]<<"  ";  
        }
        out<<"\n" ;
    }
    out<<"\n" ;
    out<<"----------------------------------------------------- \n" ;
    out<<"\n\n" ;
   
    out<<"2) Smoothing data by bin means : \n";
    out<<"\n" ;
    map<int,int> mp ;
    for(int i=0;i<edpArr.size();i++)
    {
        for(int j=0;j<edpArr[i].size();j++)
        {
            mp[i] += edpArr[i][j] ;
        }
    }
     for(int i=0;i<edpArr.size();i++)
    {
        for(int j=0;j<edpArr[i].size();j++)
        {
            out<<(mp[i] /bin_size)<<"  ";  
        }
        out<<"\n" ;
    }
    out<<"\n" ;
    out<<"----------------------------------------------------- \n" ;
    out<<"\n\n" ;

    out<<"Smoothing data by bin boundaries : \n";
    out<<"\n" ;
    for(int i=0;i<edpArr.size();i++)
    {
        for(int j=1;j<edpArr[i].size() -1;j++)
        {
            int a = edpArr[i][j-1] ;
            int b = edpArr[i][j] ;
            int c = edpArr[i][j+1] ;

            if(b-a <= c-b) b = a ;
            else b = c ;

            edpArr[i][j] = b ;
        }
    }
    for(int i=0;i<edpArr.size();i++)
    {
        for(int j=0;j<edpArr[i].size();j++)
        {
            out<<edpArr[i][j]<<"  ";  
        }
        out<<"\n" ;
    }
    out<<"\n" ;
    out<<"----------------------------------------------------- \n" ;

}