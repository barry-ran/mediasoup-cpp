#include <stdio.h>

#include "IMediasoup.hpp"

int main(int argc, char *argv[])
{   
    mediasoup::IMediasoup* mediasoup = mediasoup::CreateMediasoup();
    mediasoup->Init();
    mediasoup->Test();

    getchar();

    mediasoup->Destroy();
    mediasoup::DestroyMediasoup(mediasoup);

    getchar();
}
