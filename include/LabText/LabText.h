#ifndef LABTEXT_H
#define LABTEXT_H

/*
 __  __           _           _
|  \/  | ___  ___| |__  _   _| | __ _
| |\/| |/ _ \/ __| '_ \| | | | |/ _` |
| |  | |  __/\__ \ | | | |_| | | (_| |
|_|  |_|\___||___/_| |_|\__,_|_|\__,_|
 _____         _
|_   _|____  _| |_
  | |/ _ \ \/ / __\
  | |  __/>  <| |_
  |_|\___/_/\_\\__|

This parser was written in the distant past by Nick Porcino and is freely
available on an as is basis. It is meant for educational
purposes and is not suitable for any particular purpose. No warranty is
expressed or implied. Use at your own risk. Do not operate heavy machinery or
governments while using this code.

License BSD-2 Clause.

*/

#ifdef _MSC_VER
    // provide some C99 types
    typedef unsigned int uint32_t;
    typedef int int32_t;
    typedef unsigned short uint16_t;
    typedef short int16_t;
    typedef bool _Bool;
#else
    #include <stdint.h>
    #include <stdbool.h>
    #if defined(__cplusplus) && !defined(_Bool)
        #define _Bool bool
    #endif
#endif

#ifndef EXTERNC
    #ifdef __cplusplus
        #define EXTERNC extern "C"
    #else
        #define EXTERNC
    #endif
#endif

#ifdef __cplusplus
    #include <string>
    #include <vector>
#endif


// Get Token
EXTERNC char const* tsGetToken                      (char const* pCurr, char const* pEnd, char delim, char const** resultStringBegin, uint32_t* stringLength);
EXTERNC char const* tsGetTokenWSDelimited           (char const* pCurr, char const* pEnd, char const** resultStringBegin, uint32_t* stringLength);
EXTERNC char const* tsGetTokenAlphaNumeric          (char const* pCurr, char const* pEnd, char const** resultStringBegin, uint32_t* stringLength);
EXTERNC char const* tsGetTokenAlphaNumericExt       (char const* pCurr, char const* pEnd, char const* ext, char const** resultStringBegin, uint32_t* stringLength);
EXTERNC char const* tsGetTokenExt                   (char const* pCurr, char const* pEnd, char const* ext, char const** resultStringBegin, uint32_t* stringLength);

EXTERNC char const* tsGetNameSpacedTokenAlphaNumeric(char const* pCurr, char const* pEnd, char namespaceChar, char const** resultStringBegin, uint32_t* stringLength);

// Get Value
EXTERNC char const* tsGetString                     (char const* pCurr, char const* pEnd, bool recognizeEscapes, char const** resultStringBegin, uint32_t* stringLength);
EXTERNC char const* tsGetString2                    (char const* pCurr, char const* pEnd, char strDelim, bool recognizeEscapes, char const** resultStringBegin, uint32_t* stringLength);
EXTERNC char const* tsGetInt16                      (char const* pCurr, char const* pEnd, int16_t* result);
EXTERNC char const* tsGetInt32                      (char const* pCurr, char const* pEnd, int32_t* result);
EXTERNC char const* tsGetUInt32                     (char const* pCurr, char const* pEnd, uint32_t* result);
EXTERNC char const* tsGetHex                        (char const* pCurr, char const* pEnd, uint32_t* result);
EXTERNC char const* tsGetFloat                      (char const* pcurr, char const* pEnd, float* result);

EXTERNC char const* tsScanForCharacter              (char const* pCurr, char const* pEnd, char delim);
EXTERNC char const* tsScanBackwardsForCharacter     (char const* pCurr, char const* pEnd, char delim);
EXTERNC char const* tsScanForWhiteSpace             (char const* pCurr, char const* pEnd);
EXTERNC char const* tsScanBackwardsForWhiteSpace    (char const* pCurr, char const* pStart);
EXTERNC char const* tsScanForNonWhiteSpace          (char const* pCurr, char const* pEnd);
EXTERNC char const* tsScanForTrailingNonWhiteSpace  (char const* pCurr, char const* pEnd);
EXTERNC char const* tsScanForQuote                  (char const* pCurr, char const* pEnd, char delim, bool recognizeEscapes);
EXTERNC char const* tsScanForEndOfLine              (char const* pCurr, char const* pEnd);
EXTERNC char const* tsScanForLastCharacterOnLine    (char const* pCurr, char const* pEnd);
EXTERNC char const* tsScanForBeginningOfNextLine    (char const* pCurr, char const* pEnd);
EXTERNC char const* tsScanPastCPPComments           (char const* pCurr, char const* pEnd);
EXTERNC char const* tsSkipCommentsAndWhitespace     (char const* pCurr, char const*const pEnd);

