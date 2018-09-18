#include <iostream>
using namespace std;

#pragma pack(push, 1)
struct struct1 {
	unsigned short	bfType;
	unsigned long	bfSize;
	unsigned short	bfReserved1;
	unsigned short	bfReserved2;
	unsigned long	bfOffBits;
};
#pragma pack(pop)

#pragma pack(1)
#pragma pack(push)
struct struct2 {
	unsigned short	bfType;
	unsigned long	bfSize;
	unsigned short	bfReserved1;
	unsigned short	bfReserved2;
	unsigned long	bfOffBits;
};
#pragma pack(pop)

struct __attribute__((packed)) struct3 {
	unsigned short	bfType;
	unsigned long	bfSize;
	unsigned short	bfReserved1;
	unsigned short	bfReserved2;
	unsigned long	bfOffBits;
};

struct struct4 {
	unsigned short	bfType;
	unsigned long	bfSize;
	unsigned short	bfReserved1;
	unsigned short	bfReserved2;
	unsigned long	bfOffBits;
} __attribute__((packed));

// clang++ padding.cpp && ./a.out
int	main() {
	cout << "struct size\t" << sizeof(struct1) << endl;
	cout << "bfType     \t" << offsetof(struct1, bfType) << endl;
	cout << "bfSize     \t" << offsetof(struct1, bfSize) << endl;
	cout << "bfReserved1\t" << offsetof(struct1, bfReserved1) << endl;
	cout << "bfReserved2\t" << offsetof(struct1, bfReserved2) << endl;
	cout << "bfOffBits  \t" << offsetof(struct1, bfOffBits) << endl;
	cout << "---" << endl;

	cout << "struct size\t" << sizeof(struct2) << endl;
	cout << "bfType     \t" << offsetof(struct2, bfType) << endl;
	cout << "bfSize     \t" << offsetof(struct2, bfSize) << endl;
	cout << "bfReserved1\t" << offsetof(struct2, bfReserved1) << endl;
	cout << "bfReserved2\t" << offsetof(struct2, bfReserved2) << endl;
	cout << "bfOffBits  \t" << offsetof(struct2, bfOffBits) << endl;
	cout << "---" << endl;

	cout << "struct size\t" << sizeof(struct3) << endl;
	cout << "bfType     \t" << offsetof(struct3, bfType) << endl;
	cout << "bfSize     \t" << offsetof(struct3, bfSize) << endl;
	cout << "bfReserved1\t" << offsetof(struct3, bfReserved1) << endl;
	cout << "bfReserved2\t" << offsetof(struct3, bfReserved2) << endl;
	cout << "bfOffBits  \t" << offsetof(struct3, bfOffBits) << endl;
	cout << "---" << endl;
	
	cout << "struct size\t" << sizeof(struct4) << endl;
	cout << "bfType     \t" << offsetof(struct4, bfType) << endl;
	cout << "bfSize     \t" << offsetof(struct4, bfSize) << endl;
	cout << "bfReserved1\t" << offsetof(struct4, bfReserved1) << endl;
	cout << "bfReserved2\t" << offsetof(struct4, bfReserved2) << endl;
	cout << "bfOffBits  \t" << offsetof(struct4, bfOffBits) << endl;
	cout << "---" << endl;
	return (0);
}