#include "oroba/parse.hpp"

#include <sstream>
#include <optional>

#include "oroba/object/composite.hpp"
#include "oroba/object/collections.hpp"
#include "oroba/object/primitives.hpp"

using namespace std;

optional<ParseError> parse_statement(istream& in, int terminator,  Bytecode& out,LocalCollector& collector);
optional<ParseError> parse_expr(istream& in, uint64_t& num_exprs, Bytecode& out, LocalCollector& collector);
optional<ParseError> parse_object(istream& in, Bytecode& out, LocalCollector& collector);
optional<ParseError> parse_block(istream& in, Bytecode& out, LocalCollector& collector);
optional<ParseError> parse_message(bool has_target, istream& in, Bytecode& out, LocalCollector& collector);

variant<ParseError, pair<string, SlotDescriptor>> parse_slot(istream& in);
variant<ParseError, pair<string, MethodDescriptor>> parse_method(istream& in, LocalCollector& collector);
variant<ParseError, string> parse_methodpart(istream& in);
optional<ParseError> parse_number(istream& in, Bytecode& out, LocalCollector& collector);
optional<ParseError> parse_string(istream& in, Bytecode& out, LocalCollector& collector);

optional<ParseError> parse_symbol(istream& in, string symbol);

ParseError error(string message);
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
    bool has_run = false;
    while (true) {
        consume_whitespace(in);
        int c = in.peek();
        if (c == EOF) return error("error - end of file!");

        else if (c == terminator) {
            return std::nullopt;
        } else if (c == '.' || c == ',') {
            in.get();
            return std::nullopt;
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
            auto result = parse_message(has_run, in, out, collector);
            if (result.has_value()) return result;
        }
        has_run = true;
    }
}

optional<ParseError> parse_object(istream& in, Bytecode& out, LocalCollector& collector) {
    unordered_map<string, SlotDescriptor> slots;
    vector<string> to_initialize;
    vector<string> arg_slots;

    // consume '{'
    in.get();
    consume_whitespace(in);
    int c = in.peek();
    // slot descriptors.
    if (c == '|') {
        in.get();
        bool parsing_slots = true;
        while (parsing_slots) {
            // slot descriptors are either as 
            //   slot-name = <statement>.
            //    or 
            //   slot-value <- statement.
            auto slot_res = parse_slot(in);
            if (holds_alternative<ParseError>(slot_res)) return get<ParseError>(slot_res);
            SlotDescriptor slot_desc = get<1>(get<pair<string, SlotDescriptor>>(slot_res));
            string slot_name = get<0>(get<pair<string, SlotDescriptor>>(slot_res));
            if (slot_desc.is_initialized) {
                to_initialize.push_back(slot_name);
            }

            slots[slot_name] = slot_desc;
            if (slot_desc.is_initialized) {
                auto result = parse_statement(in, '|', out, collector);
                if (result.has_value()) return result;
            } 

            consume_whitespace(in);
            c = in.peek();
            if (c == '|') {
                parsing_slots = false;
            }
        }
        // consume closing '|'
        in.get();
    }

    c = in.peek();
    unordered_map<string, MethodDescriptor> methods;
    while (c != '}') {
        // Parse methods
        auto result = parse_method(in, collector);
        if (holds_alternative<ParseError>(result)) return get<ParseError>(result);
        auto method = get<pair<string, MethodDescriptor>>(result);
        methods[method.first] = method.second;
        consume_whitespace(in);
        c = in.peek();
    }
    in.get();

    out.ops.push_back(OpCode::make_object(slots, methods, to_initialize));
    return std::nullopt;
}

optional<ParseError> parse_block(istream& in, Bytecode& out, LocalCollector& collector) {
    shared_ptr<Bytecode> object_bytecode(new Bytecode);
    unordered_map<string, SlotDescriptor> slots;
    vector<string> to_initialize;
    vector<string> arg_slots;

    // consume '['
    in.get();
    consume_whitespace(in);
    int c = in.peek();
    // slot descriptors.
    if (c == '|') {
        in.get();
        bool parsing_slots = true;
        while (parsing_slots) {
            // slot descriptors are either as 
            //   slot-name = <statement>.
            //    or 
            //   slot-value <- statement.
            auto slot_res = parse_slot(in);
            if (holds_alternative<ParseError>(slot_res)) return get<ParseError>(slot_res);
            SlotDescriptor slot_desc = get<1>(get<pair<string, SlotDescriptor>>(slot_res));
            string slot_name = get<0>(get<pair<string, SlotDescriptor>>(slot_res));
            if (slot_desc.is_initialized) {
                to_initialize.push_back(slot_name);
            }

            slots[slot_name] = slot_desc;
            if (slot_desc.is_initialized) {
                auto result = parse_statement(in, '|', out, collector);
                if (result.has_value()) return result;
            } 

            consume_whitespace(in);
            c = in.peek();
            if (c == '|') {
                parsing_slots = false;
            }
        }
        // consume closing '|'
        in.get();
    }

    c = in.peek();
    bool run = false;
    while (c != ']') {
        if (run) object_bytecode->ops.push_back(OpCode::pop());
        auto result = parse_statement(in, ']', *object_bytecode, collector);
        if (result.has_value()) return result;
        c = in.peek();
        run = true;
    }
    in.get();

    out.ops.push_back(OpCode::make_block(slots, to_initialize, object_bytecode));
    return std::nullopt;
}

