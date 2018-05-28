#include "irtree.h"
#include "munch.h"

/*
    Create an IR Tree using a stack
*/
// String used to generate IRTree for a loop

const std::string loopTreeTemp = "SEQ\n=SEQ\n==SEQ\n===SEQ\n====SEQ\n=====SEQ\n======SEQ\n=======SEQ\n========MOVE\n=========TEMP\n=========CONST\n========MOVE\n=========TEMP\n=========CONST\n=======LABEL\n======CJUMP\n=====LABEL\n====MOVE\n=====TEMP\n=====+\n======TEMP\n======TEMP\n===MOVE\n====TEMP\n====+\n=====TEMP\n=====CONST\n==JUMP\n===NAME\n=LABEL\n";
const std::string loopTreeConst= "SEQ\n=SEQ\n==SEQ\n===SEQ\n====SEQ\n=====SEQ\n======SEQ\n=======SEQ\n========MOVE\n=========TEMP\n=========CONST\n========MOVE\n=========TEMP\n=========CONST\n=======LABEL\n======CJUMP\n=====LABEL\n====MOVE\n=====TEMP\n=====+\n======TEMP\n======CONST\n===MOVE\n====TEMP\n====+\n=====TEMP\n=====CONST\n==JUMP\n===NAME\n=LABEL\n";
const std::string loopTreeName = "SEQ\n=SEQ\n==SEQ\n===SEQ\n====SEQ\n=====SEQ\n======SEQ\n=======SEQ\n========MOVE\n=========TEMP\n=========CONST\n========MOVE\n=========TEMP\n=========CONST\n=======LABEL\n======CJUMP\n=====LABEL\n====MOVE\n=====TEMP\n=====+\n======TEMP\n======NAME\n===MOVE\n====TEMP\n====+\n=====TEMP\n=====CONST\n==JUMP\n===NAME\n=LABEL\n";
const std::string loopTreeCall= "SEQ\n=SEQ\n==SEQ\n===SEQ\n====SEQ\n=====SEQ\n======SEQ\n=======SEQ\n========MOVE\n=========TEMP\n=========CONST\n========MOVE\n=========TEMP\n=========CONST\n=======LABEL\n======CJUMP\n=====LABEL\n====CALL\n===MOVE\n====TEMP\n====+\n=====TEMP\n=====CONST\n==JUMP\n===NAME\n=LABEL\n";

/*
    Generate a IRTree from an input stream
*/
elijahrou::IRTree * stackTree(std::istream & stream){
    std::deque<elijahrou::IRTree *> stack;
    std::string line;
    if(stream){
        while(getline(stream, line)){
            int pos = line.find_last_of('=') + 1;
            line = line.substr(pos);
            while(stack.size() > pos){
                stack.pop_front();
            }
            elijahrou::IRTree * tree = new elijahrou::IRTree(line, stack.size());
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
elijahrou::IRTree * readIR(std::string file){
    std::ifstream input(file);
    return stackTree(input); 
};

/*
    Read from string
*/
elijahrou::IRTree * genIR(std::string str){
    std::istringstream input(str); 
    return stackTree(input);
}


int main(int argc, char ** argv ){
    using namespace elijahrou;
    if (argc != 1){
        /*
        // Create trees for pattern recognition
        IRTree * loopIRTemp = genIR(loopTreeTemp);
        IRTree * loopIRConst = genIR(loopTreeConst);
        IRTree * loopIRName = genIR(loopTreeName);
        IRTree * loopIRCall = genIR(loopTreeCall);

        // Push patterns to a vector
        std::vector<IRTree *> patternTrees;
        patternTrees.push_back(loopIRTemp);
        patternTrees.push_back(loopIRConst);
        patternTrees.push_back(loopIRName);
        patternTrees.push_back(loopIRCall);

        // Create IR from file and optimise
        //std::string inputFile(argv[1]);
        //IRTree * irt = readIR(inputFile);
        //irt->optimise(patternTrees);
        //irt->output();
        */

        // Create tiles for python compilation
        std::map<std::string, IRTree *> tiles;
        /*
            CALLS
        */
        // Input call
        tiles.insert(std::pair<std::string, IRTree *>("call_input", readIR("tiles/call_input.ir")));
        // Input right subtree store call
        tiles.insert(std::pair<std::string, IRTree *>("call_store_input_right", readIR("tiles/call_store_input_right.ir")));
        // Input left subtree store call
        tiles.insert(std::pair<std::string, IRTree *>("call_store_input_left", readIR("tiles/call_store_input_left.ir")));
        // Print call
        tiles.insert(std::pair<std::string, IRTree *>("call_print", readIR("tiles/call_print.ir")));

        /*
            STORES
        */
        // Store with no +
        tiles.insert(std::pair<std::string, IRTree *>("store_e", readIR("tiles/store_e.ir")));
        // Store right subtree
        tiles.insert(std::pair<std::string, IRTree *>("store_right", readIR("tiles/store_right.ir")));
        // Store left subtree
        tiles.insert(std::pair<std::string, IRTree *>("store_left", readIR("tiles/store_left.ir")));

        /*
            LOADS
        */
        tiles.insert(std::pair<std::string, IRTree *>("store_e", readIR("tiles/store_e.ir")));
        tiles.insert(std::pair<std::string, IRTree *>("store_e", readIR("tiles/store_e.ir")));

        /*
            Fetch input IR
        */
        std::string inputFile(argv[1]);
        IRTree * irt = readIR(inputFile);
        
        //IRTree * test = readIR("testdata/input&print.ir");
        //test->output();
        //std::string pyCode = munch(test, tiles);

        std::cout << munch(irt, tiles) << std::endl;
        /*
        delete irt;
        for(elijahrou::IRTree * tree : patternTrees){
            delete tree;
        }
        */
    }
    else{
        std::cout << "Invalid amount of arguments.";
    }

}