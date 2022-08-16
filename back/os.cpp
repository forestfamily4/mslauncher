#include "os.h"
#include <qglobal.h>

os::os()
{

}

#ifdef Q_OS_WIN
int os::Getos(){//0 win 1 mac 2 linux 3 None
    return 0;
}
#elif Q_OS_MACX
int os::Getos(){//0 win 1 mac 2 linux 3 None
    return 1;
}
#elif Q_OS_LINUX
int os::Getos(){//0 win 1 mac 2 linux 3 None
    return 2;
}
#else
int os::Getos(){//0 win 1 mac 2 linux 3 None
    return 3;
}
#endif
