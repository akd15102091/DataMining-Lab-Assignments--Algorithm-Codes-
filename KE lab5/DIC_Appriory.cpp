#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<fstream>
#include<iterator>
#include<algorithm>
#include <sstream> 
using namespace std ;

bool check(vector<vector<string> >L, int i,int j,int x)
{
    vector<string> v1 = L[i] ;
    vector<string> v2 = L[j] ;
    for(int p=0;p<x;p++)
    {
        if(v1[p]!=v2[p]) return false ;
    }
    return true ;
}

void createLtoCnext(vector<vector<string> >L, vector<vector<string> > &C , int k)
{
    int x = k-1 ;
    int lsize = L.size() ;
    for(int i=0;i<lsize-1;i++)
    {
        for(int j=i+1;j<lsize;j++)
        {
            if(check(L,i,j,x))
            {
                vector<string> vec;
                int s1 = L[i].size() ;
                int s2 = L[j].size() ;
                for(int p=0;p<s1;p++)
                {
                    vec.push_back(L[i][p]) ;
                }
                for(int p=x;p<s2;p++)
                {
                    vec.push_back(L[j][p]) ;
                }
                C.push_back(vec) ;
            }   
        }
    }
    return ;
}

void combinationUtil(vector<string> cpart, int n, int x, vector<string> data, int i,vector<vector<string> >&KallSubsets) 
{ 
    // Current cobination is ready, print it 
    if (data.size() == x) { 
        KallSubsets.push_back(data) ;
        return; 
    } 
  
    if(i >= n) return; 

    data.push_back(cpart[i]) ;
    combinationUtil(cpart, n, x, data, i + 1,KallSubsets); 
    data.pop_back();
    combinationUtil(cpart, n, x, data, i+1,KallSubsets ); 
} 

bool checkSubsets(vector<string> cpart,int x,double minSupp, map<vector<string> ,bool>dicMap )
{
    vector<string> data ; 
    vector<vector<string> > KallSubsets ;
    int n = cpart.size() ;
    combinationUtil(cpart, n, x,data, 0,KallSubsets); 
    for(int i=0;i<KallSubsets.size() ;i++)
    {
        //if(l1map[KallSubsets[i]] <minSupp) return false ;
        if(dicMap[KallSubsets[i]] == false) return false ;
    }
    return true ;
}

int checkCounting(vector<string> vecstr, map<string,vector<string> >DicParRecords )
{
    int cnt=0 ;
    int n1 = vecstr.size() ;
    for(auto it=DicParRecords.begin();it!=DicParRecords.end();it++)
    {
        vector<string> rcd = it->second ;
        int n2 = rcd.size() ;
        int i=0 ;
        int j=0 ;
        while(i<n1 && j<n2)
        {
            if(vecstr[i] == rcd[j])
            {
                i++ ;
                j++ ;
            }
            else
            {
                j++ ;
            }
            
        }
        if(i==n1) cnt++ ;
    }
    return cnt ;
}

void createCtoLnext(vector<vector<string> >C,vector<vector<string> > &L,int k,map<string,vector<string> >records,double minSupp,int M, map<vector<string> ,bool> &dicMap )
{
    int csize = C.size() ;
    int x = k-1 ;
    for(int i=0;i<csize;i++)
    {
        if(checkSubsets(C[i],x,minSupp,dicMap))      // check k-1 size subset of one set
        {
            int cnt=0 ;
            vector<string> vecstr = C[i] ;
            auto parItr = records.begin() ;
            while(parItr!=records.end())
            {
                map<string,vector<string> > DicParRecords; // partitioned part of original database
                int countTrans = 0 ;
                while((parItr!=records.end() ) && countTrans<M )
                {
                    DicParRecords[parItr->first] = parItr->second ;
                    countTrans++ ;
                    parItr++ ;
                }
                cnt += checkCounting(vecstr,DicParRecords) ;       // for count subset in record
                if(cnt>=minSupp)
                {
                    L.push_back(vecstr) ;
                    dicMap[vecstr] = true ;
                    break ;
                }
            }
           
        }
    }
    
    return ;
}



int main()
{
    map<vector<string> ,bool>dicMap ;  // all frequent subsets will store in this 
    ifstream in("DIC_Input.txt");
    ofstream out("DIC_Output.txt");
    string str ;    //  just string variable1 for reading data in file
    string cur_trans;// just string variable for store current transaction number  
    map<string,int> mp;
    map<string,vector<string> > records;  // it contains data of original input file
    int cnt=0;
    int numberOfTransaction = 0 ;
    while(in >> str){
        if(str[0] != 'T'){
            records[cur_trans].push_back(str);
        }
        else{
            numberOfTransaction++ ;
            cur_trans = str;
        }
    }
    in.close();

    /*
        now we have copied the records to map named records so that no need to access the file again
    */
    cout << "Transactions Scanned : \n" ;
    out << "Transactions Scanned : \n" ;

    map<string,vector<string> > :: iterator it ; // iterator for scanning transaction
    for(it=records.begin();it!=records.end();it++){
        cout << it->first << " -> ";
        out << it->first << " -> ";

        for(int i=0;i<it->second.size();i++){
            cout << it->second[i] << " ";
            out << it->second[i] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
    out << "\n\n";

        
    //  We will partitioning the database in group of at most M transactions

    cout<<"Enter the number of partitions (M) : "<<endl;
    int M ;
    cin>>M ;
    cout<<"\n" ;
    cout << "Enter the value of minimum support (like 25%,30%,50%) -> ";
    double minSupp; // minimum Support
    cin >> minSupp;
    minSupp /=10 ;
    //minSupp = (minSupp*numberOfTransaction)/100 ;
    //cout<<minSupp;



    // find c1
    map<string,int> c1Count;//  count of seperate items (frequency in c1)
    for(it=records.begin();it!=records.end();it++){
        for(int i=0;i<it->second.size();i++){
            c1Count[it->second[i]]++;
        }
    }


    vector<vector<string> > l1; // frequentset l1 (2D_vector)
    map<vector<string> ,int>l1map ; // this contains l1 items and their frequency
    for(auto itr=c1Count.begin();itr!=c1Count.end();itr++)
    {
        if(itr->second >= minSupp)
        {
            vector<string> v ;
            v.push_back(itr->first) ;
            l1.push_back(v) ;
            l1map[v] = itr->second ; 
            dicMap[v] = true ;
        } 
    }    
    int k=1 ;
    while(1)
    {

        vector<vector<string> > C ;
        createLtoCnext(l1,C,k) ;
 
        k++ ;
        vector<vector<string> >L ;
        map<vector<string> ,int>Lmap ;
        createCtoLnext(C,L,k,records,minSupp,M,dicMap) ;
     
        if(L.size() ==0)
        {
            break ;
        }

        l1 = L ;
        
    }

    cout<<"All Frequent Item Sets: "<<endl;
    out<<"All Frequent Item Sets: "<<endl;

    for(auto it=dicMap.begin();it!=dicMap.end();it++)
    {
        cout<<"{";
        out<<"{";

        for(int z=0;z<(it->first).size();z++)
        {
            cout<<it->first[z]<<" ";
            out<<it->first[z]<<" ";
        }
        cout<<"}";
        out<<"}";
        cout<<endl;
        out<<endl;

    }
        
    // input : minsupp-20%,M-2
    return 0 ;
   
}