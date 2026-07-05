#include "pre_compiler.hpp"
#include "project.hpp"
#include "outputs.hpp"

int main(int c, char *argv[]) {
    if(! argv[1] ) CompilerOutputs::Fatal("You should pass the target file path to the Norn pre-compiler");
    std::string norn(argv[0]), path(argv[1]);
    auto project_data = ProjectData::get(norn, path);
    auto code = precomp(project_data);
    return 0;
}
