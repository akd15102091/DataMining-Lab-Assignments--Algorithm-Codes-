#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<fstream>
#include<iterator>
#include<algorithm>
#include <sstream> 
#include<utility>
using namespace std ;

//ofstream out("outputFP.txt") ; // output file
map<vector<string> ,int> FPGMap ;// Frequent pattern generated map {final frequents}
int minSupCnt ;// minium support count

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

void findPaths(fptr root,int &count,vector< pair<vector<string> ,int> > &CPBVec,vector<string> vec,string item)
{
    if(root==NULL) return ;
    if(root->itemId ==item)
    {
        count = root->freq ;
        if(vec.size()!=0) CPBVec.push_back(make_pair(vec,count)) ;
        return ;
    }
    if(root->itemId !="#")
    {
        vec.push_back(root->itemId) ;
    }
    for(int i=0;i<root->cnt;i++)
    {
        findPaths(root->child[i],count,CPBVec,vec,item) ;
    }
}

bool checkItem(int &minCount,string item,vector<string> data,map<string,int> arrMp)
{
    bool x = false ;
    minCount = 10000007 ;
    for(int i=0;i<data.size();i++)
    {
        if(data[i] == item) x = true ;
        minCount = min(minCount,arrMp[data[i]]) ;
    }
    return x ;
}

void allFreqSubsets(vector<string> arr,map<string,int> arrMp,string item,vector<string> data,int i)
{
    if(i >= arr.size())
	{
		int minCount=0;
        if(checkItem(minCount,item,data,arrMp) && (data.size() >=2))
        {
            sort(data.begin(),data.end()) ;
            FPGMap[data] += minCount ;
        }
		return;
	}  
 
    data.push_back(arr[i]) ;
    allFreqSubsets(arr, arrMp, item,data,i + 1); 
    data.pop_back();
    allFreqSubsets(arr, arrMp, item,data,i + 1); 

}
void frequentPatternGenerated(string item,vector<pair<string,int> > tempVec,int itemCnt)
{
    vector<string> arr ;
    map<string,int> arrMp ;
    for(int i=0;i<tempVec.size();i++)
    {
        arr.push_back(tempVec[i].first) ;
        arrMp[tempVec[i].first] = tempVec[i].second ;
    }
    arr.push_back(item) ;
    arrMp[item] = itemCnt ; 
    vector<string> data ;
    allFreqSubsets(arr,arrMp,item,data,0) ;
    return ;
}

void conditionalFP(fptr root,string item,vector<pair<vector<string> ,int>> CPBvec,map<string,fptr> fpMap)
{
    vector<vector<pair<string,int> > > CFTVec ;// conditionalFpTree 
    int i=0 ;
    int sz = CPBvec.size() ;
    while(i<sz)
    {
        map<string,int> mp ;
        for(int j=0;j<CPBvec[i].first.size() ;j++)
        {
            mp[CPBvec[i].first[j]] += CPBvec[i].second ;
        }
        i++ ;
        while( (i<sz) && (CPBvec[i].first[0] == CPBvec[i-1].first[0] ) )
        {
            for(int j=0;j<CPBvec[i].first.size() ;j++)
            {
                mp[CPBvec[i].first[j]] += CPBvec[i].second ;
            }
            i++ ;
        }
        vector<pair<string,int> > tempVec ;
        for(auto it=mp.begin();it!=mp.end();it++)
        {
            if((it->second) >= minSupCnt)
            {
                tempVec.push_back(make_pair(it->first,it->second)) ;
            }
        }
        // minFreq in tempVec ;
        int itemCnt=0 ;
        for(int m=0;m<tempVec.size();m++)
        {
            itemCnt = max(itemCnt,tempVec[m].second) ;
        }
        frequentPatternGenerated(item,tempVec,itemCnt) ;
        CFTVec.push_back(tempVec) ;
    }
    // for(int x=0;x<CFTVec.size();x++)
    // {
    //     for(int y=0;y<CFTVec[x].size();y++)
    //     {
    //         cout<<CFTVec[x][y].first<<" "<<CFTVec[x][y].second <<endl;
    //     }
    //     cout<<"--------------------------"<<endl;
    // }
    // cout<<"<<<<<===================================================>>>>>"<<endl;
    // return ;

}

void conditionalPatternBase(string item,fptr root,map<string,fptr> fpMap)
{
    vector<pair<vector<string> ,int>> CPBVec ;
    vector<string> vec ;
    int count=0 ;
    findPaths(root,count,CPBVec,vec,item) ;
    cout<<"path : "<<endl; 
    for(int i=0;i<CPBVec.size();i++)
    {
        for(int j=0;j<CPBVec[i].first.size();j++)
        {
            cout<<CPBVec[i].first[j] <<" " ;
        }
        cout<<CPBVec[i].second<<endl <<endl;
    }
    
    conditionalFP(root,item,CPBVec,fpMap) ;
    return ;

}

int main()
{
    ifstream in("inputFP.txt");
    vector<string> items ;

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

    for(auto it=records.begin();it!=records.end();it++){
        cout << it->first << " -> ";
        for(int i=0;i<it->second.size();i++){
            cout << it->second[i] << " ";
        }
        cout << "\n";
    }
    cout<<"\n\n";
    

    for(auto it=records.begin();it!=records.end();it++){
        for(int i=0;i<it->second.size();i++){
            countMap[it->second[i]] ++ ;
        }
    }
    // cout<<"---------"<<endl;
    // for(auto it=countMap.begin();it!=countMap.end();it++){
        
    //         cout<<it->first<<" "<<it->second ;
        
    // }cout<<"---------"<<endl;

    for(auto it=countMap.begin();it!=countMap.end();it++)
    {
        items.push_back(it->first) ;
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
        // cout<<it->first<<" -> ";
        // for(int x=0;x<vec.size();x++)
        // {
        //     cout<<vec[x]<<" ";
        // }
        // cout<<endl;
        create(root,vec,0,vec.size()) ;
    }
    cout<<"PreOrder of FPtree : "<<endl;

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
    cout<<"----------------------------------------------------------------"<<endl;
    cout<<"----------------------------------------------------------------"<<endl;

    cout<<"Enter support count : "<<endl;
    cin>>minSupCnt ;

    sort(items.begin(),items.end()) ;
    sort(items.begin(),items.end(),compare) ;// sort in decreaing order

    for(int i=items.size()-1;i>0;i--)
    {
        conditionalPatternBase(items[i],root,fpMap) ;
    }
    
    cout<<"Frequent Subsets  : "<<endl;
    for(auto it=FPGMap.begin();it!=FPGMap.end();it++)
    {
        for(int z=0;z<it->first.size();z++)
        {
            cout<<it->first[z]<<" ";
        }
        cout<<it->second <<endl;
    }

    return 0 ;
   
}