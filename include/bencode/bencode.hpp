#ifndef TORRENTPLUSPLUS_BENCODE_H
#define TORRENTPLUSPLUS_BENCODE_H

#include <string>
#include <string_view>
#include <variant>
#include <vector>
#include <map>


namespace bencode {

    //
    struct Value;

    // 2) These are the 4 possible payload types, we are just creating aliases for them so we have readable code
    using Int = long long;
    using Str = std::string;
    using List = std::vector<Value>;
    using Dict = std::map<std::string, Value>;

    struct Value {
        std::variant<Int, Str, List, Dict> data{};
    };

    Value decode(std::string_view input);

}




#endif //TORRENTPLUSPLUS_BENCODE_H