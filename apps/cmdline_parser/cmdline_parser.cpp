#include <cura-formulae-engine/cura-formulae-engine.h>
#include <spdlog/spdlog.h>

#include <iostream>
#include <string>
#include <string_view>

int main(int argc, const char** argv)
{
    spdlog::set_level(spdlog::level::info);
    spdlog::info("Formula REPL, type 'exit' to quit.");

    while (true)
    {
        std::cout << "> ";
        std::string command;
        std::getline(std::cin, command);
        if (command == "exit")
        {
            break;
        }

        const auto input = std::string_view(command.begin(), command.end());
        auto message = CuraFormulaeEngine::parser::parse(input);

        if (!message.has_value())
        {
            spdlog::warn("Failed to parse input.");
            continue;
        }

        const auto& expr = message.value();
        const auto eval_result = expr.evaluate(&CuraFormulaeEngine::env::std_env);

        if (!eval_result.has_value())
        {
            spdlog::warn("Expr {} results in eval error", expr.toString());
            continue;
        }

        const auto& eval = eval_result.value();
        spdlog::info("{} = {}", expr.toString(), eval.toString());
    }

    return 0;
}
