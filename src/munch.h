#include "irtree.h"
#include <map>

namespace elijahrou{
    // Declare munch
    std::string munch(IRTree *, const std::map<std::string, IRTree *>);

    /*
        Loops
    */
    std::string loop_for(IRTree * current, const std::map<std::string, IRTree *> tiles){
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

        return "for "+boundVar+" in range("+lowerBound+", "+upperBound+", "+increment+"):\n    "+munch(body, tiles);
    }
    std::string loop_while(IRTree * current, const std::map<std::string, IRTree *> tiles){
        IRTree * body = current->children.at(0)->children.at(0)->children.at(1);
        IRTree * expr = current->children.at(0)->children.at(0)->children.at(0)->children.at(0)->children.at(1)->children.at(0);
        return "while("+munch(expr, tiles)+"):\n    "+munch(body, tiles);
    }

    /*
        Control
    */
    std::string if_then(IRTree * current, const std::map<std::string, IRTree *> tiles){
        
    }
    std::string if_else(IRTree * current, const std::map<std::string, IRTree *> tiles){
        IRTree * ifBody = current->children.at(0)->children.at(1);
        IRTree * elseBody = current->children.at(0)->children.at(0)->children.at(0)->children.at(0)->children.at(1);
        IRTree * expr = current->children.at(0)->children.at(0)->children.at(0)->children.at(0)->children.at(0)->children.at(0)->children.at(0); 
        return "if ("+munch(expr, tiles)+"):\n   "+munch(ifBody, tiles)+"\nelse:\n    "+munch(elseBody, tiles);
    }
    /*
        IR Calls
    */
    std::string call_input(IRTree * current){
        return "eval(input())";
    }
    std::string call_print(IRTree * current, const std::map<std::string, IRTree *> tiles){
        return "print(" + munch(current->children.at(1), tiles) + ")";
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
    std::string store_right(IRTree * current, const std::map<std::string, IRTree *> tiles){
        std::string variable = current->children.at(0)->children.at(0)->children.at(1)->children.at(0)->node;
        return variable + " = " + munch(current->children.at(1), tiles);
    }

    std::string store_left(IRTree * current, const std::map<std::string, IRTree *> tiles){
        std::string variable = current->children.at(0)->children.at(0)->children.at(0)->children.at(0)->node;
        return variable + " = " + munch(current->children.at(1), tiles);
    }
    std::string store_e(IRTree * current, const std::map<std::string, IRTree *> tiles){
        return current->children.at(0)->children.at(0)->node + munch(current->children.at(1), tiles);
    }

    /*
        IR Loads
    */
    std::string load_right(IRTree * current, const std::map<std::string, IRTree *> tiles){
        return current->children.at(0)->children.at(1)->children.at(0)->node;
    }
    std::string load_left(IRTree * current, const std::map<std::string, IRTree *> tiles){
        return current->children.at(0)->children.at(0)->children.at(0)->node;
    }

    std::string munch(IRTree * current, const std::map<std::string, IRTree *> tiles){
        /*
            In order from biggest to smallest
        */
        /*
            SIZE = 26
        */
        // For loop
        if(current->innerEqual(*tiles.find("loop_for")->second)){
            return loop_for(current, tiles);
        }
        /*
            SIZE = 11
        */
        else if(current->innerEqual(*tiles.find("loop_while")->second)){
            return loop_while(current, tiles);
        }
        else if(current->innerEqual(*tiles.find("if_else")->second)){
            return if_else(current, tiles);
        }
        /*
            SIZE = 7 
        */
        // Input call with a right store
        else if(current->innerEqual(*tiles.find("call_store_input_right")->second)){
            return call_store_input_right(current);
        }
        // Input call with a left store
        else if(current->innerEqual(*tiles.find("call_store_input_left")->second)){
            return call_store_input_left(current);
        }

        /*
            SIZE = 4
        */
        // Store a variable that's on the right subtree
        else if(current->innerEqual(*tiles.find("store_right")->second)){
            return store_right(current, tiles);
        }
        // Store a variable that's on the left subtree
        else if(current->innerEqual(*tiles.find("store_left")->second)){
            return store_left(current, tiles);
        }

        /*
            SIZE = 3
        */
        // Load a variable that's on the right subtree
        else if(current->innerEqual(*tiles.find("load_right")->second)){
            return load_right(current, tiles);
        }
        // Load a varaible that's on the left subtree
        else if(current->innerEqual(*tiles.find("load_left")->second)){
            return load_left(current, tiles);
        }

        /*
            SIZE = 2
        */
        // Short input call - This first or inner equal will fail
        else if(*current == *tiles.find("call_input")->second){
            return call_input(current);
        } 
        // Print call
        else if(current->innerEqual(*tiles.find("call_print")->second)){
            return call_print(current, tiles);
        }
        
        
        /*
            Trivial checks
        */
        else if(current->node == "SEQ"){
            return munch(current->children.at(0), tiles) + "\n" + munch(current->children.at(1), tiles);
        }
        else if(current->node == "+"){
            return munch(current->children.at(0), tiles) + "+" + munch(current->children.at(1), tiles);
        }
        else if(current->node == "-"){
            return munch(current->children.at(0), tiles) + "-" + munch(current->children.at(1), tiles);
        }
        else if(current->node == "/"){
            return munch(current->children.at(0), tiles) + "/" + munch(current->children.at(1), tiles);
        }
        else if(current->node == "*"){
            return munch(current->children.at(0), tiles) + "*" + munch(current->children.at(1), tiles);
        }
        else if(current->node == "=="){
            return munch(current->children.at(0), tiles) + "==" + munch(current->children.at(1), tiles);
        }
        else if(current->node == ">"){
            return munch(current->children.at(0), tiles) + ">" + munch(current->children.at(1), tiles);
        }
        else if(current->node == "<"){
            return munch(current->children.at(0), tiles) + "<" + munch(current->children.at(1), tiles);
        }
        else if(current->node == "!="){
            return munch(current->children.at(0), tiles) + "!=" + munch(current->children.at(1), tiles);
        }
        else if(current->node == "<="){
            return munch(current->children.at(0), tiles) + "<=" + munch(current->children.at(1), tiles);
        }
        else if(current->node == ">="){
            return munch(current->children.at(0), tiles) + ">=" + munch(current->children.at(1), tiles);
        }
        // Return the node if nothing else - assumed CONST->node
        return current->children.at(0)->node;
    }

    
}