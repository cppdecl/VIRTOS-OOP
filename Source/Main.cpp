#include "Core/System.h"

int main() 
{
    System& sys = System::GetInstance();

    if (!sys.Startup()) 
    {
        
    }

    while(sys.IsRunning()) 
    {
        sys.Run();
    }

    sys.Cleanup();
    
    return 0;
}