optional<ParseError> parse_expr(istream& in, uint64_t& num_exprs, Bytecode& out, LocalCollector& collector) {
    bool can_continue = true;
    consume_whitespace(in);
    while (can_continue) {
        int c = in.peek();
        if (c == EOF) return error("error - end of file!");
        num_exprs++;
        can_continue = false;

        if (c == '.') {
            return error("unexpected end of statement in exprssion.");
        } else if (c == '(')
            return error("not parsing groupings yet");
        else if (c == '{')
            return error("not parsing objects yet");
        else if (c == '[')
            return error("not parsing blocks yet");
        else if (isdigit(c)) {
            optional<ParseError> res = parse_number(in, out, collector);
            if (res.has_value()) return res;
        }
        else if (c == '"') {
            auto res = parse_string(in, out, collector);
            if (res.has_value()) return res;
        }
        else if (is_specialchar(c)) {
            ostringstream oss;
            oss << "unexpected special char: " << (char)c;
            return error(oss.str());
        }
        else {
            // continue to parse message...
            return error("not parsing message within message yet...");
            //auto result = parse_message(false, in, out, collector);
            //if (result.has_value()) return result;
        }

        consume_whitespace(in);
        c = in.peek();
        if (c == ',') {
            can_continue = true;
            in.get();
            consume_whitespace(in);
        } 
    }
    return nullopt;
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
        uint64_t num_exprs = 0;
        optional<ParseError> res = parse_expr(in, num_exprs, out, collector);
        if (res.has_value()) return res;

        if (has_target) {
            out.ops.push_back(OpCode::expl_message(messagename.str(), num_exprs));
        } else {
            out.ops.push_back(OpCode::impl_message(messagename.str(), num_exprs));
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

variant<ParseError, pair<string, SlotDescriptor>> parse_slot(istream& in) {
    SlotDescriptor slot_desc {
        Visibility::Public,
        Visibility::Public,
        false,
        true,
        0
    };
    // Step1: check for prefixes
    // ':' for arg
    // '^','_' for public/private
    consume_whitespace(in);
    int c = in.peek();
    if (c == ':') {
        in.get();
        c = in.peek();
        slot_desc.can_write = false;
    }

    ostringstream slotstream;
    while (!isdigit(c) && !is_specialchar(c) && !is_whitespace(c) && c != EOF) {
        slotstream << (char)c;
        in.get();
        c = in.peek();
    }
    
    consume_whitespace(in);
    c = in.peek();
    if (c == '.' || c == '|') {
        slot_desc.is_initialized = false;
    } else if (c == '=') {
        in.get();
        slot_desc.is_initialized = true;
        slot_desc.can_write = false;
    } else if (c == '<') {
        auto res = parse_symbol(in, "<-");
        if (res.has_value()) return res.value();
        slot_desc.is_initialized = true;
        slot_desc.can_write = true;
    } else {
        ostringstream oss;
        oss << "Unexpected character when parsing slot: " << (char)c;
        return error(oss.str());
    }
    return pair(slotstream.str(), slot_desc);
}
variant<ParseError, string> parse_methodpart(istream& in) {
    // Step1: check for prefixes
    // ':' for arg
    // '^','_' for public/private
    consume_whitespace(in);
    int c = in.peek();

    ostringstream namestream;
    while (!isdigit(c) && !is_specialchar(c) && !is_whitespace(c) && c != ':' && c != EOF) {
        namestream << (char)c;
        in.get();
        c = in.peek();
    }
    if (c == ':') {
        namestream << (char)c;
        in.get();
    }
    return namestream.str();
}

variant<ParseError, pair<string, MethodDescriptor>> parse_method(istream& in, LocalCollector& collector) {
    vector<string> args;
    std::optional<ParseError> result;

    // while parsing method
    bool parsing_method = true;
    ostringstream method_name;
    while (parsing_method) {
        consume_whitespace(in);
        int c = in.peek();
        if (c == '-') {
            parsing_method = false;
        } else {
            auto namepart = parse_methodpart(in);
            if (holds_alternative<ParseError>(namepart)) {
                return get<ParseError>(namepart);
            } else {
                string str = get<string>(namepart);
                method_name << str;
                if (str.back() == ':') {
                    auto argname = parse_methodpart(in);
                    if (holds_alternative<ParseError>(argname)) return get<ParseError>(argname);
                    args.push_back(get<string>(argname));
                } else {
                    parsing_method = false;
                }
            }
        }
    }

    result = parse_symbol(in, "->");
    if (result.has_value()) return result.value();

    shared_ptr<Bytecode> code(new Bytecode);
    result = parse_statement(in, '}', *code, collector);
    if (result.has_value()) return result.value();
    
    return pair(method_name.str(), MethodDescriptor{args, code});
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

    if (is_whitespace(c) || c == '.' || is_specialchar(c)) {
        OrobaObject* lit = new IntegerObject(val);
        collector.Add(lit);
        out.ops.push_back(OpCode::push(lit));
        return std::nullopt;
    } else {
        ostringstream oss;
        oss << "encountered non-numeric character when parsing number: " << (char)c;
        return error(oss.str());
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

optional<ParseError> parse_symbol(istream& in, string symbol) {
    consume_whitespace(in);
    for (auto tok : symbol) {
        int c = in.peek();
        if (c == tok) {
            in.get();
        } else {
            ostringstream oss;
            oss << "unexpected character - expecting symbol " << symbol;
            return error(oss.str());
        }
    }
    return nullopt;
}

ParseError error(string message) {
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
        || c == '|'
        || c == '.'
        || c == ','
        || c == '"';
}
