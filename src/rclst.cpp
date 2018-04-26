
#include "../bin/version.h"

#include <iostream>

#include "common.h"

int main(int argc, char** argv)
{
    if(argc < 3) {
        std::cout << "Usage: " << argv[0] << " N modelfname" << std::endl;
        std::cout << "\tmake 'N' cluster from stdin and store trained model to 'modelfname'" << std::endl;
        return 0;
    }

    size_t N = std::stol(argv[1]);
    std::string modelfname{argv[2]};

    auto sample_parse = [](const strings_t& tokens) {
        sample_t s;
        for(size_t n = 0; n < 6; ++n)
            s(n) = tokens[n].empty() ? 0.0 : std::stod(tokens[n]);
        size_t floor = tokens[6].empty() ? 0 : std::stoul(tokens[6]);
        size_t floor_max = tokens[7].empty() ? 0 : std::stoul(tokens[7]);
        s(6) = floor == 0 || floor == floor_max ? 0.0 : 1.0;
        return std::move(s);
    };
    samples_t samples{std::move(read_data(std::cin, 8, sample_parse))};
    samples_t centers;

    dlib::kcentroid<kernel_t> kc(kernel_t(),0.01, 8);
    dlib::kkmeans<kernel_t> kkm(kc);
    kkm.set_number_of_centers(N);

    dlib::pick_initial_centers(kkm.number_of_centers(), centers, samples, kkm.get_kernel());
    dlib::find_clusters_using_kmeans(samples, centers);

    kkm.train(samples,centers);

    std::vector<double> labels;
    labels.resize(samples.size());
    for(size_t n = 0; n < samples.size(); ++n)
        labels[n] = kkm(samples[n]);

    ovo_trainer_t ovo_trainer;
    dlib::krr_trainer<kernel_t> linear_trainer;
    ovo_trainer.set_trainer(linear_trainer);

    ovo_df_t ovo_df = ovo_trainer.train(samples, labels);

    dlib::serialize(modelfname) << samples << labels << ovo_df;

    return 0;
}