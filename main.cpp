// Assignment: Project 02 - Multiplication of Large Integers
// File: main.cpp
// Author: Jevan Smith and Juan Martinez
// Date: March, Spring 2019
// Course: CS415 - Dr. Gill
// Description : This program will compute the multiplication and/or
// the exponentiation of two positive integers less than or equal to 1000
// using Karatsuba's algorithm and the decrease-by-constant-factor algorithm for computing exponentiation.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

std::vector<int> karatsuba_mult(std::vector<int> A, std::vector<int> B);
std::vector<int> exp(int a, int b);
std::vector<int> convertToVector2(int num);
std::vector<int> addVectors(std::vector<int> A, std::vector<int> B);
std::vector<int> subtractVectors(std::vector<int> A, std::vector<int> B);
std::vector<int> removingLeadingZeros(std::vector<int> v);

void addLeadingZeroes(std::vector<int>& A, std::vector<int>& B);
void addZero(int length, std::vector<int>& v);
void shiftPower10(int n, std::vector<int>& v);
void printVector(std::vector<int> v);

//FOR TESTING PURPOSES//

//void convertToVector(unsigned long long int num, std::vector<int>& v);
//std::vector<int> stringToVector(std::string num);
//int vectorToInt(std::vector<int> v);

//FOR TESTING PURPOSES//

int main() {

    std::cout << "Input two positive integers whose values are less than or equal to 1000, press enter after each one.\n";
    int a, b;
    std::cout << "A: ";
    std::cin >> a;
    std::cout << "B: ";
    std::cin >> b;

    while ((a < 1 || a > 1000) || (b < 1 || b > 1000)) {
        std::cout << "\nERROR\nInput two positive integers whose values are less than or equal to 1000, press enter after each one\n";
        std::cout << "A: ";
        std::cin >> a;
        std::cout << "B: ";
        std::cin >> b;
    }

    char option = '\0';
    while (option != 'q') {
        std::cout << "\n--------------\nSELECT A TASK:\n--------------\nTask 1: Multiplication\nTask 2: Exponentiation\nQuit(q)\n\n";
        std::cout << "Enter 1,2, or q: ";
        std::cin >> option;

        if (option == '1') {
            std::cout << "\n-----------------------------------\n";
            std::cout << "| Selected Task 1: Multiplication |\n";
            std::cout << "-----------------------------------\n\n";
            int expectedAns = a * b;
            std::cout << "The expected output for multiplication of "<< a << " * " << b << " = " << expectedAns << std::endl;
            std::cout << "Output using Karatsuba algorithm = ";
            printVector(karatsuba_mult(convertToVector2(a), convertToVector2(b)));
            std::cout << "\n";
            std::cout << "Press Enter to Continue... ";
            std::cin.ignore();
            std::cin.ignore();
        }

        else if (option == '2') {
            std::cout << "\n-----------------------------------\n";
            std::cout << "| Selected Task 2: Exponentiation |\n";
            std::cout << "-----------------------------------\n\n";
            std::cout << "Output produced for " << a << " ^ " << b << " is =\n";
            printVector(exp(a, b));
            std::cout << "\n";
            std::cout << "Press Enter to Continue... ";
            std::cin.ignore();
            std::cin.ignore();
        }
    }
    std::cout << "Exiting...\n";
    return 0;
}

/**
 * Performs Karatsuba's algorithm to multiply two positive integers A and B
 * whose values are less than or equal to 1000, whose digits are stored in vectors
 * of type int, in reversed order.
 *
 * @param A - a vector of type int with the digits of the integer value. The vector
 * stores the digits in reversed order. For example, if the user uses 289 as
 * the value, the vector stores the digits as [9,8,2], assuming no leading zeros.
 *
 * @param B - a vector of type int with the digits of the integer value. Same as A.
 *
 * @return A vector of type int with the digits of the value of the multiplication
 * of vectors A and B (A * B), in reversed order.
 */
