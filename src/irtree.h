#ifndef IRTREE_H 
#define IRTREE_H 

#include <vector>
#include <deque>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

namespace elijahrou{
/*
    + Class to represent an IR tree in a compiler

    + Reads an itermediate representation and creates an IR tree 
*/
    typedef unsigned int uint; 
    struct IRTree{

        std::string node;
        std::vector<IRTree *> children;
        int level;
        bool optimised;

        // Default Constructor
        IRTree(){
            this->level = 0;
            this->optimised = false;
        }

        // Destructor
        ~IRTree(){
            for(IRTree * irt : children){
                delete irt;
            }
        }
        // Copy
        IRTree(const IRTree & irt){
            this->optimised = false;
            this->node = irt.node;
            this->level = irt.level;
            for(IRTree * child : irt.children){
                this->children.push_back(new IRTree(*child));
            }

        }

        // Construtor
        IRTree(std::string node, int level){
           this->node = node;
           this->level = level;
           this->optimised = false;
        }


        /* 
            Methods
        */
        // Check for equality
        bool operator==(const IRTree & irt){

            int size = this->children.size();
            if(this->node == irt.node){
                if(size == 0){
                    return true;
                }
                if((uint)size == irt.children.size()){
                    bool res = true;
                    for(int i=0; i<size; i++){
                        res &= *this->children.at(i) == *irt.children.at(i); 
                    }
                    return res;
                }
                else{
                    return false;
                }
            }
            else{
                return false;
            }
        }

        // Check for non-equality
        bool operator!=(const IRTree & irt){
            return !(*this == irt);
        }

        // Check for equality but exclude leaves and expressions/statements
        bool innerEqual(const IRTree & irt){
            uint size = this->children.size();
            std::string currentNode = this->node;
            std::string otherNode = irt.node;
            if(currentNode == "e" || otherNode == "e"){
                return true;
            }
            if(currentNode == otherNode){
                if(currentNode == "CONST" || currentNode == "LABEL" || currentNode == "NAME" || currentNode == "TEMP" || currentNode == "CJUMP" || currentNode == "CALL" || currentNode == "JUMP"){
                    return true;
                }
                if(size == irt.children.size()){
                    bool res = true;
                    for(uint i=0; i<size; i++){
                        res &= this->children.at(i)->innerEqual(*irt.children.at(i)); 
                    }
                    return res;
                }
                else{
                    return false;
                }
            }
            else{
                return false;
            }
        }
        
        // Perform constant folding as an Optimisation
        void constantFold(void);

        // Perform loop unrolling as an optimisation
        void loopUnroll(std::vector<IRTree *>);

        // Change a loop to a series of seqs
        void rollInTheDeep();

        // Create a seq tree from parameters
        IRTree * createSeq(IRTree *, std::string, int, int);

        // Optimise IR Tree
        void optimise(std::vector<IRTree *>);

        // Output representation
        void output(void);

    };
}

#endif