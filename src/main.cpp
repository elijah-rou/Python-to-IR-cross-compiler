#include "irtree.h"
#include "munch.h"
#include <stdexcept>

/*
    Create an IR Tree using a stack
*/
elijahrou::IRTree * stackTree(std::istream & stream){
    std::deque<elijahrou::IRTree *> stack;
    std::string line;
    if(stream){
        while(getline(stream, line)){
            int pos = line.find_last_of('=') + 1;
            line = line.substr(pos);
            while(stack.size() > (elijahrou::uint)pos){
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
elijahrou::IRTree * readIR(std::string filename){
    std::ifstream input(filename);
    return stackTree(input); 
};

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

/*
    Run Specified file
*/
void run(std::string filename){
    system(("python3 " + filename).c_str());
}

int main(int argc, char ** argv ){
    using namespace elijahrou;
    if (argc != 1){
        std::cout << "Welcome to the IR-Python3 cross-compiler!" << std::endl << std::endl;
        try{
            // Create trees for pattern recognition
            std::cout << "Generating loop patterns..." << std::endl;
            IRTree * loopIRTemp = readIR("patterns/loopTreeTemp.ir");
            IRTree * loopIRConst = readIR("patterns/loopTreeConst.ir");
            IRTree * loopIRName = readIR("patterns/loopTreeName.ir");
            IRTree * loopIRCall = readIR("patterns/loopTreeCall.ir");
            // Push patterns to a vector
            std::vector<IRTree *> patternTrees;
            patternTrees.push_back(loopIRTemp);
            patternTrees.push_back(loopIRConst);
            patternTrees.push_back(loopIRName);
            patternTrees.push_back(loopIRCall);
            std::cout << "DONE." << std::endl;

            // Create tiles for python compilation
            std::cout<< "Creating tiles..." << std::endl;
            std::map<std::string, IRTree *> tiles;
            /*
                LOOPS
            */
            // For loop
            tiles.insert(std::pair<std::string, IRTree *>("loop_for", readIR("tiles/loop_for.ir")));
            // While loop
            tiles.insert(std::pair<std::string, IRTree *>("loop_while", readIR("tiles/loop_while.ir")));
            /*
                IF
            */
            //tiles.insert(std::pair<std::string, IRTree *>("if_then", readIR("tiles/if_then.ir")));
            tiles.insert(std::pair<std::string, IRTree *>("if_else", readIR("tiles/if_else.ir")));
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
            tiles.insert(std::pair<std::string, IRTree *>("load_right", readIR("tiles/load_right.ir")));
            tiles.insert(std::pair<std::string, IRTree *>("load_left", readIR("tiles/load_left.ir")));
            std::cout << "DONE." << std::endl;

            /*
                Fetch input IR
            */
            std::cout << "Reading IR file into tree..." << std::endl;
            std::string inputFile(argv[1]);
            IRTree * irt = readIR(inputFile);
            std::cout << "DONE" << std::endl << std::endl;
            std::string prompt;

            /*
                Optmise?
            */
            std::cout << "Would you like to optimise the IR? [(y)es/(n)o, default n]" << std::endl;
            std::cin >> prompt;
            if(prompt[0] == 'y'){
                irt->optimise(patternTrees);
                irt->output();
            }
            prompt = "";
            std::cout << std::endl;

            // Cross compile
            std::string pyCode = munch(irt, tiles);

            std::cout << "Produced:\n" + pyCode + "\n" << std::endl;

            // Save as py file
            std::cout << "Enter a file name. [without the extenstion]" << std::endl;
            std::cin >> prompt;
            std::string filename;
            if(prompt == ""){
                filename = "output/default.py";
            }
            else{
                filename = "output/"+prompt+".py";
            }
            createFile(filename, pyCode);
            prompt = "";
            std::cout << std::endl;

            std::cout << "Would you like to run the Python3 code? [(y)es/(n)o, default n]" << std::endl;
            std::cin >> prompt;
            std::cout << std::endl;
            if(prompt[0] == 'y'){
                std::cout << "Running " << filename << std::endl;
                run(filename);
            }
            /*
                Clean-Up
            */
            delete irt;
            for(elijahrou::IRTree * tree : patternTrees){
                delete tree;
            }
            for(auto & tree : tiles){
                delete tree.second;
            }
        }
        catch(std::exception & e){
            std::cout << "IR-Python3 CC: Missing Libraries." << std::endl;
        }
    }
    else{
        std::cout << "IR-Python3 CC: Invalid amount of arguments." << std::endl;
    }

}