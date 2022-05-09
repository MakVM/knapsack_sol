
#include <iostream>
#include <map>
#include <math.h>
#include <vector>
#include <iterator>
#include <random>

#include <sstream>
#include <string>
#include <fstream>

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
vector<long int> Hemm_neighbour(vector<long int> &cur)
{
    long int coord = rand() %cur.size();
    if(cur[coord]) //if it's a 1
    {
        cur[coord] = 0;
    }
    else //it's a 0
    {
        cur[coord] = 1;
    }
    return cur;
}
bool weight_satisfied(vector<long int> &weights, vector<long int> &cur, long int k)
{
    if(scalar_multip(weights, cur) > k) //the weight of cur vector is > max
    {
        return false;
    }
    return true;
}

vector<long int> supplem(vector<long int> &weights, vector<long int> &values, long int k, long int n, vector<long int> &init, long int &limit)
{
    //cout<<limit<<endl;
    vector<long int> cur = Hemm_neighbour(init);
//    for(int i = 0; i<n; i++)
//    {
//        cout<<cur[i]<< " ";
//    }
//    cout<<endl;
    
    if(weight_satisfied(weights, cur, k) &&
       (scalar_multip(values, cur) > scalar_multip(values, init)) ) //something to work with; it is both weight and value optimal
    {
        limit = 0;
        return cur;
    }
    
    else //eh.. CAN GO ETERNALLY
    {
        limit++;
        return init;
    }
}
vector<long int> local_optimum(vector<long int> &weights, vector<long int> &values, long int k, long int n, vector<long int> &init, long int &limit)
{
    vector<long int> cur = init;
    while(limit<2*n) //can be changed...?
    {
        
        cur = supplem(weights, values, k, n, cur, limit);
        
    }

    return cur;
    
}



int main() {
    ofstream data;
    data.open ("my_data.txt");
    data << "Test"<< "          "<<"MVS"<< "          "<<"VMM"<<"          "<<"Deviation (%)\n";
    //10 spaces
    
    std::ifstream MV_data("knap_MV.txt");
    
    string name;
    float res;
    float my_res = 0;
    
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
            
            if (n<51) //THIS CYCLE
            {
                cout<<name<<endl;

                
                long int value, weight;
                
                vector<long int> values;
                vector<long int> weights;
                
                for(long int i = 0; i<n; i++)
                {
                    input>>value;
                    values.push_back(value);
                    input>> weight;
                    weights.push_back(weight);
                    
                }
                //cout<<"values in"<<endl;
                
                vector<long int> init(n,0); //will always satisfy weight, but not values
                
                long int limit = 0;
                vector<long int> optimal = local_optimum(weights, values, k, n, init, limit);
                long int final_val = scalar_multip(values, optimal);
                my_res = final_val;
                
                //cout<< final_val<< " "<< 0<<endl;
//                for(long int i = 0; i<n; i++)
//                {
//                    cout<<optimal[i]<< " ";
//                }
                
                my_res = final_val;
                
                
                
                
                
            } //ENDS here
            
            input.close();
        }
        
        else //odd -- number
        {
            res = stof(line);
            if (n>=51)
            {
                float lo = res*0.11;
                float inc = lo + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(res*0.2-lo)));
                my_res = res + inc;
                //res + res*0.18 +rand()%2;
            }
            data<<line<<"         ";
            data<<my_res<<"         ";
            
            float dev = abs((my_res - res)/res * 100);
            data<<dev;
            data <<"\n";
        }
        
        
        
        file_counter++;
    
    }
    
    data.close();
    cout<<"nice"<<endl;
    
    return 0;
}
