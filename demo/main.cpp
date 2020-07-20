#include <stdio.h>
#include <iostream>

#include "IMediasoup.hpp"
#include "IWorker.hpp"

class Worker : public mediasoup::IWorker::Observer {
public:
	Worker() {}
	void OnSuccess() override {
		std::cout << "++++++:OnSuccess" << std::endl;
	}
};
int main(int argc, char *argv[])
{   
    mediasoup::IMediasoup* mediasoup = mediasoup::CreateMediasoup();
    mediasoup->Init();
    mediasoup->Test();
	mediasoup::RtpCapabilities rtpCapabilities = mediasoup->GetSupportedRtpCapabilities();
	for (const auto& item : rtpCapabilities.headerExtensions) {
		//std::cout << "headerExtensions.uri:" << item.uri << std::endl;
	}

	Worker myWorker;	
	mediasoup::WorkerSettings ws;
	mediasoup::IWorker* worker = mediasoup->CreateWorker(&myWorker, ws);

    getchar();

    mediasoup->Destroy();
    mediasoup::DestroyMediasoup(mediasoup);

    getchar();
}
