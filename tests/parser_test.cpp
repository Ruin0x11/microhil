#include "hil/hil.hpp"

#include "thirdparty/catch2/catch.hpp"
#include <map>
#include <istream>
#include <sstream>
#include <string>

static hil::Context parse(const std::string& s)
{
    std::stringstream ss(s);
    hil::internal::Parser p(ss);

    hil::Context c = p.parse();
    if (p.errorReason().size() != 0) {
        std::cerr << s << std::endl;
        std::cerr << p.errorReason() << std::endl;
    }
    REQUIRE(c.valid());
    return c;
}

TEST_CASE("parse empty")
{
    hil::Context c = parse("");

    REQUIRE(1UL == c.textParts.size());
    REQUIRE(0UL == c.hilParts.size());
}

TEST_CASE("parse single")
{
    hil::Context c = parse("foo");

    REQUIRE(1UL == c.textParts.size());
    REQUIRE(0UL == c.hilParts.size());
    REQUIRE("foo" == c.textParts.at(0));
}

TEST_CASE("parse dollar")
{
    hil::Context c = parse("foo$foo$foo");

    REQUIRE(1UL == c.textParts.size());
    REQUIRE(0UL == c.hilParts.size());
    REQUIRE("foo$foo$foo" == c.textParts.at(0));
}

TEST_CASE("parse ident")
{
    hil::Context c = parse("${hoge}");

    REQUIRE(2UL == c.textParts.size());
    REQUIRE(1UL == c.hilParts.size());
    REQUIRE("" == c.textParts.at(0));
    REQUIRE("hoge" == c.hilParts.at(0).as<std::string>());
    REQUIRE("" == c.textParts.at(1));
}

TEST_CASE("parse function call")
{
    hil::Context c = parse("${fuga(hoge)}");

    REQUIRE(2UL == c.textParts.size());
    REQUIRE(1UL == c.hilParts.size());

    hil::FunctionCall func = c.hilParts.at(0).as<hil::FunctionCall>();
    REQUIRE("fuga" == func.name);
    REQUIRE(1UL == func.args.size());
    REQUIRE("hoge" == func.args.at(0).as<std::string>());
}

TEST_CASE("parse multi-argument function call")
{
    hil::Context c = parse("${fuga(hoge,piyo, hogera)}");

    REQUIRE(2UL == c.textParts.size());
    REQUIRE(1UL == c.hilParts.size());

    hil::FunctionCall func = c.hilParts.at(0).as<hil::FunctionCall>();
    REQUIRE("fuga" == func.name);
    REQUIRE(3UL == func.args.size());
    REQUIRE("hoge" == func.args.at(0).as<std::string>());
    REQUIRE("piyo" == func.args.at(1).as<std::string>());
    REQUIRE("hogera" == func.args.at(2).as<std::string>());
}

TEST_CASE("parse multiple items")
{
    hil::Context c = parse("doods${foo}    ${fuga(hoge,piyo, hogera)} asd${bar}");

    REQUIRE(4UL == c.textParts.size());
    REQUIRE(3UL == c.hilParts.size());

    REQUIRE("doods" == c.textParts.at(0));
    REQUIRE("    " == c.textParts.at(1));
    REQUIRE(" asd" == c.textParts.at(2));
    REQUIRE("" == c.textParts.at(3));

    REQUIRE("foo" == c.hilParts.at(0).as<std::string>());

    hil::FunctionCall func = c.hilParts.at(1).as<hil::FunctionCall>();
    REQUIRE("fuga" == func.name);
    REQUIRE(3UL == func.args.size());
    REQUIRE("hoge" == func.args.at(0).as<std::string>());
    REQUIRE("piyo" == func.args.at(1).as<std::string>());
    REQUIRE("hogera" == func.args.at(2).as<std::string>());

    REQUIRE("bar" == c.hilParts.at(2).as<std::string>());
}

TEST_CASE("parse period")
{
    hil::Context c = parse("foo${hoge}.");

    REQUIRE(2UL == c.textParts.size());
    REQUIRE(1UL == c.hilParts.size());
    REQUIRE("foo" == c.textParts.at(0));
    REQUIRE("hoge" == c.hilParts.at(0).as<std::string>());
    REQUIRE("." == c.textParts.at(1));
}

TEST_CASE("parse comma")
{
    hil::Context c = parse("foo${hoge},");

    REQUIRE(2UL == c.textParts.size());
    REQUIRE(1UL == c.hilParts.size());
    REQUIRE("foo" == c.textParts.at(0));
    REQUIRE("hoge" == c.hilParts.at(0).as<std::string>());
    REQUIRE("," == c.textParts.at(1));
}
