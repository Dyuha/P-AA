#include <iostream>
#include <list>
#include <algorithm>
#include <iomanip>
#include <vector>

struct Vertex {
    char ch;
    std::tuple < bool, size_t, size_t > label; // 1 - is_terminal_vertex?, 2 - patter_num, 3 - dist_from_root
    Vertex* parent;
    Vertex* suffLink;
    std::list <Vertex*> children;
    std::vector <int> positionsInOriginalText;

    Vertex(Vertex* p, char ch) : ch(ch), label({false, 0, 0}), parent(p), suffLink(nullptr){};

};

class Bor {
public:
    Vertex* head;

    Bor() : head(new Vertex(nullptr, '~')) {} ;
    void addString(std::string const &str, int positionInOriginalText){
        static int number = 1;
        auto ptr = head;
        for (auto const &elem : str)
            ptr = addCharacterInVertex(elem, ptr);
        ptr->label = { true, number, str.size() };
        ptr->positionsInOriginalText.push_back(positionInOriginalText);
        number++;
    }

    void processText(std::string const &str, int numberOfSubStrings){
        Vertex* cur  = head;
        std::vector <int> helpArray(str.size());

        for (size_t i = 0; i < str.size(); i++){
            cur = goTo(cur, str[i]);
            for (Vertex* ptr = cur; ptr != head; ptr = getSuffLink(ptr))
                if (std::get<0>(ptr->label)){                   // if terminal
                    int position = i - std::get<2>(ptr->label) + 1 + 1; // where pattern should have started
                    int index = std::get<1>(ptr->label);        // pattern number

                     for (auto const startPos : ptr->positionsInOriginalText){
                        int arr_index = position;
                        arr_index -= startPos;
                        arr_index += 1;
                        if (arr_index >= 0){ 
                            helpArray[arr_index]++;
                        }
                    }
                }
        }
        for (long int i = 0; i < helpArray.size(); i++){
            if (helpArray[i] == numberOfSubStrings)
                std::cout << i << std::endl;
        }
    }

    void setSuffixLinks(){
        inDeep(head);
    }

  

private:
    void inDeep(Vertex* v, size_t indent = 0){
        v->suffLink = getSuffLink(v);
        for (auto const &el : v->children)
            inDeep(el, indent+1);
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

    Vertex* goTo(Vertex* v, char ch){
        for (auto const &elem : v->children)
            if (elem->ch == ch)
                return elem;
        if (v == head)
            return head;
        return goTo(getSuffLink(v), ch);
    }

    Vertex* addCharacterInVertex(char newCh, Vertex* vertex){
        for (auto &el: vertex->children)
            if (el->ch == newCh)
                return el;
        Vertex* newElem = new Vertex(vertex, newCh);
        vertex->children.push_back(newElem);
        return newElem;
    }   
};

std::vector<std::pair<std::string, size_t>> parse(std::string str, char joker){
    std::vector <std::pair<std::string, size_t>> ret;
    int curPos = 1;         //absolute position while 'pos' is relative
    for (int pos = str.find(joker); pos != std::string::npos ; pos = str.find(joker)){  //searching for joker
        std::string found = str.substr(0, pos);
        if (found.size() == 0){     // found at the beginning
            str = str.substr(pos+1);
            curPos++;
            continue;    
        }
        pos++;
        str = str.substr(pos);   // moving forward
        ret.push_back({found, curPos});
        curPos += pos;
    }
    if (str.size() != 0){
        ret.push_back({str, curPos});
    }
    return ret;
}

int main() {
    Bor bor;
    std::string HayStack;
    std::string pattern;
    char joker;
    std::cin >> HayStack >> pattern >> joker;

    auto withoutMask = parse(pattern, joker);

    for (auto const &el : withoutMask)
        bor.addString(el.first, el.second);
    bor.setSuffixLinks();

   

    bor.processText(HayStack, withoutMask.size());
    
    return 0;
}