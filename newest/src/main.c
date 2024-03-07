#include <elf_loader.h>
#include <assimp/postprocess.h>
#undef main

int main()
{
    elf_load_model(
        "C:\\Projects\\game-engine\\newest\\src\\model.fbx",
        aiProcessPreset_TargetRealtime_MaxQuality);
    return 0;
}