EXTERNC char const* tsExpect                        (char const* pCurr, char const*const pEnd, char const* pExpect);

EXTERNC _Bool tsIsWhiteSpace(char test);
EXTERNC _Bool tsIsNumeric   (char test);
EXTERNC _Bool tsIsAlpha     (char test);
EXTERNC _Bool tsIsIn        (const char* testString, char test);

// These UTF conversions return length. If dst is nullptr, the routines can be used for measuring a conversion
EXTERNC int32_t tsConvertUtf8ToUtf16(char16_t* dst, int32_t dst_size, const char* src);
EXTERNC int32_t tsConvertUtf16ToUtf8(char* dst, int32_t dst_size, const char16_t* src);


#ifdef __cplusplus

#include <string.h>
#include <vector>

namespace lab { namespace Text {
struct StrView
{
    char const* curr;
    size_t sz;

    bool begins(StrView const& rhs) const {
        return sz <= rhs.sz && !strncmp(curr, rhs.curr, sz);
    }

    bool operator==(StrView const& rhs) const {
        return sz == rhs.sz && !strncmp(curr, rhs.curr, sz);
    }
    bool operator!=(StrView const& rhs) const {
        return !(*this == rhs);
    }

    bool begins(const char* rhs) const {
        if (!rhs)
            return false;
        return sz <= strlen(rhs) && !strncmp(curr, rhs, sz);
    }

