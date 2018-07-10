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
using original_t = dlib::matrix<double,8,1>;
using sample_t = dlib::matrix<double,7,1>;
using originals_t = std::vector<original_t>;
using samples_t = std::vector<sample_t>;

using kernel_t = dlib::linear_kernel<sample_t>;
using ovo_trainer_t = dlib::one_vs_one_trainer<dlib::any_trainer<sample_t>>;
using ovo_df_t = dlib::one_vs_one_decision_function<ovo_trainer_t, dlib::decision_function<kernel_t>>;

template<typename T>
std::vector<T> read_data(std::istream& in)
{
    std::string line;
    strings_t tokens;
    std::vector<T> ts;
    boost::char_separator<char> sep{";\n", " ", boost::keep_empty_tokens};
    size_t count = 0;
    while(std::getline(in, line)) {
        ++count;
        boost::tokenizer<boost::char_separator<char>> tok{line, sep};

        tokens.clear();
        std::copy( tok.begin(), tok.end(), std::back_inserter(tokens) );
        if(tokens.size() != T::NR) {
            std::cerr << "invalid line " << count << ", tokens found: " << tokens.size() << "; '" << line << "'" << std::endl;
            for(auto& t : tokens)
                std::cout << "\t" << t << std::endl;
            continue;
        }

        T t;
        for(size_t n = 0; n < T::NR; ++n)
            t(n) = tokens[n].empty() ? 0.0 : std::stod(tokens[n]);

        ts.emplace_back(std::move(t));
    }

    return std::move(ts);
}

samples_t sample_data(const originals_t& originals)
{
    samples_t samples;
    auto transform = [](const original_t& o) {
        sample_t s;
        for(size_t n = 0; n < sample_t::NR - 1; ++n)
            s(n) = o(n);
        s(sample_t::NR - 1) = o(original_t::NR - 2) == 0 || o(original_t::NR - 2) == o(original_t::NR - 1);
        return std::move(s);
    };
    std::transform(originals.begin(), originals.end(), std::back_inserter(samples), transform);
    return std::move(samples);
}