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

}
TEST_CASE("Stores"){

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

}
TEST_CASE("Loops"){

}
TEST_CASE("Composites"){

}