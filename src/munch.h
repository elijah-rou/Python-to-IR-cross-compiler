#include "irtree.h"
#include <map>

namespace elijahrou{
    // Declare munch
    std::string munch(IRTree *, const std::map<std::string, IRTree *>);
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
    std::string load_right(IRTree * current, std::map<std::string, IRTree *> tiles){
        return current->children.at(0)->children.at(1)->children.at(0)->node;
    }
    std::string load_left(IRTree * current, std::map<std::string, IRTree *> tiles){
        return current->children.at(0)->children.at(0)->children.at(0)->node;
    }

    std::string munch(IRTree * current, const std::map<std::string, IRTree *> tiles){
        /*
            In order from biggest to smallest
        */
        /*
            SIZE = 7 
        */
        // Input call with a right store
        if(current->innerEqual(*tiles.find("call_store_input_right")->second)){
            return call_store_input_right(current);
        }
        // Input call with a left store
        if(current->innerEqual(*tiles.find("call_store_input_left")->second)){
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

        // Return the node if nothing else
        return current->children.at(0)->node;
    }

    
}