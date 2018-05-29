#ifndef PATTERN_H
#define PATTERN_H
#include "irtree.h"

namespace elijahrou{
    /*
    Create an IR Tree using a stack
    */
    IRTree * stackTree(std::istream & stream){
        std::deque<IRTree *> stack;
        std::string line;
        if(stream){
            while(getline(stream, line)){
                uint pos = line.find_last_of('=') + 1;
                line = line.substr(pos);
                while(stack.size() > pos){
                    stack.pop_front();
                }
                elijahrou::IRTree * tree = new IRTree(line, stack.size());
                if(stack.size() > 0){
                    stack.front()->children.push_back(tree);
                }
                stack.push_front(tree);
            }

        }
        return stack.back();
    };

    /*
        Read from file
    */
    IRTree * readIR(std::string filename){
        std::ifstream input(filename);
        return stackTree(input); 
    };

    std::vector<IRTree *> * genPatterns(void){
        // Create trees for pattern recognition
        std::cout << "Generating loop patterns..." << std::endl;
        IRTree * loopIRTemp = readIR("patterns/loopTreeTemp.ir");
        IRTree * loopIRConst = readIR("patterns/loopTreeConst.ir");
        IRTree * loopIRName = readIR("patterns/loopTreeName.ir");
        IRTree * loopIRCall = readIR("patterns/loopTreeCall.ir");
        // Push patterns to a vector
        std::vector<IRTree *> * patternTrees = new std::vector<IRTree *>();
        patternTrees->push_back(loopIRTemp);
        patternTrees->push_back(loopIRConst);
        patternTrees->push_back(loopIRName);
        patternTrees->push_back(loopIRCall);
        return patternTrees;
    }
    
}

#endif