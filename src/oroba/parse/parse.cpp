#include "oroba/parse/parse.hpp"

#include <sstream>
#include <optional>

#include "oroba/object/composite.hpp"
#include "oroba/object/collections.hpp"
#include "oroba/object/primitives.hpp"

#include "oroba/parse/tokstream.hpp"

using namespace std;

/* Grammar:
 * Statement 
 * 
 */

optional<ParseError> parse_statement(TokStream& in, std::optional<TokenType> terminator,  Bytecode& out,LocalCollector& collector);
optional<ParseError> parse_message_arg(TokStream& in, uint64_t& num_exprs, Bytecode& out, LocalCollector& collector);
optional<ParseError> parse_object(TokStream& in, Bytecode& out, LocalCollector& collector);
optional<ParseError> parse_block(TokStream& in, Bytecode& out, LocalCollector& collector);
optional<ParseError> parse_unimessage(bool has_target, TokStream& in, Bytecode& out, LocalCollector& collector);
optional<ParseError> parse_message(bool has_target, TokStream& in, Bytecode& out, LocalCollector& collector);

variant<ParseError, pair<string, SlotDescriptor>> parse_slot(TokStream& in);
variant<ParseError, pair<string, MethodDescriptor>> parse_method(TokStream& in, LocalCollector& collector);
variant<ParseError, string> parse_methodpart(istream& in);

bool is_symbol(Token tok, string sym);
bool is_operator(string s);

ParseError error(string message);

variant<Bytecode, ParseError> parse(istream& in, LocalCollector& collector) {
    Bytecode bytecode;
    TokStream toks(in);
    // TODO: replace the 'terminator' tokent type with something useful... 
    optional<ParseError> out = parse_statement(toks, nullopt, bytecode, collector);
    if (out.has_value())
        return out.value();
    else
        return bytecode;
}
    
optional<ParseError> parse_statement(TokStream& in, std::optional<TokenType> terminator, Bytecode& out, LocalCollector& collector) {
    bool has_run = false;
    while (true) {
        Token tok = in.peek();

        if (terminator.has_value() && tok.type == terminator.value()) {
            return nullopt;
        }

        switch (tok.type) {
        case TokenType::Error:
            in.get();
            return error(get<string>(tok.val));
        case TokenType::EndOfStream:
            in.get();
            return error("error - end of file!");
        case TokenType::Separator:
        case TokenType::Collector:
            in.get();
            return std::nullopt;

        case TokenType::GroupBegin:
            return error("not parsing groupings yet");
        case TokenType::BlockBegin: {
            auto result = parse_block(in, out, collector);
            if (result.has_value()) return result;
            break;
        }
        case TokenType::ObjectBegin: {
            auto result = parse_object(in, out, collector);
            if (result.has_value()) return result;
            break;
        }
        case TokenType::Number: {
            in.get();
            OrobaObject* lit = new IntegerObject(get<int64_t>(tok.val));
            collector.Add(lit);
            out.ops.push_back(OpCode::push(lit));
            break;
        }
        case TokenType::String: {
            in.get();
            OrobaObject* lit = new StringObject(get<string>(tok.val));
            collector.Add(lit);
            out.ops.push_back(OpCode::push(lit));
            break;
        }
        case TokenType::QSymbol: {
            in.get();
            OrobaObject* lit = new SymbolObject(get<string>(tok.val));
            collector.Add(lit);
            out.ops.push_back(OpCode::push(lit));
            break;
        }
        case TokenType::Symbol: {
            auto result = parse_message(has_run, in, out, collector);
            if (result.has_value()) return result;
            break;
        }
        default:
            in.get();
            return error("error - unexpected special character: '|'");
        }
        has_run = true;
    }
}

optional<ParseError> parse_object(TokStream& in, Bytecode& out, LocalCollector& collector) {
    unordered_map<string, SlotDescriptor> slots;
    vector<string> to_initialize;
    vector<string> arg_slots;

    // consume '{'
    in.get();
    Token tok = in.peek();
    // slot descriptors.
    if (tok.type == TokenType::Pipe) {
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
                auto result = parse_statement(in, TokenType::Pipe, out, collector);
                if (result.has_value()) return result;
            } 

            tok = in.peek();
            if (tok.type == TokenType::Pipe) {
                parsing_slots = false;
            }
        }
        // consume closing '|'
        in.get();
    }

    tok = in.peek();
    unordered_map<string, MethodDescriptor> methods;
    while (tok.type != TokenType::ObjectEnd) {
        // Parse methods
        auto result = parse_method(in, collector);
        if (holds_alternative<ParseError>(result)) return get<ParseError>(result);
        auto method = get<pair<string, MethodDescriptor>>(result);
        methods[method.first] = method.second;
        tok = in.peek();
    }
    in.get();

    out.ops.push_back(OpCode::make_object(slots, methods, to_initialize));
    return std::nullopt;
}

