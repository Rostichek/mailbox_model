#pragma once
#include <string>
#include "Json.h"
#include <future>

namespace Header {
	struct Request {
		size_t id;
		size_t change;
	};

	struct Response {
		size_t id;
		bool is_changed;
		Json::Node coins;
	};
}

class Mailbox {
public:
	Mailbox(const std::string& filename);
	Mailbox() = delete;

	void Request(size_t id, size_t change);
	void Response(size_t id, bool is_changed, std::map<size_t, size_t> coins = {});

	bool IsRequest();
	bool IsResponse(size_t id);

	Header::Request GetRequest();
	Header::Response GetResponse(size_t);

private:
	std::string filename;
	Json::Document doc;

	Json::Document Update();
	void Write();
};

