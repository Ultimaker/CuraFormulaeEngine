#include "cura-formulae-engine/cura-formulae-engine.h"

#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <numbers>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

using namespace std::string_view_literals;
using namespace CuraFormulaeEngine::ast;

TEST_CASE("constant single digit", "[parser, constant]")
{
    auto input = "1"sv;
    const auto expected_ast = make_expr_ptr<IntExpr>(1);
    const auto expected_eval = CuraFormulaeEngine::eval::Value(int64_t(1));

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("constant multi digit", "[parser, constant]")
{
    auto input = "12345"sv;
    const auto expected_ast = make_expr_ptr<IntExpr>(12345);
    const auto expected_eval = CuraFormulaeEngine::eval::Value(int64_t(12345));

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("float", "[parser, constant]")
{
    auto input = "3.14"sv;
    const auto expected_ast = make_expr_ptr<FloatExpr>(3.14);
    const auto expected_eval = CuraFormulaeEngine::eval::Value(3.14);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("float no integer", "[parser, constant]")
{
    auto input = ".0"sv;
    const auto expected_ast = make_expr_ptr<FloatExpr>(0.0);
    const auto expected_eval = CuraFormulaeEngine::eval::Value(0.0);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("float no decimal", "[parser, constant]")
{
    auto input = "0."sv;
    const auto expected_ast = make_expr_ptr<FloatExpr>(0.0);
    const auto expected_eval = CuraFormulaeEngine::eval::Value(0.0);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("int scientific", "[.][parser, constant]")
{
    auto input = "3e10"sv;
    const auto expected_ast = make_expr_ptr<FloatExpr>(3e10);
    const auto expected_eval = CuraFormulaeEngine::eval::Value(3e10);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("int scientific plus", "[.][parser, constant]")
{
    auto input = "3e+10"sv;
    const auto expected_ast = make_expr_ptr<FloatExpr>(3e+10);
    const auto expected_eval = CuraFormulaeEngine::eval::Value(3e+10);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("int scientific minus", "[.][parser, constant]")
{
    auto input = "3e-10"sv;
    const auto expected_ast = make_expr_ptr<FloatExpr>(3e-10);
    const auto expected_eval = CuraFormulaeEngine::eval::Value(3e-10);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("float scientific", "[.][parser, constant]")
{
    auto input = "2.7e10"sv;
    const auto expected_ast = make_expr_ptr<FloatExpr>(2.7e10);
    const auto expected_eval = CuraFormulaeEngine::eval::Value(2.7e10);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("float scientific plus", "[.][parser, constant]")
{
    auto input = "2.7e+10"sv;
    const auto expected_ast = make_expr_ptr<FloatExpr>(2.7e+10);
    const auto expected_eval = CuraFormulaeEngine::eval::Value(2.7e+10);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("float scientific minus", "[.][parser, constant]")
{
    auto input = "2.7e-10"sv;
    const auto expected_ast = make_expr_ptr<FloatExpr>(2.7e-10);
    const auto expected_eval = CuraFormulaeEngine::eval::Value(2.7e-10);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("constant boolean True", "[parser, constant]")
{
    auto input = "True"sv;
    const auto expected_ast = make_expr_ptr<BoolExpr>(true);
    const auto expected_eval = CuraFormulaeEngine::eval::Value(true);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("constant boolean", "[parser, constant]")
{
    auto input = "False"sv;
    const auto expected_ast = make_expr_ptr<BoolExpr>(false);
    const auto expected_eval = CuraFormulaeEngine::eval::Value(false);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("negate int", "[parser, neg]")
{
    auto input = "-1"sv;
    const auto expected_ast = -make_expr_ptr<IntExpr>(1);
    const auto expected_eval = CuraFormulaeEngine::eval::Value(int64_t(-1));

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("negate float", "[parser, neg]")
{
    auto input = "-1.0"sv;
    const auto expected_ast = -make_expr_ptr<FloatExpr>(1.0);
    const auto expected_eval = CuraFormulaeEngine::eval::Value(-1.0);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("negate bool false", "[parser, neg]")
{
    auto input = "-False"sv;
    const auto expected_ast = -make_expr_ptr<BoolExpr>(false);
    const auto expected_eval = CuraFormulaeEngine::eval::Value(int64_t(0));

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("negate bool true", "[parser, neg]")
{
    auto input = "-[]"sv;
    const auto expected_ast = -make_list_expr();
    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE_FALSE(eval.has_value());
}

TEST_CASE("inverse int 1", "[parser, not]")
{
    auto input = "not 1"sv;
    const auto expected_ast = !make_expr_ptr<IntExpr>(1);
    const auto expected_eval = CuraFormulaeEngine::eval::Value(false);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("inverse float 1", "[parser, not]")
{
    auto input = "not 1.0"sv;
    const auto expected_ast = !make_expr_ptr<FloatExpr>(1.0);
    const auto expected_eval = CuraFormulaeEngine::eval::Value(false);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("inverse int 0", "[parser, not]")
{
    auto input = "not 0"sv;
    const auto expected_ast = !make_expr_ptr<IntExpr>(0);
    const auto expected_eval = CuraFormulaeEngine::eval::Value(true);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("inverse float 0", "[parser, not]")
{
    auto input = "not 0.0"sv;
    const auto expected_ast = !make_expr_ptr<FloatExpr>(0.0);
    const auto expected_eval = CuraFormulaeEngine::eval::Value(true);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("inverse bool false", "[parser, not]")
{
    auto input = "not False"sv;
    const auto expected_ast = !make_expr_ptr<BoolExpr>(false);
    const auto expected_eval = CuraFormulaeEngine::eval::Value(true);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("inverse bool true", "[parser, not]")
{
    auto input = "not True"sv;
    const auto expected_ast = !make_expr_ptr<BoolExpr>(true);
    const auto expected_eval = CuraFormulaeEngine::eval::Value(false);
    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();

    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("inverse empty string", "[parser, not]")
{
    auto input = "not \"\""sv;
    const auto expected_ast = !make_expr_ptr<StringExpr>("");
    const auto expected_eval = CuraFormulaeEngine::eval::Value(true);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("inverse string", "[parser, not]")
{
    auto input = "not \"foo\""sv;
    const auto expected_ast = !make_expr_ptr<StringExpr>("foo");
    const auto expected_eval = CuraFormulaeEngine::eval::Value(false);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("inverse bool empty list", "[parser, not]")
{
    auto input = "not []"sv;
    const auto expected_ast = !make_list_expr();
    const auto expected_eval = CuraFormulaeEngine::eval::Value(true);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("inverse bool list", "[parser, not]")
{
    auto input = "not [1]"sv;
    const auto expected_ast = !make_list_expr(make_expr_ptr<IntExpr>(int64_t(1)));
    const auto expected_eval = CuraFormulaeEngine::eval::Value(false);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("member empty list", "[parser, member]")
{
    auto input = "1 in []"sv;

    std::vector<ExprPtr> numbers;
    numbers.push_back(make_expr_ptr<IntExpr>(int64_t(1)));
    numbers.push_back(make_list_expr());
    const auto expected_ast = make_expr_ptr<ComparisonChainExpr>(std::move(numbers), std::vector{ComparisonOperators::Member});

    const auto expected_eval = CuraFormulaeEngine::eval::Value(false);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("member list", "[parser, member]")
{
    auto input = "1 in [1]"sv;

    std::vector<ExprPtr> expressions;
    expressions.push_back(make_expr_ptr<IntExpr>(int64_t(1)));
    std::vector<ExprPtr> numbers;
    numbers.push_back(make_expr_ptr<IntExpr>(int64_t(1)));
    expressions.push_back(make_list_expr(make_expr_ptr<IntExpr>(int64_t(1))));

    std::vector<ComparisonOperators> operators = {ComparisonOperators::Member};

    const auto expected_ast = make_expr_ptr<ComparisonChainExpr>(std::move(expressions), std::move(operators));

    const auto expected_eval = CuraFormulaeEngine::eval::Value(true);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("not member empty list", "[parser, member]")
{
    auto input = "1 not in []"sv;

    std::vector<ExprPtr> numbers;
    numbers.push_back(make_expr_ptr<IntExpr>(int64_t(1)));

    numbers.push_back(make_list_expr());

    const auto expected_ast = make_expr_ptr<ComparisonChainExpr>(std::move(numbers), std::vector{ComparisonOperators::NotMember});

    const auto expected_eval = CuraFormulaeEngine::eval::Value(true);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("not member list", "[parser, member]")
{
    auto input = "1 not in [1]"sv;

    std::vector<ExprPtr> expressions;
    expressions.push_back(make_expr_ptr<IntExpr>(int64_t(1)));

    std::vector<ExprPtr> numbers;
    numbers.push_back(make_expr_ptr<IntExpr>(int64_t(1)));
    expressions.push_back(make_list_expr(make_expr_ptr<IntExpr>(int64_t(1))));

    std::vector<ComparisonOperators> operators = {ComparisonOperators::NotMember};

    const auto expected_ast = make_expr_ptr<ComparisonChainExpr>(std::move(expressions), std::move(operators));

    const auto expected_eval = CuraFormulaeEngine::eval::Value(false);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("expression or operator", "[parser, expression]")
{
    auto input = "True or False"sv;
    const auto expected_ast = make_expr_ptr<BoolExpr>(true) || make_expr_ptr<BoolExpr>(false);
    const auto expected_eval = CuraFormulaeEngine::eval::Value(true);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("expression addition operator", "[parser, expression]")
{
    auto input = "1 + 2"sv;
    const auto expected_ast = make_expr_ptr<IntExpr>(1) + make_expr_ptr<IntExpr>(2);
    const auto expected_eval = CuraFormulaeEngine::eval::Value(int64_t(3));

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("expression addition operator with float and bool", "[parser, expression]")
{
    auto input = "True + 0.4"sv;
    const auto expected_ast = make_expr_ptr<BoolExpr>(true) + make_expr_ptr<FloatExpr>(0.4);
    const auto expected_eval = CuraFormulaeEngine::eval::Value(1.4);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}


TEST_CASE("expression subtraction operator", "[parser, expression]")
{
    auto input = "1 - 2"sv;
    const auto expected_ast = make_expr_ptr<IntExpr>(1) - make_expr_ptr<IntExpr>(2);
    const auto expected_eval = CuraFormulaeEngine::eval::Value(int64_t(-1));

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("expression pow operator", "[parser, expression]")
{
    auto input = "2 ** 3"sv;
    const auto expected_ast = make_expr_ptr<PowExpr>(make_expr_ptr<IntExpr>(2), make_expr_ptr<IntExpr>(3));
    const auto expected_eval = CuraFormulaeEngine::eval::Value(int64_t(8));

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("expression decimal pow operator", "[parser, expression]")
{
    auto input = "9 ** 0.5"sv;
    const auto expected_ast = make_expr_ptr<PowExpr>(make_expr_ptr<IntExpr>(9), make_expr_ptr<FloatExpr>(0.5));
    const auto expected_eval = CuraFormulaeEngine::eval::Value(3.0);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("expression multiply operator", "[parser, expression]")
{
    auto input = "2 * 2"sv;
    const auto expected_ast = make_expr_ptr<IntExpr>(2) * make_expr_ptr<IntExpr>(2);
    const auto expected_eval = CuraFormulaeEngine::eval::Value(int64_t(4));

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("expression division operator", "[parser, expression]")
{
    auto input = "2 / 2"sv;
    const auto expected_ast = make_expr_ptr<IntExpr>(2) / make_expr_ptr<IntExpr>(2);
    const auto expected_eval = CuraFormulaeEngine::eval::Value(1.0);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("expression multiple operators", "[parser, expression]")
{
    auto input = "4 + 4 ** 2 * 2"sv;
    const auto expected_ast
        = make_expr_ptr<IntExpr>(4)
        + make_expr_ptr<PowExpr>(make_expr_ptr<IntExpr>(4), make_expr_ptr<IntExpr>(2))
        * make_expr_ptr<IntExpr>(2);
    const auto expected_eval = CuraFormulaeEngine::eval::Value(int64_t(36));

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("2 < 3", "[parser, expression]")
{
    auto input = "2 < 3"sv;

    auto expected_ast = make_expr_ptr<IntExpr>(int64_t(2)) < make_expr_ptr<IntExpr>(int64_t(3));

    const auto expected_eval = CuraFormulaeEngine::eval::Value(true);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("15 < 300 < 25", "[parser, expression]")
{
    auto input = "15 < 300 < 25"sv;

    std::vector<ExprPtr> numbers;
    numbers.push_back(make_expr_ptr<IntExpr>(int64_t(15)));
    numbers.push_back(make_expr_ptr<IntExpr>(int64_t(300)));
    numbers.push_back(make_expr_ptr<IntExpr>(int64_t(25)));
    auto expected_ast = make_expr_ptr<ComparisonChainExpr>(
        std::move(numbers),
        std::vector{ComparisonOperators::LessThan, ComparisonOperators::LessThan}
    );
    const auto expected_eval = CuraFormulaeEngine::eval::Value(false);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("1 < 2 <= 2", "[parser, expression]")
{
    auto input = "1 < 2 <= 2"sv;

    std::vector<ExprPtr> numbers;
    numbers.push_back(make_expr_ptr<IntExpr>(int64_t(1)));
    numbers.push_back(make_expr_ptr<IntExpr>(int64_t(2)));
    numbers.push_back(make_expr_ptr<IntExpr>(int64_t(2)));
    auto expected_ast = make_expr_ptr<ComparisonChainExpr>(
        std::move(numbers),
        std::vector{ComparisonOperators::LessThan, ComparisonOperators::LessThenEqual}
    );
    const auto expected_eval = CuraFormulaeEngine::eval::Value(true);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("simple variable", "[parser, variable]")
{
    auto input = "foo"sv;
    const auto expected_ast = make_expr_ptr<VariableExpr>(std::string("foo"));
    const auto expected_eval = CuraFormulaeEngine::eval::Value(std::string("bar"));
    CuraFormulaeEngine::env::LocalEnvironment custom_env{&CuraFormulaeEngine::env::std_env};
    custom_env.set("foo", std::string("bar"));

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&custom_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("empty tuple", "[parser, tuple]")
{
    auto input = "()"sv;
    const auto expected_ast = make_tuple_expr();
    const auto expected_eval = CuraFormulaeEngine::eval::Value(std::vector<CuraFormulaeEngine::eval::Value>{});

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("single tuple", "[parser, tuple]")
{
    auto input = "(1)"sv;
    const auto expected_ast = make_expr_ptr<IntExpr>(int64_t(1));
    const auto expected_eval = CuraFormulaeEngine::eval::Value(int64_t(1));

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("tuple trailing comma", "[parser, tuple]")
{
    auto input = "(1,  )"sv;
    const auto expected_ast = make_expr_ptr<IntExpr>(int64_t(1));
    const auto expected_eval = CuraFormulaeEngine::eval::Value(int64_t(1));

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("double tuple", "[parser, tuple]")
{
    auto input = "(1, 2)"sv;
    const auto expected_ast = make_tuple_expr(make_expr_ptr<IntExpr>(int64_t(1)), make_expr_ptr<IntExpr>(int64_t(2)));
    const auto expected_eval = CuraFormulaeEngine::eval::Value{std::vector<CuraFormulaeEngine::eval::Value>{int64_t(1), int64_t(2)}};

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("trailing comma tuple", "[parser, tuple]")
{
    auto input = "(1, 2, )"sv;
    const auto expected_ast = make_tuple_expr(make_expr_ptr<IntExpr>(int64_t(1)), make_expr_ptr<IntExpr>(int64_t(2)));
    const auto expected_eval = CuraFormulaeEngine::eval::Value{std::vector<CuraFormulaeEngine::eval::Value>{int64_t(1), int64_t(2)}};

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("variable math.pi", "[parser, variable]")
{
    auto input = "math.pi"sv;
    const auto expected_ast = make_expr_ptr<VariableExpr>("math.pi");
    const auto expected_eval = CuraFormulaeEngine::eval::Value(std::numbers::pi);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("variable math.e", "[parser, variable]")
{
    auto input = "math.e"sv;
    const auto expected_ast = make_expr_ptr<VariableExpr>("math.e");
    const auto expected_eval = CuraFormulaeEngine::eval::Value(std::numbers::e);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("fn cos", "[parser, fn]")
{
    auto input = "math.cos(math.pi)"sv;
    const auto expected_ast = make_expr_ptr<VariableExpr>("math.cos")(make_expr_ptr<VariableExpr>("math.pi"));
    const auto expected_eval = CuraFormulaeEngine::eval::Value(std::cos(std::numbers::pi));

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();

    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("fn sin", "[parser, fn]")
{
    auto input = "math.sin(math.pi)"sv;
    const auto expected_ast = make_expr_ptr<VariableExpr>("math.sin")(make_expr_ptr<VariableExpr>("math.pi"));
    const auto expected_eval = CuraFormulaeEngine::eval::Value(std::sin(std::numbers::pi));

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("fn tan", "[parser, fn]")
{
    auto input = "math.tan(math.pi)"sv;
    const auto expected_ast = make_expr_ptr<VariableExpr>("math.tan")(make_expr_ptr<VariableExpr>("math.pi"));
    const auto expected_eval = CuraFormulaeEngine::eval::Value(std::tan(std::numbers::pi));

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("fn max", "[parser, fn]")
{
    auto input = "max(1, 2, 3, 4)"sv;
    const auto expected_eval = CuraFormulaeEngine::eval::Value(int64_t(4));

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("fn min", "[parser, fn]")
{
    auto input = "min(1, 2, 3, 4)"sv;
    const auto expected_eval = CuraFormulaeEngine::eval::Value(int64_t(1));

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("fn sum", "[parser, fn]")
{
    auto input = "sum([1, 2, 3, 4])"sv;
    const auto expected_eval = CuraFormulaeEngine::eval::Value(int64_t(10));

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("fn map", "[parser, fn]")
{
    auto input = "map(abs, [1, -2, 3, -4])"sv;
    const auto expected_eval = CuraFormulaeEngine::eval::Value(
            std::vector<CuraFormulaeEngine::eval::Value>{int64_t(1), int64_t(2), int64_t(3), int64_t(4)});

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("empty list", "[parser, list]")
{
    auto input = "[]"sv;
    const auto expected_ast = make_list_expr();
    const auto expected_eval = CuraFormulaeEngine::eval::Value{std::vector<CuraFormulaeEngine::eval::Value>{}};

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("empty list whitespace", "[parser, list]")
{
    auto input = "[ ]"sv;
    const auto expected_ast = make_list_expr();
    const auto expected_eval = CuraFormulaeEngine::eval::Value{std::vector<CuraFormulaeEngine::eval::Value>{}};

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("bool list", "[parser, list]")
{
    auto input = "[True, True, False]"sv;
    const auto expected_ast = make_list_expr(make_expr_ptr<BoolExpr>(true), make_expr_ptr<BoolExpr>(true),
                                             make_expr_ptr<BoolExpr>(false));
    const auto expected_eval = CuraFormulaeEngine::eval::Value{std::vector<CuraFormulaeEngine::eval::Value>{true, true, false}};

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("trailing comma bool list", "[parser, list]")
{
    auto input = "[True, True, False, ]"sv;
    std::vector<ExprPtr> list;
    const auto expected_ast = make_list_expr(make_expr_ptr<BoolExpr>(true), make_expr_ptr<BoolExpr>(true),
                                             make_expr_ptr<BoolExpr>(false));
    const auto expected_eval = CuraFormulaeEngine::eval::Value{std::vector<CuraFormulaeEngine::eval::Value>{true, true, false}};

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("expr list", "[parser, list]")
{
    auto input = "[0 if True else 1, 4 ** 4, 1 > 2]"sv;
    const auto expected_ast = make_list_expr(
            make_expr_ptr<ConditionExpr>(make_expr_ptr<IntExpr>(0), make_expr_ptr<BoolExpr>(true),
                                         make_expr_ptr<IntExpr>(1)),
            make_expr_ptr<PowExpr>(make_expr_ptr<IntExpr>(4), make_expr_ptr<IntExpr>(4)),
            make_expr_ptr<IntExpr>(1) > make_expr_ptr<IntExpr>(2));
    const auto expected_eval = CuraFormulaeEngine::eval::Value{std::vector<CuraFormulaeEngine::eval::Value>{int64_t(0), int64_t(256), false}};

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("simple list comprehension", "[parser, list comprehension]")
{
    auto input = "[x for x in dummy_array]"sv;
    auto loops = std::vector<ListComprehensionExpr::loop>{};
    loops.emplace_back(make_expr_ptr<VariableExpr>("x"), make_expr_ptr<VariableExpr>("dummy_array"),
                       std::vector<ExprPtr>{});
    const auto expected_ast = make_expr_ptr<ListComprehensionExpr>(make_expr_ptr<VariableExpr>("x"), std::move(loops));
    CuraFormulaeEngine::env::LocalEnvironment custom_env{&CuraFormulaeEngine::env::std_env};
    custom_env.set("dummy_array",
                   std::vector<CuraFormulaeEngine::eval::Value>{int64_t(0), int64_t(1), int64_t(2), int64_t(3), int64_t(4),
                                                     int64_t(5), int64_t(6), int64_t(7), int64_t(8), int64_t(9),
                                                     int64_t(10)});

    const auto expected_eval = CuraFormulaeEngine::eval::Value{
            std::vector<CuraFormulaeEngine::eval::Value>{int64_t(0), int64_t(1), int64_t(2), int64_t(3), int64_t(4), int64_t(5),
                                              int64_t(6), int64_t(7), int64_t(8), int64_t(9), int64_t(10)}};

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&custom_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("expression list comprehension", "[parser, list comprehension]")
{
    auto input = "[x * 2 for x in dummy_array]"sv;
    auto loops = std::vector<ListComprehensionExpr::loop>{};
    loops.emplace_back(make_expr_ptr<VariableExpr>("x"), make_expr_ptr<VariableExpr>("dummy_array"),
                       std::vector<ExprPtr>{});
    const auto expected_ast = make_expr_ptr<ListComprehensionExpr>(
            make_expr_ptr<VariableExpr>("x") * make_expr_ptr<IntExpr>(2), std::move(loops));
    CuraFormulaeEngine::env::LocalEnvironment custom_env{&CuraFormulaeEngine::env::std_env};
    custom_env.set("dummy_array",
                   std::vector<CuraFormulaeEngine::eval::Value>{int64_t(0), int64_t(1), int64_t(2), int64_t(3), int64_t(4),
                                                     int64_t(5), int64_t(6), int64_t(7), int64_t(8), int64_t(9),
                                                     int64_t(10)});

    const auto expected_eval = CuraFormulaeEngine::eval::Value{
            std::vector<CuraFormulaeEngine::eval::Value>{int64_t(0), int64_t(2), int64_t(4), int64_t(6), int64_t(8), int64_t(10),
                                              int64_t(12), int64_t(14), int64_t(16), int64_t(18), int64_t(20)}};

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&custom_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("simple list comprehension even", "[parser, list comprehension]")
{
    auto input = "[x for x in dummy_array if x % 2 == 0]"sv;
    auto conditions = std::vector<ExprPtr>{};
    conditions.emplace_back(make_expr_ptr<VariableExpr>("x") % make_expr_ptr<IntExpr>(2) == make_expr_ptr<IntExpr>(0));
    auto loops = std::vector<ListComprehensionExpr::loop>{};
    loops.emplace_back(make_expr_ptr<VariableExpr>("x"), make_expr_ptr<VariableExpr>("dummy_array"),
                       std::move(conditions));
    const auto expected_ast = make_expr_ptr<ListComprehensionExpr>(make_expr_ptr<VariableExpr>("x"), std::move(loops));
    CuraFormulaeEngine::env::LocalEnvironment custom_env{&CuraFormulaeEngine::env::std_env};
    custom_env.set("dummy_array",
                   std::vector<CuraFormulaeEngine::eval::Value>{int64_t(0), int64_t(1), int64_t(2), int64_t(3), int64_t(4),
                                                     int64_t(5), int64_t(6), int64_t(7), int64_t(8), int64_t(9),
                                                     int64_t(10)});

    const auto expected_eval = CuraFormulaeEngine::eval::Value{
            std::vector<CuraFormulaeEngine::eval::Value>{int64_t(0), int64_t(2), int64_t(4), int64_t(6), int64_t(8), int64_t(10)}};

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&custom_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("simple list comprehension expression and condition", "[parser, list comprehension]")
{
    auto input = "[x * 2 for x in dummy_array if x <= 1]"sv;
    auto conditions = std::vector<ExprPtr>{};
    conditions.emplace_back(make_expr_ptr<VariableExpr>("x") <= make_expr_ptr<IntExpr>(1));
    auto loops = std::vector<ListComprehensionExpr::loop>{};
    loops.emplace_back(make_expr_ptr<VariableExpr>("x"), make_expr_ptr<VariableExpr>("dummy_array"),
                       std::move(conditions));
    const auto expected_ast = make_expr_ptr<ListComprehensionExpr>(
            make_expr_ptr<VariableExpr>("x") * make_expr_ptr<IntExpr>(2), std::move(loops));
    CuraFormulaeEngine::env::LocalEnvironment custom_env{&CuraFormulaeEngine::env::std_env};
    custom_env.set("dummy_array",
                   std::vector<CuraFormulaeEngine::eval::Value>{int64_t(0), int64_t(1), int64_t(2), int64_t(3), int64_t(4),
                                                     int64_t(5), int64_t(6), int64_t(7), int64_t(8), int64_t(9),
                                                     int64_t(10)});

    const auto expected_eval = CuraFormulaeEngine::eval::Value{std::vector<CuraFormulaeEngine::eval::Value>{int64_t(0), int64_t(2)}};

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&custom_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("list comprehension inline list", "[parser, list comprehension]")
{
    auto input = "[x for x in [0, 1]]"sv;
    auto loops = std::vector<ListComprehensionExpr::loop>{};
    loops.emplace_back(make_expr_ptr<VariableExpr>("x"),
                       make_list_expr(make_expr_ptr<IntExpr>(int64_t(0)), make_expr_ptr<IntExpr>(int64_t(1))),
                       std::vector<ExprPtr>{});
    const auto expected_ast = make_expr_ptr<ListComprehensionExpr>(make_expr_ptr<VariableExpr>("x"), std::move(loops));
    const auto expected_eval = CuraFormulaeEngine::eval::Value{std::vector<CuraFormulaeEngine::eval::Value>{int64_t(0), int64_t(1)}};

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("list comprehension if chains", "[parser, list comprehension]")
{
    auto input = "[x for x in [1, 2, 3] if x <= 2 if x >= 2]"sv;
    auto conditions = std::vector<ExprPtr>{};
    conditions.emplace_back(make_expr_ptr<VariableExpr>("x") <= make_expr_ptr<IntExpr>(2));
    conditions.emplace_back(make_expr_ptr<VariableExpr>("x") >= make_expr_ptr<IntExpr>(2));
    auto loops = std::vector<ListComprehensionExpr::loop>{};
    loops.emplace_back(
            make_expr_ptr<VariableExpr>("x"),
            make_list_expr(make_expr_ptr<IntExpr>(int64_t(1)), make_expr_ptr<IntExpr>(int64_t(2)),
                           make_expr_ptr<IntExpr>(int64_t(3))),
            std::move(conditions));
    const auto expected_ast = make_expr_ptr<ListComprehensionExpr>(make_expr_ptr<VariableExpr>("x"), std::move(loops));
    const auto expected_eval = CuraFormulaeEngine::eval::Value{std::vector<CuraFormulaeEngine::eval::Value>{int64_t(2)}};
    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("Nested list comprehension", "[parser, list comprehension]")
{
    auto input = "[x for x in (y for y in [[1, 2], [3, 4]])]"sv;

    auto loops_b = std::vector<ListComprehensionExpr::loop>{};
    loops_b.emplace_back(
            make_expr_ptr<VariableExpr>("y"),
            make_list_expr(
                    make_list_expr(make_expr_ptr<IntExpr>(int64_t(1)), make_expr_ptr<IntExpr>(int64_t(2))),
                    make_list_expr(make_expr_ptr<IntExpr>(int64_t(3)), make_expr_ptr<IntExpr>(int64_t(4)))),
            std::vector<ExprPtr>{});
    auto loops_a = std::vector<ListComprehensionExpr::loop>{};
    loops_a.emplace_back(make_expr_ptr<VariableExpr>("x"),
                         make_expr_ptr<ListComprehensionExpr>(make_expr_ptr<VariableExpr>("y"), std::move(loops_b)),
                         std::vector<ExprPtr>{});
    const auto expected_ast = make_expr_ptr<ListComprehensionExpr>(make_expr_ptr<VariableExpr>("x"),
                                                                   std::move(loops_a));

    const auto expected_eval
            = CuraFormulaeEngine::eval::Value{
                    std::vector<CuraFormulaeEngine::eval::Value>{std::vector<CuraFormulaeEngine::eval::Value>{int64_t(1), int64_t(2)},
                                                      std::vector<CuraFormulaeEngine::eval::Value>{int64_t(3), int64_t(4)}}};

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("Flatten list comprehension", "[parser, list comprehension]")
{
    auto input = "[item for row in matrix for item in row]"sv;
    const auto expected_eval = CuraFormulaeEngine::eval::Value{
            std::vector<CuraFormulaeEngine::eval::Value>{int64_t(1), int64_t(2), int64_t(3), int64_t(4)}};
    CuraFormulaeEngine::env::LocalEnvironment custom_env{&CuraFormulaeEngine::env::std_env};
    custom_env.set("matrix",
                   std::vector<CuraFormulaeEngine::eval::Value>{std::vector<CuraFormulaeEngine::eval::Value>{int64_t(1), int64_t(2)},
                                                     std::vector<CuraFormulaeEngine::eval::Value>{int64_t(3), int64_t(4)}});

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    const auto eval = ast.evaluate(&custom_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("Flatten list comprehension condition", "[parser, list comprehension]")
{
    auto input = "[item for row in matrix if len(row) >= 3 for item in row]"sv;

    const auto expected_eval = CuraFormulaeEngine::eval::Value{
            std::vector<CuraFormulaeEngine::eval::Value>{int64_t(1), int64_t(2), int64_t(3), int64_t(6), int64_t(7), int64_t(8)}};
    CuraFormulaeEngine::env::LocalEnvironment custom_env{&CuraFormulaeEngine::env::std_env};
    custom_env.set(
            "matrix",
            std::vector<CuraFormulaeEngine::eval::Value>{std::vector<CuraFormulaeEngine::eval::Value>{int64_t(1), int64_t(2), int64_t(3)},
                                              std::vector<CuraFormulaeEngine::eval::Value>{int64_t(4), int64_t(5)},
                                              std::vector<CuraFormulaeEngine::eval::Value>{int64_t(6), int64_t(7), int64_t(8)}});

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    const auto eval = ast.evaluate(&custom_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("Single quoted string", "[parser, string]")
{
    auto input = "'HELLO WORLD'"sv;
    const auto expected_ast = make_expr_ptr<StringExpr>(std::string("HELLO WORLD"));
    const auto expected_eval = CuraFormulaeEngine::eval::Value(std::string("HELLO WORLD"));

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("Single quoted empty string", "[parser, string]")
{
    auto input = "''"sv;
    const auto expected_ast = make_expr_ptr<StringExpr>(std::string(""));
    const auto expected_eval = CuraFormulaeEngine::eval::Value(std::string(""));

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("Double quoted string", "[parser, string]")
{
    auto input = "\"FOO BAR\""sv;
    const auto expected_ast = make_expr_ptr<StringExpr>(std::string("FOO BAR"));
    const auto expected_eval = CuraFormulaeEngine::eval::Value(std::string("FOO BAR"));

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("String with parenthesis", "[parser, string]")
{
    auto input = "\"Marlin (Volumetric)\""sv;
    const auto expected_ast = make_expr_ptr<StringExpr>(std::string("Marlin (Volumetric)"));
    const auto expected_eval = CuraFormulaeEngine::eval::Value(std::string("Marlin (Volumetric)"));

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("Double quoted empty string", "[parser, string]")
{
    auto input = "\"\""sv;
    const auto expected_ast = make_expr_ptr<StringExpr>(std::string(""));
    const auto expected_eval = CuraFormulaeEngine::eval::Value(std::string(""));

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("fn int", "[parser, fn]")
{
    auto input = "int(0.1)"sv;
    const auto expected_ast = make_expr_ptr<VariableExpr>("int")(make_expr_ptr<FloatExpr>(0.1));
    const auto expected_eval = CuraFormulaeEngine::eval::Value(int64_t(0));

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("fn neg int", "[parser, fn]")
{
    auto input = "int(-0.1)"sv;
    const auto expected_ast = make_expr_ptr<VariableExpr>("int")(-make_expr_ptr<FloatExpr>(0.1));
    const auto expected_eval = CuraFormulaeEngine::eval::Value(int64_t(0));

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("fn int str input", "[parser, fn]")
{
    auto input = "int(\"100\")"sv;
    const auto expected_ast = make_expr_ptr<VariableExpr>("int")(make_expr_ptr<StringExpr>("100"));
    const auto expected_eval = CuraFormulaeEngine::eval::Value(int64_t(100));

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("fn int str input base", "[parser, fn]")
{
    auto input = "int(\"100\", 2)"sv;
    const auto expected_ast = make_expr_ptr<VariableExpr>(std::string("int"))(make_expr_ptr<StringExpr>("100"),
                                                                              make_expr_ptr<IntExpr>(int64_t(2)));
    const auto expected_eval = CuraFormulaeEngine::eval::Value(int64_t(4));

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("fn floor", "[parser, fn]")
{
    auto input = "math.floor(0.1)"sv;
    const auto expected_ast = make_expr_ptr<VariableExpr>("math.floor")(make_expr_ptr<FloatExpr>(0.1));
    const auto expected_eval = CuraFormulaeEngine::eval::Value(0.0);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("fn ceil", "[parser, fn]")
{
    auto input = "math.ceil(0.1)"sv;
    const auto expected_ast = make_expr_ptr<VariableExpr>("math.ceil")(make_expr_ptr<FloatExpr>(0.1));
    const auto expected_eval = CuraFormulaeEngine::eval::Value(1.0);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("fn math.log(1)", "[parser, fn]")
{
    auto input = "math.log(1)"sv;
    const auto expected_ast = make_expr_ptr<VariableExpr>("math.log")(make_expr_ptr<IntExpr>(int64_t(1)));
    const auto expected_eval = CuraFormulaeEngine::eval::Value(0.0);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("fn math.log(1.0)", "[parser, fn]")
{
    auto input = "math.log(1.0)"sv;
    const auto expected_ast = make_expr_ptr<VariableExpr>("math.log")(make_expr_ptr<FloatExpr>(1.0));
    const auto expected_eval = CuraFormulaeEngine::eval::Value(0.0);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("fn math.log(4096, 8)", "[parser, fn]")
{
    auto input = "math.log(4096, 8)"sv;
    const auto expected_ast = make_expr_ptr<VariableExpr>("math.log")(make_expr_ptr<IntExpr>(int64_t(4096)),
                                                                      make_expr_ptr<IntExpr>(int64_t(8)));
    const auto expected_eval = CuraFormulaeEngine::eval::Value(4.0);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("fn math.log(True)", "[parser, fn]")
{
    auto input = "math.log(True)"sv;
    const auto expected_ast = make_expr_ptr<VariableExpr>("math.log")(make_expr_ptr<BoolExpr>(true));
    const auto expected_eval = CuraFormulaeEngine::eval::Value(0.0);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("condition True", "[parser, condition]")
{
    auto input = "1 if True else 0"sv;
    const auto expected_ast = make_expr_ptr<ConditionExpr>(make_expr_ptr<IntExpr>(1), make_expr_ptr<BoolExpr>(true),
                                                           make_expr_ptr<IntExpr>(0));
    const auto expected_eval = CuraFormulaeEngine::eval::Value(int64_t(1));

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("condition False", "[parser, condition]")
{
    auto input = "1 if False else 0"sv;
    const auto expected_ast = make_expr_ptr<ConditionExpr>(make_expr_ptr<IntExpr>(1), make_expr_ptr<BoolExpr>(false),
                                                           make_expr_ptr<IntExpr>(0));
    const auto expected_eval = CuraFormulaeEngine::eval::Value(int64_t(0));

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("nested condition 1", "[parser, condition]")
{
    auto input = "1 if False else 2 if False else 3"sv;
    const auto expected_ast = make_expr_ptr<ConditionExpr>(
            make_expr_ptr<IntExpr>(1),
            make_expr_ptr<BoolExpr>(false),
            make_expr_ptr<ConditionExpr>(make_expr_ptr<IntExpr>(2), make_expr_ptr<BoolExpr>(false),
                                         make_expr_ptr<IntExpr>(3)));
    const auto expected_eval = CuraFormulaeEngine::eval::Value(int64_t(3));

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("nested condition 2", "[parser, condition]")
{
    auto input = "1 if False else 2 if False else 3 if False else 4"sv;
    const auto expected_ast = make_expr_ptr<ConditionExpr>(
            make_expr_ptr<IntExpr>(1),
            make_expr_ptr<BoolExpr>(false),
            make_expr_ptr<ConditionExpr>(
                    make_expr_ptr<IntExpr>(2),
                    make_expr_ptr<BoolExpr>(false),
                    make_expr_ptr<ConditionExpr>(make_expr_ptr<IntExpr>(3), make_expr_ptr<BoolExpr>(false),
                                                 make_expr_ptr<IntExpr>(4))));
    const auto expected_eval = CuraFormulaeEngine::eval::Value(int64_t(4));

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("index simple 0", "[parser, index]")
{
    auto input = "dummy_array[0]"sv;
    const auto expected_ast = make_expr_ptr<VariableExpr>(std::string("dummy_array"))[make_expr_ptr<IntExpr>(0)];
    const auto expected_eval = CuraFormulaeEngine::eval::Value(int64_t(0));
    CuraFormulaeEngine::env::LocalEnvironment custom_env{&CuraFormulaeEngine::env::std_env};
    custom_env.set("dummy_array",
                   std::vector<CuraFormulaeEngine::eval::Value>{int64_t(0), int64_t(1), int64_t(2), int64_t(3), int64_t(4),
                                                     int64_t(5), int64_t(6), int64_t(7), int64_t(8), int64_t(9),
                                                     int64_t(10)});

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&custom_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("slice index out of bounds positive", "[parser, index]")
{
    auto input = "dummy_array[11]"sv;
    const auto expected_ast = make_expr_ptr<VariableExpr>(std::string("dummy_array"))[make_expr_ptr<IntExpr>(11)];
    CuraFormulaeEngine::env::LocalEnvironment custom_env{&CuraFormulaeEngine::env::std_env};
    custom_env.set("dummy_array",
                   std::vector<CuraFormulaeEngine::eval::Value>{int64_t(0), int64_t(1), int64_t(2), int64_t(3), int64_t(4),
                                                     int64_t(5), int64_t(6), int64_t(7), int64_t(8), int64_t(9),
                                                     int64_t(10)});

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&custom_env);
    REQUIRE_FALSE(eval.has_value());
    REQUIRE(eval.error() == CuraFormulaeEngine::eval::Error::IndexOutOfBounds);
}

TEST_CASE("slice index out of bounds negative", "[parser, index]")
{
    auto input = "dummy_array[-12]"sv;
    const auto expected_ast = make_expr_ptr<VariableExpr>(std::string("dummy_array"))[-make_expr_ptr<IntExpr>(12)];
    CuraFormulaeEngine::env::LocalEnvironment custom_env{&CuraFormulaeEngine::env::std_env};
    custom_env.set("dummy_array",
                   std::vector<CuraFormulaeEngine::eval::Value>{int64_t(0), int64_t(1), int64_t(2), int64_t(3), int64_t(4),
                                                     int64_t(5), int64_t(6), int64_t(7), int64_t(8), int64_t(9),
                                                     int64_t(10)});

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&custom_env);
    REQUIRE_FALSE(eval.has_value());
    REQUIRE(eval.error() == CuraFormulaeEngine::eval::Error::IndexOutOfBounds);
}

TEST_CASE("non integer index", "[parser, slice]")
{
    auto input = "dummy_array[0.1]"sv;
    const auto expected_ast = make_expr_ptr<VariableExpr>(std::string("dummy_array"))[make_expr_ptr<FloatExpr>(0.1)];
    CuraFormulaeEngine::env::LocalEnvironment custom_env{&CuraFormulaeEngine::env::std_env};
    custom_env.set("dummy_array",
                   std::vector<CuraFormulaeEngine::eval::Value>{int64_t(0), int64_t(1), int64_t(2), int64_t(3), int64_t(4),
                                                     int64_t(5), int64_t(6), int64_t(7), int64_t(8), int64_t(9),
                                                     int64_t(10)});

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&custom_env);
    REQUIRE_FALSE(eval.has_value());
    REQUIRE(eval.error() == CuraFormulaeEngine::eval::Error::TypeMismatch);
}

TEST_CASE("simple slice", "[parser, slice]")
{
    auto input = "dummy_array[0:10:1]"sv;
    const auto expected_ast
            = make_expr_ptr<SliceExpr>(make_expr_ptr<VariableExpr>(std::string("dummy_array")),
                                       make_expr_ptr<IntExpr>(0), make_expr_ptr<IntExpr>(10),
                                       make_expr_ptr<IntExpr>(1));
    const auto expected_eval = CuraFormulaeEngine::eval::Value{
            std::vector<CuraFormulaeEngine::eval::Value>{int64_t(0), int64_t(1), int64_t(2), int64_t(3), int64_t(4), int64_t(5),
                                              int64_t(6), int64_t(7), int64_t(8), int64_t(9), int64_t(10)}};
    CuraFormulaeEngine::env::LocalEnvironment custom_env{&CuraFormulaeEngine::env::std_env};
    custom_env.set("dummy_array",
                   std::vector<CuraFormulaeEngine::eval::Value>{int64_t(0), int64_t(1), int64_t(2), int64_t(3), int64_t(4),
                                                     int64_t(5), int64_t(6), int64_t(7), int64_t(8), int64_t(9),
                                                     int64_t(10)});

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&custom_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("simple slice space", "[parser, slice]")
{
    auto input = "dummy_array[0 : 10 : 1]"sv;
    const auto expected_ast
            = make_expr_ptr<SliceExpr>(make_expr_ptr<VariableExpr>(std::string("dummy_array")),
                                       make_expr_ptr<IntExpr>(0), make_expr_ptr<IntExpr>(10),
                                       make_expr_ptr<IntExpr>(1));
    const auto expected_eval = CuraFormulaeEngine::eval::Value{
            std::vector<CuraFormulaeEngine::eval::Value>{int64_t(0), int64_t(1), int64_t(2), int64_t(3), int64_t(4), int64_t(5),
                                              int64_t(6), int64_t(7), int64_t(8), int64_t(9), int64_t(10)}};
    CuraFormulaeEngine::env::LocalEnvironment custom_env{&CuraFormulaeEngine::env::std_env};
    custom_env.set("dummy_array",
                   std::vector<CuraFormulaeEngine::eval::Value>{int64_t(0), int64_t(1), int64_t(2), int64_t(3), int64_t(4),
                                                     int64_t(5), int64_t(6), int64_t(7), int64_t(8), int64_t(9),
                                                     int64_t(10)});

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&custom_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("simple slice syntax sugar", "[parser, slice]")
{
    auto input = "dummy_array[0:10]"sv;
    const auto expected_ast
            = make_expr_ptr<SliceExpr>(make_expr_ptr<VariableExpr>(std::string("dummy_array")),
                                       make_expr_ptr<IntExpr>(0), make_expr_ptr<IntExpr>(10), std::nullopt);
    const auto expected_eval = CuraFormulaeEngine::eval::Value{
            std::vector<CuraFormulaeEngine::eval::Value>{int64_t(0), int64_t(1), int64_t(2), int64_t(3), int64_t(4), int64_t(5),
                                              int64_t(6), int64_t(7), int64_t(8), int64_t(9), int64_t(10)}};
    CuraFormulaeEngine::env::LocalEnvironment custom_env{&CuraFormulaeEngine::env::std_env};
    custom_env.set("dummy_array",
                   std::vector<CuraFormulaeEngine::eval::Value>{int64_t(0), int64_t(1), int64_t(2), int64_t(3), int64_t(4),
                                                     int64_t(5), int64_t(6), int64_t(7), int64_t(8), int64_t(9),
                                                     int64_t(10)});

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&custom_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("simple slice out of bound", "[parser, slice]")
{
    auto input = "dummy_array[0:11:1]"sv;
    const auto expected_ast
            = make_expr_ptr<SliceExpr>(make_expr_ptr<VariableExpr>(std::string("dummy_array")),
                                       make_expr_ptr<IntExpr>(0), make_expr_ptr<IntExpr>(11),
                                       make_expr_ptr<IntExpr>(1));
    const auto expected_eval = CuraFormulaeEngine::eval::Value{
            std::vector<CuraFormulaeEngine::eval::Value>{int64_t(0), int64_t(1), int64_t(2), int64_t(3), int64_t(4), int64_t(5),
                                              int64_t(6), int64_t(7), int64_t(8), int64_t(9), int64_t(10)}};
    CuraFormulaeEngine::env::LocalEnvironment custom_env{&CuraFormulaeEngine::env::std_env};
    custom_env.set("dummy_array",
                   std::vector<CuraFormulaeEngine::eval::Value>{int64_t(0), int64_t(1), int64_t(2), int64_t(3), int64_t(4),
                                                     int64_t(5), int64_t(6), int64_t(7), int64_t(8), int64_t(9),
                                                     int64_t(10)});

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&custom_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("reverse slice", "[parser, slice]")
{
    auto input = "dummy_array[10:0:-1]"sv;
    const auto expected_ast
            = make_expr_ptr<SliceExpr>(make_expr_ptr<VariableExpr>(std::string("dummy_array")),
                                       make_expr_ptr<IntExpr>(10), make_expr_ptr<IntExpr>(0),
                                       -make_expr_ptr<IntExpr>(1));
    const auto expected_eval = CuraFormulaeEngine::eval::Value{
            std::vector<CuraFormulaeEngine::eval::Value>{int64_t(10), int64_t(9), int64_t(8), int64_t(7), int64_t(6), int64_t(5),
                                              int64_t(4), int64_t(3), int64_t(2), int64_t(1), int64_t(0)}};
    CuraFormulaeEngine::env::LocalEnvironment custom_env{&CuraFormulaeEngine::env::std_env};
    custom_env.set("dummy_array",
                   std::vector<CuraFormulaeEngine::eval::Value>{int64_t(0), int64_t(1), int64_t(2), int64_t(3), int64_t(4),
                                                     int64_t(5), int64_t(6), int64_t(7), int64_t(8), int64_t(9),
                                                     int64_t(10)});

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&custom_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("array slice step size 2", "[parser, slice]")
{
    auto input = "dummy_array[0:10:2]"sv;
    const auto expected_ast
            = make_expr_ptr<SliceExpr>(make_expr_ptr<VariableExpr>(std::string("dummy_array")),
                                       make_expr_ptr<IntExpr>(0), make_expr_ptr<IntExpr>(10),
                                       make_expr_ptr<IntExpr>(2));
    const auto expected_eval = CuraFormulaeEngine::eval::Value{
            std::vector<CuraFormulaeEngine::eval::Value>{int64_t(0), int64_t(2), int64_t(4), int64_t(6), int64_t(8), int64_t(10)}};
    CuraFormulaeEngine::env::LocalEnvironment custom_env{&CuraFormulaeEngine::env::std_env};
    custom_env.set("dummy_array",
                   std::vector<CuraFormulaeEngine::eval::Value>{int64_t(0), int64_t(1), int64_t(2), int64_t(3), int64_t(4),
                                                     int64_t(5), int64_t(6), int64_t(7), int64_t(8), int64_t(9),
                                                     int64_t(10)});

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&custom_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("slice empty value start/end index step size", "[parser, slice]")
{
    auto input = "dummy_array[::]"sv;
    const auto expected_ast = make_expr_ptr<SliceExpr>(make_expr_ptr<VariableExpr>(std::string("dummy_array")),
                                                       std::nullopt, std::nullopt, std::nullopt);
    const auto expected_eval = CuraFormulaeEngine::eval::Value{
            std::vector<CuraFormulaeEngine::eval::Value>{int64_t(0), int64_t(1), int64_t(2), int64_t(3), int64_t(4), int64_t(5),
                                              int64_t(6), int64_t(7), int64_t(8), int64_t(9), int64_t(10)}};
    CuraFormulaeEngine::env::LocalEnvironment custom_env{&CuraFormulaeEngine::env::std_env};
    custom_env.set("dummy_array",
                   std::vector<CuraFormulaeEngine::eval::Value>{int64_t(0), int64_t(1), int64_t(2), int64_t(3), int64_t(4),
                                                     int64_t(5), int64_t(6), int64_t(7), int64_t(8), int64_t(9),
                                                     int64_t(10)});

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&custom_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("slice empty value start/end index negative step size", "[parser, slice]")
{
    auto input = "dummy_array[::-1]"sv;
    const auto expected_ast = make_expr_ptr<SliceExpr>(make_expr_ptr<VariableExpr>(std::string("dummy_array")),
                                                       std::nullopt, std::nullopt, -make_expr_ptr<IntExpr>(1));
    const auto expected_eval = CuraFormulaeEngine::eval::Value{
            std::vector<CuraFormulaeEngine::eval::Value>{int64_t(10), int64_t(9), int64_t(8), int64_t(7), int64_t(6), int64_t(5),
                                              int64_t(4), int64_t(3), int64_t(2), int64_t(1), int64_t(0)}};
    CuraFormulaeEngine::env::LocalEnvironment custom_env{&CuraFormulaeEngine::env::std_env};
    custom_env.set("dummy_array",
                   std::vector<CuraFormulaeEngine::eval::Value>{int64_t(0), int64_t(1), int64_t(2), int64_t(3), int64_t(4),
                                                     int64_t(5), int64_t(6), int64_t(7), int64_t(8), int64_t(9),
                                                     int64_t(10)});

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&custom_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("slice empty value start index, step size", "[parser, slice]")
{
    auto input = "dummy_array[:-2]"sv;
    const auto expected_ast = make_expr_ptr<SliceExpr>(make_expr_ptr<VariableExpr>(std::string("dummy_array")),
                                                       std::nullopt, -make_expr_ptr<IntExpr>(2), std::nullopt);
    const auto expected_eval = CuraFormulaeEngine::eval::Value{
            std::vector<CuraFormulaeEngine::eval::Value>{int64_t(0), int64_t(1), int64_t(2), int64_t(3), int64_t(4), int64_t(5),
                                              int64_t(6), int64_t(7), int64_t(8)}};
    CuraFormulaeEngine::env::LocalEnvironment custom_env{&CuraFormulaeEngine::env::std_env};
    custom_env.set("dummy_array",
                   std::vector<CuraFormulaeEngine::eval::Value>{int64_t(0), int64_t(1), int64_t(2), int64_t(3), int64_t(4),
                                                     int64_t(5), int64_t(6), int64_t(7), int64_t(8), int64_t(9),
                                                     int64_t(10)});

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&custom_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("non integer slice start index", "[parser, slice]")
{
    auto input = "dummy_array[0.1::]"sv;
    const auto expected_ast = make_expr_ptr<SliceExpr>(make_expr_ptr<VariableExpr>(std::string("dummy_array")),
                                                       make_expr_ptr<FloatExpr>(0.1), std::nullopt, std::nullopt);
    CuraFormulaeEngine::env::LocalEnvironment custom_env{&CuraFormulaeEngine::env::std_env};
    custom_env.set("dummy_array",
                   std::vector<CuraFormulaeEngine::eval::Value>{int64_t(0), int64_t(1), int64_t(2), int64_t(3), int64_t(4),
                                                     int64_t(5), int64_t(6), int64_t(7), int64_t(8), int64_t(9),
                                                     int64_t(10)});

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&custom_env);
    REQUIRE_FALSE(eval.has_value());
    REQUIRE(eval.error() == CuraFormulaeEngine::eval::Error::TypeMismatch);
}


TEST_CASE("non integer slice end index", "[parser, slice]")
{
    auto input = "dummy_array[:0.1:]"sv;
    const auto expected_ast = make_expr_ptr<SliceExpr>(make_expr_ptr<VariableExpr>(std::string("dummy_array")),
                                                       std::nullopt, make_expr_ptr<FloatExpr>(0.1), std::nullopt);
    CuraFormulaeEngine::env::LocalEnvironment custom_env{&CuraFormulaeEngine::env::std_env};
    custom_env.set("dummy_array",
                   std::vector<CuraFormulaeEngine::eval::Value>{int64_t(0), int64_t(1), int64_t(2), int64_t(3), int64_t(4),
                                                     int64_t(5), int64_t(6), int64_t(7), int64_t(8), int64_t(9),
                                                     int64_t(10)});

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&custom_env);
    REQUIRE_FALSE(eval.has_value());
    REQUIRE(eval.error() == CuraFormulaeEngine::eval::Error::TypeMismatch);
}

TEST_CASE("non integer slice step size", "[parser, slice]")
{
    auto input = "dummy_array[::0.1]"sv;
    const auto expected_ast = make_expr_ptr<SliceExpr>(make_expr_ptr<VariableExpr>(std::string("dummy_array")),
                                                       std::nullopt, std::nullopt, make_expr_ptr<FloatExpr>(0.1));
    CuraFormulaeEngine::env::LocalEnvironment custom_env{&CuraFormulaeEngine::env::std_env};
    custom_env.set("dummy_array",
                   std::vector<CuraFormulaeEngine::eval::Value>{int64_t(0), int64_t(1), int64_t(2), int64_t(3), int64_t(4),
                                                     int64_t(5), int64_t(6), int64_t(7), int64_t(8), int64_t(9),
                                                     int64_t(10)});

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&custom_env);
    REQUIRE_FALSE(eval.has_value());
    REQUIRE(eval.error() == CuraFormulaeEngine::eval::Error::TypeMismatch);
}

TEST_CASE("index simple 10", "[parser, index]")
{
    auto input = "dummy_array[10]"sv;
    const auto expected_ast = make_expr_ptr<VariableExpr>(std::string("dummy_array"))[make_expr_ptr<IntExpr>(10)];
    const auto expected_eval = CuraFormulaeEngine::eval::Value(int64_t(10));
    CuraFormulaeEngine::env::LocalEnvironment custom_env{&CuraFormulaeEngine::env::std_env};
    custom_env.set("dummy_array",
                   std::vector<CuraFormulaeEngine::eval::Value>{int64_t(0), int64_t(1), int64_t(2), int64_t(3), int64_t(4),
                                                     int64_t(5), int64_t(6), int64_t(7), int64_t(8), int64_t(9),
                                                     int64_t(10)});

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();

    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&custom_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("index last", "[parser, index]")
{
    auto input = "dummy_array[-1]"sv;
    const auto expected_ast = make_expr_ptr<VariableExpr>(std::string("dummy_array"))[-make_expr_ptr<IntExpr>(1)];
    const auto expected_eval = CuraFormulaeEngine::eval::Value(int64_t(10));
    CuraFormulaeEngine::env::LocalEnvironment custom_env{&CuraFormulaeEngine::env::std_env};
    custom_env.set("dummy_array",
                   std::vector<CuraFormulaeEngine::eval::Value>{int64_t(0), int64_t(1), int64_t(2), int64_t(3), int64_t(4),
                                                     int64_t(5), int64_t(6), int64_t(7), int64_t(8), int64_t(9),
                                                     int64_t(10)});

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&custom_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("index last first", "[parser, index]")
{
    auto input = "dummy_array[-11]"sv;
    const auto expected_ast = make_expr_ptr<VariableExpr>(std::string("dummy_array"))[-make_expr_ptr<IntExpr>(11)];
    const auto expected_eval = CuraFormulaeEngine::eval::Value(int64_t(0));
    CuraFormulaeEngine::env::LocalEnvironment custom_env{&CuraFormulaeEngine::env::std_env};
    custom_env.set("dummy_array",
                   std::vector<CuraFormulaeEngine::eval::Value>{int64_t(0), int64_t(1), int64_t(2), int64_t(3), int64_t(4),
                                                     int64_t(5), int64_t(6), int64_t(7), int64_t(8), int64_t(9),
                                                     int64_t(10)});

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&custom_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("index without index value", "[parser, index]")
{
    auto input = "dummy_array[]"sv;
    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(!message.has_value());
}

TEST_CASE("fn any", "[parser, fn]")
{
    auto input = "any([True])"sv;
    const auto expected_ast = make_expr_ptr<VariableExpr>("any")(make_list_expr(make_expr_ptr<BoolExpr>(true)));
    const auto expected_eval = CuraFormulaeEngine::eval::Value(true);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("index function application", "[parser, index]")
{
    auto input = "map(abs, [1, -2, 3, -4])[0]"sv;
    CuraFormulaeEngine::env::LocalEnvironment custom_env{&CuraFormulaeEngine::env::std_env};

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    const auto eval = ast.evaluate(&custom_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(CuraFormulaeEngine::eval::Value(int64_t(1))));
}

TEST_CASE("double index", "[parser, index]")
{
    auto input = "dummy_array[0][0]"sv;
    CuraFormulaeEngine::env::LocalEnvironment custom_env{&CuraFormulaeEngine::env::std_env};
    custom_env.set("dummy_array", std::vector<CuraFormulaeEngine::eval::Value>{
            CuraFormulaeEngine::eval::Value(std::vector<CuraFormulaeEngine::eval::Value>{int64_t(0)})});

    const auto message = CuraFormulaeEngine::parser::parse(input);

    REQUIRE(message.has_value());
    const auto &ast = message.value();
    const auto eval = ast.evaluate(&custom_env);
    REQUIRE(eval.has_value());
}

TEST_CASE("fn round", "[parser, fn]")
{
    auto input = "round(1.5)"sv;
    const auto expected_ast = make_expr_ptr<VariableExpr>("round")(make_expr_ptr<FloatExpr>(1.5));
    const auto expected_eval = CuraFormulaeEngine::eval::Value(int64_t(2));

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("fn round base=1", "[parser, fn]")
{
    auto input = "round(1.5, 1)"sv;
    const auto expected_ast = make_expr_ptr<VariableExpr>("round")(make_expr_ptr<FloatExpr>(1.5),
                                                                   make_expr_ptr<IntExpr>(int64_t(1)));
    const auto expected_eval = CuraFormulaeEngine::eval::Value(1.5);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("fn round base=-1", "[parser, fn]")
{
    auto input = "round(15, -1)"sv;
    const auto expected_ast = make_expr_ptr<VariableExpr>("round")(make_expr_ptr<IntExpr>(int64_t(15)),
                                                                   -make_expr_ptr<IntExpr>(int64_t(1)));
    const auto expected_eval = CuraFormulaeEngine::eval::Value(int64_t(20));

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("fn round base=5", "[parser, fn]")
{
    auto input = "round(0.2*60/100, 5)"sv;
    const auto expected_ast = make_expr_ptr<VariableExpr>("round")(
            make_expr_ptr<FloatExpr>(0.2) * make_expr_ptr<IntExpr>(60) / make_expr_ptr<IntExpr>(100),
            make_expr_ptr<IntExpr>(int64_t(5)));
    const auto expected_eval = CuraFormulaeEngine::eval::Value(0.12);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("fn any trailing comma", "[parser, fn]")
{
    auto input = "any([True], )"sv;
    const auto expected_ast = make_expr_ptr<VariableExpr>("any")(make_list_expr(make_expr_ptr<BoolExpr>(true)));
    const auto expected_eval = CuraFormulaeEngine::eval::Value(true);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&CuraFormulaeEngine::env::std_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("fn list comprehension", "[parser, fn]")
{
    auto input = "all(x <= 10 for x in dummy_array)"sv;
    auto loops = std::vector<ListComprehensionExpr::loop>{};
    loops.emplace_back(make_expr_ptr<VariableExpr>("x"), make_expr_ptr<VariableExpr>("dummy_array"),
                       std::vector<ExprPtr>{});
    const auto expected_ast
            = make_expr_ptr<VariableExpr>("all")(
                    make_expr_ptr<ListComprehensionExpr>(make_expr_ptr<VariableExpr>("x") <= make_expr_ptr<IntExpr>(10),
                                                         std::move(loops)));
    const auto expected_eval = CuraFormulaeEngine::eval::Value(true);
    CuraFormulaeEngine::env::LocalEnvironment custom_env{&CuraFormulaeEngine::env::std_env};
    custom_env.set("dummy_array",
                   std::vector<CuraFormulaeEngine::eval::Value>{int64_t(0), int64_t(1), int64_t(2), int64_t(3), int64_t(4),
                                                     int64_t(5), int64_t(6), int64_t(7), int64_t(8), int64_t(9),
                                                     int64_t(10)});

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
    const auto eval = ast.evaluate(&custom_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("parse endcode", "[parser, real example]")
{
    auto input
            = "\";Version _2.6 of the firmware can abort the print too early if the file ends\\n;too soon. However if the file hasn't ended yet because there are comments at\\n;the end of the file, it won't abort yet. Therefore we have to put at least 512\\n;bytes at the end of the g-code so that the file is not yet finished by the\\n;time that the motion planner gets flushed. With firmware version _3.3 this\\n;should be fixed, so this comment wouldn't be necessary any more. Now we have\\n;to pad this text to make precisely 512 bytes.\"  if machine_gcode_flavor == \"UltiGCode\" else \"M104 S0 ;extruder heater off\\nM140 S0 ;heated bed heater off (if you have it)\\nG91 ;relative positioning\\nG1 E-1 F300  ;retract the filament a bit before lifting the nozzle, to release some of the pressure\\nG1 Z+0.5 E-5 X-20 Y-20 F9000 ;move Z up a bit and retract filament even more\\nG28 X0 Y0 ;move X/Y to min endstops, so the head is out of the way\\nM84 ;steppers off\\nG90 ;absolute positioning\\n;Version _2.6 of the firmware can abort the print too early if the file ends\\n;too soon. However if the file hasn't ended yet because there are comments at\\n;the end of the file, it won't abort yet. Therefore we have to put at least 512\\n;bytes at the end of the g-code so that the file is not yet finished by the\\n;time that the motion planner gets flushed. With firmware version _3.3 this\\n;should be fixed, so this comment wouldn't be necessary any more. Now we have\\n;to pad this text to make precisely 512 bytes.\""sv;
    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
}

TEST_CASE("simple var free variable", "[free variables, variable]")
{
    auto ast = make_expr_ptr<VariableExpr>(std::string("foo"));
    auto free_variables = ast.freeVariables();
    REQUIRE(free_variables.size() == 1);
    REQUIRE(free_variables.contains(std::string("foo")));
}

TEST_CASE("float has no free variable", "[free variables, float]")
{
    auto ast = make_expr_ptr<FloatExpr>(1.0);
    auto free_variables = ast.freeVariables();
    REQUIRE(free_variables.empty());
}

TEST_CASE("int has no free variable", "[free variables, int]")
{
    auto ast = make_expr_ptr<IntExpr>(1);
    auto free_variables = ast.freeVariables();
    REQUIRE(free_variables.empty());
}

TEST_CASE("string has no free variable", "[free variables, string]")
{
    auto ast = make_expr_ptr<StringExpr>(std::string("hello world"));
    auto free_variables = ast.freeVariables();
    REQUIRE(free_variables.empty());
}

TEST_CASE("bool has no free variable", "[free variables, bool]")
{
    auto ast = make_expr_ptr<BoolExpr>(true);
    auto free_variables = ast.freeVariables();
    REQUIRE(free_variables.empty());
}

TEST_CASE("free_variables(foo()) = { foo }", "[free variables, fn]")
{
    auto ast = make_expr_ptr<FnApplicationExpr>(make_expr_ptr<VariableExpr>(std::string("foo")),
                                                std::vector<ExprPtr>{});
    auto free_variables = ast.freeVariables();
    REQUIRE(free_variables.size() == 1);
    REQUIRE(free_variables.contains(std::string("foo")));
}

TEST_CASE("free_variables(x^2) = { x }", "[free variables, x^2]")
{
    auto ast = make_expr_ptr<PowExpr>(make_expr_ptr<VariableExpr>(std::string("x")), make_expr_ptr<IntExpr>(2));
    auto free_variables = ast.freeVariables();
    REQUIRE(free_variables.size() == 1);
    REQUIRE(free_variables.contains(std::string("x")));
}

TEST_CASE("free_variables([1, x, 2, y, 3]) = { x, y }", "[free variables, list]")
{
    auto ast = make_list_expr(
            make_expr_ptr<IntExpr>(1),
            make_expr_ptr<VariableExpr>(std::string("x")),
            make_expr_ptr<IntExpr>(2),
            make_expr_ptr<VariableExpr>(std::string("y")),
            make_expr_ptr<IntExpr>(3));
    auto free_variables = ast.freeVariables();
    REQUIRE(free_variables.size() == 2);
    REQUIRE(free_variables.contains(std::string("x")));
    REQUIRE(free_variables.contains(std::string("y")));
}

TEST_CASE("free_variables([x for x in dummy_array]) = { dummy_array }", "[free variables, list comprehension]")
{
    auto loops = std::vector<ListComprehensionExpr::loop>{};
    loops.emplace_back(make_expr_ptr<VariableExpr>(std::string("x")),
                       make_expr_ptr<VariableExpr>(std::string("dummy_array")), std::vector<ExprPtr>{});
    auto ast = make_expr_ptr<ListComprehensionExpr>(make_expr_ptr<VariableExpr>(std::string("x")), std::move(loops));
    auto free_variables = ast.freeVariables();
    REQUIRE(free_variables.size() == 1);
    REQUIRE(free_variables.contains(std::string("dummy_array")));
}

TEST_CASE("free_variables([x for x in x]) = { x }", "[free variables, list comprehension]")
{
    auto loops = std::vector<ListComprehensionExpr::loop>{};
    loops.emplace_back(make_expr_ptr<VariableExpr>(std::string("x")), make_expr_ptr<VariableExpr>(std::string("x")),
                       std::vector<ExprPtr>{});
    auto ast = make_expr_ptr<ListComprehensionExpr>(make_expr_ptr<VariableExpr>(std::string("x")), std::move(loops));
    auto free_variables = ast.freeVariables();
    REQUIRE(free_variables.size() == 1);
    REQUIRE(free_variables.contains(std::string("x")));
}

TEST_CASE("free_variables([item for row in matrix for item in row]) = { matrix }",
          "[free variables, list comprehension]")
{
    auto loops = std::vector<ListComprehensionExpr::loop>{};
    loops.emplace_back(make_expr_ptr<VariableExpr>(std::string("row")),
                       make_expr_ptr<VariableExpr>(std::string("matrix")), std::vector<ExprPtr>{});
    loops.emplace_back(make_expr_ptr<VariableExpr>(std::string("item")),
                       make_expr_ptr<VariableExpr>(std::string("row")), std::vector<ExprPtr>{});
    auto ast = make_expr_ptr<ListComprehensionExpr>(make_expr_ptr<VariableExpr>(std::string("item")), std::move(loops));
    auto free_variables = ast.freeVariables();
    REQUIRE(free_variables.size() == 1);
    REQUIRE(free_variables.contains(std::string("matrix")));
}

TEST_CASE("real world expression",
          "[parser, fdm_printer.def.json example]")
{
    auto input = "int(defaultExtruderPosition()) if resolveOrValue('adhesion_type') == 'raft' else -1"sv;

    const auto expected_ast = make_expr_ptr<ConditionExpr>(
            make_expr_ptr<VariableExpr>(std::string("int"))(
                    make_expr_ptr<VariableExpr>(std::string("defaultExtruderPosition"))()),
            make_expr_ptr<VariableExpr>(std::string("resolveOrValue"))(
                    make_expr_ptr<StringExpr>(std::string("adhesion_type"))) ==
            make_expr_ptr<StringExpr>(std::string("raft")),
            -make_expr_ptr<IntExpr>(int64_t(1)));

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
    REQUIRE(ast.deepEq(expected_ast));
}

TEST_CASE("prime tower parser", "[parser, fdm_printer.def.json example]")
{
    auto input
            = "'none' if (extruders_enabled_count < 2) else ('interleaved' if (all(material_type_var == extruderValues('material_type')[0] for material_type_var in extruderValues('material_type')) and all(material_brand_var == extruderValues('material_brand')[0] for material_brand_var in extruderValues('material_brand'))) else 'normal')"sv;

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();
}

TEST_CASE("s8 bottom_thickness setting parser", "[parser, fdm_printer.def.json example]")
{
    auto input = "3*layer_height if top_layers==4 and not support_enable else top_bottom_thickness"sv;
    const auto expected_eval = CuraFormulaeEngine::eval::Value(0.6018);

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();

    CuraFormulaeEngine::env::LocalEnvironment custom_env{&CuraFormulaeEngine::env::std_env};
    custom_env.set("layer_height", 0.2006);
    custom_env.set("top_layers", int64_t(4));
    custom_env.set("support_enable", false);
    custom_env.set("top_bottom_thickness", 0.8);

    const auto eval = ast.evaluate(&custom_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}

TEST_CASE("Falcon infill", "[parser, fdm_printer.def.json example]")
{
    auto input = "[-40,50] if infill_pattern in ('grid', 'lines', 'zigzag')  else [ ]"sv;
    const auto expected_eval = CuraFormulaeEngine::eval::Value{std::vector<CuraFormulaeEngine::eval::Value>{int64_t(-40), int64_t(50)}};

    const auto message = CuraFormulaeEngine::parser::parse(input);
    REQUIRE(message.has_value());
    const auto &ast = message.value();

    CuraFormulaeEngine::env::LocalEnvironment custom_env{&CuraFormulaeEngine::env::std_env};
    custom_env.set("infill_pattern", std::string("grid"));

    const auto eval = ast.evaluate(&custom_env);
    REQUIRE(eval.has_value());
    REQUIRE(eval.value().deepEq(expected_eval));
}
