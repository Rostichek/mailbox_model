#pragma once
#include <unordered_map>
#include <string>
#include "Mailbox.h"

class Terminal {
public:
	using code = size_t;
	using price = size_t;

	Terminal(std::unordered_map<code, std::string> cities, std::unordered_map<code, price> prices, Mailbox& mail, size_t id);

private:
	const std::unordered_map<code, std::string> cities;
	const std::unordered_map<code, price> prices;
	Mailbox& mail;
	size_t id;

	std::string menu;
	const size_t basic_price = 100;

	std::string GetMenu();
	size_t Process(size_t input);
	Header::Response WaitResponse(size_t id);
};

