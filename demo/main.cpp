#include <stdio.h>
#include <iostream>

#include "IMediasoup.hpp"
#include "IWorker.hpp"

int main(int argc, char *argv[])
{   
    mediasoup::IMediasoup* mediasoup = mediasoup::CreateMediasoup();
    mediasoup->Init();
    mediasoup->Test();
	mediasoup::RtpCapabilities rtpCapabilities = mediasoup->GetSupportedRtpCapabilities();
	for (const auto& item : rtpCapabilities.headerExtensions) {
		//std::cout << "headerExtensions.uri:" << item.uri << std::endl;
	}

	mediasoup::WorkerSettings ws;
	mediasoup::IWorker* worker = mediasoup->CreateWorker(ws);

    getchar();

    mediasoup->Destroy();
    mediasoup::DestroyMediasoup(mediasoup);

    getchar();
}
