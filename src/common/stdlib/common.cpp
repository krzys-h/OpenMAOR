//! Called when a pure virtual function is called (don't ask me how, code won't even compile like that)
extern "C" void __cxa_pure_virtual()
{
    while (true);
}