std::vector<int> karatsuba_mult(std::vector<int> A, std::vector<int> B) {

    int numDigitsA = A.size();
    int numDigitsB = B.size();

    if ((numDigitsA == 1) && (numDigitsB == 1)) { //if both A and B are single digits
        std::vector<int> ans;
        int m = A[0] * B[0]; //multiplying single digits
        ans = convertToVector2(m);
        return ans;
    }
    else {

        //fix length
        addLeadingZeroes(A, B);
        //now they have the same size
        //A.size() == B.size()

        int size = A.size();
        int half = size / 2;


        std::vector<int> subArray_A0;
        std::vector<int> subArray_B0;
        std::vector<int> subArray_A1;
        std::vector<int> subArray_B1;

        int i = 0;
        int j = half;

        // split vectors in half
        while (i < half) {
            subArray_A0.push_back(A[i]);
            subArray_B0.push_back(B[i]);
            i++;
        }

        while (j < size) {
            subArray_A1.push_back(A[j]);
            subArray_B1.push_back(B[j]);
            j++;
        }

        std::vector<int> c2 = karatsuba_mult(subArray_A1, subArray_B1); //1st multiplication
        std::vector<int> c0 = karatsuba_mult(subArray_A0, subArray_B0); //2nd multiplication
        std::vector<int> p1 = karatsuba_mult(addVectors(subArray_A1, subArray_A0), addVectors(subArray_B1, subArray_B0)); //3rd multiplication
        std::vector<int> c1 = subtractVectors(p1, addVectors(c2, c0));

        if ((size % 2) != 0) { //if odd
            shiftPower10(size - 1, c2);
        }
        else {
            shiftPower10(size, c2);
        }
        shiftPower10(half, c1);

        std::vector <int> c2PLUSc1 = removingLeadingZeros(addVectors(c2,c1));
        std::vector <int> answer = removingLeadingZeros(addVectors(c2PLUSc1, c0));

        return answer;
    }
}

/**
 * Performs the decrease-by-constant-factor algorithm for computing exponentiation
 * of two positive integers less than or equal to 1000.
 *
 * @param a - a positive integer less than or equal to 1000.
 *
 * @param b - a positive integer less than or equal to 1000.
 *
 * @return A vector of type int with the digits of the value of the exponentiation
 * of integers a and b (a^b), in reversed order.
 */
std::vector<int> exp(int a, int b) {

    if (b == 0)
        return convertToVector2(1);

    std::vector<int> answer = exp(a, b/2);

    if ((b % 2) == 0) { //if b is even
        return karatsuba_mult(answer, answer); //(a^n/2)^2
    }

    else { //if b is odd
        return karatsuba_mult(convertToVector2(a), karatsuba_mult(answer, answer)); //(a^(n-1)/2)^2 * a
    }
}

/**
 * Converts an integer to a vector, storing the digits of the integer
 * in reversed order.
 *
 * @param num - an integer.
 *
 * @return A vector of type int with the digits of num in reversed order.
 */
std::vector<int> convertToVector2(int num) {
    std::vector <int> v;
    std::string number = std::to_string(num);
    for (int i = number.length() - 1; i >= 0; i--) { //go backward
        std::string n (1, number[i]);
        v.push_back(std::stoi(n));
    }
    return v;
}

/**
 * Adds two vectors of type int by individually adding each digit in the vectors
 * Example: 999 + 99 = 1098
 *
 *  [9,9,9,0] it will add leading zeroes if necessary
 * +[9,9,0,0] keeps track of carrying a one and adds it at the end if necessary
 * ----------
 *  [8,9,0,1]
 *
 * @param A - a vector of type int with the digits of the integer value. The vector
 * stores the digits in reversed order. For example, if the user uses 289 as
 * the value, the vector stores the digits as [9,8,2], assuming no leading zeros.
 *
 * @param B - a vector of type int with the digits of the integer value. Same as A.
 *
 * @return A vector of type int with the digits of the sum of A + B in reversed order.
 */
