//Note on code: Everything functions correctly. I have heavily commented on code to clarify all steps for myself and instructor 
//so no need for any details here. Just FYI, i am using cfloat and limits library to be able to use DLB_MAX which is float equivalent of INT_MAX.
#include <iostream>
#include <fstream>
#include <string>
#include <cfloat>
#include <limits>
#include <vector>
using namespace std;


class Weighted_Graph
{
	private:
		double** Main_Matrix; //double pointer serves the purpose of pointing to the pointer (array of pointers)
							  //to create our 2D-Matrix.
		double* lengths;	 // to store shortest weights/lengths of city-city (vertices) edges
		int *parent; // Stores Shortest Path Structure
		int dimension;  //matrix dimension (40x40 for this assingment.)
	public:
	Weighted_Graph () //default constructor
		{
		Main_Matrix = NULL;
		parent = NULL;
		lengths = NULL;
		dimension = 0;
		}
	Weighted_Graph (int D) //user will enter the dimension of matrix
		{
			dimension = D;
			Main_Matrix = new double* [dimension]; //we initialized our double pointer with an array of pointers of 'double' data type.
											  //this will let us initialize each entry of this array of pointer with new array of type 'double' - essentially
											 //creating our 2D-Matrix.
			for (int n = 0; n < dimension; n++)
				{
					Main_Matrix[n] = new double[dimension]; //this creates our 40x40 matrix skeletal structure.
				}
				
			lengths = new double [dimension]; //similar process to map out all relevant distances/weights.
			
			parent = new int [dimension]; //same method to map out all relevant pathways.

				
			for (int x = 0; x < dimension; x++)
			{
				for (int y = 0; y < dimension; y++)
				{ // set all values of our 2d matrix to max for a comparative analysis when we do plug 
				  //in the weights at each point of the adjacency matrix.
					Main_Matrix[x][y] = INT_MAX;
					if (x == y)  
					{ //exception being diagonals as [0][0] = [1][1] = ... = [40][40] = 0 (Venice would be 0 Km away from venice itself.)
						Main_Matrix[x][y] = 0;
					}
					
					parent[x] = -1;
				}
			}
		}
		
double Obtain_Distances ( int x, int y)
	{
		return lengths[x];
	}
	
void Initialize_Graph ()
	{
	int city, adj_city; double weight = 0; 	 //starting at [0][0]
	//dimension size (should be 40)
	ifstream fin("map.txt");
    if (!fin)
    {
    	cout << "Text file could not be located! Try again." << endl;
	}

	fin >> dimension;
	
	cout<<endl<<"DEBUG*********"<<endl<<dimension<<endl;  //for debugging

	
	Main_Matrix = new double*[dimension]; // n = 40 rows created for weighted matrix
	for (int n = 0; n < dimension; n++)
	{
		Main_Matrix[n] = new double[dimension]{}; //create a nxn = [40][40] 2d-Matrix for our weighted Graph
	}

	for (int n = 0; n < dimension; n++)
	{ 

			fin >> city;
			fin >> adj_city;
			
			while ( adj_city != -999)
				{
					fin >> weight; //take in the weight/length of the adjacent cities' route.
					/*
					if (adj_city == city)
					{
						Main_Matrix[n][m] = 0;
					} */
					Main_Matrix[city][adj_city] = weight; //now plug in this weight value into the respective position of city -> adj city on our matrix.
														  //e.g.: [venice][venice] = 0 as we previously set, but this mgiht set [venice][lyon] = 289.7 (kilometers)
														  //this will essentially override certain respective entries from all the INT_MAX we set all matrix entries except diagonals previously when we created our matrix.
					fin >> adj_city; //repeat process 
					//until text file reads -999
				}
	
	}
	
			fin.close();
	for (int i = 0; i < dimension; i++)
		{
			for (int j = 0; j < dimension; j++)
			{
				cout<<Main_Matrix[i][j]<<" ";
			}
			cout<<endl;
		}
		cout<<"Adjacency matrix created correctly! "<<endl<<endl;
	}
	
int selectMinVertex(vector<double>& value, vector<bool>& processed) {
    int minimum = INT_MAX;
    int vertex;
    for (int i = 0; i < dimension; ++i) {
        if (processed[i] == false && value[i] < minimum) {
            vertex = i;
            minimum = value[i];
        }
    }
    return vertex;
}

string city2num (string Cities[], int city)
	{
		return Cities[city];
	}

void printPath(string Cities[], int city, int destination) {
    if (destination == city) {
        cout << " " << city2num(Cities, destination);
        return;
    }
    printPath(Cities, city, parent[destination]);
    cout << " -> " << city2num(Cities, destination);
}

void dijkstra(string Cities[], int city, int destination) 
	{
	parent = new int [dimension];
    vector<double> value(dimension, DBL_MAX);
    vector<bool> processed(dimension, false);

    parent[city] = -1; // Start vertex has no parent
    value[city] = 0;   // Start vertex has value = 0 to get picked first.

    for (int i = 0; i < dimension - 1; ++i) {
        int U = selectMinVertex(value, processed);
        processed[U] = true;

        for (int j = 0; j < dimension; ++j) {
            if (Main_Matrix[U][j] != 0 && !processed[j] && value[U] != INT_MAX &&
                (value[U] + Main_Matrix[U][j] < value[j])) {
                value[j] = value[U] + Main_Matrix[U][j];
                parent[j] = U;
            }
        }
    }

    cout << "Shortest Distance: " << value[destination] << " Kilometers." << endl;
    cout << "Shortest Path: ";
    printPath(Cities, city, destination);
    cout << endl;
	}

~Weighted_Graph()
	{
		for(int n=0;n<dimension;n++)
		{
			delete []Main_Matrix[n];
			Main_Matrix[n]=NULL;
		}
		delete []Main_Matrix;
		delete []parent;
		delete []lengths;
	}

}; 

