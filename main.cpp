#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

std::vector<int> karatsuba_multi(std::vector<int> A, std::vector<int> B);
void convertToVector(unsigned long long int num, std::vector<int>& v);

std::vector<int> convertToVector2(int num);
std::vector<int> stringToVector(std::string num);

void addLeadingZeroes(std::vector<int>& A, std::vector<int>& B);
std::vector <int> removingLeadingZeros(std::vector<int> v);
void addZero(int length, std::vector<int>& v);
void shiftPower10(int n, std::vector<int>& v);

int printVector(std::vector<int> v);

std::vector<int> addVectors(std::vector<int> A, std::vector<int> B);
std::vector<int> subtractVectors(std::vector<int> A, std::vector<int> B);

std::vector<int> power(int a, int b);
int vectorToInt(std::vector<int> v);

int main() {

    //std::cout << "Input two positive integers whose values are less than or equal to 1000, press enter after each one\n";
    int a, b;
    //std::cin >> a >> b;

    for (int i = 1; i < 2; i++) { //to test 1*1 1*2 ... 999*999 1000*1000 worked for every case

        //std::vector<int> mainAnswer = karatsuba_multi(convertToVector2(1000), convertToVector2(1000));

        for (int j = 1; j < 2; j++) {

            /* Create a vector of size n zeroes.
            unsigned n = 1500;
            std::vector<int> A(n, 0);
            std::vector<int> B(n, 0);

            A.push_back(1);
            B.push_back(1);
             */

            //std::vector<int> A{3,7,7,2,3,5,4,9,4,6,7,8,3,1,3,8,6,1,6,5,7,3,0,5,9,3,5,2,5,7};
            //std::vector<int> B{6,0,9,7,7,0,9,0,8,7,6,2,9,0,8,5,4,3,0,9,5,7,3,3,0,8,0,5,9,7};

            //std::vector<int> A = stringToVector(a);
            //std::vector<int> B = stringToVector(b);

            //std::vector<int> A;
            //std::vector<int> B;

            //convertToVector(a, A); //ex 453 -> {3,5,4}
            //convertToVector(b, B); //switched i = a, j = b

            //printVector(addVectors(stringToVector("109"), stringToVector("99"))); //works now
            //subtractVectors(A, B);

            //unsigned long long int check = a * b;

            /*
             max = 18446744073709551615

             874983279
             47239857234

             wolfram =         41334085182097190286
             Using karatsuba = 41334085182097190286
             */

            //std::cout << i << " x " << j << std::endl;

            std::vector<int> A = convertToVector2(i);
            a = i;
            std::vector<int> B = convertToVector2(j);
            b = j;

            std::vector<int> answer = karatsuba_multi(A, B);

            //std::cout << "Using karatsuba = ";
            //printVector(answer);
            //std::cout << number << std::endl;
            int check = vectorToInt(answer);

            if (check != (a * b)) {
                std::cout << a << " x " << b << std::endl;
                //std::cout << "Does not match\n";
                std::cout << "Using karatsuba = ";
                printVector(answer);
                std::cout << "Using regular = " << i * j << std::endl << std::endl; //switched i = a, j = b
            }

            //std::cout << "Using regular = " << i * j << std::endl << std::endl; //switched i = a, j = b
            //int count = printVector(answer);
            //std::cout << count << " zeros" << std::endl; //shows the number of zeros ex: 100000000000000 14 zeros
        }
    }

    std::vector<int> answer = power(78, 78);
    printVector(answer);
    //removingLeadingZeros(answer);
    std::cout << "Finished\n";

    return 0;
}

