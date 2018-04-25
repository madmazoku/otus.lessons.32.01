#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <utility>

#include <boost/tokenizer.hpp>

#include <dlib/matrix.h>

using strings_t = std::vector<std::string>;
using sample_t = dlib::matrix<double,7,1>;
using samples_t = std::vector<sample_t>;

samples_t read_samples(std::istream& in) {
    std::string line;
    strings_t tokens;
    samples_t samples;
    boost::char_separator<char> sep{";\n", " ", boost::keep_empty_tokens};
    size_t count = 0;
    while(std::getline(in, line)) {
        ++count;
        boost::tokenizer<boost::char_separator<char>> tok{line, sep};

        tokens.clear();
        std::copy( tok.begin(), tok.end(), std::back_inserter(tokens) );
        if(tokens.size() != 8) {
            std::cerr << "invalid line " << count << ", tokens found: " << tokens.size() << "; '" << line << "'" << std::endl;
            for(auto& t : tokens)
                std::cout << "\t" << t << std::endl;
            continue;
        }

        sample_t s;
        for(size_t n = 0; n < 6; ++n)
            s(n) = tokens[n].empty() ? 0.0 : std::stod(tokens[n]);
        size_t floor = tokens[6].empty() ? 0 : std::stoul(tokens[6]);
        size_t floor_max = tokens[7].empty() ? 0 : std::stoul(tokens[7]);
        s(6) = floor == 0 || floor == floor_max ? 0.0 : 1.0;
        samples.emplace_back(std::move(s));
    }

    return std::move(samples);
}
