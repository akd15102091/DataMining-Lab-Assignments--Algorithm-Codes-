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

string notAttribute;
map<string,bool> includeCol ;// which colums are used till any time

struct Dtree{
    string data ;
    bool isClass = false ;
    int cnt = 0 ; // count of childs pointers
    string condition[50] ;
    struct Dtree *child[50] ;
} ;
typedef struct Dtree *dtptr ;

dtptr head = NULL ;

void checkLeafs(dtptr root,bool &flag)
{
    if(root)
    {
        if((root->cnt ==0) && (root->isClass == false)) flag = false ;
        for(int i=0;i<root->cnt ;i++)
        {
            checkLeafs(root->child[i],flag) ;
        }
    }
}

string colSelectionByEntrophyandGain(map<string,vector<string> > records,string className,double infoGain,double s)
{
    string ans="" ;
    double maxGain = -10000000 ;
    for(auto it=records.begin();it!=records.end();it++)
    {
        if((it->first!=className) && (it->first !=notAttribute) && (includeCol[it->first] ==false))
        {
            map<string,map<string,double> > temp ;
            for(int i=0;i<it->second.size();i++)
            {
                (temp[it->second[i]])[records[className][i]] ++ ;
            }
            double IG = 0 ;
            double Entrophy = 0 ;
            double Gain = 0 ;
            for(auto itr1=temp.begin();itr1!=temp.end();itr1++)
            {
                double tempIG = 0 ;
                double tempEntro = 0 ;
                map<string,double> mp = itr1->second ;
                double s1 = 0 ;
                for(auto itr2=mp.begin();itr2!=mp.end();itr2++)
                {
                    s1 += itr2->second ;
                }
                for(auto itr2=mp.begin();itr2!=mp.end();itr2++)
                {
                    tempIG += (((-1)*(itr2->second))/s1)*(log2((itr2->second)/s1)) ;
                }
                Entrophy += (s1/s)*(tempIG) ;
            }
            Gain = infoGain- Entrophy ;
            if(maxGain <Gain) 
            {
                maxGain = Gain ;
                ans = it->first ;
            }
        }
    }
    includeCol[ans] = true ;
    return ans ;
}
dtptr createNewNode(string str)
{
    dtptr temp = new struct Dtree() ;
    temp->cnt=0;
    temp->data = str ;
    temp->isClass = false ;
    for(int i=0;i<50;i++)
    {
        temp->child[i] = NULL ;
    }
    return temp ;
}

string checkAllClass(map<string,vector<string> > tempRecord,string className,string selectedColumn)
{
    int size = tempRecord[selectedColumn].size() ;
    map<string,double> tempMP ;
    for(int i=0;i<tempRecord[className].size();i++)
    {
        tempMP[tempRecord[className][i]]++ ;
    }
    if(tempMP.size()==1)
    {
        return  tempRecord[className][0] ;
    }
    return "notApertucularClass" ;

}
dtptr createClassNode(string data)
{
    dtptr temp = new struct Dtree() ;
    temp->cnt=0;
    temp->data = data ;
    temp->isClass = true ;
    for(int i=0;i<50;i++)
    {
        temp->child[i] = NULL ;
    }
    return temp ;
}

