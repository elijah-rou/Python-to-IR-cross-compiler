#ifndef MUNCH_H
#define MUNCH_H

#include "pattern.h"
#include <map>

namespace elijahrou{
    // Declare munch
    std::string munch(IRTree *, std::map<std::string, IRTree *> *, int level);

    /*
        Loops
    */
    std::string loop_for(IRTree * current, std::map<std::string, IRTree *> * tiles, int level){
        // Increment is 2 seq's down and all the way to the right
        std::string increment = current->children.at(0)->children.at(0)->children.at(1)->children.at(1)->children.at(1)->children.at(0)->node;

        // Loop body is 3 seq's down and to the right
        IRTree * body = current->children.at(0)->children.at(0)->children.at(0)->children.at(1);

        // Pointer to the last seq node
        IRTree * lastSeq = current->children.at(0)->children.at(0)->children.at(0)->children.at(0)->children.at(0)->children.at(0)->children.at(0)->children.at(0)->children.at(0)->children.at(0);

        // Bound variable name is all the way to the left of lastSeq
        std::string boundVar = lastSeq->children.at(0)->children.at(0)->children.at(0)->node;

        // Lower Bound val is left subtree and all the way to the right
        std::string lowerBound = lastSeq->children.at(0)->children.at(1)->children.at(0)->node; 

        // Upper Bound val is right subtree and all the way to the right
        std::string upperBound = lastSeq->children.at(1)->children.at(1)->children.at(0)->node;

        return "for "+boundVar+" in range("+lowerBound+", "+upperBound+", "+increment+"):\n"+std::string(level+4, ' ')+munch(body, tiles, level + 4);
    }
    std::string loop_while(IRTree * current, std::map<std::string, IRTree *> * tiles, int level){
        IRTree * body = current->children.at(0)->children.at(0)->children.at(1);
        IRTree * expr = current->children.at(0)->children.at(0)->children.at(0)->children.at(0)->children.at(1)->children.at(0);
        return "while("+munch(expr, tiles, 0)+"):\n"+std::string(level+4, ' ')+munch(body, tiles, level+4);
    }

    /*
        Control
    */
    std::string if_then(IRTree * current, std::map<std::string, IRTree *> * tiles, int level){
        IRTree * body = current->children.at(0)->children.at(1);
        IRTree * expr = current->children.at(0)->children.at(0)->children.at(0)->children.at(0)->children.at(0);
        return "if("+munch(expr, tiles, 0)+"):\n"+std::string(level+4, ' ')+munch(body, tiles, level+1);
    }
    std::string if_else(IRTree * current, std::map<std::string, IRTree *> * tiles, int level){
        IRTree * ifBody = current->children.at(0)->children.at(1);
        IRTree * elseBody = current->children.at(0)->children.at(0)->children.at(0)->children.at(0)->children.at(1);
        IRTree * expr = current->children.at(0)->children.at(0)->children.at(0)->children.at(0)->children.at(0)->children.at(0)->children.at(0); 
        return "if("+munch(expr, tiles, 0)+"):\n"+std::string(level+4, ' ')+munch(ifBody, tiles, level+1)+"\n"+std::string(level, ' ')+"else:\n"+std::string(level+4, ' ')+munch(elseBody, tiles, level+1);
    }
    /*
        IR Calls
    */
    std::string call_input(IRTree * current){
        return "eval(input())";
    }
    std::string call_print(IRTree * current, std::map<std::string, IRTree *> * tiles, int level){
        return "print(" + munch(current->children.at(1), tiles, level) + ")";
    }
    std::string call_store_input_right(IRTree * current){
        std::string variable = current->children.at(0)->children.at(0)->children.at(1)->children.at(0)->node;
        return variable + " = eval(input())";
    }

    std::string call_store_input_left(IRTree * current){
        std::string variable = current->children.at(0)->children.at(0)->children.at(0)->children.at(0)->node;
        return variable + " = eval(input())";
    }


    /*
        IR Stores
    */
    std::string store_right(IRTree * current, std::map<std::string, IRTree *> * tiles, int level){
        std::string variable = current->children.at(0)->children.at(0)->children.at(1)->children.at(0)->node;
        return variable + " = " + munch(current->children.at(1), tiles, level);
    }

    std::string store_left(IRTree * current, std::map<std::string, IRTree *> * tiles, int level){
        std::string variable = current->children.at(0)->children.at(0)->children.at(0)->children.at(0)->node;
        return variable + " = " + munch(current->children.at(1), tiles, level);
    }
    std::string store_e(IRTree * current, std::map<std::string, IRTree *> * tiles, int level){
        return current->children.at(0)->children.at(0)->node + munch(current->children.at(1), tiles, level);
    }

    /*
        IR Loads
    */
    std::string load_right(IRTree * current){
        return current->children.at(0)->children.at(1)->children.at(0)->node;
    }
    std::string load_left(IRTree * current){
        return current->children.at(0)->children.at(0)->children.at(0)->node;
    }

