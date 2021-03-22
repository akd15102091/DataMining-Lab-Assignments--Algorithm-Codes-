#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<fstream>
#include<iterator>
#include<algorithm>
#include <sstream>
#include<cmath> 
using namespace std ;

ofstream out("outputNBC.txt") ;
string notAttribute;
int NOC=0; // Number of columns
string className ;// decision column

/*  map<columnName, map<columnValue, map<classType,count> > > classTypeCount */
map<string,map<string,map<string,double> > > classTypeCount ;

/*  map<columnName, map<columnValue, map<classType,prob> > > classTypeProbability */
map<string,map<string,map<string,double> > > classTypeProbability ;

map<string,double> countTotal ;
map<string,double> ProbTotal ;

void findAllProbabilities(map<string,vector<string> > records)
{
    for(int i=0;i<records[className].size();i++)
    {
        countTotal[records[className][i]] ++ ;
    }

    for(auto it=records.begin();it!=records.end();it++)
    {
        if((it->first != className) && (it->first != notAttribute) )
        {
            for(int i=0;i<it->second.size();i++)
            {
                classTypeCount[it->first][it->second[i]][records[className][i]] ++ ;
            }
        }
    }

    for(auto it1=classTypeCount.begin();it1!=classTypeCount.end();it1++)
    {
        for(auto it2=it1->second.begin();it2!=it1->second.end();it2++)
        { 
            for(auto it3=it2->second.begin();it3!=it2->second.end();it3++)
            {
                classTypeProbability[it1->first][it2->first][it3->first] = (it3->second)/(countTotal[it3->first]) ;
            }
        }
    }

    double sum=0 ;
    for(auto it=countTotal.begin();it!=countTotal.end();it++)
    {
        sum = sum + it->second ;
    }
    for(auto it=countTotal.begin();it!=countTotal.end();it++)
    {
        ProbTotal[it->first] = (it->second)/sum ;
    }

    return ;
}

void predictResult(map<string,string> forPredValues)
{
    map<string,double> pclass ;
    for(auto it=countTotal.begin();it!=countTotal.end();it++)
    {
        double x=1 ;
        for(auto it1=forPredValues.begin();it1!=forPredValues.end();it1++)
        {
            x *= classTypeProbability[it1->first][it1->second][it->first] ; 
        }
        pclass[it->first] = x ;
    }

    double sum=0 ;
    for(auto it=pclass.begin();it!=pclass.end();it++)
    {
        sum += it->second ;
    }
    for(auto it=pclass.begin();it!=pclass.end();it++)
    {
        pclass[it->first] = (it->second)/sum ;
    }
    int maxProb=0 ;
    string predctedResult ;
    for(auto it=pclass.begin();it!=pclass.end();it++)
    {
        if(it->second > maxProb)
        {
            maxProb = it->second ;
            predctedResult = it->first ;
        }
    }
    cout<<"Predicted result is : "<<predctedResult<<endl;
    
    out<<"Predicted result is : "<<predctedResult<<endl;
}

int main()
{
    cout<<"Enter number of column in file : "<<endl ;
    cin>>NOC ;
    cout<<"Enter  name of class name : "<<endl;
    cin>>className ;
    cout<<"Enter  name of notAttribute column : "<<endl;
    cin>>notAttribute ;

    string str ;    //  just string variable1 for reading data in file
    string cur_trans;// just string variable for store current transaction number  
    map<string,int> mp;
    map<string,vector<string> > records;  // it contains data of original input file
    int cnt=0;  
    ifstream in("inputNBC.txt");
    int abc=0 ;
    vector<string> vec ;
    while(in >> str){
        if((abc / NOC) ==0)
        {
            vec.push_back(str) ;
        }
        else
        {
            records[vec[abc%NOC]].push_back(str) ;
        }
        abc++ ;
    }

    in.close();
    cout<<" Input Data : "<<endl ;
    out<<" Input Data : "<<endl ;
    for(auto it = records.begin();it!=records.end();it++)
    {
        cout<<it->first<<" --  ";
        out<<it->first<<" --  ";
        for(int i=0;i<it->second.size();i++)
        {
            cout<<it->second[i]<<"   ";
            out<<it->second[i]<<"   ";
        }
        cout<<endl ;
        out<<endl ;
    }
    cout<<"\n\n" ;
    out<<"\n\n" ;
    //cout<<"Class name : "<<className<<"\n\n" ;

    findAllProbabilities(records) ;

    cout<<endl<<endl;
    cout<<"lets predict some values  : "<<endl;
    out<<"lets predict some values  : "<<endl;
    cout<<"Enter corresponding values (if don't want then enter '@' ) : "<<endl;
    map<string,string> forPredValues ;
    string outlook,temperature,humidity,wind ;
    cout<<"Enter value for outlook : "<<endl;
    cin>> outlook ;
    forPredValues["outlook"] = outlook ;

    cout<<"Enter value for temperature : "<<endl;
    cin>>temperature ;
    forPredValues["temperature"] = temperature ;

    cout<<"Enter value for humidity : "<<endl;
    cin>>humidity ;
    forPredValues["humidity"] = humidity ;

    cout<<"Enter value for wind : "<<endl;
    cin>>wind ;
    forPredValues["wind"] = wind ;

    out<<"Entered data for prediction : "<<"outlook : "<<outlook<<" , temperature : "<<temperature<<" , humidity: "<<humidity<<" , wind : "<<wind<<endl;
    predictResult(forPredValues) ;
    
    return 0;
}