// Block parsing: 
// [ body ]
// [ :arg1 arg2... | body ]
// [ :arg1 arg2... | slot <- val. | body ]
optional<ParseError> parse_block(TokStream& in, Bytecode& out, LocalCollector& collector) {
    shared_ptr<Bytecode> object_bytecode(new Bytecode);
    unordered_map<string, SlotDescriptor> slots;
    vector<string> to_initialize;
    vector<string> arg_slots;


    // consume '['
    in.get();

    Token tok = in.peek();
    // Step 1: parse arguments

    // TODO: add in slot descriptor parsing
    // slot descriptors.
    // if (c == '|') {
    //     in.get();
    //     bool parsing_slots = true;
    //     while (parsing_slots) {
    //         // slot descriptors are either as 
    //         //   slot-name = <statement>.
    //         //    or 
    //         //   slot-value <- statement.
    //         auto slot_res = parse_slot(in);
    //         if (holds_alternative<ParseError>(slot_res)) return get<ParseError>(slot_res);
    //         SlotDescriptor slot_desc = get<1>(get<pair<string, SlotDescriptor>>(slot_res));
    //         string slot_name = get<0>(get<pair<string, SlotDescriptor>>(slot_res));
    //         if (slot_desc.is_initialized) {
    //             to_initialize.push_back(slot_name);
    //         }

    //         slots[slot_name] = slot_desc;
    //         if (slot_desc.is_initialized) {
    //             auto result = parse_statement(in, '|', out, collector);
    //             if (result.has_value()) return result;
    //         } 

    //         consume_whitespace(in);
    //         c = in.peek();
    //         if (c == '|') {
    //             parsing_slots = false;
    //         }
    //     }
    //     // consume closing '|'
    //     in.get();
    // }

    tok = in.peek();
    bool run = false;
    while (tok.type != TokenType::BlockEnd) {
        if (run) object_bytecode->ops.push_back(OpCode::pop());
        auto result = parse_statement(in, TokenType::BlockEnd, *object_bytecode, collector);
        if (result.has_value()) return result;
        tok = in.peek();
        run = true;
    }
    in.get();

    out.ops.push_back(OpCode::make_block(slots, to_initialize, arg_slots, object_bytecode));
    return std::nullopt;
}

optional<ParseError> parse_message_arg(TokStream& in, uint64_t& num_exprs, Bytecode& out, LocalCollector& collector) {
    bool has_target = false;
    num_exprs++;
    while (true) {
        Token tok = in.peek();

        switch (tok.type) {
        case TokenType::Error:
            in.get();
            return error(get<string>(tok.val));
        case TokenType::EndOfStream:
            in.get();
            return error("error - end of file!");
        case TokenType::Separator:
            return std::nullopt;
        case TokenType::Collector:
            return error("unexpected , ");
        case TokenType::GroupBegin:
            return error("not parsing groupings yet");
        case TokenType::BlockBegin: {
            auto result = parse_block(in, out, collector);
            if (result.has_value()) return result;
            break;
        }
        case TokenType::ObjectBegin: {
            auto result = parse_object(in, out, collector);
            if (result.has_value()) return result;
            break;
        }
        case TokenType::Number: {
            OrobaObject* lit = new IntegerObject(get<int64_t>(tok.val));
            collector.Add(lit);
            out.ops.push_back(OpCode::push(lit));
            return std::nullopt;
        }
        case TokenType::String: {
            OrobaObject* lit = new StringObject(get<string>(tok.val));
            collector.Add(lit);
            out.ops.push_back(OpCode::push(lit));
            return std::nullopt;
        }
        case TokenType::QSymbol: {
            in.get();
            OrobaObject* lit = new SymbolObject(get<string>(tok.val));
            collector.Add(lit);
            out.ops.push_back(OpCode::push(lit));
            break;
        }
        case TokenType::Symbol: {
            string sym = get<string>(tok.val);
            if (sym.back() == ':') {
                return std::nullopt;
            } else {
                if (is_operator(sym)) {
                    return error("not parsing message within multi-message yet...");
                }

                in.get();
                if (has_target) {
                    out.ops.push_back(OpCode::expl_message(sym, 0));
                } else {
                    out.ops.push_back(OpCode::impl_message(sym, 0));
                }
            };
            break;
        }
        default:
            in.get();
            return error("error - unexpected special character: ");
        }

        has_target = true;
        tok = in.peek();
        if (tok.type == TokenType::Collector) {
            in.get();
            has_target = false;
        } 
    }
}

