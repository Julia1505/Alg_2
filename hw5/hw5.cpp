//Задача 5. Алгоритм сжатия данных Хаффмана
//        (6 баллов и более)
//Напишите две функции для создания архива из одного файла и извлечения файла из архива.
//Требования: коэффициент сжатия < 1.
//В архиве сохраняйте дерево Хаффмана и код Хаффмана от исходных данных.
//Дерево Хаффмана требуется хранить эффективно - не более 10 бит на каждый 8-битный символ.
//В контест необходимо отправить .cpp файл содержащий функции Encode, Decode, а также включающий файл Huffman.h. Тестирующая программа выводит размер сжатого файла в процентах от исходного.
//
//Лучшие 3 решения с коэффициентом сжатия < 1 из каждой группы оцениваются в 10, 7 и 4 баллов соответственно.
//

#include <iostream>
#include <utility>
#include <vector>

typedef unsigned char byte;

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
// Метод архивирует данные из потока original
void Encode(IInputStream& original, IOutputStream& compressed);
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

