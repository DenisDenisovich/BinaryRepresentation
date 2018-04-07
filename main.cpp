#include <iostream>
#include <vector>
#include <iterator>
#include <iomanip>
#include <windows.h>

using namespace std;

union lndbl{                                        //struct for storing long double
    unsigned char arrayByte[sizeof(long double)];
    long double value;
};

void setDoubleConsoleColor(int number, int selectedBit){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // variable for set color to console
    int backbround;
    int foreground;

    if(selectedBit != number){
        backbround = 0;
    }else{
        backbround = 6;
    }

    if(number >= 80){
        foreground = 7;
    }else if(number == 79){
        foreground = 9;
    }else if(number >= 63 && number <= 78){
        foreground = 2;
    }else if(number <= 62){
        foreground = 4;
    }
    SetConsoleTextAttribute(hConsole, foreground + (backbround*16));
}

void PrintChar(unsigned char &a){ //function for display a binary representation of symbol
    unsigned char b;
    int array[8];
    b = a;
    for(int i = 0; i < 8; i++){
        b = b&1;             // take last bit
        array[7-i] = (int)b; // write it to an array
        b = a;               // update buffer value
        b = b >> (i+1);      // readed bit is deleted with shift
    }
    cout << "binary number: ";
    for(int i = 0; i < 8; i++)
        cout << array[i];
    cout << endl << endl;
}

void PrintDouble(unsigned char arrayByte[sizeof(long double)], int selectedBit){  // function for display a binary representation of number

    unsigned char arrayPrint[8];                       // array for storing displaying bits
    unsigned char byte, buffer;                        // additional variables
    vector<unsigned char> arrayExponent;               // array for storing binary representation of exponent
    vector<unsigned char> arrayMantis;                 // array for storing binary representation of mantissa
    unsigned char sign;                                // signed byte
    unsigned char counterBit = sizeof(long double)*8;  // counter of bits
    for(int i = sizeof(long double) -1; i >= 0; i--){  // take byte from end
        byte = arrayByte[i];
        buffer = byte;
        for(int j = 0; j < 8; j++){   // put bits of selected byte to array
            buffer = buffer&1;
            arrayPrint[j] = (int)buffer;
            buffer = byte >> (j+1);
        }
        for(int j = 7; j >= 0; j--){    // display copy byte and save sign, exponent and mantissa
            counterBit--;
            if(counterBit >= 80){
                setDoubleConsoleColor(counterBit, selectedBit);
                cout << (int)arrayPrint[j];
            }else if(counterBit == 79){                            // save signed bit
                sign = (int)arrayPrint[j];
                setDoubleConsoleColor(counterBit, selectedBit);
                cout << (int)arrayPrint[j];
            }else if(counterBit >= 63 && counterBit <= 78){  // save bits of exponent
                arrayExponent.insert(arrayExponent.end(),(int)arrayPrint[j]);
                setDoubleConsoleColor(counterBit, selectedBit);
                cout << (int)arrayPrint[j];
            }else if(counterBit <= 62){                      // save bits of mantisa
                arrayMantis.insert(arrayMantis.end(),(int)arrayPrint[j]);
                setDoubleConsoleColor(counterBit, selectedBit);
                cout << (int)arrayPrint[j];
           }
        }
        if(i == 10){
            cout << " ";
        }
    }
    setDoubleConsoleColor(79, -1);
    cout << endl;                       // display sign, exponent and mantissa
    cout << "Sign     - " << (int)sign << endl;
    setDoubleConsoleColor(78, -1);
    cout << "Exponent - ";
    copy(arrayExponent.begin(), arrayExponent.end(), ostream_iterator<int>(cout,""));
    setDoubleConsoleColor(62, -1);
    cout << endl << "Mantis   - ";
    copy(arrayMantis.begin(), arrayMantis.end(), ostream_iterator<int>(cout,""));
    cout << endl;
    setDoubleConsoleColor(80, -1);
}
unsigned char revers(unsigned char &a){ // function for save bits on reverse sequence
    unsigned char rev;   // variable for save resulting sequence
    unsigned char b = a; // buffer variable
    rev = rev << 8;
    for(int i = 0; i < 8; i++){
        rev = rev << 1;
        b = b&1;
        rev = rev|b;
        b = a;
        b = b >> (i+1);
    }
    return rev;
}

unsigned char ChangeByte(unsigned char &a, int numberOfBit, unsigned int valueOfBit){
    unsigned char b, newByte;
    b = a;
    newByte = 'j';
    newByte = newByte << 8;            // set sequence of zero
    for(int i = 0; i < 8; i++){
        newByte = newByte << 1;
        if( i != numberOfBit)
            b = b&1;             // take last bit
        else
            b = valueOfBit;           // set value of selected bit

        newByte = newByte | b;         // set read bit to new version of byte
        b = a;                   // update buffer variable
        b = b >> (i+1);          // delete read bits with shift
    }
    newByte = revers(newByte);
    return newByte;
}

int main()
{
    // binary long double
    lndbl number;
    unsigned char a;
    int numberOfBit;
    int valueOfBit;


    // binary char
    cout << "              UNSIGNED CHAR" << endl;
    cout << "inter symbol:";
    cin >> a;
    cout << "/////////////////////" << endl;
    cout << "// ASCII: " << a << "  - " << setw(3) << (int)a << " //" << endl;
    cout << "/////////////////////" << endl;
    PrintChar(a);

    while(numberOfBit != -1){
        cout << "Inter a number of changed bit( for EXIT (-1) ): ";
        cin >> numberOfBit;
        if(numberOfBit == -1)
            break;
        cout << "Inter a new value of changed bit(1 or 0): ";
        cin >> valueOfBit;
        if( valueOfBit != 0 && valueOfBit != 1){
            cout << endl << "!!!incorrect value of changed bit!!!" << endl << endl;
            continue;
        }
        a = ChangeByte(a,numberOfBit-1,valueOfBit);
        cout << "/////////////////////" << endl;
        cout << "// ASCII: " << a << "  - " << setw(3) << (int)a << " //" << endl;
        cout << "/////////////////////" << endl;
        PrintChar(a);
    }
    numberOfBit = 0;


    cout << endl << "              LONG DOUBLE" << endl;
    cout << "Inter value:  ";
    cin >> number.value;
    cout << "19 precision: " << setiosflags(ios::left) << fixed << setprecision(19) << number.value << endl;
    cout << endl;
    PrintDouble(number.arrayByte, -1);
    cout << endl;
    while(numberOfBit != -1){
        cout << "Inter a number of changed bit( for EXIT (-1) ): ";
        cin >> numberOfBit;
        if(numberOfBit == -1)
            break;
        cout << "Inter a new value of changed bit(1 or 0): ";
        cin >> valueOfBit;
        if( valueOfBit != 0 && valueOfBit != 1){
            cout << endl << "!!!incorrect value of changed bit!!!" << endl << endl;
            continue;
        }
        number.arrayByte[(numberOfBit-1)/8] = ChangeByte(number.arrayByte[(numberOfBit-1)/8],(numberOfBit-1)%8,valueOfBit);
        cout << "New value :   " << setiosflags(ios::left) << fixed << setprecision(19) << number.value << endl;
        PrintDouble(number.arrayByte, numberOfBit - 1);
        cout << endl;
    }


    return 0;
}

