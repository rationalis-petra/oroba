#include "oroba/parse.hpp"

#include <sstream>
#include <optional>

#include "oroba/object/collections.hpp"
#include "oroba/object/primitives.hpp"

using namespace std;

optional<ParseError> parse_statement(istream& in, int terminator,  Bytecode& out,LocalCollector& collector);
optional<ParseError> parse_expr(istream& in, Bytecode& out, LocalCollector& collector);
optional<ParseError> parse_object(istream& in, Bytecode& out, LocalCollector& collector);
optional<ParseError> parse_block(istream& in, Bytecode& out, LocalCollector& collector);
optional<ParseError> parse_message(bool has_target, istream& in, Bytecode& out, LocalCollector& collector);
optional<ParseError> parse_number(istream& in, Bytecode& out, LocalCollector& collector);
optional<ParseError> parse_string(istream& in, Bytecode& out, LocalCollector& collector);
optional<ParseError> error(string message);
void consume_whitespace(istream& in);
bool is_whitespace(int c);
bool is_specialchar(int c);

variant<Bytecode, ParseError> parse(istream& in, LocalCollector& collector) {
    Bytecode bytecode;
    optional<ParseError> out = parse_statement(in, 0, bytecode, collector);
    if (out.has_value())
        return out.value();
    else
        return bytecode;
}
    
optional<ParseError> parse_statement(istream& in, int terminator, Bytecode& out, LocalCollector& collector) {
    bool has_target = false;
    while (true) {
        consume_whitespace(in);
        int c = in.peek();
        if (c == EOF) return error("error - end of file!");

        else if (c == terminator) {
            return std::nullopt;
        } else if (c == '.') {
            in.get();
            if (terminator == 0) {
                return std::nullopt;
            } else {
                consume_whitespace(in);
                int c = in.peek();
                if (c != terminator) {
                    ostringstream oss;
                    oss << "expected terminating character: " << (char)terminator;
                    return error(oss.str());
                } else {
                    return std::nullopt;
                }
            }
        } else if (c == '(') {
            return error("not parsing groupings yet");
        } else if (c == '{') {
            auto result = parse_object(in, out, collector);
            if (result.has_value()) return result;
        } else if (c == '[') {
            auto result = parse_block(in, out, collector);
            if (result.has_value()) return result;
        } else if (isdigit(c)) {
            auto result = parse_number(in, out, collector);
            if (result.has_value()) return result;
        } else if (c == '"') {
            auto result = parse_string(in, out, collector);
            if (result.has_value()) return result;
        } else if (is_specialchar(c)) {
            ostringstream oss;
            oss << "unexpected special char: " << (char)c << " expecting: " << (char)terminator;
            return error(oss.str());
        } else {
            auto result = parse_message(has_target, in, out, collector);
            if (result.has_value()) return result;
        }
        has_target = true;
    }
}

optional<ParseError> parse_object(istream& in, Bytecode& out, LocalCollector& collector) {
    bool has_target = false;
    shared_ptr<Bytecode> object_bytecode(new Bytecode);
    unordered_map<string, SlotDescriptor> slots;
    vector<string> to_initialize;

    // consume '{'
    in.get();
    consume_whitespace(in);
    int c = in.peek();
    // slot descriptors.
    if (c == '|') {
        bool parsing_slots = true;
        while (parsing_slots) {
            // slot descriptors are either as 
            //  slot-name = <statement>.
            //   or 
            //  slot-value <- statement.
            //auto slot_desc = parse_slot(in);

            auto result = parse_statement(in, '|', *object_bytecode, collector);
            if (result.has_value()) return result;
        }
    }

    auto result = parse_statement(in, '}', *object_bytecode, collector);
    in.get();

    if (result.has_value()) return result;

    if (object_bytecode->ops.size() == 0) {
        out.ops.push_back(OpCode::make_object(slots, to_initialize));
    } else {
        out.ops.push_back(OpCode::make_method(slots, to_initialize, object_bytecode));
    }
    return std::nullopt;
}

optional<ParseError> parse_block(istream& in, Bytecode& out, LocalCollector& collector) {
    in.get();
    bool has_target = false;
    shared_ptr<Bytecode> block_bytecode(new Bytecode);
    unordered_map<string, SlotDescriptor> slots;
    vector<string> to_initialize;

    auto result = parse_statement(in, ']', *block_bytecode, collector);
    in.get();

    if (result.has_value()) return result;

    out.ops.push_back(OpCode::make_block(slots, to_initialize, block_bytecode));
    return std::nullopt;
}

optional<ParseError> parse_expr(istream& in, Bytecode& out, LocalCollector& collector) {
    consume_whitespace(in);
    int c = in.peek();
    if (c == EOF) return error("error - end of file!");

    if (c == '.') {
        return error("unexpected end of statement in exprssion.");
    }
    else if (c == '(')
        return error("not parsing groupings yet");
    else if (c == '{')
        return error("not parsing objects yet");
    else if (c == '[')
        return error("not parsing blocks yet");
    else if (isdigit(c)) {
        return parse_number(in, out, collector);
    }
    else if (c == '"') {
        return parse_string(in, out, collector);
    }
    else if (is_specialchar(c)) {
        return error("unexpected special char");
    }
    else {
        // continue to parse message...
        return error("not parsing message within message yet...");
        //auto result = parse_message(false, in, out, collector);
        //if (result.has_value()) return result;
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
    string message = messagename.str();
    if (message.back() == ':') {
        parse_expr(in, out, collector);
        if (has_target) {
            out.ops.push_back(OpCode::expl_message(messagename.str(), 1));
        } else {
            out.ops.push_back(OpCode::impl_message(messagename.str(), 1));
        }
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
