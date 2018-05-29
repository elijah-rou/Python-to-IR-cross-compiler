#include "munch.h"
#include <stdexcept>

/*
    Run Specified file
*/
void run(std::string filename){
    system(("python3 " + filename).c_str());
}
namespace elijahrou{

}
int main(int argc, char ** argv ){
    using namespace elijahrou;
    if (argc != 1){
        std::cout << "Welcome to the IR-Python3 cross-compiler!" << std::endl << std::endl;
        try{
            // Create trees for pattern recognition
            std::vector<IRTree *> * patternTrees = genPatterns(); 
            std::cout << "DONE." << std::endl;

            // Create tiles for python compilation
            std::cout << "Creating tiles..." << std::endl;
            std::map<std::string, IRTree *> * tiles = genTiles();
            std::cout << "DONE>" << std::endl;

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
            /*
            std::cout << "Would you like to optimise the IR? [(y)es/(n)o, default n]" << std::endl;
            std::cin >> prompt;
            if(prompt[0] == 'y'){
                irt->optimise(*patternTrees);
                irt->output();
            }
            prompt = "";
            std::cout << std::endl;
            */
            
            // Cross compile
            std::string pyCode = munch(irt, tiles, 0);

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
            

            /////
            // TEST
            /////
            /*
            IRTree * test = readIR("tiles/if_else.ir");
            test->output();
            irt->output();
            std::cout << irt->innerEqual(*test) << std::endl;
            */
            /////
            // TEST
            /////

            /*
                Clean-Up
            */
            delete irt;
            for(elijahrou::IRTree * tree : *patternTrees){
                delete tree;
            }
            delete patternTrees;
            for(auto & tree : *tiles){
                delete tree.second;
            }
            delete tiles;
        }
        catch(std::exception & e){
            std::cout << "IR-Python3 CC: Missing Libraries." << std::endl;
        }
    }
    else{
        std::cout << "IR-Python3 CC: Invalid amount of arguments." << std::endl;
    }

}