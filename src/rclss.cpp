
#include "../bin/version.h"

#include <iostream>
#include <map>

#include "common.h"

using cluster_sample_t = std::tuple<size_t, sample_t>;
using cluster_samples_t = std::vector<cluster_sample_t>;
using clusters_samples_t = std::map<size_t, cluster_samples_t>;

int main(int argc, char** argv)
{
    if(argc < 2) {
        std::cout << "Usage: " << argv[0] << " modelfname" << std::endl;
        std::cout << "\tread trained model from 'modelfname', read sample from stdin, obtain cluster for it and print cluster content sorted by distance" << std::endl;
        return 0;
    }

    std::string modelfname{argv[1]};

    auto question_parse = [](const strings_t& tokens) {
        sample_t s;
        for(size_t n = 0; n < 7; ++n)
            s(n) = tokens[n].empty() ? 0.0 : std::stod(tokens[n]);
        return std::move(s);
    };
    samples_t questions{std::move(read_data(std::cin, 7, question_parse))};

    samples_t samples;
    ovo_df_t ovo_df;
    dlib::deserialize(modelfname) >> samples >> ovo_df;

    clusters_samples_t clusters;
    for(size_t n = 0; n < samples.size(); ++n) {
        sample_t& s = samples[n];
        clusters[size_t(ovo_df(s))].emplace_back(std::move(std::make_tuple(n, s)));
    }

    for(size_t n = 0; n < questions.size(); ++n) {
        sample_t& q = questions[n];
        size_t label = size_t(ovo_df(q));
        cluster_samples_t& cluster = clusters[label];
        std::sort(cluster.begin(), cluster.end(), 
            [q](const cluster_sample_t& a, const cluster_sample_t& b) {
                return length(q-std::get<1>(a)) < length(q-std::get<1>(b));
            }
        );

        for(auto& c : cluster) {
            sample_t& s = std::get<1>(c);
            std::cout << n << ';' << std::get<0>(c) << ';' << label << ';' << length(q-s);
            for(size_t m = 0;m < s.size(); ++m)
                std::cout << ';' << s(m);
            std::cout << '\n';
        }

    }

    return 0;
}