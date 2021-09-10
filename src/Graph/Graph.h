#pragma once
#pragma once
#include<iostream>
#include<list>
#include<vector>
#include<fstream>
#include<queue>
#include<string>
#include<assert.h>

struct Vertex {
	int weight;
	int ID;				// vertex ID
	std::string type;	// type of vertex

	Vertex(int ID = 0, std::string type = "", int weight = 0) {
		this->ID = ID;
		this->type = type;
		this->weight = weight;
	}
	Vertex(const Vertex& v) {
		this->ID = v.ID;
		this->type = v.type;
		this->weight = v.weight;
	}
	Vertex operator = (const Vertex& v) {
		if (&v != this) {
			this->ID = v.ID;
			this->type = v.type;
			this->weight = v.weight;
		}
		return *this;
	}
	// return true, if two vertices are equal
	bool operator == (const Vertex& v) {
		return (this->ID == v.ID);
	}
};

class Graph {
private:
	std::vector<std::list<Vertex*>> vList;		// list of vertexes
	int totalEdges;		// total number of edges in a connected graph

	// to increase allocated space of vertices list
	inline void reserveSpace(int size) {
		assert(size > 0 && size > vList.size());
		vList.reserve(size);
	}
public:
	Graph(unsigned int size = 1) {
		assert(size > 0);
		vList.reserve(size);
		this->totalEdges = 0;
	}
	Graph(const Graph& g) {
		this->vList = g.vList;
		this->totalEdges = g.totalEdges;
	}

	Graph operator = (const Graph& g) {
		if (&g != this) {
			this->vList.resize(g.vList.size());
			this->vList = g.vList;
		}
		return *this;
	}

	// to add vertex in a graph
	void addVertex(int vID, std::string vType) {
		assert(vID >= 0);
		std::list<Vertex*> newVertex;
		newVertex.push_back(new Vertex(vID, vType));

		if (vID >= vList.size()) vList.resize(vID + 1);

		vList.at(vID) = (newVertex);
	}

	// to add an edge b/w two vertices
	void addEdge(int start_ID, int end_ID, int weight)
	{
		assert(start_ID >= 0 && start_ID < vList.size() && end_ID >= 0 && end_ID < vList.size());

		std::list<Vertex*>* start = &(vList[start_ID]);
		std::list<Vertex*>* end = &(vList[end_ID]);

		if (start->size() <= 0 || end->size() <= 0) return;

		// update adjacent nodes
		Vertex* adjA = new Vertex(*(*(end->begin()))), * adjB = new Vertex(*(*(start->begin())));
		adjA->weight = adjB->weight = weight;		// update weights
		++totalEdges;
		start->push_back(adjA);
		//end->push_back(adjB);
	}

	// return total number of vertices
	int getTotalVertices() { return vList.size(); }
	// return total number of edges
	int getTotalEdges() { return this->totalEdges; }

	// BFS traversal to calculate total wight of a graph
	int calculateWeight(int start_ID = 0) {
		assert(start_ID >= 0 && start_ID < vList.size());

		int V = this->vList.size();  // total vertices
		bool* visited = new bool[V];

		for (int i = 0; i < V; ++i) visited[i] = false;
		int startVertexInd = start_ID;

		std::queue<int> vQue;
		vQue.push(startVertexInd);
		int totalWeight = 0;

		visited[startVertexInd] = true;
		while (!vQue.empty()) {
			int currVertex = vQue.front();
			vQue.pop();
			// adjacent nodes
			std::list<Vertex*>::iterator itr = vList[currVertex].begin();
			for (; itr != vList[currVertex].end(); ++itr) {
				if (!visited[(*(itr))->ID]) {
					totalWeight += (*(itr))->weight;
					visited[(*(itr))->ID] = true;
					vQue.push((*(itr))->ID);
				}
			}
		}
		return totalWeight;
	}
	// to write/print edges in a graph 
	friend std::ostream& operator << (std::ostream& _cout, Graph& g) {
		for (int i = 0; i < g.vList.size(); ++i) {
			if (g.vList[i].size() > 0) {
				std::list<Vertex*>::iterator itr = g.vList[i].begin();
				for (; itr != g.vList[i].end();) {
					std::cout << "(" << (*(itr))->ID + 1 << ", " << (*(itr))->weight << ")";
					++itr;
					if (itr != g.vList[i].end()) std::cout << " --> ";
				}
				std::cout << "\n";
			}
		}
		return _cout;
	}

