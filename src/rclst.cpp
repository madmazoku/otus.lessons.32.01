
#include "../bin/version.h"

#include <iostream>

#include <boost/tokenizer.hpp>

#include <dlib/clustering.h>

#include "common.h"

using kernel_t = dlib::radial_basis_kernel<sample_t>;
// using ovo_trainer = dlib::one_vs_one_trainer<dlib::any_trainer<sample_t>>;

int main(int argc, char** argv)
{
    if(argc < 3) {
        std::cout << "Usage: " << argv[0] << " N modelfname" << std::endl;
        std::cout << "\tmake 'N' cluster from stdin and store trained model to 'modelfname'" << std::endl;
        return 0;
    }

    size_t N = std::stol(argv[1]);
    std::string modelfname{argv[2]};

    samples_t samples{std::move(read_samples(std::cin))};
    samples_t centers;

    dlib::kcentroid<kernel_t> kc(kernel_t(0.1),0.01, 8);
    dlib::kkmeans<kernel_t> kkm(kc);
    kkm.set_number_of_centers(N);
    dlib::pick_initial_centers(kkm.number_of_centers(), centers, samples, kkm.get_kernel());

    std::cerr << "Resolve clusters" << std::endl;
    kkm.train(samples,centers);
    std::cerr << "\tResolved" << std::endl;

    std::cerr << "Assign clusters" << std::endl;
    std::vector<double> assignments;
    assignments.resize(samples.size());
    for(size_t n = 0; n < samples.size(); ++n)
        assignments[n] = kkm(samples[n]);
    std::cerr << "\tAssigned" << std::endl;

    // std::cerr << "Train model" << std::endl;
    // ovo_trainer trainer
    // std::cerr << "\tTrained" << std::endl;

    for(size_t n = 0; n < assignments.size(); ++n)
        std::cout << n << "\t" << assignments[n] << std::endl;

    return 0;
}