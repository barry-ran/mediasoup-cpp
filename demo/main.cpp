#include <stdio.h>
#include <iostream>

#include "IMediasoup.hpp"

int main(int argc, char *argv[])
{   
    mediasoup::IMediasoup* mediasoup = mediasoup::CreateMediasoup();
    mediasoup->Init();
    mediasoup->Test();
	mediasoup::RtpCapabilities rtpCapabilities = mediasoup->GetSupportedRtpCapabilities();
	for (const auto& item : rtpCapabilities.headerExtensions) {
		std::cout << "headerExtensions.uri:" << item.uri << std::endl;
	}

    getchar();

    mediasoup->Destroy();
    mediasoup::DestroyMediasoup(mediasoup);

    getchar();
}
