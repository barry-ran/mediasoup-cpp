#include "MediasoupEngine.hpp"
#include "Logger.hpp"

namespace mediasoup
{

MediasoupEngine::MediasoupEngine() {

}

MediasoupEngine::~MediasoupEngine() {
   
}

bool MediasoupEngine::Init() {
    MS_lOGGERD("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
    MS_lOGGERW("Some error message with arg: {}", 1);
    MS_lOGGERE("Easy padding in numbers like {:08d}", 12);
	return true;
}

}