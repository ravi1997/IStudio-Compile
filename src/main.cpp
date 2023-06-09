#include <iostream>
#include <Exception.h>
#include <UUID.h>
#include <Nonterminal.hpp>
#include <Terminal.hpp>
#include <Grammer.hpp>
#include <Clouser.hpp>
#include <First.hpp>
#include <Follow.hpp>

int main()
{
	using namespace IStudio::Exception;

	std::cout << "Hello World!\n"
			  << std::endl;

	IStudio::Util::UUID id;
	constexpr IStudio::Compiler::Nonterminal start{"start"};
	constexpr IStudio::Compiler::Nonterminal ImportStatement{"ImportStatement"};
	constexpr IStudio::Compiler::Nonterminal package{"package"};
	constexpr IStudio::Compiler::Nonterminal packages{"packagespackages"};

	constexpr IStudio::Compiler::Terminal import{"import", "import", 1000, IStudio::Compiler::Associativity::LEFT, IStudio::Compiler::TerminalType::KEYWORD};
	constexpr IStudio::Compiler::Terminal from{"from", "from", 1000, IStudio::Compiler::Associativity::LEFT, IStudio::Compiler::TerminalType::KEYWORD};
	constexpr IStudio::Compiler::Terminal as{"as", "as", 1000, IStudio::Compiler::Associativity::LEFT, IStudio::Compiler::TerminalType::KEYWORD};
	constexpr IStudio::Compiler::Terminal identifier{
		"identifier", "^[a-zA-Z_][a-zA-Z0-9_]*$", 10, IStudio::Compiler::Associativity::LEFT, IStudio::Compiler::TerminalType::IDENTIFIER};

					  constexpr IStudio::Compiler::Rule FirstRule = start <= IStudio::Compiler::rule(ImportStatement);

		using namespace IStudio::Compiler;

		constexpr Grammer g{
			start,
			Grammer::Terminals_Type{import},
			Grammer::NonTerminals_Type{start, ImportStatement},
			Grammer::Rules_Type{
				start <= rule(ImportStatement),
				ImportStatement <= rule(from, package, import, package),
				ImportStatement <= rule(from, package, import, packages),
				ImportStatement <= rule(from, package, import, package, as, identifier),
				ImportStatement <= rule()

			}};

		[[maybe_unused]] constexpr auto count = [](auto arr) -> std::size_t
		{
			for (std::size_t i = 0; i < arr.size(); i++)
			{
				if (!arr[i].isValid())
					return i;
			}
			return 0;
		};

		// static_assert(count(g.getRules()) == 2);

		std::cout << g << std::endl;

		constexpr auto myFirst = FIRST(start, g);
		constexpr auto myFollow = FOLLOW(as, g);

		std::cout << "First of the start symbol" << std::endl;
		for (auto f : myFirst | std::views::filter([](auto f)
												   { return f.isValid(); }))
			std::cout << "\t" << f << std::endl;

		std::cout << "Follow of the as symbol" << std::endl;
		for (auto f : myFollow | std::views::filter([](auto f)
													{ return f.isValid(); }))
			std::cout << "\t" << f << std::endl;

		try
		{
			throw Exception{};
	}
	catch (const Exception &e)
	{
		std::cerr << e << std::endl;
	}

	return 0;
}