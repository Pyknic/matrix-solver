//#include <sstream>
//#include <iostream>
//#include <functional>
//#include "constant.hpp"
//#include "variable.hpp"
//#include "matrix.hpp"
//#include "helper.hpp"
//#include "latex-formatter.hpp"
//#include "glm-formatter.hpp"
//#include "parser.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include "parser.hpp"
#include "symbol.hpp"
#include "default-formatter.hpp"

const char* executableName;

void printHelp(FILE* stream, int exitCode) {
    fprintf(stream, "Usage: %s options [ equation ... ]\n", executableName);
    fprintf(stream,
        " -h --help Display this usage information.\n"
        " -s --src filename to read input from.\n"
        " -d --dest filename to write the results to.\n"
        " -f --find name of the symbol to find the value of.\n"
        " -p --pretty add spaces and new-lines to make output more pretty.\n"
        " -v --verbose Print verbose debug information.\n"
    );
    exit(exitCode);
}

int main(int argc, char* argv[]) {
    int nextOption;

    const char* const shortOptions = "hsdf:v";
    const struct option longOptions[] = {
        {"help", 0, nullptr, 'h'},
        {"src",  1, nullptr, 's'},
        {"dest", 1, nullptr, 'd'},
        {"find", 1, nullptr, 'f'},
        {"verbose", 0, nullptr, 'v'},
        {"pretty", 0, nullptr, 'p'},
        {nullptr, 0, nullptr, 0}
    };

    const char* srcFilename = nullptr;
    const char* destFilename = nullptr;
    const char* findSymbolName = nullptr;
    bool verbose = false;
    bool pretty = false;

    executableName = argv[0];
    do {
        nextOption = getopt_long(argc, argv, shortOptions, longOptions, nullptr);
        switch (nextOption) {
            case 'h':
                printHelp(stdout, 0);
                break;
            case 's':
                srcFilename = optarg;
                break;
            case 'd':
                destFilename = optarg;
                break;
            case 'f':
                findSymbolName = optarg;
                break;
            case 'v':
                verbose = true;
                break;
            case 'p':
                pretty = true;
                break;
            case '?':
                printHelp(stderr, 1);
            case -1:
                break;
            default:
                abort();
        }
    } while (nextOption != -1);

    if (verbose) {
        for (int i = 0; i < argc; i++) {
            printf("Argument %d: %s\n", i, argv[i]);
        }
    }

    std::stringstream buffer {};
    Formatter* formatter {new DefaultFormatter{pretty}};
    Parser parser {};

    if (srcFilename == nullptr) {
        int lines = 0;
        std::string line;
        while (std::getline(std::cin, line))
        {
            buffer << line << std::endl;
            lines++;
        }

        if (verbose) {
            std::cout << lines << " lines read from input stream." << std::endl;
        }
    } else {
        std::ifstream t(srcFilename);
        std::string str((std::istreambuf_iterator<char>(t)),
                        std::istreambuf_iterator<char>());
        buffer << str;
    }

    parser.parse(buffer);

    if (verbose) {
        std::cout << "--- Input Begin ---" <<
                  std::endl << buffer.str() <<
                  "--- Input End ---" << std::endl;
    }

    std::string result = findSymbolName == nullptr
        ? parser.format(*formatter)
        : parser.get(findSymbolName)->format(*formatter);
    if (destFilename == nullptr) {
        std::cout << result;
    } else {
        std::ofstream out(destFilename);
        out << result;
        out.close();
    }

    if (verbose) std::cout << "Finished!" << std::endl;

    delete formatter;
    return 0;
//
//
//
//    Parser parser {};
//
//    std::stringstream text {
//        "x = 5 + 2;\n"
//        "y = 3 - x;"
//    };
//
//    if (parser.parse(text)) {
//        std::cout << "Compilation successful!" << std::endl;
//        LatexFormatter latex {};
//        std::cout << "y = " << parser.get("y")->format(latex) << std::endl;
//    }


//    // The matrix below is a model-to-world matrix
//
//    GlmFormatter toGLM{};
//
//    auto* A = Matrix::square({
//        _("a_x"), _(0.0f),  _(0.0f), _(0.0f),
//        _(0.0f),  _("a_y"), _(0.0f), _(0.0f),
//        _(0.0f),  _(0.0f),  _(1.0f), _(0.0f),
//        _(0.0f),  _(0.0f),  _(0.0f), _(1.0f)
//    });
//
//    auto* O = Matrix::square({
//        _(1.0f), _(0.0f), _(0.0f), -*_("o_x"),
//        _(0.0f), _(1.0f), _(0.0f), *_("o_y")-_(1.0f),
//        _(0.0f), _(0.0f), _(1.0f), _(0.0f),
//        _(0.0f), _(0.0f), _(0.0f), _(1.0f)
//    });
//
//    auto* P = Matrix::square({
//        _(1.0f), _(0.0f), _(0.0f), _("p_x"),
//        _(0.0f), _(1.0f), _(0.0f), _("p_y"),
//        _(0.0f), _(0.0f), _(1.0f), _(0.0f),
//        _(0.0f), _(0.0f), _(0.0f), _(1.0f)
//    });
//
//    auto* R = Matrix::square({
//          _("cosR"), _("sinR"), _(0.0f), _(0.0f),
//        -*_("sinR"), _("cosR"), _(0.0f), _(0.0f),
//            _(0.0f),   _(0.0f), _(1.0f), _(0.0f),
//            _(0.0f),   _(0.0f), _(0.0f), _(1.0f)
//    });
//
//    auto* S = Matrix::square({
//        _("s_x"), _(0.0f), _(0.0f), _(0.0f),
//        _(0.0f), _("s_y"), _(0.0f), _(0.0f),
//        _(0.0f), _(0.0f), _(1.0f), _(0.0f),
//        _(0.0f), _(0.0f), _(0.0f), _(1.0f)
//    });
//
//    auto* M = Matrix::square({
//        _(1),  _(0), _(0), _(0),
//        _(0), _(-1), _(0), _(0),
//        _(0),  _(0), _(1), _(0),
//        _(0),  _(0), _(0), _(1)
//    });
//
//    auto* modelToWorld = *P * (*M * (*S * (*R * (*A * O))));
//
//    std::cout << "mWorldMatrix = " << modelToWorld->format(toGLM) << std::endl;
//
//    delete modelToWorld;
//
//    return 0;
}