    bool operator==(const char* rhs) const {
        if (!rhs)
            return false;

        return strlen(rhs) == sz && !strncmp(rhs, curr, sz);
    }
    bool operator<(StrView const& rhs) {
        return strncmp(curr, rhs.curr, sz<rhs.sz?sz:rhs.sz) < 0;
    }
};

inline bool IsEmpty(StrView const& s)
{
    return (s.curr == nullptr) || (s.sz == 0);
}

inline StrView GetToken(StrView s, char delim, StrView& result)
{
    uint32_t sz;
    char const* next = tsGetToken(s.curr, s.curr + s.sz, delim, &result.curr, &sz);
    result.sz = sz;
    return { next, static_cast<size_t>(s.curr + s.sz - next) };
}

inline StrView GetTokenWSDelimited(StrView s, char delim, StrView& result)
{
    uint32_t sz;
    char const* next = tsGetTokenWSDelimited(s.curr, s.curr + s.sz, &result.curr, &sz);
    result.sz = sz;
    return { next, static_cast<size_t>(s.curr + s.sz - next) };
}

inline StrView GetTokenAlphaNumeric(StrView s, StrView& result)
{
    uint32_t sz;
    char const* next = tsGetTokenAlphaNumeric(s.curr, s.curr + s.sz, &result.curr, &sz);
    result.sz = sz;
    return { next, static_cast<size_t>(s.curr + s.sz - next) };
}

inline StrView GetTokenAlphaNumericExt (StrView s, char const* ext, StrView& result)
{
    uint32_t sz;
    char const* next = tsGetTokenAlphaNumericExt(s.curr, s.curr + s.sz, ext, &result.curr, &sz);
    result.sz = sz;
    return { next, static_cast<size_t>(s.curr + s.sz - next) };
}

inline StrView GetTokenExt(StrView s, char const* ext, StrView& result)
{
    uint32_t sz;
    char const* next = tsGetTokenExt(s.curr, s.curr + s.sz, ext, &result.curr, &sz);
    result.sz = sz;
    return { next, static_cast<size_t>(s.curr + s.sz - next) };
}

inline StrView GetNameSpacedTokenAlphaNumeric(StrView s, char namespaceChar, StrView& result)
{
    uint32_t sz;
    char const* next = tsGetNameSpacedTokenAlphaNumeric(s.curr, s.curr + s.sz, namespaceChar, &result.curr, &sz);
    result.sz = sz;
    return { next, static_cast<size_t>(s.curr + s.sz - next) };
}

inline StrView GetString(StrView s, bool recognizeEscapes, StrView& result)
{
    uint32_t sz;
    char const* next = tsGetString(s.curr, s.curr + s.sz, recognizeEscapes, &result.curr, &sz);
    result.sz = sz;
    return { next, static_cast<size_t>(s.curr + s.sz - next) };
}

inline StrView GetString2(StrView s, char namespaceChar, char strDelim, bool recognizeEscapes, StrView& result)
{
    uint32_t sz;
    char const* next = tsGetString2(s.curr, s.curr + s.sz, strDelim, recognizeEscapes, &result.curr, &sz);
    result.sz = sz;
    return { next, static_cast<size_t>(s.curr + s.sz - next) };
}

inline StrView GetInt16(StrView s, int16_t& result)
{
    char const* next = tsGetInt16(s.curr, s.curr + s.sz, &result);
    return { next, static_cast<size_t>(s.curr + s.sz - next) };
}

inline StrView GetInt32(StrView s, int32_t& result)
{
    char const* next = tsGetInt32(s.curr, s.curr + s.sz, &result);
    return { next, static_cast<size_t>(s.curr + s.sz - next) };
}

inline StrView GetUInt32(StrView s, uint32_t& result)
{
    char const* next = tsGetUInt32(s.curr, s.curr + s.sz, &result);
    return { next, static_cast<size_t>(s.curr + s.sz - next) };
}

inline StrView GetHex(StrView s, uint32_t& result)
{
    char const* next = tsGetHex(s.curr, s.curr + s.sz, &result);
    return { next, static_cast<size_t>(s.curr + s.sz - next) };
}

inline StrView GetFloat(StrView s, float& result)
{
    char const* next = tsGetFloat(s.curr, s.curr + s.sz, &result);
    return { next, static_cast<size_t>(s.curr + s.sz - next) };
}

inline StrView ScanForCharacter(StrView s, char delim)
{
    char const* next = tsScanForCharacter(s.curr, s.curr + s.sz, delim);
    return { next, static_cast<size_t>(s.curr + s.sz - next) };
}

inline StrView ScanBackwardsForCharacter(StrView s, char delim)
{
    char const* next = tsScanBackwardsForCharacter(s.curr, s.curr + s.sz, delim);
    return { next, static_cast<size_t>(s.curr + s.sz - next) };
}

inline StrView ScanForWhiteSpace(StrView s)
{
    char const* next = tsScanForWhiteSpace(s.curr, s.curr + s.sz);
    return { next, static_cast<size_t>(s.curr + s.sz - next) };
}

inline StrView ScanBackwardsForWhiteSpace(StrView s)
{
    char const* next = tsScanBackwardsForWhiteSpace(s.curr, s.curr + s.sz);
    return { next, static_cast<size_t>(s.curr + s.sz - next) };
}

inline StrView ScanForNonWhiteSpace(StrView s)
{
    char const* next = tsScanForNonWhiteSpace(s.curr, s.curr + s.sz);
    return { next, static_cast<size_t>(s.curr + s.sz - next) };
}

inline StrView ScanForTrailingNonWhiteSpace(StrView s)
{
    char const* next = tsScanForTrailingNonWhiteSpace(s.curr, s.curr + s.sz);
    return { next, static_cast<size_t>(s.curr + s.sz - next) };
}

inline StrView ScanForQuote(StrView s, char delim, bool recognizeEscapes)
{
    char const* next = tsScanForQuote(s.curr, s.curr + s.sz, delim, recognizeEscapes);
    return { next, static_cast<size_t>(s.curr + s.sz - next) };
}

inline StrView ScanForEndOfLine(StrView s)
{
    char const* next = tsScanForEndOfLine(s.curr, s.curr + s.sz);
    return { next, static_cast<size_t>(s.curr + s.sz - next) };
}

inline StrView ScanForEndOfLine(StrView s, StrView& skipped)
{
    skipped = s;
    char const* next = tsScanForEndOfLine(s.curr, s.curr + s.sz);
    skipped.sz = next - skipped.curr;
    return { next, static_cast<size_t>(s.curr + s.sz - next) };
}

inline StrView ScanForLastCharacterOnLine(StrView s)
{
    char const* next = tsScanForLastCharacterOnLine(s.curr, s.curr + s.sz);
    return { next, static_cast<size_t>(s.curr + s.sz - next) };
}

inline StrView ScanForBeginningOfNextLine(StrView s)
{
    char const* next = tsScanForBeginningOfNextLine(s.curr, s.curr + s.sz);
    return { next, static_cast<size_t>(s.curr + s.sz - next) };
}

inline StrView ScanPastCPPComments(StrView s)
{
    char const* next = tsScanPastCPPComments(s.curr, s.curr + s.sz);
    return { next, static_cast<size_t>(s.curr + s.sz - next) };
}

inline StrView SkipCommentsAndWhitespace(StrView s)
{
    char const* next = tsSkipCommentsAndWhitespace(s.curr, s.curr + s.sz);
    return { next, static_cast<size_t>(s.curr + s.sz - next) };
}

inline StrView Expect(StrView s, StrView expect)
{
    char const* next = tsExpect(s.curr, s.curr + s.sz, expect.curr);
    return { next, static_cast<size_t>(s.curr + s.sz - next) };
}

inline StrView Strip(StrView s)
{
    StrView result = ScanForNonWhiteSpace(s);
    while (result.sz > 0)
    {
        if (!tsIsWhiteSpace(result.curr[result.sz - 1]))
            break;
        --result.sz;
    }
    return result;
}

std::vector<StrView> Split(StrView s, char split);


struct Sexpr {

