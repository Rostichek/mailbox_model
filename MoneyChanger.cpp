#include "MoneyChanger.h"

using namespace std;

MoneyChanger::MoneyChanger(backordered_map cashbox) : cashbox(cashbox) {
	cashbox.key_comp();
}

MoneyChanger::~MoneyChanger() {

}

void MoneyChanger::Add(denomination denom, amount amount) {
	cashbox[denom] += amount;
}

bool MoneyChanger::Substract(denomination denom, amount amount) {
	if (cashbox.count(denom)) {
		if (cashbox[denom] > amount) {
			cashbox[denom] -= amount;
			return true;
		}
	}
	return false;
}

optional<std::map<size_t, size_t>> MoneyChanger::GetChange(size_t change) {
	size_t sum = 0;
	backordered_map temp_cashbox = cashbox;
	map<denomination, amount> change_coins;
	for (auto& coin : change_coins) coin.second = 0;
	
	for(auto& coin : temp_cashbox) {
		while ((sum + coin.first <= change) && coin.second) {
			coin.second--;
			change_coins[coin.first]++;
			sum += coin.first;
		}
	}
	if (sum == change) {
		cashbox = move(temp_cashbox);
		return change_coins;
	}
	return {};
}