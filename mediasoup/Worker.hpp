#pragma once
#include "IWorker.hpp"

namespace mediasoup
{

class Worker : public IWorker {
public:
    Worker();
    virtual ~Worker();
};

}