int Obtain_Index (string inp, string vertices [])
{
    for(int n = 0; n <40; n++)
    {
        if(vertices[n] == inp)
            return n;
    }
    return 0;
}

bool verify_city (string inp, string vertices []) {
    for(int n = 0; n < 40; n++) 
	{
		if(vertices[n] == inp)
		return true;
	}
	cout<<endl<<"City not found in database. Please verify your spellings."<<endl;
	return false;
}

int main ()
{
	string Cities[40]={"Glasgow", "Edinburgh", "Manchester", "Birmingham", "London", "Felixstowe", "Southampton", "Dover",
						  "Calais", "Lille", "Luxembourg", "Paris", "Bordeaux", "Vitoria", "Valladolid", "Aveiro", "Madrid", "Sines", "Zaragoza",
						  "Antequera", "Dijon", "Lyon", "Perpignan", "Barcelona", "Tarragona", "Valencia", "Marcia", "Cartagena", "Agleciras", "Marseille",
						  "Livorno", "Rome", "Bologna", "Ancona", "Ravenna", "Venice", "Basel", "Turin", "Munich", "Strasbourg"};

	string StartPoint;
	string EndPoint;
		Weighted_Graph World;
		World.Initialize_Graph();
	
	cout<<"\t\tWELCOME TO ASSIGNMENT 5 OF Data Structures & Algorithms."<<endl<<
	"\tPlease note that the menu options are selected by inputting integer values only."<<endl;
	
	int choice;
	while (choice !=2)
	{
    	cout << "\n\t\t*****************************************************************\n";
    	cout << "\t\t Press 1    >\t To Find Your Shortest Way Through Our World Map \n";
    	cout << "\t\t Press 2    >\t To End This Program. \n\n";
		cout << "\t\t*****************************************************************\n\t\t Choose: ";
		cin >> choice;
		
		if (choice == 1)
		{
				cout<< endl << "Importing all the information from Map File."<<endl;
		cout << "Importing.";  usleep(250000); cout<<"."; usleep(250000); cout<<"."<<endl; sleep(1);
		cout<<endl<<"Imported!"<<endl; 
		cout<<endl<<"Please make sure the first letter of each city is in Capital."<<endl;
		cout << "From: "<<endl<<endl;
			cin.ignore(); 
			getline(cin, StartPoint);
			if (!verify_city(StartPoint, Cities))
			{
				cout << "\n\t\tWrong Starting City Name!\n";
				continue;		
			}	
		
			cout << "To (Destination): ";
		    getline(cin, EndPoint);
		    if(!verify_city(EndPoint, Cities))
			{
				cout << "\n\t\tWrong Final City Name!\n";
				continue;
			}
			
			int s = Obtain_Index (StartPoint, Cities);
			int f = Obtain_Index (EndPoint, Cities);
			cout << "\n\t\tPath from " << StartPoint << " to " << EndPoint << ": " << endl;
			//World.dijkestra(s,f);
			World.dijkstra(Cities, s,f);
		    cout << "Thankyou for your participation.";
		}
	else if (choice == 2)
		{
			cout<<"Thankyou for availing our services!"<<endl;
			cout << "Terminating.";  usleep(500000); cout<<"."; usleep(500000); cout<<"."<<endl; sleep(1);
            cout<<"Terminated!"<<endl;
			break;
			}
	else if (choice < 1 || choice > 2)
		{
        	cout << "INVALID Option Chosen, PLEASE ENTER 1, OR 2 only. Thank you!" << endl << endl;
        } 	
	}
	return 0;
}