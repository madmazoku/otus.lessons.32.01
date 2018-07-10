
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

    originals_t originals{std::move(read_data<original_t>(std::cin))};
    samples_t samples{std::move(sample_data(originals))};
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

    dlib::serialize(modelfname) << originals << labels << ovo_df;

    return 0;
}