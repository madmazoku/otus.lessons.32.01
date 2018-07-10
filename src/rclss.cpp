
#include "../bin/version.h"

#include <iostream>
#include <map>

#include "common.h"

using cluster_sample_t = std::tuple<size_t, sample_t>;
using cluster_samples_t = std::vector<cluster_sample_t>;
using clusters_samples_t = std::map<size_t, cluster_samples_t>;

enum ColumnType {
    Float2,
    Float6,
    Int
};
const ColumnType columns[original_t::NR] = {Float6, Float6, Int, Float2, Float2, Float2, Int, Int};

int main(int argc, char** argv)
{
    if(argc < 2) {
        std::cout << "Usage: " << argv[0] << " modelfname" << std::endl;
        std::cout << "\tread trained model from 'modelfname', read sample from stdin, obtain cluster for it and print cluster content sorted by distance" << std::endl;
        return 0;
    }

    std::string modelfname{argv[1]};

    samples_t questions{std::move(read_data<sample_t>(std::cin))};

    originals_t originals;
    std::vector<double> labels;
    ovo_df_t ovo_df;
    dlib::deserialize(modelfname) >> originals >> labels >> ovo_df;
    samples_t samples{std::move(sample_data(originals))};

    clusters_samples_t clusters;
    for(size_t n = 0; n < samples.size(); ++n)
        clusters[size_t(labels[n])].emplace_back(std::move(std::make_tuple(n, samples[n])));

    size_t default_precision = std::cout.precision();
    for(size_t n = 0; n < questions.size(); ++n) {
        sample_t& q = questions[n];
        size_t label = size_t(ovo_df(q));
        cluster_samples_t& cluster = clusters[label];
        auto compare = [q](const cluster_sample_t& a, const cluster_sample_t& b) {
            return length(q-std::get<1>(a)) < length(q-std::get<1>(b));
        };
        std::sort(cluster.begin(), cluster.end(), compare);

        for(auto& c : cluster) {
            bool next = false;
            original_t& o = originals[std::get<0>(c)];
            for(size_t n = 0; n < original_t::NR; ++n) {
                if(n > 0)
                    std::cout << ';';
                switch(columns[n]) {
                case Int:
                    std::cout << size_t(o(n));
                    break;
                case Float2:
                    std::cout << std::fixed << std::setprecision(2) << o(n) << std::setprecision(default_precision) << std::defaultfloat;
                    break;
                case Float6:
                    std::cout << std::fixed << std::setprecision(6) << o(n) << std::setprecision(default_precision) << std::defaultfloat;
                    break;
                }
            }
            std::cout << '\n';
        }
    }

    return 0;
}