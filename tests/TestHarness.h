#pragma once
#include <cmath>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace test {
using Function=std::function<void()>;
struct Case{std::string name;Function fn;};
inline std::vector<Case>& registry(){static std::vector<Case> r;return r;}
struct Register{Register(std::string n,Function f){registry().push_back({std::move(n),std::move(f)});}};
inline void require(bool value,const char* expression,const char* file,int line){if(!value)throw std::runtime_error(std::string(file)+":"+std::to_string(line)+" REQUIRE("+expression+") failed");}
inline void near(double a,double b,double e,const char* file,int line){if(std::abs(a-b)>e)throw std::runtime_error(std::string(file)+":"+std::to_string(line)+" expected "+std::to_string(a)+" ~= "+std::to_string(b));}
}
#define TEST_JOIN_INNER(a,b) a##b
#define TEST_JOIN(a,b) TEST_JOIN_INNER(a,b)
#define TEST_CASE(name) static void TEST_JOIN(test_fn_,__LINE__)(); static test::Register TEST_JOIN(test_reg_,__LINE__)(name,TEST_JOIN(test_fn_,__LINE__)); static void TEST_JOIN(test_fn_,__LINE__)()
#define REQUIRE(expr) test::require((expr),#expr,__FILE__,__LINE__)
#define REQUIRE_NEAR(a,b,e) test::near((a),(b),(e),__FILE__,__LINE__)

