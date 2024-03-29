#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "munch.h"

using namespace elijahrou;
using namespace std;

std::map<std::string, IRTree *> * tiles = genTiles();
string readFile(string filename){
    string result;
    string s;
    ifstream f(filename, fstream::in);
    while(getline(f, s, '\0')){
        result += s;
    }
    f.close();
    return result;
}

TEST_CASE("Hussein's Tests"){
    SECTION("Test 1"){
        IRTree * test = readIR("test/testdata IS/testdata1.ir");
        string result = "print(1)";
        REQUIRE(munch(test, tiles, 0) == result);
        delete test;
    }
    SECTION("Test 2"){
        IRTree * test = readIR("test/testdata IS/testdata2.ir");
        string result = "print(1+3-2)";
        REQUIRE(munch(test, tiles, 0) == result);
        delete test;
    }
    SECTION("Test 3"){
        IRTree * test = readIR("test/testdata IS/testdata3.ir");
        string result = "x = eval(input())\nprint(x+1)";
        REQUIRE(munch(test, tiles, 0) == result);
        delete test;
    }
    SECTION("Test 4"){
        IRTree * test = readIR("test/testdata IS/testdata4.ir");
        string result = "x = eval(input())\nif(x):\n    print(2222)\nelse:\n    print(1111)";
        REQUIRE(munch(test, tiles, 0) == result);
        delete test;
    }
}
TEST_CASE("Loads"){
    SECTION("Simple Load"){
        IRTree * test = readIR("test/loads/load_s.ir");
        string result = "x";
        REQUIRE(munch(test, tiles, 0) == result);
        delete test;
    }
    SECTION("Load left"){
        IRTree * test = readIR("test/loads/load_l.ir");
        string result = "x";
        REQUIRE(munch(test, tiles, 0) == result);
        delete test;
    }
    SECTION("Load right"){
        IRTree * test = readIR("test/loads/load_r.ir");
        string result = "x";
        REQUIRE(munch(test, tiles, 0) == result);
        delete test;
    }
}
TEST_CASE("Stores"){
    SECTION("Simple store"){
        IRTree * test = readIR("test/stores/store_s.ir");
        string result = "x = 1+2";
        REQUIRE(munch(test, tiles, 0) == result);
        delete test;
    }
    SECTION("Store left"){
        IRTree * test = readIR("test/stores/store_l.ir");
        string result = "z = 1>3";
        REQUIRE(munch(test, tiles, 0) == result);
        delete test;
    }
    SECTION("Store right"){
        IRTree * test = readIR("test/stores/store_r.ir");
        string result = "y = 1-1";
        REQUIRE(munch(test, tiles, 0) == result);
        delete test;
    }
}
TEST_CASE("Calls"){
    SECTION("Input & Print"){
        IRTree * test = readIR("test/calls/input&print.ir");
        string result = readFile("test/calls/input&print.py"); 
        REQUIRE(munch(test, tiles, 0) == result);
        delete test;
    }
    SECTION("Input then print"){
        IRTree * test = readIR("test/calls/inputThenPrint.ir");
        string result = readFile("test/calls/inputThenPrint.py"); 
        REQUIRE(munch(test, tiles, 0) == result);
        delete test;
    }
    SECTION("Call store input left"){
        IRTree * test = readIR("test/calls/inputLeft.ir");
        string result = readFile("test/calls/inputLeft.py"); 
        REQUIRE(munch(test, tiles, 0) == result);
        delete test;
    }
}
TEST_CASE("Control"){
    SECTION("If simple"){
        IRTree * test = readIR("test/control/ifsimple.ir");
        string result = "if(x):\n    print(x)";
        REQUIRE(munch(test, tiles, 0) == result);
    }
    
    SECTION("If else"){
        IRTree * test = readIR("test/control/ifElsePrint.ir");
        string result = readFile("test/control/ifElsePrint.py"); 
        REQUIRE(munch(test, tiles, 0) == result);
        delete test;
    }
}
TEST_CASE("Loops"){
    SECTION("While"){
        IRTree * test = readIR("test/loops/while.ir");
        string result = readFile("test/loops/while.py"); 
        REQUIRE(munch(test, tiles, 0) == result);
        delete test;
    }
    SECTION("For"){
        IRTree * test = readIR("test/loops/for.ir");
        string result = readFile("test/loops/for.py"); 
        REQUIRE(munch(test, tiles, 0) == result);
        delete test;
    }
}
TEST_CASE("Composites"){
    SECTION("If Print"){
        IRTree * test = readIR("test/composites/ifPrint.ir");
        string result = readFile("test/composites/ifPrint.py"); 
        REQUIRE(munch(test, tiles, 0) == result);
        delete test;
    }
    SECTION("Big if-else"){
        IRTree * test = readIR("test/composites/big_if_else.ir");
        string result = readFile("test/composites/big_if_else.py"); 
        REQUIRE(munch(test, tiles, 0) == result);
        delete test;
    }
    SECTION("Big while"){
        IRTree * test = readIR("test/composites/bigwhile.ir");
        string result = readFile("test/composites/bigwhile.py"); 
        REQUIRE(munch(test, tiles, 0) == result);
        delete test;
    }SECTION("Insane"){
        IRTree * test = readIR("test/composites/insane.ir");
        string result = readFile("test/composites/insane.py"); 
        REQUIRE(munch(test, tiles, 0) == result);
        delete test;
    }
}