    std::string munch(IRTree * current, std::map<std::string, IRTree *> * tiles, int level){
        /*
            In order from biggest to smallest
        */
        /*
            SIZE = 26
        */
        // For loop
        if(current->innerEqual(*tiles->find("loop_for")->second)){
            return loop_for(current, tiles, level);
        }
        /*
            SIZE = 11
        */
        else if(current->innerEqual(*tiles->find("loop_while")->second)){
            return loop_while(current, tiles, level);
        }
        else if(current->innerEqual(*tiles->find("if_else")->second)){
            return if_else(current, tiles, level);
        }
        /*
            SIZE = 8
        */
        else if(current->innerEqual(*tiles->find("if_then")->second)){
            return if_then(current, tiles, level);
        }
        /*
            SIZE = 7 
        */
        // Input call with a right store
        else if(current->innerEqual(*tiles->find("call_store_input_right")->second)){
            return call_store_input_right(current);
        }
        // Input call with a left store
        else if(current->innerEqual(*tiles->find("call_store_input_left")->second)){
            return call_store_input_left(current);
        }

        /*
            SIZE = 4
        */
        // Store a variable that's on the right subtree
        else if(current->innerEqual(*tiles->find("store_right")->second)){
            return store_right(current, tiles, level);
        }
        // Store a variable that's on the left subtree
        else if(current->innerEqual(*tiles->find("store_left")->second)){
            return store_left(current, tiles, level);
        }

        /*
            SIZE = 3
        */
        // Load a variable that's on the right subtree
        else if(current->innerEqual(*tiles->find("load_right")->second)){
            return load_right(current);
        }
        // Load a varaible that's on the left subtree
        else if(current->innerEqual(*tiles->find("load_left")->second)){
            return load_left(current);
        }

        /*
            SIZE = 2
        */
        // Short input call - This first or inner equal will fail
        else if(*current == *tiles->find("call_input")->second){
            return call_input(current);
        } 
        // Print call
        else if(current->innerEqual(*tiles->find("call_print")->second)){
            return call_print(current, tiles, level);
        }
        
        /*
            Trivial checks
        */
        else if(current->node == "SEQ"){
            return munch(current->children.at(0), tiles, level) + "\n" + std::string(level, ' ') + munch(current->children.at(1), tiles, level);
        }
        else if(current->node == "+"){
            return munch(current->children.at(0), tiles, level) + "+" + munch(current->children.at(1), tiles, level);
        }
        else if(current->node == "-"){
            return munch(current->children.at(0), tiles, level) + "-" + munch(current->children.at(1), tiles, level);
        }
        else if(current->node == "/"){
            return munch(current->children.at(0), tiles, level) + "/" + munch(current->children.at(1), tiles, level);
        }
        else if(current->node == "*"){
            return munch(current->children.at(0), tiles, level) + "*" + munch(current->children.at(1), tiles, level);
        }
        else if(current->node == "=="){
            return munch(current->children.at(0), tiles, level) + "==" + munch(current->children.at(1), tiles, level);
        }
        else if(current->node == ">"){
            return munch(current->children.at(0), tiles, level) + ">" + munch(current->children.at(1), tiles, level);
        }
        else if(current->node == "<"){
            return munch(current->children.at(0), tiles, level) + "<" + munch(current->children.at(1), tiles, level);
        }
        else if(current->node == "!="){
            return munch(current->children.at(0), tiles, level) + "!=" + munch(current->children.at(1), tiles, level);
        }
        else if(current->node == "<="){
            return munch(current->children.at(0), tiles, level) + "<=" + munch(current->children.at(1), tiles , level);
        }
        else if(current->node == ">="){
            return munch(current->children.at(0), tiles, level) + ">=" + munch(current->children.at(1), tiles, level);
        }
        // Return the node if nothing else - assumed CONST->node
        return current->children.at(0)->node;
    }

    std::map<std::string, IRTree *> * genTiles(void){
        std::map<std::string, IRTree *> * tiles = new std::map<std::string, IRTree *>();
        /*
            LOOPS
        */
        // For loop
        tiles->insert(std::pair<std::string, IRTree *>("loop_for", readIR("tiles/loop_for.ir")));
        // While loop
        tiles->insert(std::pair<std::string, IRTree *>("loop_while", readIR("tiles/loop_while.ir")));
        /*
            IF
        */
        tiles->insert(std::pair<std::string, IRTree *>("if_then", readIR("tiles/if_then.ir")));
        tiles->insert(std::pair<std::string, IRTree *>("if_else", readIR("tiles/if_else.ir")));
        /*
            CALLS
        */
        // Input call
        tiles->insert(std::pair<std::string, IRTree *>("call_input", readIR("tiles/call_input.ir")));
        // Input right subtree store call
        tiles->insert(std::pair<std::string, IRTree *>("call_store_input_right", readIR("tiles/call_store_input_right.ir")));
        // Input left subtree store call
        tiles->insert(std::pair<std::string, IRTree *>("call_store_input_left", readIR("tiles/call_store_input_left.ir")));
        // Print call
        tiles->insert(std::pair<std::string, IRTree *>("call_print", readIR("tiles/call_print.ir")));
        /*
            STORES
        */
        // Store with no +
        tiles->insert(std::pair<std::string, IRTree *>("store_e", readIR("tiles/store_e.ir")));
        // Store right subtree
        tiles->insert(std::pair<std::string, IRTree *>("store_right", readIR("tiles/store_right.ir")));
        // Store left subtree
        tiles->insert(std::pair<std::string, IRTree *>("store_left", readIR("tiles/store_left.ir")));
        /*
            LOADS
        */
        tiles->insert(std::pair<std::string, IRTree *>("load_right", readIR("tiles/load_right.ir")));
        tiles->insert(std::pair<std::string, IRTree *>("load_left", readIR("tiles/load_left.ir")));
        return tiles;
    }
    
    /*
        Create a file with the supplied code
    */
    void createFile(std::string filename, std::string file){
        std::ofstream output(filename);
        std::istringstream code(file);
        std::string line;
        if(code){
            while(getline(code, line)){
                output << line << std::endl;
            }
        }
    }
}

#endif