optional<ParseError> parse_message(bool has_target, TokStream& in, Bytecode& out, LocalCollector& collector) {
    Token token = in.peek();
    if (token.type != TokenType::Symbol) {
        return error ("Invalid message part.");
    }
    in.get();
    string message = get<string>(token.val);

    if (message.back() == ':') {
        ostringstream messagename;
        uint64_t num_exprs = 0;
        bool continue_parse = true;
        while (continue_parse) {
            continue_parse = false;
            messagename << message;

            // May be multipart message
            optional<ParseError> res = parse_message_arg(in, num_exprs, out, collector);
            if (res.has_value()) return res;

            token = in.peek();
            if (token.type == TokenType::Symbol) {
                in.get();
                message = get<string>(token.val);
                continue_parse = message.back() == ':';
            }
        }

        if (has_target) {
            out.ops.push_back(OpCode::expl_message(messagename.str(), num_exprs));
        } else {
            out.ops.push_back(OpCode::impl_message(messagename.str(), num_exprs));
        }
    } else if (is_operator(message)) {
        // TODO: rework precedence and order of operations correctly.
        // - Pass value on stack 
        out.ops.push_back(OpCode::impl_message(message, 0));
        out.ops.push_back(OpCode::swap());
        uint64_t num_exprs = 0;
        optional<ParseError> res = parse_message_arg(in, num_exprs, out, collector);
        if (res.has_value()) return res;

        out.ops.push_back(OpCode::expl_message("_invoke:", num_exprs + 1));
        return nullopt;
    } else {
        // no argument
        if (has_target) {
            out.ops.push_back(OpCode::expl_message(message, 0));
        } else {
            out.ops.push_back(OpCode::impl_message(message, 0));
        }
    }
    return nullopt;
}

variant<ParseError, pair<string, SlotDescriptor>> parse_slot(TokStream& in) {
    SlotDescriptor slot_desc {
        Visibility::Public,
        Visibility::Public,
        false,
        true,
        0
    };
    // TODO: check for prefixes
    // '^','_' for public/private
    Token tok = in.peek();
    if (tok.type != TokenType::Symbol) {
        return error("Slot value is invalid token.");
    }
    string slotname = get<string>(tok.val);
    
    tok = in.peek();
    if (tok.type == TokenType::Separator || tok.type == TokenType::Pipe) {
        slot_desc.is_initialized = false;
    } else if (is_symbol(tok, "=")) {
        in.get();
        slot_desc.is_initialized = true;
        slot_desc.can_write = false;
    } else if (is_symbol(tok, "<-")) {
        in.get();
        slot_desc.is_initialized = true;
        slot_desc.can_write = true;
    } else {
        ostringstream oss;
        oss << "Unexpected token when parsing slot: "; // << (char)c;
        return error(oss.str());
    }
    return pair(slotname, slot_desc);
}

variant<ParseError, pair<string, MethodDescriptor>> parse_method(TokStream& in, LocalCollector& collector) {
    vector<string> args;
    std::optional<ParseError> result;

    // while parsing method
    bool parsing_method = true;
    ostringstream method_name;
    while (parsing_method) {
        Token tok = in.peek();
        if (tok.type == TokenType::Symbol && get<string>(tok.val) == "->") {
            parsing_method = false;
        } else {
            if (tok.type != TokenType::Symbol) {
                return error("Unexpected token when parsing method name.");
            }
            auto namepart = get<string>(tok.val);
            method_name << namepart;
            if (namepart.back() == ':') {
                Token arg = in.get();
                if (arg.type == TokenType::Symbol) {
                    args.push_back(get<string>(arg.val));
                } else {
                    return error("expected argument when parsing method description.");
                }
            } else {
                parsing_method = false;
            }
        }
    }

    Token tok = in.get();
    if (!is_symbol(tok, "->")) {
        return error("Expected '->' symbol while parsing method.");
    }

    shared_ptr<Bytecode> code(new Bytecode);
    result = parse_statement(in, TokenType::ObjectEnd, *code, collector);
    if (result.has_value()) return result.value();
    
    return pair(method_name.str(), MethodDescriptor{args, code});
}

ParseError error(string message) {
    return ParseError{message, 0, 0};
}

bool is_symbol(Token tok, string sym) {
    return tok.type == TokenType::Symbol && get<string>(tok.val) == sym;
}

bool is_operator(string s) {
    bool is_op = true;
    for (char c : s) {
        is_op &= ((c == '=')
                  | (c == '+')
                  | (c == '-')
                  | (c == '*')
                  | (c == '/')
                  | (c == '<')
                  | (c == '>') 
                  | (c == '$') 
                  | (c == '%') 
                  | (c == '|') 
                  | (c == '&'));
    }
    return is_op;
}
