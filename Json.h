#pragma once

#include <istream>
#include <map>
#include <string>
#include <ostream>
#include <variant>
#include <vector>

namespace Json {

    class Node : std::variant<std::vector<Node>,
        std::map<std::string, Node>,
        bool,
        double,
        std::string> {
    public:
        using variant::variant;

        auto& AsArray() {
            return std::get<std::vector<Node>>(*this);
        }
        auto& AsMap() {
            return std::get<std::map<std::string, Node>>(*this);
        }
        double AsNumber() {
            return std::get<double>(*this);
        }
        auto& AsString() {
            return std::get<std::string>(*this);
        }
        auto& AsBool() {
            return std::get<bool>(*this);
        }
    };

    class Document {
    public:
        explicit Document(Node root);

        Node& GetRoot();

    private:
        Node root;
    };

    Document Load(std::istream& input);
}

std::ostream& operator<< (std::ostream& os, Json::Node& node);
std::ostream& operator<< (std::ostream& os, Json::Document& doc);