#include <iostream>
#include <list>
#include <algorithm>
#include <cstring>
#include <iomanip>

bool test = false;

struct Vertex {
    char ch;
    std::tuple <bool, size_t, size_t> label; // 1 - is_terminal_vertex?, 2 - patter_num, 3 - dist_from_root
    Vertex* parent;
    Vertex* suffLink;
    std::list <Vertex*> children;

    Vertex(Vertex* p, char ch) : ch(ch), label({false, 0, 0}), parent(p), suffLink(nullptr){};
};

class Bor {
public:
    Vertex* head;
    Bor() : head(new Vertex(nullptr, '~')) {} ;

    void addString(std::string const &str){ // adding patterns in bor
        static int number = 1;
        auto ptr = head;                    // starting to move from head
        for (auto const &elem : str)
            ptr = addCharacterInVertex(elem, ptr); 
        ptr->label = { true, number, str.size() };
        number++;
    }

    void processText(std::string const &str){
        Vertex* cur  = head;
        for (size_t i = 0; i < str.size(); i++){    // going through the text
            cur = goTo(cur, str[i]);
            for (Vertex* ptr = cur; ptr != head; ptr = getSuffLink(ptr))
                if (std::get<0>(ptr->label)){       // if vertex is termial (have pattern ended)
                    std::cout << i - std::get<2>(ptr->label) + 2 << " " << std::get<1>(ptr->label) << std::endl;
                }
        }
    }

    void setSuffixLinks(){
        inDeep(head);
    }

    int startMax(Vertex* vertex){
        int max = 0;
        for(auto child : vertex->children)
            max = std::max(startMax(child), max);
        max = std::max(maxSuffLink(vertex), max);
        return max;
    }

private:
    void inDeep(Vertex* v){
        v->suffLink = getSuffLink(v);
        for (auto const &el : v->children)
            inDeep(el);
    }

    Vertex* getSuffLink(Vertex* v){  
        if (v->suffLink == nullptr){
            if (v == head || v->parent == head)
                v->suffLink = head;
            else 
                v->suffLink = goTo(getSuffLink(v->parent), v->ch); 
        }
        
        return v->suffLink;
    }

    Vertex* goTo(Vertex* v, char ch){   // determines where we go from vertex 'v' having char 'ch'
        for (auto const &elem : v->children){
            if (elem->ch == ch)
                return elem;
            
        }
       
        if (v == head)
            return head;
        
        return goTo(getSuffLink(v), ch);
    }

    Vertex* addCharacterInVertex(char newCh, Vertex* vertex){
        for (auto &el: vertex->children)
            if (el->ch == newCh)           // found the character
                return el;
        Vertex* newElem = new Vertex(vertex, newCh); // not found -> adding a new node
        vertex->children.push_back(newElem);
        return newElem;
    }   


    int maxSuffLink(Vertex* vertex){
        if(vertex == head)
            return 0;
        return 1 + maxSuffLink(getSuffLink(vertex));
    }

};

int main(int argc, char** argv) {
    
    Bor bor;
    std::string HayStack;
    std::string pattern;
    int numberOfPatterns;
    std::cin >> HayStack >> numberOfPatterns;
    for (int i = 0; i < numberOfPatterns; i++){
        std::cin >> pattern;
        bor.addString(pattern);
    }
    bor.setSuffixLinks();
    bor.processText(HayStack);

 
    return 0;
}