#include <iostream>
#include <Grammar.hpp>
#include <Exception.hpp>
#include <Nonterminal.hpp>
#include <Terminal.hpp>
#include <Lexer.hpp>
#include <Compiler.hpp>
#include <fs/File.hpp>
#include <Logger.hpp>


int main()
{
	using namespace IStudio::Compiler;
	using namespace IStudio::Log;

	Logger logger("logfile.txt", LogLevel::DEBUG);

	// Setup logger
	logger.setDefaultLogLevel({LogLevel::DEBUG, LogLevel::INFO, LogLevel::ERROR});
	logger.setDefaultDepth(2);


	logger(LogLevel::INFO) << "Starting Compiler Program...";

	try {
		// Define grammar elements
		logger(LogLevel::DEBUG) << "Defining NonTerminals...";
		Nonterminal start{"start"};
		Nonterminal ImportStatement{"ImportStatement"};
		Nonterminal package{"package"};
		Nonterminal packages{"packages"};

	

		logger(LogLevel::DEBUG) << "Defining Terminals...";
		Terminal import{"import", "import", 10, Associativity::LEFT, TerminalType::KEYWORD};
		Terminal from{"from", "from", 10, Associativity::LEFT, TerminalType::KEYWORD};
		Terminal as{"as", "as", 10, Associativity::LEFT, TerminalType::KEYWORD};
		Terminal semicolon{"semicolon", ";", 400, Associativity::LEFT, TerminalType::SEPARATOR};
		Terminal space{"space", "\\s", 500, Associativity::LEFT, TerminalType::SPECIAL};
		Terminal newline{"newline", "\r\n|\r|\n", 500, Associativity::LEFT, TerminalType::SPECIAL};
		Terminal identifier{"identifier", "[a-zA-Z_][a-zA-Z0-9_]*", 1000, Associativity::LEFT, TerminalType::IDENTIFIER};


		logger(LogLevel::DEBUG) << "Creating rules...";
		Rule FirstRule = start <= rule(ImportStatement);


		logger(LogLevel::INFO) << "Constructing grammar...";
		Grammar grammar{
			start,
			Grammar::Terminals_Type{import, from, as, identifier, semicolon},
			Grammar::Terminals_Type{space, newline},
			Grammar::NonTerminals_Type{start, ImportStatement, package, packages},
			FirstRule,
			Grammar::Rules_Type{
				FirstRule,
				ImportStatement <= rule(from, package, import, package, semicolon),
				ImportStatement <= rule(from, package, import, packages, semicolon),
				ImportStatement <= rule(from, package, import, package, as, identifier, semicolon),
				ImportStatement <= rule(),
				package <= rule(identifier)
			},
			logger
		};


		logger(LogLevel::INFO) << "Grammar constructed successfully.";

		logger(LogLevel::INFO) << "Initializing compiler with grammar.";
		Compiler compiler{grammar,logger};

		std::string code = "from IStudio import Lang ;";
		logger(LogLevel::INFO) << "Source Code: " << code;

		logger(LogLevel::INFO) << "Running compiler...";
		auto root = code | compiler;
		if (!root)
		{
			logger(LogLevel::INFO) << "[SyntaxAnalyser] Empty AST.\n";
		}
		else{
			logger(LogLevel::INFO) << "[SyntaxAnalyser] Analyzing AST:\n";
			ASTPrinter::print(root,logger);
		}

		// std::cout << "Parser : " << ((success_code) ? "Passed" : "Not Passed") << std::endl;
	}
	catch (const IStudio::Exception::Exception &e)
	{
		logger(LogLevel::CRITICAL) << "Custom Exception: " << e.what();
		std::cerr << "Exception: " << e.what() << std::endl;
	}
	catch (const std::exception &e)
	{
		logger(LogLevel::CRITICAL) << "Standard Exception: " << e.what();
		std::cerr << "Standard Exception: " << e.what() << std::endl;
	}
	catch (...)
	{
		logger(LogLevel::CRITICAL) << "Unknown Exception caught";
		std::cerr << "Unknown Exception caught" << std::endl;
	}

	logger(LogLevel::INFO) << "Program terminated.";
	return 0;
}
