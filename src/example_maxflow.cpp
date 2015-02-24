/**
 * 
 * A simple example to demnostrate how to use
 * Min-Cut/Max-Flow Algorithms developed by 
 * Yuri Boykov and Vladimir Kolmogorov.
 * Kindly read the GPL.txt and README.txt for details
 * 
 * This example(example_maxflow.cpp) is written by: Suryansh Kumar 
 * 
 **/

#include <iostream>
#include <vector>
#include "graph.h"
#define H 255
#define R 3
#define C 3
#define K 5
#define lambda 1.0

using namespace std;

struct unary
{
    int sourceVal;
    int sinkVal;
};

struct smoothness
{
    int current_node_id;
    int neighbour_node_id;
    int edge_potential;
};


class node_energy
{
    public: 
       void cal_unary(int a[R][C]);
       
       void cal_smoothness(int a[R][C], int b[R][C]);   
    
       std::vector<unary> nodeW;
       
       std::vector<smoothness> neigW;
};



void node_energy :: cal_unary(int a[R][C])
{

    for(int y = 0; y<R; y++)
    {
        for(int x =0; x<C; x++)
        {
            unary un;
            
            int mVal = a[y][x];
            
            un.sourceVal = (H - mVal);
            
            un.sinkVal   = (mVal);
            
            nodeW.push_back(un);
        }
    }
}

void node_energy :: cal_smoothness(int a[R][C], int b[R][C])
{
    int count = 0;
    
    for(int y = 0; y<R; y++)
    {
        for(int x = 0; x<C; x++)
        {
            int sum =0;
            int nodeVal = a[y][x];
            
            for(int i = y-1; i <= y+1; i++)
            {
                for(int j = x-1; j<= x+1; j++)
                {
                    if(i<0 || i >= R || j<0 || j>=C)
                    {
                        //cout<<"Exceeding Matrix Dimension"<<endl;
                    }
                    else
                    {
                        int matVal = a[i][j];
                        sum = lambda*(matVal-nodeVal)*(matVal-nodeVal);
                        smoothness smt;
                        smt.current_node_id = count;
                        smt.neighbour_node_id = b[i][j];
                        //cout<<"Sum "<<sum<<endl;
                        if(sum<K)
                            smt.edge_potential = sum;
                        else
                            smt.edge_potential = K;
                            
                        neigW.push_back(smt);
                    }
                    
                }
            }
            count++;
        }
    }
    
}

/**
 * Example Matrix;
 * 10, 90, 150
 * 230, 70, 120
 * 50, 200, 40
 * 
 * Unary function = (255-xi)*l' + xi*l;
 * Pair-wise function = min(k, lambda*||xi - xj||^2);
 * You can make your own functions to use it.
 * 
 * **/

int main(int argc, char *argv[])
{
    /*3x3 Matrix*/
    int data_array[R][C];
    int index_array[R][C]; 
    
    /*Data Matrix Initialization*/
    data_array[0][0] = 10;  data_array[0][1] = 90;  data_array[0][2] = 150; 
    data_array[1][0] = 230; data_array[1][1] = 70;  data_array[1][2] = 120; 
    data_array[2][0] = 50;  data_array[2][1] = 200; data_array[2][2] = 40; 
    
    /*Node id Matrix Initialization*/
    index_array[0][0] = 0;  index_array[0][1] = 1;  index_array[0][2] = 2; 
    index_array[1][0] = 3;  index_array[1][1] = 4;  index_array[1][2] = 5; 
    index_array[2][0] = 6;  index_array[2][1] = 7;  index_array[2][2] = 8; 
    
    /*Object to intialise unary and smoothness weight to graph nodes*/
    node_energy nE;
    
    nE.cal_unary(data_array);
    
    nE.cal_smoothness(data_array, index_array);
    
    /* Assign the values to the graphs*/
    
    
    std::vector<unary>:: iterator ssnode_weight     = nE.nodeW.begin();
    std::vector<smoothness>:: iterator bnode_weight = nE.neigW.begin();
    
    typedef Graph<int,int,int> GraphType;
    
    /**Second term is substracting node size because, the node itself is also considered while calculating smoothness term**/ 
    GraphType *g = new GraphType(nE.nodeW.size(), nE.neigW.size() - nE.nodeW.size()); 
    
   
    int node=0;  
    while(ssnode_weight != nE.nodeW.end())
    {
		unary temp =  *ssnode_weight;
		g -> add_node(); 
		g -> add_tweights( node, temp.sourceVal, temp.sinkVal );
		//cout<<"Source Weight"<<" "<<"Sink Weight "<<temp.sourceVal<<" "<<temp.sinkVal<<endl;
		*ssnode_weight++;
		node++;
    }
    
    while(bnode_weight!=nE.neigW.end())
    {
		smoothness linkw =  *bnode_weight;
        if(linkw.current_node_id != linkw.neighbour_node_id)
        {
            g -> add_edge( linkw.current_node_id, linkw.neighbour_node_id, linkw.edge_potential, linkw.edge_potential );
            //cout<<"Node_id1"<<" "<<"Node_id2 "<<" Distance "<< linkw.current_node_id <<" "<<linkw.neighbour_node_id<<" "<<linkw.edge_potential<<endl;
        }
		*bnode_weight++;
    }
    
    //Max-flow algorithm
    int flow = g -> maxflow();
    
    for(int i = 0; i<nE.nodeW.size(); i++)
    {
        if (g->what_segment(i) == GraphType::SOURCE)
        {
            cout<<"Node "<< i << " is in the source "<<endl;
        }
	else
        {
            cout<<"Node "<< i << " is in the sink "<<endl;
	}

    }

    return 0;
}



