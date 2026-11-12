#include "TestHarness.h"
int main(){int failed=0;for(const auto& c:test::registry()){try{c.fn();std::cout<<"[PASS] "<<c.name<<'\n';}catch(const std::exception&e){++failed;std::cerr<<"[FAIL] "<<c.name<<" - "<<e.what()<<'\n';}}std::cout<<test::registry().size()-failed<<"/"<<test::registry().size()<<" tests passed\n";return failed?1:0;}

