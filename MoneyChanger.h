#pragma once
#include <unordered_map>
#include <map>
#include <optional>


class MoneyChanger {
public:
	using denomination = size_t;
	using amount = size_t;
	using backordered_map = std::map<denomination, amount, std::greater<size_t>>;

	MoneyChanger(backordered_map);
	~MoneyChanger();

	void Add(denomination, amount);
	bool Substract(denomination, amount);

	const auto& GetCashbox() { return cashbox; }

	std::optional<std::map<size_t, size_t>> GetChange(size_t);
	
private:
	backordered_map cashbox;
};

