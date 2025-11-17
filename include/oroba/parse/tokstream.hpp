#ifndef __OROBA_PARSE_TOKSTREAM_HPP
#define __OROBA_PARSE_TOKSTREAM_HPP

#include <istream>
#include <variant>

#include "gc/collector.hpp"
#include "oroba/data/bytecode.hpp"
#include "oroba/data/error.hpp"

enum class TokenType {
    Error,

    EndOfStream,
    Pipe,
    Separator,
    Collector,

    GroupBegin,
    GroupEnd,
    BlockBegin,
    BlockEnd,
    ObjectBegin,
    ObjectEnd,

    Number,
    String,
    Symbol,
    QSymbol,
};

struct Token {
    Token();

    TokenType type;
    std::variant<std::string, int64_t> val;
};

class TokStream {
public:
    TokStream(std::istream& in);
    Token peek();
    Token get();

private:
    void UpdateCache();

    std::istream& m_istream;
    bool m_is_cached;
    Token m_cached;
};

#endif
