#include "StringMath.h"
#include <vector>
#include <algorithm>

StringMath::StringMath(const std::string& longNumber)
{
    std::string numberNormalize = normalize(longNumber);
    m_sLongNumber = numberNormalize;
}

StringMath::StringMath(const char* longNumber) :StringMath(std::string(longNumber))
{

}

StringMath::StringMath(const StringMath& strMath)
{
    m_sLongNumber = strMath.m_sLongNumber;
}

StringMath StringMath::operator*(const StringMath& rhs)
{
    StringMath num1(this->abs());
    StringMath num2(rhs.abs());

    int len1 = num1.m_sLongNumber.size();
    int len2 = num2.m_sLongNumber.size();

    if (len1 == 0 || len2 == 0)
        return "0";

    // will keep the result number in vector 
    // in reverse order 
    std::vector<int> tmpResult(len1 + len2, 0);

    // Below two indexes are used to find positions 
    // in result.  
    int i_n1 = 0;
    int i_n2 = 0;

    // Go from right to left in num1 
    for (int i = len1 - 1; i >= 0; i--)
    {
        int carry = 0;
        int n1 = num1.m_sLongNumber[i] - '0';

        // To shift position to left after every 
        // multiplication of a digit in num2 
        i_n2 = 0;

        // Go from right to left in num2              
        for (int j = len2 - 1; j >= 0; j--)
        {
            // Take current digit of second number 
            int n2 = num2.m_sLongNumber[j] - '0';

            // Multiply with current digit of first number 
            // and add result to previously stored result 
            // at current position.  
            int sum = n1 * n2 + tmpResult[i_n1 + i_n2] + carry;

            // Carry for next iteration 
            carry = sum / 10;

            // Store result 
            tmpResult[i_n1 + i_n2] = sum % 10;

            i_n2++;
        }

        // store carry in next cell 
        if (carry > 0)
            tmpResult[i_n1 + i_n2] += carry;

        // To shift position to left after every 
        // multiplication of a digit in num1. 
        i_n1++;
    }

    // ignore '0's from the right 
    int i = tmpResult.size() - 1;
    while (i >= 0 && tmpResult[i] == 0)
        i--;

    // If all were '0's - means either both or 
    // one of num1 or num2 were '0' 
    if (i == -1)
        return "0";

    // generate the result string 
    std::string ans = "";

    while (i >= 0)
        ans += std::to_string(tmpResult[i--]);

    if (this->isNegative() && rhs.isNegative()) {}
    else if (this->isNegative() || rhs.isNegative())
        ans.insert(ans.begin(), '-');

    return ans;
}

StringMath StringMath::operator/(int divisor)
{
    if (divisor == 0)
        throw std::logic_error("Math error: divide by 0");

    std::string result;
    StringMath absLongNumber = this->abs();

    // Find prefix of number that is larger than divisor
    int idx = 0;
    int temp = absLongNumber.m_sLongNumber[idx] - '0';
    while (temp < divisor)
        temp = temp * 10 + (absLongNumber.m_sLongNumber[++idx] - '0');

    // Repeatedly divide divisor with temp. After  
    // every division, update temp to include one  
    // more digit. 
    while (absLongNumber.m_sLongNumber.size() > idx)
    {
        result += (temp / divisor) + '0';
        temp = (temp % divisor) * 10 + absLongNumber.m_sLongNumber[++idx] - '0';
    }

    // If divisor is greater than number 
    if (result.length() == 0)
        return "0";

    if (this->isNegative())
        result.insert(result.begin(), '-');

    return result;
}

std::ostream& operator<<(std::ostream& out, const StringMath& str)
{
    out << str.m_sLongNumber;
    return out;
}

StringMath StringMath::abs() const
{
    std::string result = m_sLongNumber;
    if (m_sLongNumber.find('-', 0) == 0)
        result.erase(result.begin());

    return result;
}

bool StringMath::isNegative() const
{
    if (m_sLongNumber.find('-', 0) == 0)
        return true;

    return false;
}

std::string StringMath::normalize(const std::string longNumber)
{
    std::string result = longNumber;
    if (result.size() == 0)
    {
        result = "0";
        return result;
    }

    // Erase space
    result.erase(std::remove_if(result.begin(), result.end(), std::isspace), result.end());

    // Check position "-"
    if (result.find("-", 0) > 0 && result.find("-", 0) < result.size())
        throw std::logic_error("Error format");

    // Check sLongNumber have all digit
    std::string digit = "-0123456789";
    for (uint i = 0; i < result.size(); i++)
    {
        if (digit.find(result[i], 0) == std::string::npos)
            throw std::logic_error("Error format");
    }

    return result;
}