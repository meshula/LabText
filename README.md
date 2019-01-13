
LabText.h is now a single file text parsing utility. There are some legacy files
in LabText, which are all slated for demolition.

To use LabText.h, include it in your project, and include it at once defining
LABTEXT_ODR to cause the functions to be compiled into your program.

There are two interfaces to LabText; the old char* oriented version, and a new
interface based around a StrView struct. The char* interfaces are deprecated and
will be removed.

StrView is extremely simple, and has no std dependencies or affordances. The
sole purpose of StrView is to represent a non-owning view on a buffer of char.

```cpp
struct StrView
{
    char const* curr;
    size_t sz;

    bool operator==(StrView const& rhs) const
    {
        return sz == rhs.sz && !strncmp(curr, rhs.curr, sz);
    }
    bool operator!=(StrView const& rhs) const
    {
        return !(*this == rhs);
    }
};
```

There are several utilities available for StrView. In general, they take a
StrView, and return a new StrView of remaining unprocessed input.

The functions are listed here, and documented only if non-obvious, or if the
return is not a StrView of remaining unprocessed input.

WS, where it appears, is an abbreviation for Whitespace.

Alphanumeric refers to letters and numbers.

```cpp
bool IsEmpty(StrView const& s);
StrView GetToken(StrView s, char delim, StrView& result);
StrView GetTokenWSDelimited(StrView s, char delim, StrView& result);
StrView GetTokenAlphaNumeric(StrView s, StrView& result);
StrView GetTokenAlphaNumericExt(StrView s, char const* additional_characters, StrView& result);
StrView GetString(StrView s, bool recognizeEscapes, StrView& result);
StrView GetInt16(StrView s, int16_t& result);
StrView GetInt32(StrView s, int32_t& result);
StrView GetUInt32(StrView s, uint32_t& result);
StrView GetHex(StrView s, uint32_t& result);
StrView GetFloat(StrView s, float& result);
StrView ScanForCharacter(StrView s, char delim);
StrView ScanBackwardsForCharacter(StrView s, char delim);
StrView ScanForWhiteSpace(StrView s);
StrView ScanBackwardsForWhiteSpace(StrView s);
StrView ScanForNonWhiteSpace(StrView s);
StrView ScanForTrailingNonWhiteSpace(StrView s);
StrView ScanForEndOfLine(StrView s);
StrView ScanForEndOfLine(StrView s, StrView& skipped);
StrView ScanForLastCharacterOnLine(StrView s);
StrView ScanForBeginningOfNextLine(StrView s);
StrView ScanPastCPPComments(StrView s);
StrView SkipCommentsAndWhitespace(StrView s);
StrView Expect(StrView s, StrView expect); // if expect not found return equals s
StrView Strip(StrView s); // strips leading and trailing whitespace
std::vector<StrView> Split(StrView s, char split);
```