    enum Token { PushList, PopList, Integer, Float, String, Atom };

    struct Elem {
        Token token;
        int ref;
    };

    std::vector<Elem> expr;
    std::vector<int> ints;
    std::vector<float> floats;
    std::vector<std::string> strings;

    int balance = 0;

    explicit Sexpr(StrView s) {
        Parse(s);
    }

private:
    StrView Parse(StrView s) {
        StrView curr = s;
        while (true) {
            curr = ScanForNonWhiteSpace(s);
            if (curr.sz == 0)
                return curr; // parsing finished
            if (*curr.curr == ';') {
                curr = ScanForBeginningOfNextLine(curr); // LISP comment
                continue;
            }
            if (*curr.curr != '(') {
                curr.sz = 0; // stop parsing
                return curr; // error
            }
            break;
        }
        ++balance;
        expr.push_back({ PushList, 0 });

        curr.curr++;
        curr.sz--;

        while (true) {
            curr = ScanForNonWhiteSpace(curr);
            if (curr.sz == 0)
                return curr;

            if (*curr.curr == ';') {
                curr = ScanForNonWhiteSpace(ScanForBeginningOfNextLine(curr));
                continue;
            }
            if (*curr.curr == '"') {
                StrView str;
                curr = ScanForNonWhiteSpace(GetString(curr, true, str));
                expr.push_back({ String, (int)strings.size() });
                strings.push_back(std::string(str.curr, str.sz));
                if (curr.sz == 0)
                    return curr;
                continue;
            }
            if (*curr.curr == ')') {
                --balance;
                expr.push_back({ PopList, 0 });
                curr.curr++;
                curr.sz--;
                continue;
            }
            if (*curr.curr == '(') {
                curr = Parse(curr);
                continue;
            }

            StrView token = curr;
            token.sz = 0;
            while (curr.sz > 0) {
                if (*curr.curr == '"')
                    break;
                if (*curr.curr == '(')
                    break;
                if (*curr.curr == ')')
                    break;
                if (tsIsWhiteSpace(*curr.curr))
                    break;
                if (*curr.curr == ';')
                    break;
                token.sz++;
                curr.curr++;
                curr.sz--;
            }
            if (!token.sz)
                continue;

            float f;
            StrView test = GetFloat(token, f);
            if (test.curr != token.curr) {
                expr.push_back({ Float, (int)floats.size() });
                floats.push_back(f);
                curr.curr = test.curr;
                curr.sz -= test.sz;
            }
            else {
                int32_t i;
                StrView test = GetInt32(token, i);
                if (test.curr != token.curr) {
                    expr.push_back({ Integer, (int)ints.size() });
                    ints.push_back(i);
                    curr.curr = test.curr;
                    curr.sz -= test.sz;
                }
                else {
                    expr.push_back({ Atom, (int)strings.size() });
                    strings.push_back(std::string(token.curr, token.sz));
                }
            }
            curr = ScanForNonWhiteSpace(curr);
        }
    }
};



}} // lab::Text

#endif // cplusplus


#endif // LABTEXT_H

#ifdef LABTEXT_ODR

//------------------------------------------------------------------------------
// IMPLEMENTATION
//------------------------------------------------------------------------------

#include <math.h>
#include <stdbool.h>

//! @todo replace Assert with custom error reporting mechanism
#include <assert.h>
#define Assert assert


