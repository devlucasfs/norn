#include "pre_compiler.hpp"
#include "project.hpp"
#include "outputs.hpp"

int main(int c, char *argv[]) {
    if(! argv[1] ) CompilerOutputs::Fatal("You should pass the target file path to the Norn pre-compiler");
    std::string norn(argv[0]), path(argv[1]);
    ProjectData data = ProjectData::get(norn, path);
    std::ofstream file(argv[2]);
    file << precomp(data);
    file.close();
    return 0;
}
