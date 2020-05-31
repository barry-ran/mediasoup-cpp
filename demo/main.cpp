#include <stdio.h>

#include "IMediasoupEngine.hpp"

int main(int argc, char *argv[])
{   
    mediasoup::IMediasoupEngine* engine = mediasoup::CreateMediasoupEngine();
    engine->Init();
    engine->Test();

    getchar();

    engine->Destroy();
    mediasoup::DestroyMediasoupEngine(engine);

    getchar();
}
