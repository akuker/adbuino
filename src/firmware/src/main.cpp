#ifdef ADBUINO
#include "adbuino.cpp"
#endif

#ifdef RP2040_PLATFORM
int core0_main(void);
int main(void) {
    return core0_main();
}
#endif
