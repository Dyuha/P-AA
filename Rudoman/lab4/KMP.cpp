//#include <bits/stdc++.h>
#include <iostream> 
#include <thread>
#include <vector>

void KMPSearch(std::string &pat, std::string &txt, int* lps, int begin, int end, std::vector<int> &positions) { 
    int M = pat.size();

    // values for pattern   
    int i = begin; // index for txt[] 
    int j = 0; // index for pat[] 
    while(i < end) { 
        if(pat[j] == txt[i]) { 
            j++; 
            i++; 
        } 
  
        if(j == M) {
            positions.emplace_back(i-j); 
            j = lps[j - 1]; 
        } 
        // mismatch after j matches 
        else if(i < end && pat[j] != txt[i]) { 
            // Do not match lps[0..lps[j-1]] characters, 
            // they will match anyway 
            (j != 0) ? j = lps[j - 1] : i = i + 1;
        } 
    } 
} 
  
// Fills lps[] for given patttern pat[0..M-1] 
void computeLPSArray(std::string &pat, int M, int* lps) { 
    // length of the previous longest prefix suffix 
    int len = 0; 
  
    lps[0] = 0; // lps[0] is always 0 
  
    // the loop calculates lps[i] for i = 1 to M-1 
    int i = 1; 
    while (i < M) { 
        if (pat[i] == pat[len]) { 
            len++; 
            lps[i] = len; 
            i++; 
        } 
        else { // (pat[i] != pat[len]) 
            // This is tricky. Consider the example. 
            // AAACAAAA and i = 7. The idea is similar 
            // to search step. 
            if (len != 0) { 
                len = lps[len - 1]; 
            } 
            else { 
                lps[i] = 0; 
                i++; 
            } 
        } 
    } 
} 

int main() { 
    std::string txt;
    std::string pat;
    std::cin >> pat >> txt; 

    // create lps[] that will hold the longest prefix suffix 
    int pSize = pat.size();
    int tSize = txt.size();

    if(pSize > tSize){
        std::cout << -1;
        return 0;
    }

    int* lps = new int[pSize]; 
    // Preprocess the pattern (calculate lps[] array) 
    computeLPSArray(pat, pSize, lps); 

    unsigned concurentThreadsSupported = std::thread::hardware_concurrency();
    int part = (tSize-pSize+1)/concurentThreadsSupported + pSize - 1 ;  
    std::vector<int> positions;

    #pragma omp parallel for
    for(size_t i = 0; i < concurentThreadsSupported; i++) {
        KMPSearch(pat, txt, lps, i*part, (i+1)*part, positions);
    }

    #pragma omp parallel for
    for(size_t i = 1; i < concurentThreadsSupported; i++) {
        if(i*part+(pSize-1) > tSize)
            KMPSearch(pat, txt, lps, i*part-(pSize-1), tSize, positions);
        else
            KMPSearch(pat, txt, lps, i*part-(pSize-1), i*part+(pSize-1), positions);
    }

    if(!positions.size()){
        std::cout << -1;
        return 0;
    }

    sort(positions.begin(), positions.end());

    for(auto &elem : positions){
        std::cout << "Pattern matched at " << elem << " position: ";
        //colored output
        for(int i = 0; i < tSize; ++i)
        	if(i == elem){
        		std::cout << "\033[0;32m" << pat << "\033[0m";
        		i += pSize-1;
        	}
        	else
        		std::cout << txt[i];
        std::cout << std::endl;
    }
     
    return 0; 
} 