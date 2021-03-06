// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

#define BITS_IN_ONE_MEM (sizeof(TELEM) * 8)

TBitField::TBitField(int len)
{
	if (len < 0)
		throw "Negative length of array";
	BitLen = len;
	MemLen = (BitLen / BITS_IN_ONE_MEM) + 1;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	if (MemLen == 0)
		throw "Zero length of BitLen";
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if (n > BitLen)
		throw "Index out of array";
	if (n < 0)
		throw "Index if negative";
	int temp;
	temp = n / BITS_IN_ONE_MEM;
	return temp;
	//return 0; // 70-ому биту соответствует 2 число
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if (n > BitLen)
		throw "Index out of array";
	if (n < 0)
		throw "Index if negative";
	TELEM temp = 1 << (n % BITS_IN_ONE_MEM);
	return temp;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n > BitLen)
		throw "Index out of array";
	if (n < 0)
		throw "Index if negative";
	int k = GetMemIndex(n);
	pMem[k] = pMem[k] | GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n > BitLen)
		throw "Index out of array";
	if (n < 0)
		throw "Index if negative";
	TELEM mask = GetMemMask(n);
	int k = GetMemIndex(n);
	pMem[k] &= ~mask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	int k = GetMemIndex(n);
	/*if (k > BitLen)
	throw "Index out of array";
	if (k < 0)
	throw "Index if negative";*/
	TELEM temp = pMem[k] & GetMemMask(n);
	if (temp > 0) return 1; else return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
	return *this;

}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (MemLen != bf.MemLen)
		return 0;
	else
		if (BitLen != bf.BitLen)
			return 0;

	for (int i = 0; i < MemLen; i++)
		if (pMem[i] != bf.pMem[i])
			return 0;
	/*for (int i = (MemLen - 1)*BITS_IN_ONE_MEM; i > 0; i--)
	{
	int a, b;
	a = GetBit(i); b = bf.GetBit(i);
	if (a != b)  return 0;

	}*/
	return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	if ((*this) == bf) return 0;
	else return 1;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int min = (BitLen > bf.BitLen) ? bf.BitLen : BitLen;
	int max = (BitLen < bf.BitLen) ? bf.BitLen : BitLen;
	int minMemLen = (MemLen > bf.MemLen) ? bf.MemLen : MemLen;
	TBitField res(max);

	for (int i = 0; i < minMemLen; i++)
		res.pMem[i] = pMem[i] | bf.pMem[i];

	if (min = bf.BitLen)
	{
		for (int i = minMemLen; i < res.MemLen; i++)
			res.pMem[i] = pMem[i];
	}
	else
		for (int i = minMemLen; i < res.MemLen; i++)
			res.pMem[i] = bf.pMem[i];

	return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int min = (BitLen > bf.BitLen) ? bf.BitLen : BitLen;
	TBitField res(min);
	for (int i = 0; i < res.MemLen; i++)
		res.pMem[i] = pMem[i] & bf.pMem[i];
	for (int i = (res.MemLen - 1)*BITS_IN_ONE_MEM; i < res.BitLen; i++)
	{
		int a = GetBit(i);
		int b = bf.GetBit(i);
		if (a + b == 2) res.SetBit(i);
	}
	return res;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField res(BitLen);

	for (int i = 0; i < BitLen; i++)
	{
		if (GetBit(i) == 0)
			res.SetBit(i);
	}

	return res;

	//TBitField res(BitLen);
	//for (int i = 0; i < res.MemLen - 1; i++)
	//	res.pMem[i] = ~pMem[i];
	//for (int i = (res.MemLen - 1)*BITS_IN_ONE_MEM; i < res.BitLen; i++)
	//{
	//	int a = GetBit(i);
	//	if (a == 0) 
	//		res.SetBit(i);
	//}
	//return res;

}

// ввод/вывод

istream &operator >> (istream &istr, TBitField &bf) // ввод
{
	int k;
	cin >> k;
	while (k > -1)
	{
		bf.SetBit(k);
		cin >> k;
	}

	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	cout << "{";
	for (int k = 0; k < bf.BitLen; k++)
		cout << bf.GetBit(k);
	cout << "}\n";
	return ostr;
}
