#include "irtree.h"
namespace elijahrou{

    /*
        Output IR Representation
    */
    void IRTree::output(void){
        for(int i=0; i<this->level; i++){
            std::cout << "=";
        }
        std::cout << this->node << std::endl;
        for(IRTree * tree : children){
            tree->output();
        }
    }

    /* 
        Optimise the IR Tree
    */
    void IRTree::optimise(std::vector<IRTree *> patterns){
        while(!this->optimised){
            IRTree * irt = new IRTree(*this);
            this->constantFold();
            this->loopUnroll(patterns);
            if(*this == *irt){
                this->optimised = true;
            }
            delete irt;
        }
    }

    /*
        Apply the constant folding optimisation
    */
    void IRTree::constantFold(void){
        std::string op = this->node;
        if(op == "+" || op == "-" || op == "/" || op == "*"){
            IRTree * child_1 = this->children.at(0);
            IRTree * child_2 = this->children.at(1);
            if(child_1->node == "CONST" && child_2->node == "CONST"){
                int num_1 = stoi(child_1->children.at(0)->node);
                int num_2 = stoi(child_2->children.at(0)->node);
                this->node = "CONST";
                if(op == "+"){
                    this->children.at(0) = new IRTree(std::to_string(num_1+num_2), this->level+1);
                }
                else if(op == "-"){
                    this->children.at(0) = new IRTree(std::to_string(num_1-num_2), this->level+1);
                }
                else if(op == "*"){
                    this->children.at(0) = new IRTree(std::to_string(num_1*num_2), this->level+1);
                }
                else{
                    this->children.at(0) = new IRTree(std::to_string(num_1/num_2), this->level+1);
                }
                delete this->children.at(1);
                this->children.pop_back();
            }
        }
        for(IRTree * irt : this->children){
            irt->constantFold(); 
        }
    }

    /* 
        Apply the loop unroll optimisation
    */
    void IRTree::loopUnroll(std::vector<IRTree *> loopPatterns){
        for(IRTree * pattern : loopPatterns){
            if(this->innerEqual(*pattern)){
                rollInTheDeep();
                break;
            }
            for(IRTree * irt :children){
                irt->loopUnroll(loopPatterns);
            }
        }
    }

    /*
        Create the subtree for loop unrolling
    */
    void IRTree::rollInTheDeep(){
        // Increment is 2 seq's down and all the way to the right
        int increment = stoi(this->children.at(0)->children.at(0)->children.at(1)->children.at(1)->children.at(1)->children.at(0)->node);

        // Loop body is 3 seq's down and to the right
        IRTree * body = this->children.at(0)->children.at(0)->children.at(0)->children.at(1);

        // Pointer to the last seq node
        IRTree * lastSeq = this->children.at(0)->children.at(0)->children.at(0)->children.at(0)->children.at(0)->children.at(0)->children.at(0);

        // Bound variable name is all the way to the left of lastSeq
        std::string boundVar = lastSeq->children.at(0)->children.at(0)->children.at(0)->node;

        // Lower Bound val is left subtree and all the way to the right
        int lowerBound = stoi(lastSeq->children.at(0)->children.at(1)->children.at(0)->node); 

        // Upper Bound val is right subtree and all the way to the right
        int upperBound = stoi(lastSeq->children.at(1)->children.at(1)->children.at(0)->node);

        // Current lowest level of new tree
        int lowLevel = this->level + 3 + (upperBound-lowerBound)/increment;
        
        // Create initial seq node
        IRTree * upperSeq = new IRTree("SEQ", lowLevel-4);
        upperSeq->children.push_back(createSeq(body, boundVar, lowLevel, lowerBound));

        // Create further seq's
        for(int i = lowerBound + increment; i <= upperBound; i += increment){
            upperSeq->children.push_back(createSeq(body, boundVar, lowLevel, i));
            --lowLevel;
            IRTree * s = new IRTree("SEQ", lowLevel-4);
            s->children.push_back(upperSeq);
            upperSeq = s;
        } 

        // Set this to upperSeq 
        // Clean up
        for(IRTree * irt : this->children){
            this->children.pop_back();
            delete irt;
        }
        this->node = "SEQ";

        for(IRTree * child : upperSeq->children.at(0)->children){
            this->children.push_back(new IRTree(*child));
        }
        delete upperSeq;
    }

    /*
        Create a seq Sub Tree for loop unrolling
    */
    IRTree * IRTree::createSeq(IRTree * body, std::string boundVar, int lowLevel, int bound){
        // Move Children
        IRTree * temp = new IRTree("TEMP", lowLevel-1);
        temp->children.push_back(new IRTree(boundVar, lowLevel));

        IRTree * constant = new IRTree("CONST", lowLevel-1);
        constant->children.push_back(new IRTree(std::to_string(bound), lowLevel));

        // Move Node
        IRTree * move = new IRTree("MOVE", lowLevel-2);
        move->children.push_back(temp);
        move->children.push_back(constant);

        // Seq Node
        IRTree * seq = new IRTree("SEQ", lowLevel - 3);
        seq->children.push_back(move);
        seq->children.push_back(new IRTree(*body));

        // Set level for body
        IRTree * currentBody = seq->children.at(1); 
        currentBody->level = lowLevel - 2;
        for(IRTree * child : currentBody->children){
            child->level = lowLevel - 1;
            for(IRTree * grandChild : child->children){
                grandChild->level = lowLevel;
                for(IRTree * greatGrandChild : grandChild->children){
                    greatGrandChild->level = lowLevel + 1;
                }
            }
        }
        return seq;
    }
}