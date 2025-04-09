
#define LABTEXT_ODR
#include "include/LabText/LabText.h"
#include <stdio.h>

char const* test = R"(
(a '(b c))
(+ 123.4 (* 30 45)) (elephant banana canary)
(print "hello world")
(hello-world)

(LabSoundGraphToy

    :name "SN76477"

    (ls-node :name "Gain-3" :kind "Gain" :pos 869 116
        (pins
            '(:name "gain" :kind "param" :value 1.0))) 
    (ls-node :name "Gain-2" :kind "Gain" :pos 871 332
        (pins
            '(:name "gain" :kind "param" :value 1.0))) 
    (ls-node :name "Gain-1" :kind "Gain" :pos 1140 260
        (pins
            '(:name "gain" :kind "param" :value 1.0))) 
    (ls-node :name "Oscillator-2" :kind "Oscillator" :pos 56 170
        (pins
            '(:name "type" :kind "setting" :value "Enumeration Sine")
            '(:name "frequency" :kind "param" :value 2.0)
            '(:name "amplitude" :kind "param" :value 10.0)
            '(:name "bias" :kind "param" :value 0.0)
            '(:name "detune" :kind "param" :value 0.0)))
    (ls-node :name "Oscillator-1" :kind "Oscillator" :pos 450 110
        (pins
            '(:name "type" :kind "setting" :value "Enumeration Sine")
            '(:name "frequency" :kind "param" :value 440.0)
            '(:name "amplitude" :kind "param" :value 1.0)
            '(:name "bias" :kind "param" :value 0.0)
            '(:name "detune" :kind "param" :value 0.0)))
    (ls-node :name "Device-1" :kind "Device" :pos 1477 263)
    (ls-node :name "Group-1" :kind "Group" :pos 31 33)
    (ls-node :name "BiquadFilter-1" :kind "BiquadFilter" :pos 466 330
        (pins
            '(:name "type" :kind "setting" :value "Enumeration Low Pass")
            '(:name "frequency" :kind "param" :value 350.0)
            '(:name "Q" :kind "param" :value 1.0)
            '(:name "gain" :kind "param" :value 0.0)
            '(:name "detune" :kind "param" :value 0.0)))
    (ls-node :name "Noise-1" :kind "Noise" :pos 60 330
        (pins '(:name "type" :kind "setting" :value "Enumeration White")))
    (ls-node :name "ADSR-1" :kind "ADSR" :pos 1069 510
        (pins
            '(:name "type" :kind "attackTime" :value 0.125)
            '(:name "type" :kind "attackLevel" :value 1.0)
            '(:name "type" :kind "decayTime" :value 0.125)
            '(:name "type" :kind "sustainLevel" :value 0.5)
            '(:name "type" :kind "ReleaseTime" :value 0.125)
  
    (ls-connection :from "Gain-2" "out" :to "Gain-1" "Gain-1")
    (ls-connection :from "Gain-3" "out" :to "Gain-1" "Gain-1")
    (ls-connection :from "Oscillator-1" "out" :to "Gain-3" "Gain-3")
    (ls-connection :from "Gain-1" "out" :to "Device-1" "Device-1")
    (ls-connection :from "Gain-2" "out" :to "ADSR-1" "ADSR-1")
    (ls-connection :from "BiquadFilter-1" "out" :to "Gain-2" "Gain-2")
    (ls-connection :from "Oscillator-2" "out" :to "Oscillator-1" "Oscillator-1")
    (ls-connection :from "Noise-1" "out" :to "BiquadFilter-1" "BiquadFilter-1")
    (ls-connection :from "ADSR-2" "out" :to "Gain-1" "Gain-1")
)


)";


int main() {
    lab::Text::Sexpr s(lab::Text::StrView{test, strlen(test)});
    for (auto& e : s.expr) {
        switch (e.token) {
        case tsSexprPushList: printf("("); break;
        case tsSexprPopList: printf(")"); break;
        case tsSexprInteger: printf("%d ", s.ints[e.ref]); break;
        case tsSexprFloat: printf("%f ", s.floats[e.ref]); break;
        case tsSexprString: printf("\"%s\" ", s.strings[e.ref].c_str()); break;
        case tsSexprAtom: printf("%s ", s.strings[e.ref].c_str()); break;
        }
    }
    printf("\n----------------\n");

    tsParsedSexpr_t* parsed = tsParsedSexpr_New();
    parsed->token = tsSexprAtom;
    tsStrView_t str = (tsStrView_t){test, strlen(test)};

    /*tsStrView_t end_of_str =*/ tsStrViewParseSexpr(&str, parsed, 0);
    tsParsedSexpr_t* curr = parsed;
    while (curr) {
        switch (curr->token) {
        case tsSexprAtom: {
            std::string s = std::string(curr->str.curr, curr->str.sz);
            printf("%s ", s.c_str());
            break;
        }
        case tsSexprPushList: printf("("); break;
        case tsSexprPopList: printf(")"); break;
        case tsSexprInteger: printf("%lld ", curr->i); break;
        case tsSexprFloat: printf("%f ", curr->f); break;
        case tsSexprString: {
            std::string s = std::string(curr->str.curr, curr->str.sz);
            printf("\"%s\" ", s.c_str());
            break;
        }
        }
        curr = curr->next;
    } // while
    printf("\n");
    return 0;
}
