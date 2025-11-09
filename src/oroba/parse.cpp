#include "oroba/parse.hpp"

#include <sstream>
#include <optional>

#include "oroba/object/collections.hpp"
#include "oroba/object/primitives.hpp"

using namespace std;

optional<ParseError> parse_expr(istream& in, Bytecode& out, LocalCollector& collector);
optional<ParseError> parse_message(bool has_target, istream& in, Bytecode& out, LocalCollector& collector);
optional<ParseError> parse_number(istream& in, Bytecode& out, LocalCollector& collector);
optional<ParseError> parse_string(istream& in, Bytecode& out, LocalCollector& collector);
optional<ParseError> error(string message);
void consume_whitespace(istream& in);
bool is_whitespace(int c);
bool is_specialchar(int c);

variant<Bytecode, ParseError> parse(istream& in, LocalCollector& collector) {
    Bytecode bytecode;
    optional<ParseError> out = parse_expr(in, bytecode, collector);
    if (out.has_value())
        return out.value();
    else
        return bytecode;
}
    
optional<ParseError> parse_expr(istream& in, Bytecode& out, LocalCollector& collector) {
    bool has_target = false;
    while (true) {
        consume_whitespace(in);
        int c = in.peek();
        if (c == EOF) return error("error - end of file!");

        if (c == '.') {
            in.get();
            return std::nullopt;
        }
        if (c == '(')
            return error("not parsing groupings yet");
        else if (c == '{')
            return error("not parsing objects yet");
        else if (c == '[')
            return error("not parsing blocks yet");
        else if (isdigit(c)) {
            auto result = parse_number(in, out, collector);
            if (result.has_value()) return result;
        }
        else if (c == '"') {
            auto result = parse_string(in, out, collector);
            if (result.has_value()) return result;
        }
        else if (is_specialchar(c)) {
            return error("unexpected special char");
        }
        else {
            auto result = parse_message(has_target, in, out, collector);
            if (result.has_value()) return result;
        }
        has_target = true;
    }
}

optional<ParseError> parse_message(bool has_target, istream& in, Bytecode& out, LocalCollector& collector) {
    int c = in.peek();
    ostringstream messagename;
    while (!isdigit(c) && !is_specialchar(c) && !is_whitespace(c) && c != EOF) {
        messagename << (char)c;
        in.get();
        c = in.peek();
    }
    if (c == ':') {
        return error("not implemented - parse message with argument");
    } else {
        // no argument
        if (has_target) {
            out.ops.push_back(OpCode::expl_message(messagename.str(), 0));
        } else {
            out.ops.push_back(OpCode::impl_message(messagename.str(), 0));
        }
    }
    return nullopt;
}

optional<ParseError> parse_number(istream& in, Bytecode& out, LocalCollector& collector) {
    int64_t val = 0;
    int64_t digit = 1;
    int c = in.peek();
    while (isdigit(c)) {
        val += (c - 48) * digit;
        in.get();
        c = in.peek();
    }
    if (is_whitespace(c) || c == '.') {
        OrobaObject* lit = new IntegerObject(val);
        collector.Add(lit);
        out.ops.push_back(OpCode::push(lit));
        return std::nullopt;
    } else {
        return error("encountered non-numeric character when parsing number!");
    }
}

optional<ParseError> parse_string(istream& in, Bytecode& out, LocalCollector& collector) {
    // consume starting '"'
    in.get();
    ostringstream oss;
    while (in.peek() != '"') {
        oss << (char)in.get();
    }
    in.get();
    OrobaObject* lit = new StringObject(oss.str());
    collector.Add(lit);

    out.ops.push_back(OpCode::push(lit));
    return std::nullopt;
}

optional<ParseError> error(string message) {
    return ParseError{message, 0, 0};
}

void consume_whitespace(istream& in) {
    while (is_whitespace(in.peek())) {
        in.get();
    }
}

bool is_whitespace(int c) {
    return c == ' '
        || c == '\r'
        || c == '\n'
        || c == '\t';
}

bool is_specialchar(int c) {
    return c == '('
        || c == ')'
        || c == '['
        || c == ']'
        || c == '{'
        || c == '}'
        || c == '.'
        || c == '"';
}
