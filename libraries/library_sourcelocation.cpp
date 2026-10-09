//releases unused memory, useful after elements release
#include <iostream>
#include <source_location>
#include <string_view>

// The default argument is evaluated at the call site, so loc holds the caller's location.
void log(std::string_view msg,
         const std::source_location loc = std::source_location::current())
{
    std::cout << loc.file_name() << ":" << loc.line() << ":" << loc.column()
              << " [" << loc.function_name() << "] " << msg << '\n';
}

// The old way: needs a macro to capture the call site.
#define LOG_OLD(msg) \
    std::cout << __FILE__ << ":" << __LINE__ << " " << msg << '\n'

void compute()
{
    log("inside compute");          // reports this line, in compute()
    LOG_OLD("same thing, old way"); // reports this line via macros
}

int main()
{
    log("hello from main");
    compute();
}