//a and b positive integers less than OR equal to 1000
std::vector<int> karatsuba_multi(std::vector<int> A, std::vector<int> B) {

    int numDigitsA = A.size();
    int numDigitsB = B.size();

    if ((numDigitsA == 1) && (numDigitsB == 1)) { //if both A and B are single digits
        std::vector<int> ans;
        int m = A[0] * B[0]; //multiplying single digits
        ans = convertToVector2(m);
        return ans;
    }
    else {
        //fix length, if 2 digits its fine, if 3 make 4, if 4 its fine
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

        std::vector<int> c2 = karatsuba_multi(subArray_A1, subArray_B1); //1st multiplication
        std::vector<int> c0 = karatsuba_multi(subArray_A0, subArray_B0); //2nd multiplication
        std::vector<int> p1 = karatsuba_multi(addVectors(subArray_A1, subArray_A0), addVectors(subArray_B1, subArray_B0)); //3rd multiplication
        std::vector<int> c1 = subtractVectors(p1, addVectors(c2, c0));

        if ((size % 2) != 0) {
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

void convertToVector(unsigned long long int num, std::vector<int>& v) {

    std::string number = std::to_string(num);
    for (int i = number.length() - 1; i >= 0; i--) {
        std::string n (1, number[i]);
        v.push_back(std::stoi(n));
    }
}

std::vector<int> convertToVector2(int num) {
    std::vector <int> v;

    std::string number = std::to_string(num);
    for (int i = number.length() - 1; i >= 0; i--) {
        std::string n (1, number[i]);
        v.push_back(std::stoi(n));
    }

    return v;
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

        else {
            addZero(maxSize, A);
            addZero(maxSize, B);
        }
    }
}

void addZero(int length, std::vector<int>& v) {
    while (v.size() < length)
        v.push_back(0);
}

std::vector<int> addVectors(std::vector<int> A, std::vector<int> B) {

    std::vector <int> answer;
    int carryFlag = 0;
    addLeadingZeroes(A, B);

    int i = 0;

    while (i < A.size() + 1) {
        //adding single digits ex 9+9 = 18
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

                //answer.push_back((num + 1) % 10); //FIX THIS CHECK THIS not single digit
                numVec = convertToVector2(numVec[0] + 1); //9 + 1 = 10 -> {0,1} so 10 mod 10 to get first digit is numVec[0]
                answer.push_back(numVec[0]);

                if(num == 9)
                    carryFlag = 1;
                else
                    carryFlag = 0;
            }

            else {
                //answer.push_back(num % 10); //FIX THIS CHECK THIS not single digit, TURN TO STRING AND ADD OTHER HALF
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

void shiftPower10(int n, std::vector<int>& v) {

    int i = 0;
    while (i < n) {
        //add zero to front example -> 3 * 10^2 -> 300 -> represented as 003
        v.insert(v.begin(),0);
        i++;
    }
}

int printVector(std::vector<int> v) {

    std::string answer = "";
    int zeroFlag = 0;
    int zeroCount = 0;

    for (std::vector<int>::reverse_iterator it = v.rbegin(); it != v.rend(); ++it) {
        if ((*it == 0) && (zeroFlag == 0)) {}
            //do nothing

        else {
            std::cout << *it;
            if(*it == 0)
                zeroCount++;
            //answer+=std::to_string(*it);
            zeroFlag = 1;
        }
    }
    std::cout << std::endl;
    //return std::stoi(answer);
    return zeroCount;
}

std::vector<int> power(int a, int b) {

    if (b == 0)
        return convertToVector2(1);

    std::vector<int> answer = power(a, b/2);

    if ((b % 2) == 0) { //if b is even
        return karatsuba_multi(answer, answer); //(a^n/2)^2
    }

    else { //if b is odd
        return karatsuba_multi(convertToVector2(a), karatsuba_multi(answer, answer)); //(a^(n-1)/2)^2 * a
    }
}

std::vector <int> removingLeadingZeros(std::vector<int> v) {

    std::vector <int> no;
    int zeroFlag = 0;

    for (std::vector<int>::reverse_iterator it = v.rbegin(); it != v.rend(); ++it) {
        if ((*it == 0) && (zeroFlag == 0)) {}
            //do nothing
        else {
            zeroFlag = 1;
            no.push_back(*it);
        }
    }

    std::reverse(no.begin(),no.end());
    return no;
}

int vectorToInt(std::vector<int> v) {

    std::string number = "";
    removingLeadingZeros(v);

    for (std::vector<int>::reverse_iterator it = v.rbegin(); it != v.rend(); ++it) {
        number += std::to_string(*it);
    }

    return std::stoi(number);
}
