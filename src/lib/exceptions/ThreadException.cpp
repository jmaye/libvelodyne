#include "ThreadException.h"

using namespace std;

ThreadException::ThreadException(const string &msg) : runtime_error(msg) {
}
