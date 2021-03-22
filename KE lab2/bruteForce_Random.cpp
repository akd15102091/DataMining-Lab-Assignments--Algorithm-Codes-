#include<iostream>
#include<algorithm>
#include<string>
#include<ctime>
#include<fstream>
#include<map>
#include<vector>
#include<iterator>

using namespace std;

/*
    This function is to give all subsets of length k
*/

void func(vector<string> vals,vector<string> cur,int k,vector<vector<string> > &ans,int i=0){
    if(i>=vals.size()) return ;
    else{
        func(vals,cur,k,ans,i+1);
        cur.push_back(vals[i]);
        if(cur.size()==k)   ans.push_back(cur);
        else func(vals,cur,k,ans,i+1);
    }
    return ;
}

int main(){
    ofstream out1("bruteforce_Random_input.txt") ;
	
	 srand(time(0));
    //ofstream out("");
    int max_size,total_trans;
    cin >> max_size >> total_trans;
    for(int i=1;i<=total_trans;i++){
        bool flag = true;
        out1 << "T" << i << " ";
        for(int j=1;j<max_size;j++){
            if(rand()%2){
                flag=false;
                out1 << "i" << j << " ";
            }
        }
        if(flag) out1 << "i" << 1+rand()%100;
        out1 << "\n";
    }
    out1.close();
	
	// OCCURANCES OF EACH ITEM
	//ifstream input("KELab1RandomInput.txt") ;
    ifstream in1("bruteforce_Random_input.txt");
    string str ;
    string cur_trans;
    map<string,int> mp;
    map<string,vector<string> > records;
    int cnt=0;

    ofstream out("bruteforce_Random_output.txt") ;
    while(in1 >> str){
        if(str[0] != 'T'){
            records[cur_trans].push_back(str);
        }
        else{
            cur_trans = str;
        }
    }
    in1.close();

 /*
        now we have copied the records to map named records so that no need to access the file again
    */
    out << "Transactions Scanned : \n" ;
        map<string,vector<string> > :: iterator it ;
    for(it=records.begin();it!=records.end();it++){
        out << it->first << " -> ";
        for(int i=0;i<it->second.size();i++){
            out << it->second[i] << " ";
        }
        out << "\n";
    }
    out << "\n\n";
    /*
        Input Threshold value
    */
    cout << "Enter the value of threshold -> ";
    int threshold;
    cin >> threshold;
    /*
        cntre map will store the count of items
    */
    map<string,int> cnter;
    for(it=records.begin();it!=records.end();it++){
        for(int i=0;i<it->second.size();i++){
            cnter[it->second[i]]++;
        }
    }

    /*
        Extract items into vector from map cnter
    */
    out << "Items Scanned Along With Their Freq. >= threshold \n";
    vector<string> v;
    map<string,int> :: iterator itr ;
    for(itr=cnter.begin();itr!=cnter.end();itr++){
        if(itr->second >= threshold){
            out << itr->first << " " << itr->second << "\n";
            v.push_back(itr->first);
        }
    }
    out << "\n\n";
    map<vector<string>,int> temp_ans;
    
    for(int k=1;;k++){
        vector<vector<string> > lengthKvectors;
        vector<string> cur;
        func(v,cur,k,lengthKvectors);
        /*
            Sorting each vector so that we can use map easily to check cnt of occurance of vectors
        */

        for(int i=0;i<lengthKvectors.size();i++){
            sort(lengthKvectors.begin(),lengthKvectors.end());
        }
        int mflag=false;
        map<vector<string>,int> validPairs;

        for(int i=0;i<lengthKvectors.size();i++){
            for( it=records.begin();it!=records.end();it++){
                map<string,int> temp;
                for(int i=0;i<it->second.size();i++){
                    temp[it->second[i]]++;
                }
                bool flag = true;
                for(int j=0;j<lengthKvectors[i].size();j++){
                    if(temp[lengthKvectors[i][j]]==0){
                        flag = false;
                        break;
                    }
                }
                if(flag){
                    validPairs[lengthKvectors[i]]++;
                    if(validPairs[lengthKvectors[i]] >= threshold)
                    mflag = true;
                }
            }
        }
        if(mflag)   temp_ans = validPairs;
        else break;
    }

    /*
        Final list of transactions we have
        but we again have to check whether they are above threshold or not
        because we that there is at least one element above threshold but not about others
    */
    out << "Final list of most frequent subsets is : \n";
     map<vector<string>,int> :: iterator it1 ;
    for( it1=temp_ans.begin();it1!=temp_ans.end();it1++){
        if(it1->second >= threshold){
            out << "{ ";
            for(int j=0;j<it1->first.size();j++){
                out << it1->first[j] ;
                if(j!=it1->first.size()-1) out << ", " ;
            }
            out << " } (freq : " << it1->second << ")\n";
        }
    }
    out.close() ;


    return 0;
}
