
#include "../bin/version.h"

#include <iostream>
#include <cmath>

#include <boost/tokenizer.hpp>

#include <dlib/clustering.h>
#include <dlib/image_io.h>
#include <dlib/image_transforms.h>

int main(int argc, char** argv)
{
    if(argc < 2) {
        std::cout << "Usage: " << argv[0] << " modelfname" << std::endl;
        std::cout << "\tread trained model from 'modelfname', read sample from stdin, obtain cluster for it and print cluster content sorted by distance" << std::endl;
        return 0;
    }

    std::string modelfname{argv[1]};

    return 0;
}