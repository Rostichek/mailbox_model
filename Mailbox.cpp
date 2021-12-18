#include "Mailbox.h"
#include <map>
#include <algorithm>
#include <vector>
#include <fstream>
#include <string>

using namespace std;
using namespace Json;

Mailbox::Mailbox(const string& filename) : filename(filename),
    doc(Node(map<string, Node>{
        { "requests", Node(vector<Node>{}) },
        { "responses", Node(vector<Node>{}) }
    }))
{
    ofstream out(filename);
    out << R"( { "requests" : [], "responses" : [] } )";
}

void Mailbox::Request(size_t id, size_t change) {
    doc = Update();
    Node request(
        map<string, Node>{
            { "id", Node(static_cast<double>(id)) },
            { "change", Node(static_cast<double>(change)) }
    });

    doc.GetRoot().AsMap()["requests"].AsArray().push_back(request);

    Write();
}

void Mailbox::Response(size_t id, bool is_changed, map<size_t, size_t> coins) {
    doc = Update();
    map<string, Node> coins_to_write;
    for (const auto& coin : coins) {
        coins_to_write[to_string(coin.first)] = Node(static_cast<double>(coin.second));
    }
    Node response(
        map<string, Node>{
            { "id", Node(static_cast<double>(id)) },
            { "is_changed", Node(is_changed) },
            { "coins", Node(coins_to_write) }
    });

    doc.GetRoot().AsMap()["responses"].AsArray().push_back(response);

    Write();
}

bool Mailbox::IsRequest() {
    doc = Update();
    return doc.GetRoot().AsMap()["requests"].AsArray().size();
}

bool Mailbox::IsResponse(size_t id) {
    doc = Update();
    auto responses = doc.GetRoot().AsMap()["responses"].AsArray();
    auto it = find_if(responses.begin(), responses.end(), [&id](Node& response) { 
        return response.AsMap().at("id").AsNumber() == id; });
    return it != responses.end();
}

Header::Request Mailbox::GetRequest() {
    auto json_request = doc.GetRoot().AsMap()["requests"].AsArray().back();
    Header::Request result;
    result.id = json_request.AsMap()["id"].AsNumber();
    result.change = json_request.AsMap()["change"].AsNumber();

    doc.GetRoot().AsMap()["requests"].AsArray().pop_back();
    Write();

    return result;
}

Header::Response Mailbox::GetResponse(size_t id) {
    auto finder = [&id](Node& response) {
        return response.AsMap().at("id").AsNumber() == id;
    };

    auto& responses = doc.GetRoot().AsMap()["responses"].AsArray();
    auto it = find_if(responses.begin(), responses.end(), finder);

    Header::Response result;
    result.id = id;
    result.is_changed = it->AsMap().at("is_changed").AsBool();
    result.coins = result.is_changed ? 
        it->AsMap().at("coins").AsMap() :
        map<string, Node>{};

    responses.erase(it);
    Write();

    return result;
}

void Mailbox::Write() {
    while (1) {
        ofstream out(filename);
        if (out.is_open()) {
            out << doc;
            return;
        }
    }
}

Json::Document Mailbox::Update() {
    ifstream in(filename);
    doc = Load(in);
    return doc;
}