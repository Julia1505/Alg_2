//Задача 5. Алгоритм сжатия данных Хаффмана
//(6 баллов и более)
//Напишите две функции для создания архива из одного файла и извлечения файла из архива.
//Требования: коэффициент сжатия < 1.
//В архиве сохраняйте дерево Хаффмана и код Хаффмана от исходных данных.
//Дерево Хаффмана требуется хранить эффективно - не более 10 бит на каждый 8-битный символ.
//В контест необходимо отправить .cpp файл содержащий функции Encode, Decode, а также включающий файл Huffman.h. Тестирующая программа выводит размер сжатого файла в процентах от исходного.
//
//Лучшие 3 решения с коэффициентом сжатия < 1 из каждой группы оцениваются в 10, 7 и 4 баллов соответственно.

#include <iostream>
#include <utility>
#include <vector>
#include <array>

typedef unsigned char byte;

// для ввода и вывода

struct IInputStream {
    std::vector<unsigned char> buffer;

    // Возвращает false, если поток закончился
    bool Read(byte& value) {
        if (std::cin >> value) {
            return true;
        }
        return false;
    }

    std::vector<unsigned char> ReadToBuffer() {
        unsigned char byte;
        while (Read(byte)) {
            buffer.push_back(byte);
        }
        return buffer;
    }
};

struct IOutputStream {
    void Write(byte value){
        std::cout << value;
    }
};

// куча

template <class T>
bool compDefault(const T& l, const T& r) {
    return l > r;
}

template<class T, class Compare>
class Heap {
public:
    Heap(Compare cmp);
    Heap(const Heap&) = delete;
    ~Heap();

    Heap<T,Compare>& operator=(const Heap& l);

    Heap(T *arr, int size, Compare cmp);

    T PopMin();

    const T &PeekMin() const;

    void Insert(const T &el);

    void Print();

private:
    T *buffer;
    int size;
    int bufferSize;
    Compare cmp;

    void siftDown(int i);
};

template<class T, class Compare>
Heap<T, Compare>::Heap(Compare cmp):size(0), bufferSize(0), cmp(cmp) {}

template<class T, class Compare>
Heap<T, Compare>::~Heap() {
    delete[] buffer;
}

template<class T, class Compare>
Heap<T, Compare> &Heap<T, Compare>::operator=(const Heap &l) {
    if (bufferSize != 0) {
        delete[] buffer;
    }

    bufferSize = l.bufferSize;
    size = l.size;
    for (int i = 0; i < size; ++i) {
        buffer[i] = l.buffer[i];
    }
    return *this;
}

template<class T, class Compare>
Heap<T, Compare>::Heap(T *arr, int size, Compare cmp):cmp(cmp), size(size), bufferSize(size) {
    buffer = new T[size];
    for (int i = 0; i < size; i++) {
        buffer[i] = arr[i];
    }
    for (int i = size / 2 - 1; i >= 0; --i) {
        siftDown(i);
    }
}

template<class T, class Compare>
T Heap<T, Compare>::PopMin() {
    T temp = buffer[0];
    swap(buffer[0], buffer[size - 1]);
    size--;
    siftDown(0);
    return temp;
}

template<class T, class Compare>
const T &Heap<T, Compare>::PeekMin() const {
    return buffer[0];
}

template<class T, class Compare>
void Heap<T, Compare>::Insert(const T &el) {
    buffer[0] = el;
    siftDown(0);
}

template<class T, class Compare>
void Heap<T, Compare>::Print() {
    for (int i = 0; i < size; ++i) {
        std::cout << buffer[i] << "\n";
    }
}

template<class T, class Compare>
void Heap<T, Compare>::siftDown(int i) {
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    while (left < size || right < size) {
        int min = i;
        if (left < size && cmp(buffer[left], buffer[i])) {
            min = left;
        }
        if (right < size && cmp(buffer[right], buffer[min])) {
            min = right;
        }
        if (min == i ) {
            return;
        }
        swap(buffer[i], buffer[min]);
        i = min;
        left = 2 * i + 1;
        right = 2 * i + 2;
    }
}


struct Node {
    Node* left;
    Node* right;
    int frequency;
    int simbol;
};



void CountSimbols(std::array<int,256> &simbols, std::vector<unsigned char> buffer) {
    for (elem : buffer) {
        simbols[(int)elem]++;
    }
}



// Метод архивирует данные из потока original
void Encode(IInputStream& original, IOutputStream& compressed) {
    std::vector<unsigned char> buffer = original.ReadToBuffer();
    std::array<int, 256> simbols ={0};
    CountSimbols(simbols, buffer);


}

// Метод восстанавливает оригинальные данные
void Decode(IInputStream& compressed, IOutputStream& original);


using std::vector;

class OutBitStream {
public:
    OutBitStream() = default;

    void WriteBit(unsigned char bit);
    void WriteByte(unsigned char byte);

    const unsigned char* GetBuffer() const {return buffer.data();}
    int GetBitsCount() const {return bitsCount;}
private:
    vector<unsigned char> buffer;
    int bitsCount = 0;
};

void OutBitStream::WriteBit(unsigned char bit) {
    if (bitsCount + 1 > buffer.size() * 8) {
        buffer.push_back(0);
    }

    if (bit != 0) {
        int bitPos = bitsCount % 8;
        buffer[bitsCount / 8] |= 1 << bitPos;
    }
    ++bitsCount;
}

void OutBitStream::WriteByte(unsigned char byte) {
    if (bitsCount % 8 == 0) {
        buffer.push_back(byte);
    } else {
        int offset = bitsCount % 8;
        buffer[bitsCount / 8] |= 1 << offset;
        buffer.push_back(byte >> (8 - offset));
    }
    bitsCount += 8;
}

//class InBitStream {
//public:
//    InBitStream(vector<unsigned char> buffer):buffer(std::move(buffer)){ };
//
//    unsigned char ReadBit();
//    unsigned char ReadByte();
//
//    const unsigned char* GetBuffer() const {return buffer.data();}
//    int GetBitsCount() const {return bitsCount;}
//private:
//    vector<unsigned char> buffer;
//    int bitsCount;
//};

