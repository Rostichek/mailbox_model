#include "Terminal.h"
#include <iostream>
#include <sstream>
#include <ctime>
#include <chrono>
#include <thread>

using namespace std;

Terminal::Terminal(unordered_map<code, std::string> cities, std::unordered_map<code, price> prices, Mailbox& mail, size_t id) : cities(cities), prices(prices), mail(mail), id(id) {
	menu = GetMenu();
	size_t input;
	while (1) {
		cout << menu << endl;
		cin >> input;
		if (!input) return;
		cout << "\n\n";
		Header::Response response = WaitResponse(Process(input));
		if (response.is_changed)
			cout << response.coins << endl;
		else 
			cout << "Can't be changed;" << endl;
		system("pause");
		system("cls");
	}
}

string Terminal::GetMenu() {
	stringstream ss;

	ss << "0 : " << "Shut down terminal" << endl;
	for (const auto& city : cities)
		ss << city.first << " : " << city.second << " - " << prices.at(city.first) << endl;

	return ss.str();
}

size_t Terminal::Process(size_t input) {
	stringstream ss;
	ss << id << input << rand() % 255;
	mail.Request(atoi(ss.str().c_str()), basic_price - prices.at(input));
	return atoi(ss.str().c_str());
}

Header::Response Terminal::WaitResponse(size_t id) {
	while (true) {
		if (mail.IsResponse(id)) return mail.GetResponse(id);
		this_thread::sleep_for(chrono::seconds(3));
	}
}
