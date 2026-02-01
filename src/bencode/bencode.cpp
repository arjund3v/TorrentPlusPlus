#include "../include/bencode/bencode.hpp"

#include <charconv>
#include <string_view>
#include <stdexcept>

namespace bencode {

    class Parser {
        public:
            explicit Parser(std::string_view input) {
                input_ = input;
                index_ = 0;
            };

        Value parseRoot() {
            Value result = parseValue();
        }
        private:
            std::string_view input_;
            size_t index_;

            // Looks at specified index, but doesn't actually move forward
            // We need this because before we parse, we need to know the datatype.
            // If we know the next char is a integer we wouldn't want to consume it so peek will help with that
            char peek() const {
                if (index_ >= input_.size()) {
                    throw std::runtime_error("Bencode: Trying to read char at end of input");
                };

                return input_[index_];
            };

            char read() {
                char c = peek();
                index_++;
                return c;
            };

            void expect(char expectedChar) {
                char got = read();
                if (got != expectedChar) {
                    throw std::runtime_error("Bencode: Expected '" + std::string{expectedChar});
                };
            };

            Value parseValue() {
                // We will take a look at the next character first to figure out our parse method
                char nextChar = peek();

                // Bencode integers are denoted by i<integer>e
                if (nextChar == 'i') {
                    return bencode::Value{parseInt()};
                }

                // Bencode lists are denoted by l<value><value><value>...e
                if (nextChar == 'l') {
                    return bencode::Value{parseList()};
                }

                // Bencode dictionaries are denoted by d<key><value>...e
                if (nextChar == 'd') {
                    return bencode::Value{parseDict()};
                }

                // Bencode strings are denoted by # of chars in string followed by the string
                // Example: If the string is "hello" in bencode it would be 6:hello
                if (isdigit(nextChar)) {
                    return bencode::Value{parseString()};
                }

                // If it's not one of the types above, then there is an error
                throw std::runtime_error("Bencode: invalid value start");
            }

            Str parseString() {
                // Since the size is denoted at the start of the string, we can use that as our size.
                size_t length_start = index_;

                // Move forward while current char is a digit
                // We do this to read the full size of the string.
                // This loop will stop once it hits the colon
                while (index_ < input_.size() && isdigit(input_[index_])) {
                    index_++;
                };

                // After we finish scanning the digits, our next char MUST be :
                // We check if we ran out of input to prevent out-of-bounds access
                // we also check for the existence of the :
                if (index_ >= input_.size() || input_[index_] != ':') {
                    throw std::runtime_error("Bencode: Invalid string length prefix");
                };

                // This essentially extracts the length of our string
                // Remember, index_ stops at the colon so we are extracting everything before the colon which is the
                // length that the string is going to be.
                std::string_view length_view = input_.substr(length_start, index_ - length_start);

                // Consume that : char since we don't need it for anything
                expect(':');

                // Since we extracted the string length, we need to actually convert it to a number, right now it is
                // a string_view
                size_t length = 0;

                // We need to pass this function data and data + size because data gives us the address of the first.
                // But to specify a last position, we add size to data to move the pointer all the way to the last pos
                auto [stopped_parsing, err] = std::from_chars(length_view.data(),
                                                            length_view.data() + length_view.size(),
                                                            length);

                // We check for an error code or if the stopped parsing pointer is not at the end of the view
                if (err != std::errc{} || stopped_parsing != length_view.data() + length_view.size()) {
                    throw std::runtime_error("Bencode: Bad string length");
                };

                if (index_ + length > input_.size()) {
                    throw std::runtime_error("Bencode: String length out of range");
                };

                // We copy the exact length
                Str output(input_.substr(index_, length));

                // Now we can move the pointer forward across our bencode string
                index_ += length;

                return output;
            };

            Int parseInt();
            List parseList();
            Dict parseDict();
    };



}