std::vector<int> addVectors(std::vector<int> A, std::vector<int> B) {

    std::vector <int> answer;
    int carryFlag = 0;
    addLeadingZeroes(A, B);
    int i = 0;

    while (i < A.size() + 1) {

        if (i == A.size()) {
            if(carryFlag == 1)
                answer.push_back(1); //if 1 carry left over, add it ex 9+9=(1)8
            else
                return answer;
        }
        else {

            int num = (A[i] + B[i]); //ex 9 + 9 = 18
            std::vector<int> numVec = convertToVector2(num); // as vec = {8,1}

            if (carryFlag == 1) {
                numVec = convertToVector2(numVec[0] + 1); //9 + 1 = 10 -> {0,1} so 10 mod 10 to get first digit is numVec[0]
                answer.push_back(numVec[0]);

                if(num == 9)
                    carryFlag = 1;
                else
                    carryFlag = 0;
            }

            else {

                answer.push_back(numVec[0]);
            }

            int carry = num + 1;
            if (carry == 10) {
                //do nothing
            }
            else if ((carry / 10) == 1) {
                carryFlag = 1;
            }
        }
        i++;
    }
    return answer;
}

/**
 * Subtracts two vectors of type int by individually subtracting each digit in the vectors
 * Example: 955 - 99 = 856
 *
 *  [5,5,9,0] it will add leading zeroes if necessary
 * +[9,9,0,0] keeps track of borrowing
 * ----------
 *  [6,5,8,0]
 *
 * @param A - a vector of type int with the digits of the integer value. The vector
 * stores the digits in reversed order. For example, if the user uses 289 as
 * the value, the vector stores the digits as [9,8,2], assuming no leading zeros.
 *
 * @param B - a vector of type int with the digits of the integer value. Same as A.
 *
 * @return A vector of type int with the digits of the difference of A - B in reversed order.
 */
std::vector<int> subtractVectors(std::vector<int> A, std::vector<int> B) {

    std::vector<int> answer;

    int carryFlag = 0;
    addLeadingZeroes(A, B);
    int i = 0;

    while (i < A.size()) {

        int num;
        std::vector <int> number;

        if (A[i] < B[i]) {

            num = 10 - (B[i] - A[i]); //single digit subtraction FIX THIS CHECK THIS not single digit

            if (carryFlag == 1) {
                num -= 1;
            }
            carryFlag = 1;
            answer.push_back(num);
        }
        else {

            if (carryFlag == 1) {

                num = A[i] - 1;
                if(num < B[i]) {
                    num = 10 - (B[i] - num); //single digit subtraction FIX THIS CHECK THIS not single digit
                    answer.push_back(num);
                    carryFlag = 1;
                }
                else if(num == B[i]) {
                    answer.push_back(0);
                    carryFlag = 0;
                }
                else {
                    answer.push_back(num - B[i]);
                    carryFlag = 0;
                }
            }
            else {
                num = (A[i] - B[i]);
                answer.push_back(num);
            }
        }
        i++;
    }
    return answer;
}

/**
 * Removes the leading zeros in a vector of type int.
 * Example: [3,2,1,0,0,0,0,0] -> [3,2,1]
 *
 * @param v - a vector of type int with the digits of the integer value. The vector
 * stores the digits in reversed order. For example, if the user uses 289 as
 * the value, the vector stores the digits as [9,8,2], may include leading zeros.
 *
 * @return A vector of type int with the digits of v, without the leading zeros.
 */
std::vector <int> removingLeadingZeros(std::vector<int> v) {

    std::vector <int> updated_v;
    int zeroFlag = 0;

    for (std::vector<int>::reverse_iterator it = v.rbegin(); it != v.rend(); ++it) {
        if ((*it == 0) && (zeroFlag == 0)) {}
            //do nothing
        else {
            zeroFlag = 1;
            updated_v.push_back(*it);
        }
    }

    std::reverse(updated_v.begin(),updated_v.end());
    return updated_v;
}


/**
 * Determines the necessary leading zeroes to make two vectors the same size/length.
 *
 * @param A - a vector of type int with the digits of the integer value. The vector
 * stores the digits in reversed order. For example, if the user uses 289 as
 * the value, the vector stores the digits as [9,8,2], assuming no leading zeros.
 *
 * @param B - a vector of type int with the digits of the integer value. Same as A.
 */
