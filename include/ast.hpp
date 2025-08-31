#ifndef __AST_HPP__
#define __AST_HPP__
#include "Types_Compiler.hpp"

namespace IStudio::Compiler
{
    class ASTNode
    {
    public:
        Symbol symbol;
        std::vector<std::shared_ptr<ASTNode>> children;

        ASTNode(Symbol s) : symbol(std::move(s)) {}

        void addChild(const std::shared_ptr<ASTNode> &child)
        {
            children.push_back(child);
        }

        friend std::ostream &operator<<(std::ostream &os, const ASTNode &node)
        {
            os << node.symbol;
            return os;
        }
    };
    class ASTPrinter
    {
    public:
        static void print(
            const std::shared_ptr<ASTNode> &node,
            IStudio::Log::Logger &logger,
            const std::string &prefix = "",
            bool isLast = true,
            IStudio::Log::LogLevel level = IStudio::Log::LogLevel::INFO,
            int depth = 1)
        {
            using namespace IStudio::Log;

            if (!node)
                return;

            std::ostringstream oss;
            oss << prefix;
            oss << (isLast ? "└── " : "├── ");
            oss << node->symbol;
            logger(level, depth) << oss.str();

            for (size_t i = 0; i < node->children.size(); ++i)
            {
                bool last = (i == node->children.size() - 1);
                std::string newPrefix = prefix + (isLast ? "    " : "│   ");
                print(node->children[i], logger, newPrefix, last, level, depth);
            }
        }
    };

} // namespace IStudio::Compiler

#endif // !__AST_HPP__