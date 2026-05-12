#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <string>

TEST_CASE("Hello world") {
    std::string v = "Hello, world";
    CHECK(v == "Hello, world");
}
