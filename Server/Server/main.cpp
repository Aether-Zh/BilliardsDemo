#include "Server.h"

int main(int argc, char* argv[])
{
    listener_ = CreateListener();
    if (listener_ == -1)
        return -1;
    
    int kq = kqueue();
    if (!Register(kq, listener_))
    {
        std::cerr << "Register listener to kq failed.\n";
        return -1;
    }
    
    WaitEvent(kq);
    
    return 0;
}
