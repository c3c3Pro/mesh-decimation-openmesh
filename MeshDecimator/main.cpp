#include<iostream>
#include<string>
#include"io_helper.h"
#include "Headers/MeshDecimator.h"
#include "Headers/MeshTypes.h"

using namespace decim;

int main(int argc, char** argv)
{
    // the argument should contain the main .obj file for reading, the output file that will be saved
    // and the number of targeted faces
    if (argc >= 2 && (std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h"))
    {
        print_usage();
        wait_if_no_arg(argc);
        return 0;
    }

    //default parameters for mesh decimation
    //can be overridden by args
    DecimateOptions opt;

    opt.target_faces      = (argc >= 4) ? std::atoi(argv[3]) : 3000;
    opt.preserve_boundary = true;
    opt.sharp_angle_degree = 10.0; //5-15 for very smooth models; 30-50 for general use
    opt.min_angle_degree = 12.0f;

    std::string inputPath, outputPath;

    if (!run_by_default_no_args(argc, argv ? argv[0] : nullptr, inputPath, outputPath))
    {
        //fallback to normal CLI parsing
        if (argc < 3)
        {
            print_usage();
            wait_if_no_arg(argc);
            return 1;
        }
        inputPath = argv[1];
        outputPath = argv[2];
        if (argc >= 4) opt.target_faces = std::atoi(argv[3]);
    }

    MyMesh mesh;

    if (!loadMesh(inputPath.c_str(), mesh))
    {
        wait_if_no_arg(argc);
        return 1; // stop if read failed
    }
        
    decimateByNormalCost(mesh,opt);

    if (!saveMesh(outputPath.c_str(), mesh))
    {
        wait_if_no_arg(argc);
        return 1; // stop if write failed
    }

    std::cout << "Wrote: " << outputPath << "\n";
    wait_if_no_arg(argc);

    return 0;
}