/*
* The two functions, tsConvertUtf16ToUtf8 and tsConvertUtf8ToUtf16 are
* originally from the Effekseer library, and bear the following license:

The MIT License (MIT)

Copyright (c) 2011 Effekseer Project

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

/**
    @brief    Convert UTF16 into UTF8
    @param    dst    a pointer to destination buffer
    @param    dst_size    a length of destination buffer
    @param    src            a source buffer
    @return    length except 0
*/
int32_t tsConvertUtf16ToUtf8(char* dst, int32_t dst_size, const char16_t* src)
{
    int32_t cnt = 0;
    const char16_t* wp = src;
    char* cp = dst;

    if (dst_size == 0)
        return 0;

    dst_size -= 3;

    for (cnt = 0; cnt < dst_size;)
    {
        char16_t wc = *wp++;
        if (wc == 0)
        {
            break;
        }
        if ((wc & ~0x7f) == 0)
        {
            if (cp)
                *cp++ = wc & 0x7f;
            cnt += 1;
        }
        else if ((wc & ~0x7ff) == 0)
        {
            if (cp) {
                *cp++ = ((wc >> 6) & 0x1f) | 0xc0;
                *cp++ = ((wc) & 0x3f) | 0x80;
            }
            cnt += 2;
        }
        else
        {
            if (cp) {
                *cp++ = ((wc >> 12) & 0xf) | 0xe0;
                *cp++ = ((wc >> 6) & 0x3f) | 0x80;
                *cp++ = ((wc) & 0x3f) | 0x80;
            }
            cnt += 3;
        }
    }
    *cp = '\0';
    return cnt;
}

/**
    @brief    Convert UTF8 into UTF16
    @param    dst    a pointer to destination buffer
    @param    dst_size    a length of destination buffer
    @param    src            a source buffer
    @return    length except 0
*/
int32_t tsConvertUtf8ToUtf16(char16_t* dst, int32_t dst_size, const char* src)
{
    int32_t i, code = 0;
    int8_t c0, c1, c2 = 0;
    int8_t* srci = reinterpret_cast<int8_t*>(const_cast<char*>(src));
    if (dst_size == 0)
        return 0;

    dst_size -= 1;

    for (i = 0; i < dst_size; i++)
    {
        uint16_t wc;

        c0 = *srci;
        srci++;
        if (c0 == '\0')
        {
            break;
        }
        // convert UTF8 to UTF16
        code = (uint8_t)c0 >> 4;
        if (code <= 7)
        {
            // 8bit character
            wc = c0;
        }
        else if (code >= 12 && code <= 13)
        {
            // 16bit  character
            c1 = *srci;
            srci++;
            wc = ((c0 & 0x1F) << 6) | (c1 & 0x3F);
        }
        else if (code == 14)
        {
            // 24bit character
            c1 = *srci;
            srci++;
            c2 = *srci;
            srci++;
            wc = ((c0 & 0x0F) << 12) | ((c1 & 0x3F) << 6) | (c2 & 0x3F);
        }
        else
        {
            continue;
        }
        if (dst)
            dst[i] = wc;
    }
    if (dst)
        dst[i] = 0;
    return i;
}

//----------------------------------------------------------------------------

char const* tsScanForQuote(
    char const* pCurr, char const* pEnd,
    char delim,
    bool recognizeEscapes)
{
    Assert(pCurr && pEnd && pEnd >= pCurr);

    while (pCurr < pEnd) {
        if (*pCurr == '\\' && recognizeEscapes) // not handling multicharacter escapes such as \u23AB
            ++pCurr;
        else if (*pCurr == delim)
            break;
        ++pCurr;
    }

    return pCurr;
}

char const* tsScanForWhiteSpace(
    char const* pCurr, char const* pEnd)
{
    Assert(pCurr && pEnd && pEnd >= pCurr);

    while (pCurr < pEnd && !tsIsWhiteSpace(*pCurr))
        ++pCurr;

    return pCurr+1;
}

char const* tsScanForNonWhiteSpace(
   char const* pCurr, char const* pEnd)
{
    Assert(pCurr && pEnd && pEnd >= pCurr);

    while (pCurr < pEnd && tsIsWhiteSpace(*pCurr))
        ++pCurr;

    return pCurr;
}

char const* tsScanBackwardsForWhiteSpace(
    char const* pCurr, char const* pStart)
{
    Assert(pCurr && pStart && pStart <= pCurr);

    while (pCurr >= pStart && !tsIsWhiteSpace(*pCurr))
        --pCurr;

    return pCurr;
}

char const* tsScanForTrailingNonWhiteSpace(
    char const* pCurr, char const* pEnd)
{
    Assert(pCurr && pEnd && pEnd >= pCurr);

    while (pCurr < pEnd && tsIsWhiteSpace(*pEnd))
        --pEnd;

    return pEnd;
}

char const* tsScanForCharacter(
    char const* pCurr, char const* pEnd,
    char delim)
{
    Assert(pCurr && pEnd);

    while (pCurr < pEnd && *pCurr != delim)
        ++pCurr;

    return pCurr;
}

