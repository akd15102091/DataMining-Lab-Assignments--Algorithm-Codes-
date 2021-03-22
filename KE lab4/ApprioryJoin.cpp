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

bool checkSubsets(vector<string> cpart,int x,map<vector<string> ,int> l1map,int minSupp)
{
    vector<string> data ; 
    vector<vector<string> > KallSubsets ;
    int n = cpart.size() ;
    combinationUtil(cpart, n, x,data, 0,KallSubsets); 
    for(int i=0;i<KallSubsets.size() ;i++)
    {
        if(l1map[KallSubsets[i]] <minSupp) return false ;
    }
    return true ;
}

int checkCounting(vector<string> vecstr, map<string,vector<string> >records )
{
    int cnt=0 ;
    int n1 = vecstr.size() ;
    for(auto it=records.begin();it!=records.end();it++)
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

void createCtoLnext(vector<vector<string> >C,vector<vector<string> > &L,int k,map<string,vector<string> >records,map<vector<string> ,int> l1map,map<vector<string> ,int> &Lmap,int minSupp,map<vector<string> ,int> &apprioryMap)
{
    int csize = C.size() ;
    int x = k-1 ;
    for(int i=0;i<csize;i++)
    {
        if(checkSubsets(C[i],x,l1map,minSupp))      // check k-1 size subset of one set
        {
            vector<string> vecstr = C[i] ;
            int cnt = checkCounting(vecstr,records) ;       // for count subset in record
            if(cnt>=minSupp)
            {
                L.push_back(vecstr) ;
                Lmap[vecstr] = cnt ;
                apprioryMap[vecstr] = cnt ;
            }
        }
    }
    
    return ;
}

void combAllSubsets(vector<string> fastr, int n,  vector<string> data, int i,vector<vector<string> >&allSubsets) 
{ 
  
    if(i >= n)
	{
		if((data.size() >0) && (data.size() <n))allSubsets.push_back(data) ;
		return;
	}  
  
    data.push_back(fastr[i]) ;
    combAllSubsets(fastr, n, data, i + 1,allSubsets); 
    data.pop_back();
    combAllSubsets(fastr, n, data, i+1,allSubsets ); 
} 

int main()
{
    map<vector<string> ,int> apprioryMap ;
    ifstream in("inputJoin.txt");
    string str ;    //  just string variable1 for reading data in file
    string cur_trans;// just string variable for store current transaction number  
    map<string,int> mp;
    map<string,vector<string> > records;  // it contains data of original input file
    int cnt=0;

    while(in >> str){
        if(str[0] != 'T'){
            records[cur_trans].push_back(str);
        }
        else{
            cur_trans = str;
        }
    }
    in.close();

    /*
        now we have copied the records to map named records so that no need to access the file again
    */
    cout << "Transactions Scanned : \n" ;
    map<string,vector<string> > :: iterator it ; // iterator for scanning transaction
    for(it=records.begin();it!=records.end();it++){
        cout << it->first << " -> ";
        for(int i=0;i<it->second.size();i++){
            cout << it->second[i] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
  
    cout << "Enter the value of minimum support (like 20%,30%,50%) -> ";
    int minSupp; // minimum Support
    cin >> minSupp;
    minSupp /=10 ; 


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
            apprioryMap[v] = itr->second ;
        } 
    }
    vector<vector<string> >finalAnswer ; // it contains all frequest subsets
    map<vector<string> ,int>finalAnswerMap ; // it contains frequinces of all frequent subsets

    int k=1 ;
    while(1)
    {
        vector<vector<string> > C ;
        createLtoCnext(l1,C,k) ;

        // for(int i=0;i<C.size();i++)     // print "C"
        // {   for(int j=0;j<C[i].size();j++)
        //     { cout<<C[i][j]<<" "; }
        //     cout<<endl;
        // }
       // cout<<"-------------------------------------------------------"<<endl;
        k++ ;
        vector<vector<string> >L ;
        map<vector<string> ,int>Lmap ;
        createCtoLnext(C,L,k,records,l1map,Lmap,minSupp,apprioryMap) ;
       
        // for(int a=0;a<L.size();a++)  // print next L with frequencies ;
        // {   for(int b=0;b<(L[a]).size();b++)
        //     {   cout<<L[a][b]<<" "; }
        //     cout<<Lmap[L[a]]<<endl ;
        // }
        
        if(L.size() ==0)
        {
            finalAnswer = l1 ;
            finalAnswerMap = l1map ;
            break ;
        }

        l1 = L ;
        l1map = Lmap ;
    }

    cout<<"Frequent subsets with frequency : -" <<endl ;
    for(int i=0;i<finalAnswer.size();i++)
    {
        for(int j=0;j<finalAnswer[i].size();j++)
        {
            cout<<finalAnswer[i][j]<<" ";
        }
        cout<<finalAnswerMap[finalAnswer[i]] <<endl ;
    }

    // for(auto it1 = apprioryMap.begin();it1!=apprioryMap.end();it1++)
    // {
    //     for(int ab=0;ab<(it1->first).size();ab++)
    //     {
    //         cout<<it1->first[ab]<<" ";
    //     }
    //     cout<<it1->second <<endl;
    // }
    // return 0;

    cout<<"Enter minConfidence (like 20%,30%,50%) : "<<endl;
    double minConf ;
    cin>>minConf;
    minConf = (minConf)/( 100*1.0) ;
    for(int i=0;i<finalAnswer.size();i++)
    {
        vector<string> fastr = finalAnswer[i] ;
        vector<string> data ; 
        vector<vector<string> > allSubsets ;
        int n = fastr.size() ;
        combAllSubsets(fastr, n,data, 0,allSubsets); 
        for(int p=0;p<allSubsets.size();p++)
        {
            vector<string> vec1 ;
            vector<string> vec2 ;
            vec1 = allSubsets[p] ;
            int sz1 = vec1.size() ;
            int x=0 ;
            int y=0 ;
            while(x<sz1 && y<n)
            {
                if(vec1[x] !=fastr[y])
                {
                    vec2.push_back(fastr[y]) ;
                    y++ ;
                }
                else
                {
                    x++ ;
                    y++ ;
                }
            }
            while(y<n)
            {
                vec2.push_back(fastr[y]) ;
                y++ ;
            }
            double conf = (apprioryMap[fastr])/((apprioryMap[vec1])*1.0) ;
            //cout<<conf<<" "<<minConf<<endl;
            if(conf>=minConf)
            {
                cout<<"Confidence of : {" ;
                for(int var=0;var<sz1;var++)
                {
                    cout<<vec1[var]<<",";
                }
                cout<<"} ==> {";
                for(int var=0;var<vec2.size();var++)
                {
                    cout<<vec2[var]<<",";
                }
                cout<<"}  : ";
                cout<<conf<<endl;
            } 

        }
    }

    return 0 ;
   
}