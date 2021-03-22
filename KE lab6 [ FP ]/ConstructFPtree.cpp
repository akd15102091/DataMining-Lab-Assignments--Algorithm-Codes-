#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<fstream>
#include<iterator>
#include<algorithm>
#include <sstream> 
using namespace std ;

ofstream out("outputFP.txt") ; // output file


map<string,int> countMap ; // store frequencies of items
bool compare(string a,string b)
{
    return ((countMap[a])>(countMap[b])) ;
}

struct fpNode
{
    string itemId ; // Item id
    int cnt ;// count of child pointers
    struct fpNode* child[50] ; // child pointers 
    struct fpNode* next ; // for connecting the same itemId nodes in fp tree
    int freq ; // frequency
} ;
typedef struct fpNode *fptr ;

bool check(fptr t,string str)
{
    for(int i=0;(i<50 && (t->child[i] !=NULL) );i++)
    {
        string s = t->child[i]->itemId ;
        if(s== str) return true ;
    }
    return false ;
}

fptr newNode(string str)
{
    fptr temp = new struct fpNode ;
    temp->itemId = str ;
    temp->cnt = 0 ;
    temp->freq = 1 ;
    for(int i=0;i<50;i++)
    {
        temp->child[i] = NULL ;
    }
    temp->next = NULL ;
    return temp ;
}

void create(fptr &root, vector<string> vec, int i,int n)
{
    if(i>=n) return ;
    if(check(root,vec[i]))
    {
        int x ;
        for( x=0;(x<50 && (root->child[x] !=NULL) );x++)
        {
            string s = root->child[x]->itemId ;
            if(s== vec[i])
            {    
                break ;   
            }
        }
        root->child[x]->freq ++ ;
        create(root->child[x],vec,i+1,n) ;
    }
    else
    {
        int x=0 ;
        while(x<50)
        {
            if(root->child[x] ==NULL) break ;
            x++ ;
        }
        root->child[x] = newNode(vec[i]) ;
        root->cnt++ ;
        create(root->child[x],vec,i+1,n) ;
    }
    
}

void preOrder(fptr t)
{
    if(t)
    {
        cout<<t->itemId<<" "<<t->freq<<endl;
        out<<t->itemId<<" "<<t->freq<<endl;
        for(int i=0;i<t->cnt;i++)
        {
            preOrder(t->child[i]) ;
        }
    }
}
void checkLinks(fptr t)
{
    if(t)
    {
        //cout<<t->itemId<<" "<<t->freq<<endl;
        fptr temp = t ;
        while(temp)
        {
            cout<<temp->itemId<<" "<<temp->freq<<endl;
            temp = temp->next ;
        }
        cout<<"---------------------"<<endl ;
        for(int i=0;i<t->cnt;i++)
        {
            checkLinks(t->child[i]) ;
        }
    }
}

void connectSameId(fptr root,map<string,fptr> &fpMap,map<string,fptr> &lm)
{
    if(root==NULL) return ;
    if((root->itemId !="#") && (fpMap[root->itemId] ==NULL))
    {
        fpMap[root->itemId] = root ;
    }
    if(lm[root->itemId] ==NULL)
    {
        lm[root->itemId] = root ;
    }
    else
    {
        fptr temp = lm[root->itemId] ;
        temp->next = root ;
        temp = temp->next ;
        lm[root->itemId] = temp ;
    }

    for(int i=0;i<root->cnt;i++)
    {
        connectSameId(root->child[i],fpMap,lm) ;
    }

}

int main()
{
    ifstream in("inputFP.txt");
    string str ;    //  just string variable1 for reading data in file
    string cur_trans;// just string variable for store current transaction number  
    map<string,int> mp;
    map<string,vector<string> > records;  // it contains data of original input file
    int cnt=0;
    int NOT=0; // Number of transactions
    while(in >> str){
        if(str[0] != 'T'){
            records[cur_trans].push_back(str);
        }
        else{
            cur_trans = str ;
            NOT++ ;
        }
    }
    in.close();

    /*
        now we have copied the records to map named records so that no need to access the file again
    */
    cout << "Transactions Scanned : \n" ;
    out << "Transactions Scanned : \n" ;

    for(auto it=records.begin();it!=records.end();it++){
        cout << it->first << " -> ";
        out << it->first << " -> ";

        for(int i=0;i<it->second.size();i++){
            cout << it->second[i] << " ";
            out << it->second[i] << " ";
        }
        cout << "\n";
        out << "\n";
    }
    cout<<"\n\n";
    out<<"\n\n";
    

    for(auto it=records.begin();it!=records.end();it++){
        for(int i=0;i<it->second.size();i++){
            countMap[it->second[i]] ++ ;
        }
    }

    map<string,fptr> fpMap ;// it stores itemId and its next pointer
    map<string,fptr> lm; // it will link the sameItem Pointers
    for(auto it=countMap.begin();it!=countMap.end();it++)
    {
        fpMap[it->first] = NULL ;
        lm[it->first] = NULL ;
    }
    

    fptr root = NULL ;  // make root
    root = new struct fpNode() ;
    root->itemId = "#" ;
    for(int i=0;i<50;i++)
    {
        root->child[i] = NULL ;
    }
    root->cnt = 0 ;
    root->freq = 0 ;
    root->next = NULL ;

    // Now we will make the FPtree
    for(auto it=records.begin(); it!=records.end();it++)
    {
        vector<string> vec = it->second ;
        sort(vec.begin(),vec.end(),compare) ; // sort transaction acc. to their frequency
        create(root,vec,0,vec.size()) ;
    }
    cout<<"PreOrder of FPtree : "<<endl;
    out<<"PreOrder of FPtree : "<<endl;

    preOrder(root) ;
    connectSameId(root,fpMap,lm) ;
    //checkLinks(root) ;

    for(auto it=fpMap.begin();it!=fpMap.end();it++)
    {
        cout<<it->first <<" ---> ";
        fptr temp = it->second ;
        while(temp)
        {
            cout<<temp->itemId<<" ";
            temp = temp->next ;
        }
        cout<<endl;
    }

    return 0 ;
   
}