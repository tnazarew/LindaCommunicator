//
// Created by paulina on 08.06.16.
//

#ifndef LINDACOMMUNICATOR_PATTERNCOMPARATOR_H
#define LINDACOMMUNICATOR_PATTERNCOMPARATOR_H
#include <string>
#include <sstream>

namespace linda {
    class PatternComparator {
        enum Type {
            Int, String, Float
        };
        enum Operator {
            Equal, Less, LessEqual, Greater, GreaterEqual, Any
        };
        static Type getType(std::string val);

        static bool compareStrings(std::string string1, std::string string2, Operator op);
        static bool compareFloats(float float1, float float2, Operator op);
        static bool compareInts(int int1, int int2, Operator op);
        static bool compare(std::string string1, std::string string2, Operator op, Type type);
        static Operator getOperator(std::string& pattern, int& index);
        static std::string & trim(std::string &s);
        static std::string & rtrim(std::string &s);
        static std::string & ltrim(std::string &s);
        static void ignoreWhitespaces(std::string& pattern, int& index);
        static bool notRightType(std::string& pattern, int & index, Type type);
    public:
        static bool matches(std::string tuple, std::string pattern);
    };
}


#endif //LINDACOMMUNICATOR_PATTERNCOMPARATOR_H
