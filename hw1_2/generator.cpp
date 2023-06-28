#include <iostream>
#include <cstdlib>
#include <ctime>

char op(int a) {
    if (a%3 == 0) {
        return '+';
    }
    if (a%3 == 1) {
        return '-';
    }
    if (a%3 == 2) {
        return '?';
    }

    return '+';
}

void generator(int size) {
    srand(time(NULL));

    std::string text;

    for ( int i = 0, j = 0; i < size; i++ ) {
        j = rand() % 3 + 1;

        if ( j > size - i ) j = size - i;
        i += j;

        for ( int k = 0; k < j; k++ ) {
            text += (char)(rand() % 26 + 97);
        }
        text += ' ';
        text += op(rand());
    }
    text[text.size()-1] = '.';

    std::cout << text << std::endl;
}

int main() {
    int textLength;

    std::cin >> textLength;

    generator(textLength);

    return 0;
}