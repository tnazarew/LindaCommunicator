//
// Created by paulina on 08.06.16.
//

#include <algorithm>
#include "PatternComparator.h"

bool linda::PatternComparator::matches(std::string tuple, std::string pattern) {
    int tuple_index = 0, pattern_index = 0;
    int tuple_size = tuple.size();
    int pattern_size = pattern.size();

    while(tuple_index < tuple_size && pattern_index < pattern_size) {
        std::string tuple_val = "";

        ignoreWhitespaces(tuple, tuple_index);
        ignoreWhitespaces(pattern, pattern_index);

        while(tuple[tuple_index] != ',' && tuple_index < tuple_size)
            tuple_val += tuple[tuple_index++];

        tuple_val = trim(tuple_val);
        Type type = getType(tuple_val);

        if(notRightType(pattern, pattern_index, type))
            return false;

        ignoreWhitespaces(pattern, pattern_index);

        Operator op = getOperator(pattern, pattern_index);

        std::string pattern_value = "";
        while(pattern[pattern_index] != ',' && pattern_index < pattern_size)
            pattern_value += pattern[pattern_index++];
        pattern_value = trim(pattern_value);

        if(op == Any) {
            ++pattern_index;
            ++tuple_index;
            continue;
        }
        if(!compare(tuple_val, pattern_value, op, type))
            return false;
        ++pattern_index;
        ++tuple_index;
    }
    return true;
}

linda::PatternComparator::Type linda::PatternComparator::getType(std::string val) {
    if(val[0] == '"' && val[val.size()-1] == '"')
        return String;
    for(int i = 0; i < val.size(); ++i) {
        if(val[i] == '.')
            return Float;
    }
    return Int;
}

linda::PatternComparator::Operator linda::PatternComparator::getOperator(std::string& pattern, int& index) {
    switch(pattern[index]) {
        case '*': ++index; return Any;
        case '<': ++index;
            if(pattern[index] == '=') {
                ++index;
                return LessEqual;
            }
            return Less;
        case '>': ++index;
            if(pattern[index] == '=') {
                ++index;
                return GreaterEqual;
            }
            return Greater;
    }
    return Equal;
}

bool linda::PatternComparator::compare(std::string string1, std::string string2, Operator op, Type type) {
    std::istringstream iss1(string1);
    std::istringstream iss2(string2);
    switch(type) {
        case String:
            return compareStrings(string1, string2, op);
        case Int:
            int i1, i2;
            iss1 >> i1;
            iss2 >> i2;
            return compareInts(i1, i2, op);
        case Float:
            float f1, f2;
            iss1 >> f1;
            iss2 >> f2;
            return compareFloats(f1, f2, op);
    }
    return false;
}

bool linda::PatternComparator::compareStrings(std::string string1, std::string string2, Operator op) {
    switch(op) {
        case Equal: return string1 == string2;
        case Less: return string1 < string2;
        case LessEqual: return string1 <= string2;
        case Greater: return string1 > string2;
        case GreaterEqual: return string1 >= string2;
    }
    return false;
}

bool linda::PatternComparator::compareFloats(float float1, float float2, Operator op) {
    switch(op) {
        case Equal: return float1 == float2;
        case Less: return float1 < float2;
        case LessEqual: return float1 <= float2;
        case Greater: return float1 > float2;
        case GreaterEqual: return float1 >= float2;
    }
    return false;
}

bool linda::PatternComparator::compareInts(int int1, int int2, Operator op) {
    switch(op) {
        case Equal: return int1 == int2;
        case Less: return int1 < int2;
        case LessEqual: return int1 <= int2;
        case Greater: return int1 > int2;
        case GreaterEqual: return int1 >= int2;
    }
    return false;
}

std::string &linda::PatternComparator::trim(std::string &s) {
    return ltrim(rtrim(s));
}

std::string &linda::PatternComparator::rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

std::string &linda::PatternComparator::ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

void linda::PatternComparator::ignoreWhitespaces(std::string &pattern, int &index) {
    while(isspace(pattern[index]))
        ++index;
}

bool linda::PatternComparator::notRightType(std::string &pattern, int &pattern_index, Type type) {
    char c;
    switch(type) {
        case String : c = 's'; break;
        case Int : c = 'i'; break;
        case Float : c = 'f'; break;
        default:
            return true;
    }
    ignoreWhitespaces(pattern, pattern_index);
    if (pattern[pattern_index++] == c) {
        ignoreWhitespaces(pattern,pattern_index);
        if (pattern[pattern_index++] == ':')
            return false;
    }
    return true;
}


