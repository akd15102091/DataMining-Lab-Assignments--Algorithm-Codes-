#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<cmath>
#include<climits>

using namespace std;

void print_vector_pair(vector<pair<double, double> > num)
{
    for(int i=0;i<num.size();i++)
    {
        cout<<num[i].first<<", "<<num[i].second<<"\n";
    }
}

double eucleadian_distance(pair<double,double> p1, pair<double,double> p2)
{
    return sqrt((p1.first-p2.first)*(p1.first-p2.first)+(p1.second-p2.second)*(p1.second-p2.second));
}

void print_clusters(vector<vector<pair<double,double> > > clusters, vector<pair<double,double> > centroid)
{
    ofstream out;
    out.open("output.txt");
    for(int i=0;i<clusters.size();i++)
    {
        out<<"CLUSTER NUMBER: "<<i+1<<"\n";
        out<<"CENTROID: ( "<<centroid[i].first<<", "<<centroid[i].second<<" )\n";
        out<<"[\n";
        for(int j=0;j<clusters[i].size();j++)
        {
            out<<"("<<clusters[i][j].first<<", "<<clusters[i][j].second<<"),\n";
        }
        out<<"]\n";
    }
}
int main()
{
    ifstream inp;
    inp.open("input.txt");

    int num_of_clusters;
    vector<pair<double, double> > num;
    int i=0;
    //In the file, the first number is the number of clusters and the rest of them are the data tuples
    while(!inp.eof())
    {
        if(i==0)
        {
            inp>>num_of_clusters;
            i++;
        }
        else
        {
            double x, y;
            pair<double,double> p;
            inp>>x>>y;
            p.first=x;
            p.second=y;
            num.push_back(p);
        }
    }

    num.pop_back(); //eof condition for the last iteration makes the last tuple enter the array twice, so poped
    vector<vector<pair<double,double> > > clusters(num_of_clusters);
    vector<pair<double,double> > centroid;
    for(int i=0;i<num_of_clusters;i++)
    {
        clusters[i].push_back(num[i]);
        centroid.push_back(num[i]);
    }
    
    for(int i=num_of_clusters;i<num.size();i++)
    {
        int min=-1, min_dist=INT_MAX;
        for(int j=0;j<num_of_clusters;j++)
        {
            if(eucleadian_distance(num[i],centroid[j])<min_dist)
            {
                min_dist=eucleadian_distance(num[i],centroid[j]);
                min=j;
            }
        }
        clusters[min].push_back(num[i]);
        centroid[min].first=(centroid[min].first + num[i].first)/2;
        centroid[min].second=(centroid[min].second + num[i].second)/2;
    }
    
    print_clusters(clusters, centroid);
}