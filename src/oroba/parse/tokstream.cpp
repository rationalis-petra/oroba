#include "oroba/parse/tokstream.hpp"

#include <sstream>

using namespace std;

void consume_whitespace(istream& in);
bool is_operator(string str);
bool is_whitespace(int c);
bool is_specialchar(int c);

Token parse_number(istream& in);
Token parse_string(istream& in);
Token parse_symbol(istream& in);

Token::Token() {
    type = TokenType::EndOfStream;
    val = 0;
}

TokStream::TokStream(std::istream& in) : m_istream(in) {
}

Token TokStream::peek() {
    if (m_is_cached) return m_cached;

    m_is_cached = true;
    UpdateCache();
    return m_cached;
}

Token TokStream::get() {
    if (m_is_cached) {
        m_is_cached = false;
        return m_cached;
    } else {
        UpdateCache();
        return m_cached;
    }
}

void TokStream::UpdateCache() {
    consume_whitespace(m_istream);
    int c = m_istream.peek();
    switch (c) {
    case '.': 
        m_istream.get();
        m_cached.type = TokenType::Separator;
        break;
    case ',': 
        m_istream.get();
        m_cached.type = TokenType::Collector;
        break;
    case '(': 
        m_istream.get();
        m_cached.type = TokenType::GroupBegin;
        break;
    case ')': 
        m_istream.get();
        m_cached.type = TokenType::GroupEnd;
        break;
    case '[': 
        m_istream.get();
        m_cached.type = TokenType::BlockBegin;
        break;
    case ']': 
        m_istream.get();
        m_cached.type = TokenType::BlockEnd;
        break;
    case '{': 
        m_istream.get();
        m_cached.type = TokenType::ObjectEnd;
        break;
    case '}': 
        m_istream.get();
        m_cached.type = TokenType::ObjectEnd;
        break;
    case '"': 
        m_cached = parse_string(m_istream);
        break;
    default:
        if (isdigit(c)) {
            m_cached = parse_number(m_istream);
        } else {
            m_cached = parse_symbol(m_istream);
        }
        break;
    }
}

Token parse_number(istream& in) {
    int64_t val = 0;
    int64_t digit = 1;
    int c = in.peek();
    while (isdigit(c)) {
        val += (c - 48) * digit;
        in.get();
        c = in.peek();
    }

    if (is_whitespace(c) || c == '.' || is_specialchar(c)) {
        Token tok;
        tok.type = TokenType::Number;
        tok.val = val;
        return tok;
    } else {
        ostringstream oss;
        oss << "encountered non-numeric character when parsing number: " << (char)c;
        Token tok;
        tok.type = TokenType::Error;
        tok.val = oss.str();
        return tok;
    }
}

Token parse_string(istream& in) {
    // consume starting '"'
    in.get();
    ostringstream oss;
    while (in.peek() != '"') {
        oss << (char)in.get();
    }

    Token tok;
    tok.type = TokenType::String;
    tok.val = oss.str();
    return tok;
}

Token parse_symbol(istream& in) {
    consume_whitespace(in);
    int c = in.peek();
    ostringstream oss;
    while (!is_whitespace(c) && !is_specialchar(c)) {
        oss << (char)c;
        in.get();
        c = in.peek();
    }
    Token out;
    out.type = TokenType::Symbol; 
    out.val = oss.str();
    return out;
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