void createDecisionTree(dtptr &root,map<string,vector<string> > records,int NOC,string className)
{
    bool flag = true ;
    checkLeafs(head,flag) ;
    if((head!=NULL) && (flag))
    {
        return ;
    }
    //-----------------------------------------------------------------------------------------------
    map<string,double> recordPNval ;
    for(int i=0;i<records[className].size();i++)
    {
        recordPNval[records[className][i]]++ ;
    }
    double infoGain = 0;
    double s = 0 ;
    for(auto it=recordPNval.begin();it!=recordPNval.end();it++)
    {
        s += it->second ;
    }
    for(auto it=recordPNval.begin();it!=recordPNval.end();it++)
    {
        infoGain += (((-1)*(it->second))/s)*(log2((it->second)/s)) ;
    }
    //cout<<infoGain;
    //---------------------------------------------------------------------------------------------

    string selectedColumn = colSelectionByEntrophyandGain(records,className,infoGain,s) ;
    //cout<<selectedColumn;

    map<string,bool> tempMap1 ; // what are different values in selected column
    for(int i=0;i<records[selectedColumn].size();i++)
    {
        tempMap1[records[selectedColumn][i]] = true ;
    }
    root=createNewNode(selectedColumn) ;
    int count=0;
    for(auto it=tempMap1.begin();it!=tempMap1.end();it++)
    {
        string cond = it->first ;
        map<string,vector<string> > tempRecord ;
        
        for(int i=0;i<records[selectedColumn].size();i++)
        {
            if(records[selectedColumn][i] == it->first)
            {
                for(auto it2=records.begin();it2!=records.end();it2++)
                {
                    tempRecord[it2->first].push_back(it2->second[i]) ;
                }
            }
        }
        root->condition[count] = it->first ;

        string cac = checkAllClass(tempRecord,className,selectedColumn) ;    
        if(cac!="notApertucularClass")  
        {
            root->child[count] = createClassNode(cac) ;
            root->cnt ++ ;
        }  
        else
        {
            createDecisionTree(root->child[count],tempRecord,NOC,className) ;
            root->cnt ++ ;
        }
        count++;
    }

}

void preOrder(dtptr root)
{
    if(root)
    {
        cout<<root->data<<" ";
        for(int i=0;i<root->cnt ;i++)
        {
            preOrder(root->child[i]) ;
        }
    }
}

void printPaths(dtptr root,string str[],int i)
{
    if(root==NULL) return ;
    if(root->child[0] ==NULL)
    {
        str[i] = root->data ;
        i++ ;
        for(int j=0;j<i;j++)
        {
            cout<<str[j]<<" ";
        }
        cout<<endl;
        return ;
    }
    str[i] = root->data ;
    i++ ;
    str[i] = "-->";
    i++ ;
    
    for(int k=0;k<root->cnt;k++)
    {
        str[i] = root->condition[k] ;
        i++ ;
        printPaths(root->child[k],str,i) ;
         i--;
    }
    
}

void predictResult(dtptr root,map<string,string> forPredValues)
{
    while(root->isClass !=true)
    {
        for(int i=0;i<root->cnt;i++)
        {
            if(root->condition[i] == forPredValues[root->data])
            {
                root = root->child[i] ;
                break ;
            }
        }
    }
    cout<<"predicted class is : "<<root->data<<endl;
}

int main()
{
    int NOC=0; // Number of columns
    string className ;// decision column
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
    ifstream in("inputDT1.txt");
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
    for(auto it = records.begin();it!=records.end();it++)
    {
        cout<<it->first<<" --  ";
        includeCol[it->first] = false ;
        for(int i=0;i<it->second.size();i++)
        {
            cout<<it->second[i]<<"   ";
        }
        cout<<endl ;
    }
    cout<<"\n\n" ;
    cout<<"Class name : "<<className<<"\n\n" ;

    dtptr root = NULL ;
    head = root ;
    createDecisionTree(root,records,NOC,className) ;
    

    preOrder(root) ;
    cout<<endl ;
    cout<<"print paths : "<<endl ;
    string arr[50] ;
    int i=0 ;
    printPaths(root,arr,i) ;

    cout<<endl<<endl;
    cout<<"lets predict some values  : "<<endl;
    cout<<"Enter corresponding values (if don't want then enter '@' ) : "<<endl;
    map<string,string> forPredValues ;
    string age,income,student,credit_rating ;
    cout<<"Enter value for age : "<<endl;
    cin>>age ;
    forPredValues["age"] = age ;

    cout<<"Enter value for income : "<<endl;
    cin>>income ;
    forPredValues["income"] = income ;

    cout<<"Enter value for student : "<<endl;
    cin>>student ;
    forPredValues["student"] = student ;

    cout<<"Enter value for credit_rating : "<<endl;
    cin>>credit_rating ;
    forPredValues["credit_rating"] = credit_rating ;

    predictResult(root,forPredValues) ;
    
    return 0;
}