////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  main.cpp
//  RLEencoding
//
//  Created by Mikhail Scherbakov on 17/07/2017.
//  Copyright © 2017 sh. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include <cassert>

static const size_t MAX_BUFFER_SIZE = 1000;

////////////////////////////////////////////////////////////////////////////////////////////////////
int WriteCount( char *pBuffer, const int anchorPos, int writePos, const int count )
{
    assert( count >= 1 );
    assert( writePos <= anchorPos );
    
    // Write symbol itself
    pBuffer[writePos] = pBuffer[anchorPos];
    ++writePos;
    
    if( count == 1 )
        return 1;
        
    // First let's define the number of digits in count
    int testCount = count;
    int digitCount = 1;
    int divider = 1;
    while( testCount > 9 )
    {
        divider *= 10;
        testCount /= 10;
        ++digitCount;
    }
    
    // Write count digit by digit
    int retValue = 1;
    testCount = count;
    while( divider > 0 )
    {
        const int digit = count / divider;
        assert( digit >= 0 && digit < 10 );
        const char digitSymbol = 48 + digit;    // We know that "0" == 48, "1" == 49, "2" == 50, etc
        pBuffer[writePos] = digitSymbol;
        ++writePos;
        divider /= 10;
        ++retValue;
    }
    
    return retValue;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void ProvideInplaceEncoding( char *pBuffer )
{
    const size_t symbolCount = strlen( pBuffer );
    
    int anchorPos = 0;  // Position of begining of the sequence
    int testPos = 1;    // Current test position
    int writePos = 0;   // Current position to write count
    int countInRow = 1; // Counts of repeated symbols in series
    
    bool bHasSomethingToWrite = false;
    while( anchorPos < symbolCount && testPos < symbolCount )    
    {
        const char anchorSymbol = pBuffer[anchorPos];
        const char testSymbol = pBuffer[testPos];
        
        if( anchorSymbol != testSymbol )
        {
            const int displace = WriteCount( pBuffer, anchorPos, writePos, countInRow );
            writePos += displace;
            anchorPos = testPos;
            countInRow = 1;
            ++testPos;
            bHasSomethingToWrite = false;
        }
        else
        {
            ++countInRow;
            ++testPos;
            bHasSomethingToWrite = true;
        }
    }
    
    // Last writing
    if( bHasSomethingToWrite )
    {
        const int displace = WriteCount( pBuffer, anchorPos, writePos, countInRow );
        writePos += displace;
    }
    
    // And put zeros at the end of string
    pBuffer[writePos] = 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
int main( int argc, const char * argv[] )
{
    
    char buffer[MAX_BUFFER_SIZE];
    memset( buffer, 0, MAX_BUFFER_SIZE ); 
    
    std::string inputString;
    
    std::cout << "Please enter string to encode..." << std::endl;
    fgets( buffer, MAX_BUFFER_SIZE - 1, stdin );
    
    std::cout << "Your string is: " << buffer;
    ProvideInplaceEncoding( buffer );
    std::cout << "Your encoded is: " << buffer;
    
    return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
