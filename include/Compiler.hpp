#ifndef COMPILER_HPP_
#define COMPILER_HPP_

#include "Types_Compiler.hpp"
#include "Lang.hpp"
#include "Grammar.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Logger.hpp"
#include <fstream>

namespace IStudio::Compiler
{

    class Compiler {
    public:
        class Config
        {
        private:
            Lang::String filename;
            Lang::Integer column;
            Lang::Integer line;
            Lang::Integer index;

        public:
            Config(const Lang::String &filename, const Lang::Integer column = 1, const Lang::Integer line = 1, const Lang::Integer index = 1)
                : filename(filename), column(column), line(line), index(index) {}

            const Lang::String &getFilename() const noexcept { return filename; }
            const Lang::Integer &getColumn() const noexcept { return column; }
            const Lang::Integer &getLine() const noexcept { return line; }
            const Lang::Integer &getIndex() const noexcept { return index; }

            void setFilename(const Lang::String &filename) { this->filename = filename; }
            void setColumn(const Lang::Integer &column) { this->column = column; }
            void setLine(const Lang::Integer &line) { this->line = line; }
            void setIndex(const Lang::Integer &index) { this->index = index; }

            Config(const Config &other) = default;
            Config &operator=(const Config &other) = default;
            Config(Config &&other) noexcept = default;
            Config &operator=(Config &&other) noexcept = default;
        };

    private:
        Lexer lexer;
        Parser parser;
        IStudio::Log::Logger logger;

    public:
        Compiler(const Grammar& grammar, IStudio::Log::Logger logger = Logger("logfile.txt", LogLevel::DEBUG, /*depth=*/0, /*defaultDepth=*/2))
            : lexer(grammar.getTerminals(), grammar.getSkipTerminals(), logger),
              parser(grammar, logger),
              logger(std::move(logger))
        {
            this->logger.setDefaultLogLevel({LogLevel::DEBUG, LogLevel::INFO});
            this->logger.setDefaultDepth(1);

            std::ofstream file {"my parser table.txt"};
            parser.summary(file);
            file << grammar;
        }

        friend std::shared_ptr<ASTNode> operator|(const std::string code, Compiler &c)
        {
            return code | c.lexer | c.parser;
        }
    };

} // namespace IStudio::Compiler

#endif // COMPILER_HPP_
