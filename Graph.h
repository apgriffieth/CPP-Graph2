//********************************************************************
// ASU CSE310 Assignment #7
// Name of Author: Alan Griffieth
// ASU ID:1212575453
// Description: This is the header file that defines an undirected graph
//********************************************************************

#include "Edge.h"

using namespace std;

class Graph
{
 private:
  int V, E;   //number of vertices and edges in the graph
  City** setOfCities;   //an array of cities
  Edge** setOfEdges; //an array of edges

 public:
  Graph(int numOfCities, int numOfEdges, City* cityArray[], Edge* edgeArray[]);
  void destructor();
  void make_set(City* aCity);
  City* find_set(City* aCity);
  void link(City* aCity, City* bCity);
  void Union(City* aCity, City* bCity);
  void printEdgeArray();
  void MergeSortEdges();
  void mergeSort(Edge* edgeArray[], int start, int end);
  void merge(Edge* edgeArray[], int start, int mid, int end);

  void MST_Kruskal();
};

//*******************************************************************
//Constructor
Graph::Graph(int numOfCities, int numOfEdges, City* cityArray[], Edge* edgeArray[])
{
  this->V = numOfCities;
  this->E = numOfEdges;
  this->setOfCities = cityArray;
  this->setOfEdges = edgeArray;
}

//*******************************************************************
//Destructor
void Graph::destructor()
{
  for(int i = 0; i < E; i++) {
    delete(setOfEdges[i]);
  }

  for(int j = 0; j < V; j++) {
    delete(setOfCities[j]);
  }
}

//*******************************************************************
//Given a City, this function creates a new set whose only member is
//the City.
void Graph::make_set(City* aCity)
{
  aCity->parent = aCity;
  aCity->rank = 0;
}

//*******************************************************************
//Given a City, this function returns a pointer to the representative
//of the (unique) set containing 'aCity'
City* Graph::find_set(City* aCity)
{
  if (aCity != aCity->parent) {
    aCity->parent = find_set(aCity->parent);
  }
  return(aCity->parent);
}

//*******************************************************************
//Given two cities, this function checks the dynamic sets that contain
//'aCity' and 'bCity' and update their rank and parent info. accordingly
void Graph::link(City* aCity, City* bCity)
{
  if (aCity->rank > bCity->rank) {
    bCity->parent = aCity;
  }

  else {
    aCity->parent = bCity;
    if (aCity->rank == bCity->rank) {
      bCity->rank++;
    }
  }
}

//*******************************************************************
//Given two cities, this function unites the dynamic sets that contain
//'aCity' and 'bCity' into a new set that is the union of the two sets.
void Graph::Union(City* aCity, City* bCity)
{
  this->link(this->find_set(aCity), this->find_set(bCity));
}

//*******************************************************************
//This function print each of the edges of edge array. It can be
//used as an auxillary function to check the MergeSort result
void Graph::printEdgeArray()
{
  for (int i = 0; i < E; i++) {
    setOfEdges[i]->printEdge();
  }
}

//*******************************************************************
//This function performs the Merge Sort on the graph edges according
//to the weight. Sort all edges in non-decreasing order
void Graph::MergeSortEdges()
{
  this->mergeSort(this->setOfEdges, 0, this->E - 1);
}

//*******************************************************************
//This function performs the Merge Sort on a sub edgeArray.
void Graph::mergeSort(Edge* edgeArray[], int start, int end)
{
  if (start < end) {
    int mid = (start + end) / 2;
    this->mergeSort(edgeArray, start, mid);
    this->mergeSort(edgeArray, mid + 1, end);
    this->merge(edgeArray, start, mid, end);
  }
}

//*******************************************************************
//This function merges the two sorted sub edgeArray.
void Graph::merge(Edge* edgeArray[], int start, int mid, int end)
{
  int n1 = mid-start+1;  //get the length of the first half subarray
  int n2 = end-mid;      //get the length of the second half subarray

  //Dynamically create two new arrays each stored the sorted half
  Edge** left = (Edge**)malloc(sizeof(Edge*) * n1);
  Edge** right = (Edge**)malloc(sizeof(Edge*) * n2);

  for (int i = 0; i < n1; i++) {//copy data from first subarray
    left[i] = edgeArray[start + i];
  }

  for (int j = 0; j < n2; j++) {//copy data from second subarray
    right[j] = edgeArray[mid + j + 1];
  }
  int i = 0;
  int j = 0;
  int k = start;
  while (i < n1 && j < n2) {//Merge the temporary arrays back into the original array in sorted order
    if(left[i]->getPrice() <= right[j]->getPrice()) {
      edgeArray[k] = left[i];
      i++;
    }

    else {
      edgeArray[k] = right[j];
      j++;
    }
    k++;
  }

  while (i < n1) {//copies any remaining elements of left[] into the original array
    edgeArray[k] = left[i];
    i++;
    k++;
  }

  while (j < n2) {//copies any remaining elements fo right[] into the original array
    edgeArray[k] = right[j];
    j++;
    k++;
  }
  //at the end, release the memory left and right occupied
  delete left;
  delete right;
}


//*******************************************************************
//This function performs the Kruskal algorithm on the graph.
void Graph::MST_Kruskal()
{
  cout << "MST Kruskal Algorithm Result\n" << endl;
  double totalWeight = 0.0;
  
  for (int i = 0; i < this->V; i++) {
    this->make_set(this->setOfCities[i]);
  }
  
  this->MergeSortEdges();

  for (int j = 0; j < this->E; j++) {
    if (this->find_set(this->setOfEdges[j]->getCity1()) != this->find_set(this->setOfEdges[j]->getCity2())) {
      this->setOfEdges[j]->printEdge();
      totalWeight += this->setOfEdges[j]->getPrice();
      this->Union(this->setOfEdges[j]->getCity1(), this->setOfEdges[j]->getCity2());
    }
  }

  cout << "=================================================" << endl;
  cout << "Total Price: " << totalWeight << endl;
}