char const* tsScanBackwardsForCharacter(
    char const* pCurr, char const* pStart,
    char delim)
{
    Assert(pCurr && pStart && pStart <= pCurr);

    while (pCurr >= pStart && *pCurr != delim)
        --pCurr;

    return pCurr;
}

char const* tsScanForEndOfLine(
    char const* pCurr, char const* pEnd)
{
    while (pCurr < pEnd)
    {
        if (*pCurr == '\r')
        {
            ++pCurr;
            if (*pCurr == '\n')
                ++pCurr;
            break;
        }
        if (*pCurr == '\n')
        {
            ++pCurr;
            if (*pCurr == '\r')
                ++pCurr;
            break;
        }

        ++pCurr;
    }
    return pCurr;
}

char const* tsScanForLastCharacterOnLine(
    char const* pCurr, char const* pEnd)
{
    while (pCurr < pEnd)
    {
        if (pCurr[1] == '\r' || pCurr[1] == '\n' || pCurr[1] == '\0')
        {
            break;
        }

        ++pCurr;
    }
    return pCurr;
}

char const* tsScanForBeginningOfNextLine(
    char const* pCurr, char const* pEnd)
{
    pCurr = tsScanForEndOfLine(pCurr, pEnd);
    return (tsScanForNonWhiteSpace(pCurr, pEnd));
}

char const* tsScanPastCPPComments(
    char const* pCurr, char const* pEnd)
{
    if (*pCurr == '/')
    {
        if (pCurr[1] == '/')
        {
            pCurr = tsScanForEndOfLine(pCurr, pEnd);
        }
        else if (pCurr[1] == '*')
        {
            pCurr = &pCurr[2];
            while (pCurr < pEnd)
            {
                if (pCurr[0] == '*' && pCurr[1] == '/')
                {
                    pCurr = &pCurr[2];
                    break;
                }

                ++pCurr;
            }
        }
    }

    return pCurr;
}

char const* tsSkipCommentsAndWhitespace(
    char const* curr, char const*const end)
{
    bool moved = true;
    while (moved)
    {
        char const* past = tsScanForNonWhiteSpace(curr, end);
        curr = past;

        past = tsScanPastCPPComments(curr, end);
        moved = past != curr;
        curr = past;
    }

    return tsScanForNonWhiteSpace(curr, end);
}

char const* tsGetToken(
    char const* pCurr, char const* pEnd,
    char delim,
    char const** resultStringBegin, uint32_t* stringLength)
{
    Assert(pCurr && pEnd);

    pCurr = tsScanForNonWhiteSpace(pCurr, pEnd);
    *resultStringBegin = pCurr;
    char const* pStringEnd = tsScanForCharacter(pCurr, pEnd, delim);
    *stringLength = (uint32_t)(pStringEnd - *resultStringBegin);
    return pStringEnd;
}

char const* tsGetTokenWSDelimited(
    char const* pCurr, char const* pEnd,
    char const** resultStringBegin, uint32_t* stringLength)
{
    pCurr = tsScanForNonWhiteSpace(pCurr, pEnd);
    *resultStringBegin = pCurr;
    char const* pStringEnd = tsScanForWhiteSpace(pCurr, pEnd);
    *stringLength = (uint32_t)(pStringEnd - *resultStringBegin);
    return pStringEnd;
}

char const* tsGetTokenAlphaNumericExt(
    char const* pCurr, char const* pEnd,
    char const* ext_,
    char const** resultStringBegin, uint32_t* stringLength)
{
    Assert(pCurr && pEnd);

    pCurr = tsScanForNonWhiteSpace(pCurr, pEnd);
    *resultStringBegin = pCurr;
    *stringLength = 0;

    while (pCurr < pEnd)
    {
        char test = pCurr[0];

        if (tsIsWhiteSpace(test))
            break;

        _Bool accept = tsIsNumeric(test) || tsIsAlpha(test);
        char const* ext = ext_;
        for ( ; *ext && !accept; ++ext)
            accept |= *ext == test;

        if (!accept)
            break;

        ++pCurr;
        *stringLength += 1;
    }

    return pCurr;
}

char const* tsGetTokenExt(
    char const* pCurr, char const* pEnd,
    char const* ext_,
    char const** resultStringBegin, uint32_t* stringLength)
{
    Assert(pCurr && pEnd);

    pCurr = tsScanForNonWhiteSpace(pCurr, pEnd);
    *resultStringBegin = pCurr;
    *stringLength = 0;

    while (pCurr < pEnd)
    {
        char test = pCurr[0];

        if (tsIsWhiteSpace(test))
            break;

        _Bool accept = false;
        char const* ext = ext_;
        for (; *ext && !accept; ++ext)
            accept |= *ext == test;

        if (!accept)
            break;

        ++pCurr;
        *stringLength += 1;
    }

    return pCurr;
}

