
#include "include/LabText/LabText.h"
#include <stdio.h>

char const* test = R"(
(+ 1 (* 3 4)) )";


int main() {
    lab::Text::Sexpr s(lab::Text::StrView{test, strlen(test)});
    printf("hello world\n");

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
