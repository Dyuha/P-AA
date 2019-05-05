#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <cstring>
#include <map>
#include <queue>

using namespace std;

class AdjPair {
public:
    AdjPair(char name, double weight): name(name), weight(weight) {};
    char name = 0;
    double weight = 0;
};

class VertexInAdjList {
public:
    VertexInAdjList(char name) : name(name){};

    std::list <AdjPair> AdjacentVertices;

    char get_name() const {
        return name;
    }
private:
    char name;
};

class AdjacencyList {
public:
    std::vector <VertexInAdjList> AdjList;

    void aStar(char start, char end); 

    double heuristic(char curVertex, char end) {   
        return abs(end - curVertex);
    }

    void addVertex(char name){   // добавляет вершину в граф
        for (auto &cur_vertex : AdjList)
            if (cur_vertex.get_name() == name)
                return;
        AdjList.emplace_back(name);
    }

    void addEdge(char start, char end, double length){   //добавление ребра в граф
        addVertex(start);
        addVertex(end);
        for (auto &vectorElem : AdjList){
            if (vectorElem.get_name() == start){
                vectorElem.AdjacentVertices.emplace_back(end, length);
            }
        }
    }
    // returns a copy of adjacency list of given vertex
    std::list <AdjPair> operator[](char name){
        for (auto vertex : AdjList)
            if (vertex.get_name() == name)
                return vertex.AdjacentVertices;
    }

    //need to check that for each rib (a,b)  h(a) - h(b) <= d(a,b)
    bool isConsistent(char end){
        for(int i = 0; i < AdjList.size(); ++i){
            char a = AdjList[i].get_name();
            double ha = heuristic(a, end);
            std::list <AdjPair> AdjV = (*this)[a];
            for(auto cur : AdjV){
                if(ha - heuristic(cur.name, end) > cur.weight)
                    return false;
            }
        }
        return true;
    }
};

class AdjPair_prior { //Вспомогательный класс для определения элементов очереди с приоритетом.
public:
    AdjPair_prior(char vert, double prior): vert(vert), prior(prior){};
    char vert;
    double prior;
    bool operator<(const AdjPair_prior b) const {
        return prior > b.prior;
    }
};

void AdjacencyList::aStar(char start, char end) { // реализация алгоритма A*
    std::priority_queue<AdjPair_prior> toExpandPriorQueue;
    toExpandPriorQueue.emplace(start, 0);
    std::map <char, char> prevChain;
    std::map <char, double> curCost;

    prevChain[start] = start;
    curCost[start] = 0;

    while(!toExpandPriorQueue.empty()) { // cout << ""<<endl;
        const char curVertex = toExpandPriorQueue.top().vert;
cout << "Достали вершину " << curVertex <<endl;
        toExpandPriorQueue.pop();

        if(curVertex == end) {
            break;
        }

        for(const AdjPair& child : (*this)[curVertex]) {  // placing in queue each child of cur vertex
             cout << "Новый потомок " << child.name <<endl;
            double new_cost = curCost[curVertex] + child.weight;
            if(curCost.find(child.name) == curCost.end() || new_cost < curCost[child.name]) { //not found or smaller than existing one
                curCost[child.name] = new_cost;
                double priority = new_cost + heuristic(child.name, end);
                cout << "Ее приоритет "<<priority <<endl;
                prevChain[child.name] = curVertex;
                cout << "В prevChain записываем " << child.name  << "и" << curVertex <<endl;
                toExpandPriorQueue.emplace(child.name, priority);  
                cout << "Записываем в очередь" << child.name <<endl;
            }
        }
    }
        // reconstructing the path
       std::vector<char> path;
       char elem = end;
       path.push_back(elem);
       while(elem!= start) {
           elem = prevChain[elem];
           path.push_back(elem);
       }
       reverse(path.begin(), path.end());
       for(int i = 0; i < path.size(); i++)
           std::cout << path[i];
       std::cout << std::endl;

      // std::cout << isConsistent(end) << std::endl;
       return;
}


int main(){
    AdjacencyList graph;
    double length;
    char start, end;

    std::cin >> start >> end;

    char u, v;

    while (std::cin >> u >> v >> length){
        graph.addEdge(u, v, length);
    }

    graph.aStar(start, end);

    return 0;
}
