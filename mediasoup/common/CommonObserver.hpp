#pragma once
#include <memory>
#include <algorithm>

namespace mediasoup
{

template<typename ObserverType>
class CommonObserver {
public:
    void RegisterObserver(ObserverType* obs) {
        m_observer.reset(obs);
    }

    void UnregisterObserver() {
        m_observer.reset(nullptr);
    }
    
    template <typename FuncType>
    void NotifyObserver(FuncType func) {
        if (!m_observer) {
            return;
        }
        func(m_observer);
    }

private:
    std::unique_ptr<ObserverType> m_observer;
};

}