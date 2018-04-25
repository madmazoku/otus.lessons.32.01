#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <utility>

#include <boost/tokenizer.hpp>

#include <dlib/svm_threaded.h>
#include <dlib/clustering.h>
#include <dlib/matrix.h>

using strings_t = std::vector<std::string>;
using sample_t = dlib::matrix<double,7,1>;
using samples_t = std::vector<sample_t>;

using kernel_t = dlib::linear_kernel<sample_t>;
using ovo_trainer_t = dlib::one_vs_one_trainer<dlib::any_trainer<sample_t>>;
using ovo_df_t = dlib::one_vs_one_decision_function<ovo_trainer_t, dlib::decision_function<kernel_t>>;

samples_t read_data(std::istream& in, size_t tokens_size, std::function<sample_t (const strings_t&)> parse) {
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
        if(tokens.size() != tokens_size) {
            std::cerr << "invalid line " << count << ", tokens found: " << tokens.size() << "; '" << line << "'" << std::endl;
            for(auto& t : tokens)
                std::cout << "\t" << t << std::endl;
            continue;
        }

        samples.emplace_back(std::move(parse(tokens)));
    }

    return std::move(samples);
}

