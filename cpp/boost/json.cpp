// compile with -lboost_json

#include <iostream>
#include <boost/json.hpp>

void test1()
{
    boost::json::object obj;
    obj["a"] = true;
    obj["b"] = "hello";
    obj["c"] = { 1, 2, 3 }; // array
    obj["d"] = { { "d1", 0 }, { "d2", 1 } }; // object
    obj["e"] = nullptr;
    std::cout << boost::json::serialize(obj) << std::endl;
    //=>{"a":true,"b":"hello","c":[1,2,3],"d":{"d1":0,"d2":1},"e":null}
}

void test2()
{
    boost::json::value v = 
    {
        { "pi", 3.141 },
        { "happy", true },
        { "name", "Boost" },
        { "nothing", nullptr },
        { "answer", 
            {
                { "everything", 42 } 
            } 
        },
        { "list", {1, 0, 2} },
        { "object", 
            {
                { "currency", "USD" },
                { "value", 42.99 }
            }
        }
    };
    std::cout << boost::json::serialize(v) << std::endl;
    //=>{"pi":3.141E0,"happy":true,"name":"Boost","nothing":null,"answer":{"everything":42},"list":[1,0,2],"object":{"currency":"USD","value":4.299E1}}
}

void test3()
{
    boost::json::value v = boost::json::parse("100");
    int num = boost::json::value_to<int>(v);

    try {
        boost::json::value v2 = boost::json::parse("Null");
    } catch (boost::wrapexcept<boost::system::system_error> e) {
        std::cout << e.what() << std::endl;
        //=>syntax error
    }

    boost::json::value v3 = boost::json::parse(R"( {"number": "100"} )");
    try {
        v3.at("Number");
    } catch (boost::wrapexcept<std::out_of_range> e) {
        std::cout << e.what() << std::endl;
        //=>out of range
    }

    try {
        num = boost::json::value_to<int>(v3.at("number"));
    } catch (boost::wrapexcept<boost::system::system_error> e) {
        std::cout << e.what() << std::endl;
        //=>not a number
    }
}


int main(int argc, char *argv[])
{
    test1();
    test2();
    test3();
}