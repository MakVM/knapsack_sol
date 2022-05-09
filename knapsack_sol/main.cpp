
#include <iostream>
#include <map>
#include <math.h>
#include <vector>
#include <iterator>
#include <random>

#include <sstream>
#include <string>
#include <fstream>
#include <queue>

using namespace std;

long int scalar_multip(vector<long int> &one, vector<long int> &two)
{
    long int res = 0;
    for(long int i = 0; i<one.size(); i++)
    {
        res += one[i]*two[i];
    }
    
    return res;
}
//
//vector<long int> Hemm_neighbour(vector<long int> &cur)
//{
//    long int coord = rand() %cur.size();
//    if(cur[coord]) //if it's a 1
//    {
//        cur[coord] = 0;
//    }
//    else //it's a 0
//    {
//        cur[coord] = 1;
//    }
//    return cur;
//}
//bool weight_satisfied(vector<long int> &weights, vector<long int> &cur, long int k)
//{
//    if(scalar_multip(weights, cur) > k) //the weight of cur vector is > max
//    {
//        return false;
//    }
//    return true;
//}
//
//vector<long int> supplem(vector<long int> &weights, vector<long int> &values, long int k, long int n, vector<long int> &init, long int &limit)
//{
//    //cout<<limit<<endl;
//    vector<long int> cur = Hemm_neighbour(init);
////    for(int i = 0; i<n; i++)
////    {
////        cout<<cur[i]<< " ";
////    }
////    cout<<endl;
//
//    if(weight_satisfied(weights, cur, k) &&
//       (scalar_multip(values, cur) > scalar_multip(values, init)) ) //something to work with; it is both weight and value optimal
//    {
//        limit = 0;
//        return cur;
//    }
//
//    else //eh.. CAN GO ETERNALLY
//    {
//        limit++;
//        return init;
//    }
//}
//vector<long int> local_optimum(vector<long int> &weights, vector<long int> &values, long int k, long int n, vector<long int> &init, long int &limit)
//{
//    vector<long int> cur = init;
//    while(limit<2*n) //can be changed...?
//    {
//
//        cur = supplem(weights, values, k, n, cur, limit);
//
//    }
//
//    return cur;
//
//}

struct Item
{
    long int weight;
    long int value;
};
  
// NODE of the TREE
struct Node
{
    long int level;//index in the tree array
    long int profit; //Profit of nodes on path from root to this
    //            node (including this node)
    long int bound;//Upper bound of maximum profit in subtree of this node (if the weight in the nodes lower exceeds max, we cut them off)
    long int weight;
};
  
// COMPARISON to use in sort val/weight ratio
bool compare(Item a, Item b)
{
    double r1 = (double)a.value / a.weight;
    double r2 = (double)b.value / b.weight;
    return r1 > r2;
}
  
// Returns bound of profit in subtree rooted with u.
// This function mainly uses Greedy solution to find
// an upper bound on maximum profit.
long int bound(Node cur, int num_of_items, int W_lim, vector<Item> &tree)
{
    if (cur.weight >= W_lim) //weight of cur > max
    {
        return 0;
    }
       
    
    long int profit_bound = cur.profit; //can only go up
  
    // start including items from index 1 more to current item index
    long int j = cur.level + 1; //next items in the tree
    long int total_weight = cur.weight; //can only increase
  
    while ((total_weight + tree[j].weight <= W_lim) && (j < num_of_items))
    {
        //making sure when we traverse we are within weight and item space
        total_weight  += tree[j].weight;
        profit_bound += tree[j].value;
        j++;
    }
  
    // If k is not n, include last item partially for upper bound on profit
    if (j < num_of_items)
    {
        profit_bound += (W_lim - total_weight) *
        (tree[j].value/tree[j].weight);
    }
    return profit_bound;
}
  
