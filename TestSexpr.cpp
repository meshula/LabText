
#include "include/LabText/LabText.h"
#include <stdio.h>

char const* test = R"(
(a '(b c))
(+ 123.4 (* 30 45)) (elephant banana canary)
(print "hello world")
(hello-world)
)";


int main() {
    lab::Text::Sexpr s(lab::Text::StrView{test, strlen(test)});
    for (auto& e : s.expr) {
        switch (e.token) {
        case lab::Text::Sexpr::PushList: printf("("); break;
        case lab::Text::Sexpr::PopList: printf(")"); break;
        case lab::Text::Sexpr::Integer: printf("%d ", s.ints[e.ref]); break;
        case lab::Text::Sexpr::Float: printf("%f ", s.floats[e.ref]); break;
        case lab::Text::Sexpr::String: printf("\"%s\" ", s.strings[e.ref].c_str()); break;
        case lab::Text::Sexpr::Atom: printf("%s ", s.strings[e.ref].c_str()); break;
        }
    }
    printf("\n");

    return 0;
}