char const* tsGetTokenAlphaNumeric(
    char const* pCurr, char const* pEnd,
    char const** resultStringBegin, uint32_t* stringLength)
{
    Assert(pCurr && pEnd);

    pCurr = tsScanForNonWhiteSpace(pCurr, pEnd);
    *resultStringBegin = pCurr;
    *stringLength = 0;

    while (pCurr < pEnd)
    {
        char test = pCurr[0];

        if (tsIsWhiteSpace(test))
            break;

        _Bool accept = ((test == '_') || tsIsNumeric(test) || tsIsAlpha(test));

        if (!accept)
            break;

        ++pCurr;
        *stringLength += 1;
    }

    return pCurr;
}

char const* tsGetNameSpacedTokenAlphaNumeric(
    char const* pCurr, char const* pEnd,
    char namespaceChar,
    char const** resultStringBegin, uint32_t* stringLength)
{
    Assert(pCurr && pEnd);

    pCurr = tsScanForNonWhiteSpace(pCurr, pEnd);
    *resultStringBegin = pCurr;
    *stringLength = 0;

    while (pCurr < pEnd)
    {
        char test = pCurr[0];

        if (tsIsWhiteSpace(test))
            break;

        // should pass in a string of acceptable characters, ie "$^_"
        _Bool accept = ((test == namespaceChar) || (test == '$') || (test == '^') || (test == '_') || tsIsNumeric(test) || tsIsAlpha(test));

        if (!accept)
            break;

        ++pCurr;
        *stringLength += 1;
    }

    return pCurr;
}

char const* tsGetString(
    char const* pCurr, char const* pEnd,
    bool recognizeEscapes,
    char const** resultStringBegin, uint32_t* stringLength)
{
    Assert(pCurr && pEnd && pEnd >= pCurr);

    pCurr = tsScanForQuote(pCurr, pEnd, '\"', recognizeEscapes);

    if (pCurr < pEnd)
    {
        ++pCurr;    // skip past quote
        *resultStringBegin = pCurr;

        pCurr = tsScanForQuote(pCurr, pEnd, '\"', recognizeEscapes);

        if (pCurr <= pEnd)
            *stringLength = (uint32_t)(pCurr - *resultStringBegin);
        else
            *stringLength = 0;

        ++pCurr;    // point past closing quote
    }
    else
        *stringLength = 0;

    return pCurr;
}

char const* tsGetString2(
                        char const* pCurr, char const* pEnd,
                        char delim,
                        bool recognizeEscapes,
                        char const** resultStringBegin, uint32_t* stringLength)
{
    Assert(pCurr && pEnd && pEnd >= pCurr);

    pCurr = tsScanForQuote(pCurr, pEnd, delim, recognizeEscapes);

    if (pCurr < pEnd)
    {
        ++pCurr;    // skip past quote
        *resultStringBegin = pCurr;

        pCurr = tsScanForQuote(pCurr, pEnd, delim, recognizeEscapes);

        if (pCurr <= pEnd)
            *stringLength = (uint32_t)(pCurr - *resultStringBegin);
        else
            *stringLength = 0;

        ++pCurr;    // point past closing quote
    }
    else
        *stringLength = 0;

    return pCurr;
}

// Match pExpect. If pExect is found in the input stream, return pointing
// to the character that follows, otherwise return the start of the input stream

char const* tsExpect(
    char const* pCurr, char const*const pEnd,
    char const* pExpect)
{
    char const* pScan = pCurr;
    while (pScan != pEnd && *pScan == *pExpect && *pExpect != '\0') {
        ++pScan;
        ++pExpect;
    }
    return (*pExpect == '\0' ? pScan : pCurr);
}

char const* tsGetInt16(
    char const* pCurr, char const* pEnd,
    int16_t* result)
{
    int32_t longresult;
    char const* retval = tsGetInt32(pCurr, pEnd, &longresult);
    *result = (int16_t) longresult;
    return retval;
}

char const* tsGetInt32(
    char const* pCurr, char const* pEnd,
    int32_t* result)
{
    char const* start = pCurr;

    _Bool foundNumeric = false;
    pCurr = tsScanForNonWhiteSpace(pCurr, pEnd);

    int ret = 0;

    _Bool signFlip = false;

    if (*pCurr == '+')
    {
        ++pCurr;
    }
    else if (*pCurr == '-')
    {
        ++pCurr;
        signFlip = true;
    }

    while (pCurr < pEnd)
    {
        if (!tsIsNumeric(*pCurr))
        {
            break;
        }
        foundNumeric = true;
        ret = ret * 10 + *pCurr - '0';
        ++pCurr;
    }

    if (!foundNumeric)
        return start;

    if (signFlip)
    {
        ret = -ret;
    }
    *result = ret;
    return pCurr;
}

