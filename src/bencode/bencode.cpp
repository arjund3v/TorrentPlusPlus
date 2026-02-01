#include "../include/bencode/bencode.hpp"
#include <string_view>
#include <stdexcept>

namespace bencode {

    class Parser {
        private:
        std::string_view input_;
        size_t index_;


        char peek() const {
            if (index_ >= input_.size()) {
                throw std::runtime_error("Bencode: Trying to read char at end of input");
            }

            return input_[index_];
        };

        char read() {
            char c = peek();
            index_++;
            return c;
        }

        public:


    };



}