#include "Json.h"

using namespace std;

namespace Json {

    Document::Document(Node root) : root(move(root)) {}

    Node& Document::GetRoot() {
        return root;
    }

    Node LoadNode(istream& input);

    Node LoadArray(istream& input) {
        vector<Node> result;

        for (char c; input >> c && c != ']'; ) {
            if (c != ',') {
                input.putback(c);
            }
            result.push_back(LoadNode(input));
        }

        return Node(move(result));
    }

    Node LoadNumber(istream& input) {
        double result;
        input >> result;
        return Node(result);
    }

    Node LoadString(istream& input) {
        string line;
        getline(input, line, '"');
        return Node(move(line));
    }

    Node LoadBool(istream& input) {
        string line;
        char c;
        for (int i = 0; i < 4; ++i) {
            input >> c;
            line.push_back(c);
        }
        if (line == "true")
            return Node(true);
        input >> c;
        return Node(false);
    }

    Node LoadDict(istream& input) {
        map<string, Node> result;

        for (char c; input >> c && c != '}'; ) {
            if (c == ',') {
                input >> c;
            }

            string key = LoadString(input).AsString();
            input >> c;
            result.emplace(move(key), LoadNode(input));
        }

        return Node(move(result));
    }

    Node LoadNode(istream& input) {
        char c;
        input >> c;

        if (c == '[') {
            return LoadArray(input);
        }
        else if (c == '{') {
            return LoadDict(input);
        }
        else if (c == '"') {
            return LoadString(input);
        }
        else if (c == 't' || c == 'f') {
            input.putback(c);
            return LoadBool(input);
        }
        else {
            input.putback(c);
            return LoadNumber(input);
        }
    }

    Document Load(istream& input) {
        return Document{ LoadNode(input) };
    }
}

ostream& operator<< (ostream& os, Json::Node& node) {
    try {
        auto val = node.AsArray();
        os << "[";
        bool is_first = true;
        for (auto& element : val) {
            if (!is_first)
                os << ", ";
            os << element;
            is_first = false;
        }
        os << "]";
        return os;
    }
    catch (exception& ex) {}
    try {
        auto val = node.AsMap();
        os << "{";
        bool is_first = true;
        for (auto& element : val) {
            if (!is_first)
                os << ", ";
            os << "\"" << element.first << "\" : " << element.second;
            is_first = false;
        }
        os << "}";
        return os;
    }
    catch (exception& ex) {}
    try {
        auto val = node.AsString();
        os << "\"" << val << "\"";
        return os;
    }   
    catch (exception& ex) {}
    try {
        auto val = node.AsNumber();
        os << val;
        return os;
    }
    catch (exception& ex) {}
    try {
        auto val = node.AsBool();
        os << (val ? "true" : "false");
        return os;
    }
    catch (exception& ex) {
        throw exception("Unknown JSON type");
    }
    return os;
}

ostream& operator<< (ostream& os, Json::Document& doc) {
    os << doc.GetRoot();
    return os;
}