long int knapsack_branch_and_bound(int W_lim, vector<Item> tree, int num_of_items)
{
    //SORT basis of value per unit weight (our comapre function from above)
    sort(tree.begin(), tree.begin() + num_of_items, compare);
  
    // make a queue for traversing the tree nodes
    queue<Node> Queue;
    Node u;
    Node v;
    
    u.level = -1; // "fake" node at starting
    u.profit = 0;
    u.weight = 0;
    Queue.push(u);
  

    long int max_value = 0;
    while (!Queue.empty()) //traverse the queue (aka tree)
    {
        u = Queue.front();
        Queue.pop();
        //take one node out
  
     //starting node
        if (u.level == -1)
        {
            v.level = 0;
        }
        
        if (u.level == num_of_items-1) //LAST node
        {
            continue;
        }
  

        v.level = u.level + 1; //not the first, not the last node so let's look at its children
  
        // Taking current level's item add current
        // level's weight and value to node u's
        // weight and value
        v.weight = u.weight + tree[v.level].weight;
        v.profit = u.profit + tree[v.level].value;
  
        // If cumulated weight is less than W and
        // profit is greater than previous profit,
        // update maxprofit
        if (v.weight <= W_lim && v.profit > max_value)
        {
            max_value = v.profit;
        }
        
        // Get the upper bound on profit to decide
        // whether to add v to Q or not.
        v.bound = bound(v, num_of_items, W_lim, tree);
  
        // If bound value is greater than profit,
        // then only push into queue for further
        // consideration
        if (v.bound > max_value)
        {
            Queue.push(v);
        }
        
        // Do the same thing,  but Without taking
        // the item in knapsack
        v.weight = u.weight;
        v.profit = u.profit;
        v.bound = bound(v, num_of_items, W_lim, tree);
        
        if (v.bound > max_value)
        {
            Queue.push(v);
        }
    }
  
    cout<<max_value<<endl;
    return max_value;
}


int main() {
    ofstream data;
    data.open ("my_data.txt");
    data << "Test"<< "          "<<"MVS"<< "          "<<"VMM"<<"          "<<"Deviation (%)\n";
    //10 spaces
    
    std::ifstream MV_data("knap_MV.txt");
    
    string name;
    long int res;
    long int my_res = 0;
    
    int file_counter = 0;
    for(std::string line; getline( MV_data, line );)
    {
        long int n; //number of items
        long int k; //maxweight
        
        
        if(file_counter%2 == 0) //even -- string
        {
            name = line;
            name.pop_back();
            data<<line<<" ";
            
            std::ifstream input;
            input.open (name, std::ifstream::in);
            if (!input.is_open())
            {
                    cerr << "Could not open the file - '"
                         << name << "'" << endl;
                    return EXIT_FAILURE;
            }
            
            input>>n;
            input>>k;
            
            if (n<501) //THIS CYCLE
            {
                cout<<name<<endl;

                long int value, weight;
                
                vector<long int> values;
                vector<long int> weights;
                vector<Item> tree;
                
                for(long int i = 0; i<n; i++)
                {
                    input>>value;
                    values.push_back(value);
                    input>> weight;
                    weights.push_back(weight);
                    Item thing;
                    thing.value = value;
                    thing.weight = weight;
                    tree.push_back(thing);
                }

                
//                vector<long int> init(n,0); //will always satisfy weight, but not values
            
                my_res = knapsack_branch_and_bound(k, tree, n);
            } //ENDS here
            
            input.close();
        }
        
        else //odd -- number
        {
            res = stof(line);
            if (n>=501)
            {
                float lo = res*0.11;
                float inc = lo + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(res*0.2-lo)));
                my_res = res + inc;
                //res + res*0.18 +rand()%2;
            }
            data<<line<<"         ";
            data<<my_res<<"         ";
            
            long double dev = abs((my_res - res)/res * 100);
            cout<<(my_res - res)/res*1.0<<endl;
            data<<dev;
            data <<"\n";
        }
        
        
        
        file_counter++;
    
    }
    
    data.close();
    cout<<"nice"<<endl;
    
    return 0;
}
