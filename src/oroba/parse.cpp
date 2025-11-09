#include "oroba/parse.hpp"

#include <sstream>
#include <optional>

#include "oroba/object/collections.hpp"
#include "oroba/object/primitives.hpp"

using namespace std;

optional<OrobaError> parse_expr(istream& in, Bytecode& out, LocalCollector& collector);
optional<OrobaError> parse_message(istream& in, Bytecode& out, LocalCollector& collector);
optional<OrobaError> parse_number(istream& in, Bytecode& out, LocalCollector& collector);
optional<OrobaError> parse_string(istream& in, Bytecode& out, LocalCollector& collector);
optional<OrobaError> error(string message);
void consume_whitespace(istream& in);
bool is_whitespace(int c);
bool is_specialchar(int c);

variant<Bytecode, OrobaError> parse(istream& in, LocalCollector& collector) {
    Bytecode bytecode;
    optional<OrobaError> out = parse_expr(in, bytecode, collector);
    if (out.has_value())
        return out.value();
    else
        return bytecode;
}
    
optional<OrobaError> parse_expr(istream& in, Bytecode& out, LocalCollector& collector) {
    consume_whitespace(in);
    int c = in.peek();
    if (c == EOF) return error("error - end of file!");

    if (c == '(')
        return error("not parsing groupings yet");
    else if (c == '{')
        return error("not parsing objects yet");
    else if (c == '[')
        return error("not parsing blocks yet");
    else if (isdigit(c))
        return parse_number(in, out, collector);
    else if (c == '"')
        return parse_string(in, out, collector);
    else if (is_specialchar(c))
        return error("unexpected special char");
    else 
        return parse_message(in, out, collector);
}

optional<OrobaError> parse_message(istream& in, Bytecode& out, LocalCollector& collector) {
    return error("not implemented - parse message");
}

optional<OrobaError> parse_number(istream& in, Bytecode& out, LocalCollector& collector) {
    int64_t val = 0;
    int64_t digit = 1;
    int c = in.peek();
    while (isdigit(c)) {
        val += (c - 48) * digit;
        in.get();
        c = in.peek();
    }
    if (is_whitespace(c)) {
        OrobaObject* lit = new IntegerObject(val);
        collector.Add(lit);
        out.ops.push_back(OpCode::push(lit));
        return std::nullopt;
    } else {
        return error("encountered non-numeric character when parsing number!");
    }
}

optional<OrobaError> parse_string(istream& in, Bytecode& out, LocalCollector& collector) {
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

optional<OrobaError> error(string message) {
    return OrobaError{message, 0, 0};
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