char const* tsGetUInt32(
    char const* pCurr, char const* pEnd,
    uint32_t* result)
{
    char const* start = pCurr;
    pCurr = tsScanForNonWhiteSpace(pCurr, pEnd);

    uint32_t ret = 0;
    _Bool foundNumeric = false;

    while (pCurr < pEnd)
    {
        if (!tsIsNumeric(*pCurr))
        {
            break;
        }
        foundNumeric = true;
        ret = ret * 10 + *pCurr - '0';
        ++pCurr;
    }

    if (!foundNumeric)
        return start;

    *result = ret;
    return pCurr;
}

char const* tsGetFloat(
    char const* pCurr, char const* pEnd,
    float* result)
{
    char const* start = pCurr;
    pCurr = tsScanForNonWhiteSpace(pCurr, pEnd);

    float ret = 0.0f;

    _Bool signFlip = false;
    _Bool foundNumeric = false;

    if (*pCurr == '+')
    {
        ++pCurr;
    }
    else if (*pCurr == '-')
    {
        ++pCurr;
        signFlip = true;
    }

    // get integer part
    uint32_t uintPart;
    const char* test = tsGetUInt32(pCurr, pEnd, &uintPart);
    if (test == pCurr)
        return start;
    pCurr = test;

    ret = (float) uintPart;

    // get fractional part
    if (*pCurr != '.')
        return start;   // no decimal, not a float

    ++pCurr;

    float scaler = 0.1f;
    while (pCurr < pEnd)
    {
        if (!tsIsNumeric(*pCurr))
        {
            break;
        }
        foundNumeric = true;
        ret = ret + (float)(*pCurr - '0') * scaler;
        ++pCurr;
        scaler *= 0.1f;
    }

    // get exponent
    if (*pCurr == 'e' || *pCurr == 'E')
    {
        ++pCurr;

        int32_t intPart = 0;
        test = tsGetInt32(pCurr, pEnd, &intPart);

        if (pCurr == test)
            return start;   // e without a number following it is malformed

        ret *= powf(10.0f, (float) intPart);
    }

    if (signFlip)
    {
        ret = -ret;
    }
    *result = ret;
    return pCurr;
}

char const* tsGetHex(
    char const* pCurr, char const* pEnd,
    uint32_t* result)
{
    pCurr = tsScanForNonWhiteSpace(pCurr, pEnd);

    int ret = 0;

    while (pCurr < pEnd)
    {
        if (tsIsNumeric(*pCurr))
        {
            ret = ret * 16 + *pCurr - '0';
        }
        else if (*pCurr >= 'A' && *pCurr <= 'F')
        {
            ret = ret * 16 + *pCurr - 'A' + 10;
        }
        else if (*pCurr >= 'a' && *pCurr <= 'f')
        {
            ret = ret * 16 + *pCurr - 'a' + 10;
        }
        else
        {
            break;
        }
        ++pCurr;
    }

    *result = ret;
    return pCurr;
}

_Bool tsIsIn(const char* testString, char test)
{
    for (; *testString != '\0'; ++testString)
        if (*testString == test)
            return true;
    return false;
}

_Bool tsIsWhiteSpace(char test)
{
    return (test == 9 || test == ' ' || test == 13 || test == 10);
}

_Bool tsIsNumeric(char test)
{
    return (test >= '0' && test <= '9');
}

_Bool tsIsAlpha(char test)
{
    return ((test >= 'a' && test <= 'z') || (test >= 'A' && test <= 'Z'));
}


namespace lab { namespace Text {
std::vector<StrView> Split(StrView s, char splitter)
{
    std::vector<StrView> result;
    char const* src = s.curr;
    char const* curr = src;
    size_t remaining = s.sz;
    size_t sz = 0;
    while (remaining > 0)
    {
        if (*src != splitter)
        {
            ++sz;
            ++src;
        }
        else
        {
            result.push_back(StrView{curr, sz});
            sz = 0;
            ++src;
            curr = src;
        }
        --remaining;
    }

    // capture last crumb
    if (sz > 0)
        result.push_back(StrView{curr, sz});

    return result;
}
}} // lab::Text

#endif // LABTEXT_ODR

