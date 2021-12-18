#include <iostream>
#include <exception>
#include <sstream>
#include <string>
#include <vector>
#include <array>

#include <cstdio>
#include <filesystem>
#include <fstream>
#include "MoneyChanger.h"
#include "Mailbox.h"
#include <fstream>
#include "Terminal.h"

using namespace std;

void StartTerminal(size_t t_number) {
	unordered_map<Terminal::code, std::string> cities = {
		{1, "Kyiv"},
		{2, "Moscow"},
		{3, "London"},
		{4, "Berlin"},
		{5, "Paris"},
	};
	unordered_map<Terminal::code, Terminal::price> prices = {
		{1, 28},
		{2, 37},
		{3, 50},
		{4, 77},
		{5, 91},
	};
	Mailbox mail("123");

	Terminal terminal(cities, prices, mail, t_number);
}

void StartChanger() {
	MoneyChanger changer(
		{
			{ 1, 50 },
			{ 2, 25 },
			{ 5, 20 },
			{ 10, 15 },
			{ 25, 0 }, // 10
			{ 50, 0 } // 5
		});

	Mailbox mail("123");
	while (1) {
		if (mail.IsRequest()) {
			auto request = mail.GetRequest();
			auto result = changer.GetChange(request.change);
			if (result.has_value())
				mail.Response(request.id, true, result.value());
			else 
				mail.Response(request.id, false);
		}
		this_thread::sleep_for(chrono::seconds(5));
	}
}

void SetProgram(int argc, char* argv[]) {
	std::stringstream err;
	if (argc != 2) {
		err << argv[0] << " must contain only one argument";
		throw std::invalid_argument(err.str());
	}
	
	std::string arg(argv[1]);
	if (arg == "-t" || arg == "terminal") {
		StartTerminal(1);
	}
	else if (arg == "-c" || arg == "changer") {
		StartChanger();
	}
	else {
		err << argv[0] << ": allowed arguments: -t (terminal) or -c (changer)";
		throw std::invalid_argument(err.str());
	}
}

template<typename Key, typename Val>
std::ostream& operator<< (std::ostream& os, std::map<Key, Val> container) {
	os << "{ ";
	bool is_first = true;
	for (const auto& el : container) {
		if(!is_first) os << ", ";
		os << " { " << el.first << ", " << el.second << " } ";
		is_first = false;
	}
	os << " }";
	return os;
}

int main(int argc, char* argv[]) {
	//StartTerminal(1);
	//return 5;
	try {
		SetProgram(argc, argv);
	}
	catch (std::exception& ex) {
		std::cerr << ex.what() << std::endl;
		return 1;
	}
    return 0;
}