	void printEdges() {
		if (this->vList.size() <= 0) return;

		std::cout << "[(start, end), weight]\n";
		bool* visitedFalgs = new bool[this->vList.size()];
		for (int i = 0; i < this->vList.size(); ++i) visitedFalgs[i] = false;
		for (int i = 0; i < this->vList.size(); ++i) {
			visitedFalgs[i] = true;
			if (this->vList[i].size() <= 1) continue;		// if no edge

			if (this->vList[i].size() > 0) {
				std::list<Vertex*>::iterator itr, head;
				itr = head = this->vList[i].begin();
				++itr;
				for (; itr != this->vList[i].end(); ++itr) {
					if (!visitedFalgs[(*(itr))->ID]) {
						std::cout << "[(" << (*head)->ID + 1 << ", " << (*(itr))->ID + 1 << "), " << (*(itr))->weight << "]\n";
					}
				}
			}
		}
	}

	// return true, if path exists b/w two nodes
	bool isPathExisted(int start_ID, int end_ID) {
		assert(start_ID >= 0 && start_ID < vList.size() && end_ID >=0 && end_ID < vList.size());

		int V = this->vList.size();  // total vertices
		bool* visited = new bool[V];

		for (int i = 0; i < V; ++i) visited[i] = false;
		
		int startVertexInd = start_ID;
		std::queue<int> vQue;
		vQue.push(startVertexInd);

		visited[startVertexInd] = true;
		while (!vQue.empty()) {
			int currVertex = vQue.front();
			vQue.pop();
			// check for adjacent nodes
			std::list<Vertex*>::iterator itr = vList[currVertex].begin();
			for (; itr != vList[currVertex].end(); ++itr) {
				if ((*(itr))->ID == end_ID) return true;
				if (!visited[(*(itr))->ID]) {
					visited[(*(itr))->ID] = true;
					vQue.push((*(itr))->ID);
				}
			}
		}
		return false;
	}


	bool isUndirected() const {
		int vertices = vList.size();
		// This keeps a counter for all vertices.
		// We will see later how it is utilized.
		std::vector <int> counter(vertices);

		// Traversing the entire adjacency list.
		// currentList => the current list in consideration. (also, it is the current vertex)
		for (unsigned int currentList = 0; currentList < vertices; currentList++) {

			// Traversing all elements in the current list.
			for (auto elementInCurrentList : vList[currentList]) 
			{
				// We add the elementInCurrentList at the currentList index of the counter
				// because it is a connected vertex.
				counter[currentList] += (elementInCurrentList->ID + 1);					// +1 for remapping

				// We subtract current vertex from the currentElement index so it evens out later.
				counter[elementInCurrentList->ID] -= (currentList + 1);					// - 1 for remapping.
			}
		}

		// At the end of the O(V + E) traversal, if the entire counter array is 0, then the graph is undirected.
		for (auto i : counter)
			if (i != 0) return false;
		return true;
	}

	//// return true, if graph is undirected
	//bool isUndirected() {
	//	int V = vList.size();
	//	for (int i = 0; i < V; i++) {
	//		if (this->vList[i].size() > 1) {
	//			std::list<Vertex*>::iterator itr, head;
	//			itr = head = this->vList[i].begin();
	//			++itr;
	//			for (; itr != this->vList[i].end(); ++itr) {
	//				if (vList[(*(itr))->ID].size() <= 1) continue;
	//				std::list<Vertex*>::iterator itr2 = vList[(*(itr))->ID].begin();
	//				++itr2;
	//				bool isFound = false;
	//				for (; itr2 != vList[(*(itr))->ID].end(); ++itr2) {
	//					if ((*(itr2))->ID == (*(head))->ID) {
	//						isFound = true;
	//						break;
	//					}
	//				}
	//				if (!isFound) return false;
	//			}
	//		}

	//	}
	//	return true;
	//}

	// read data from the file and build a graph
	void inputGraph(std::string filename) {
		std::fstream fin(filename);
		if (fin.fail()) std::cout << "OOPS! FAIL TO OPEN FILE!\n";
		else {
			// input vertices information
			int totalVertices = 0, totalEdges = 0;
			fin >> totalVertices;
			this->reserveSpace(totalVertices);
			for (int i = 0; i < totalVertices; ++i) {
				int ID = 0;
				std::string type = "";
				fin >> ID;
				fin.ignore();
				fin.ignore();
				std::getline(fin, type);

				this->addVertex(ID - 1, type);
			}
			// input edges information
			fin >> totalEdges;
			for (int i = 0; i < totalEdges; ++i) {
				int start_ind = 0, end_ind = 0, weight = 0;

				fin >> start_ind;
				fin.ignore();
				fin >> end_ind;
				fin >> weight;

				this->addEdge(start_ind - 1, end_ind - 1, weight);
			}
		}
		fin.close();
	}


	// destructor
	~Graph() {
		this->totalEdges = 0;
		// to deallocate memory
		for (std::list<Vertex*> v : vList) {
			if (vList.size() > 0) {
				std::list<Vertex*>::iterator itr = v.begin();
				for (; itr != v.end(); ++itr) {
					if ((*(itr)) != nullptr) delete (*(itr));
				}
			}
		}
	}
};