void addLeadingZeroes(std::vector<int>& A, std::vector<int>& B) {

    int sizeA = A.size();
    int sizeB = B.size();

    if ((sizeA == sizeB) && sizeA == 1) {}
        //do nothing

    else if ((sizeA == sizeB) && (sizeA % 2 == 0)) {}
        //do nothing

    else { //not same but one might be even

        int maxSize = std::max(sizeA, sizeB);

        if (maxSize == 2) {
            //since maxSize is 2, that means the min vector is 1 digit so just add one leading zero
            if (sizeA > sizeB)
                addZero(2, B); //add zero until Array is first parameter long
            else
                addZero(2, A);
        }
            //else max size is 3 or 4, either way making 4 long
        else if (maxSize == 3 || maxSize == 4) {
            addZero(4, A);
            addZero(4, B);
        }

        else { //add maxSize zeros to make same length
            addZero(maxSize, A);
            addZero(maxSize, B);
        }
    }
}

/**
 * Adds the necessary leading zeroes to a vector to achieve the length desired.
 * Example: length = 4, v = [3,2,1] -> [3,2,1,0]
 * v.size() = 3, but ends at v.size() = 4
 *
 * @param length - the length desired of the vector
 *
 * @param v - a vector of type int with the digits of the integer value. The vector
 * stores the digits in reversed order. For example, if the user uses 289 as
 * the value, the vector stores the digits as [9,8,2], assuming no leading zeros.
 */
void addZero(int length, std::vector<int>& v) {
    while (v.size() < length)
        v.push_back(0);
}

/**
 * Adds n zeros to the front to achieve v * 10^n
 * Example: 3 * 10^2 -> 300 -> represented as [3] so add 2 zeros [0,0,3]
 *
 * @param n - the number of zeros to be added
 *
 * @param v - a vector of type int with the digits of the integer value. The vector
 * stores the digits in reversed order. For example, if the user uses 289 as
 * the value, the vector stores the digits as [9,8,2], assuming no leading zeros.
 */
void shiftPower10(int n, std::vector<int>& v) {
    int i = 0;
    while (i < n) {
        v.insert(v.begin(),0);
        i++;
    }
}

/**
 * Outputs a vector, in the correcr order, without leading zeros
 * Example: [3,2,1,0] -> Outputs 123
 *
 * @param v - a vector of type int with the digits of the integer value. The vector
 * stores the digits in reversed order. For example, if the user uses 289 as
 * the value, the vector stores the digits as [9,8,2], assuming no leading zeros.
 */
void printVector(std::vector<int> v) {

    std::string answer = "";
    int zeroFlag = 0;
    int zeroCount = 0;
    int newLine = 0;

    for (std::vector<int>::reverse_iterator it = v.rbegin(); it != v.rend(); ++it) {
        if ((*it == 0) && (zeroFlag == 0)) {}
            //do nothing

        else {
            if (newLine == 45) {
                std::cout << "\n";
                newLine = 0;
            }

            std::cout << *it;
            if(*it == 0)
                zeroCount++;
            //answer+=std::to_string(*it);
            zeroFlag = 1;
            newLine++;
        }
    }
    std::cout << std::endl;
    //return std::stoi(answer);
    //return zeroCount; to count number of zeros in string, used to test 1000^1000 which has 3000 zeros
}

/*///////////////////// FOR TESTING PURPOSES //////////////////////////
void convertToVector(unsigned long long int num, std::vector<int>& v) {

    std::string number = std::to_string(num);
    for (int i = number.length() - 1; i >= 0; i--) {
        std::string n (1, number[i]);
        v.push_back(std::stoi(n));
    }
}


std::vector<int> stringToVector(std::string num) {

    std::vector <int> v;

    std::string number = num;
    for (int i = number.length() - 1; i >= 0; i--) {
        std::string n (1, number[i]);
        v.push_back(std::stoi(n));
    }

    return v;
}

int vectorToInt(std::vector<int> v) {

    std::string number = "";
    removingLeadingZeros(v);

    for (std::vector<int>::reverse_iterator it = v.rbegin(); it != v.rend(); ++it) {
        number += std::to_string(*it);
    }

    return std::stoi(number);
}
/////////////////////// FOR TESTING PURPOSES ////////////////////////*/
