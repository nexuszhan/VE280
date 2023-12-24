#include <iostream>
#include <cmath>
#include <string>
#include <cstdlib>

void print_prompt()
{
    std::cout << "Please enter the integer and the test number: ";
}

int testTetrahedral(int num)
{
    // Use binary search to find whether n exists.
    int left=1;
    int right=pow(6*num,1.0/3);
    int mid = (left+right)/2;
    while (1)
    {
        int comp = mid*(mid+1)*(mid+2)/6;
        if (left==right)
        {
            if (comp==num)
                return 1;
            else
                return 0;
        }

        if (comp > num)
            right = (mid==right ? mid-1:mid);
        else if (comp < num)
            left = (mid==left ? mid+1:mid);
        else
            return 1;

        mid = (left+right)/2;
    }
    return 0;
}

int testPalindrome(int num)
{
    //Convert num to string first and use a loop to see whether it is palindrome.
    std::string str=std::to_string(num);
    unsigned len = str.length();
    for (unsigned i=0; i<(len/2); i++)
    {
        if (str[i]!=str[len-1-i])
            return 0;
    }
    return 1;
}

int testSelfDividing(int num)
{
    //Convert num to string first and use a loop to see whether it can be divided by each digit.
    std::string str=std::to_string(num);
    for (unsigned i=0; i<str.length(); i++)
    {
        int digit = str[i] - 48;
        if (digit==0 || num%digit!=0)
            return 0;
    }
    return 1;
}

int testBoring(int num)
{
    //Divide num by one of {2,3,5} until it becomes 1 or such division fails.
    //Number 1 should be considered separately.
    if (num==1)
        return 0;
    int factors[3]={2,3,5};
    while (1)
    {
        if (num==1)
            return 1;
        int temp=0;
        for (int i=0;i<3;i++)
        {
            if (num%factors[i]==0)
            {
                num/=factors[i];
                temp=1;
            }
        }

        if (temp==0)
            return 0;
    }
    return 0;
}

int main()
{
    //TODO: write your code here
    int num,test;

    print_prompt();
    std::cin >> num >> test;
    while (num<1 || num>10000000 || test<1 || test>4)
    {
        print_prompt();
        std::cin >> num >> test;
    }

    int result = 0;
    if (test==1)
        result = testTetrahedral(num);
    else if (test==2)
        result = testPalindrome(num);
    else if (test==3)
        result = testSelfDividing(num);
    else
        result = testBoring(num);

    std::cout << result << std::endl;

    return 0;
}
