#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
    if (len <= 0) throw FAKE_INT;
    BitLen = len;
    MemLen = (len / (sizeof(TELEM) * 8)) + 1;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[BitLen];
    for (int i = 0; i < MemLen; i++) 
    {
        pMem[i] = bf.pMem[i];
    }

}

TBitField::~TBitField()
{
    delete[]pMem;

}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if ((n < 0)||(n>=BitLen))
    {
        throw FAKE_INT;
    }
    return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if ((n < 0) || (n >= BitLen))
    {
        throw FAKE_INT;
    }

    return 1<<(n%(sizeof(TELEM)*8));

}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if ((n < 0) || (n >= BitLen)) 
    {
        throw FAKE_INT;
    }
    pMem[GetMemIndex(n)] |= GetMemMask(n);

}

void TBitField::ClrBit(const int n) // очистить бит
{
    if ((n < 0) || (n >= BitLen))
    {
        throw FAKE_INT;
    }
    TELEM mask = ~GetMemMask(n);
    pMem[GetMemIndex(n)] &= mask;

}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if ((n < 0) || (n >= BitLen))
    {
        throw FAKE_INT;
    }
    TELEM mask = GetMemMask(n);
    if (pMem[GetMemIndex(n)] & mask)
    {
          return 1;
    }
    else 
    {
        return 0;
    }
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this == &bf) 
    {
        return *this;
    }
    if (MemLen != bf.MemLen) 
    {
        delete[]pMem;
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
    }
    BitLen = bf.BitLen;
    for (int i = 0; i < MemLen; i++) 
    {
        pMem[i] = bf.pMem[i];
    }
    return *this;

}

int TBitField::operator==(const TBitField &bf) const  // сравнение
{
    if (this->BitLen != bf.BitLen) 
    {
        return 0;
    }
    for (size_t i = 0; i < MemLen - 1; i++) //полные части памяти
    {
        if (this->pMem[i] != bf.pMem[i]) {
            return 0;
        }
    }
    for (size_t i = (MemLen - 1) * sizeof(TELEM) * 8; i < BitLen; i++) 
    {
        if (this->GetBit(i) != bf.GetBit(i)) 
        {
            return 0;
        }
    }

  return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    if (this == &bf) 
    {
        return 0;
    }
    else 
    {
        return 1;
    }
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int mxbit = bf.BitLen;
    int smem = this->MemLen;
    if (this->BitLen > bf.BitLen) 
    {
        mxbit = this->BitLen;
        smem = bf.MemLen;
    }

    TBitField res(mxbit);

    int i;
    for (i = 0; i < smem; i++) {
        res.pMem[i] = bf.pMem[i] | this->pMem[i];
    }

    if (this->MemLen > bf.MemLen) {
        while (i != this->MemLen) {
            res.pMem[i] = this->pMem[i];
            i++;
        }
    }
    else {
        while (i != bf.MemLen) {
            res.pMem[i] = bf.pMem[i];
            i++;
        }
    }
    return res;

}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int mxbit = bf.BitLen;
    int smem = this->MemLen;
    if (this->BitLen > bf.BitLen)
    {
        mxbit = this->BitLen;
        smem = bf.MemLen;
    }

    TBitField res(mxbit);

    for (int i = 0; i < smem; i++) 
    {
        res.pMem[i] = bf.pMem[i] & this->pMem[i];
    }
    return res;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField res(this->BitLen);
    for (size_t i = 0; i < (this->MemLen - 1); i++)
    {
        res.pMem[i] = ~this->pMem[i];
    }
    for (size_t i = (this->MemLen - 1) * sizeof(TELEM) * 8; i < this->BitLen; i++)
    {
        if (this->GetBit(i)) 
        { 
            res.ClrBit(i); 
        }
        else 
        { 
            res.SetBit(i);
        }
    }
    return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    int bit;
    for (size_t i = 0; i < bf.BitLen; i++)
    {
        istr >> bit;
        if (bit) bf.SetBit(i);
        else bf.ClrBit(i);
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (size_t i = 0; i < bf.BitLen; i++)
    {
        ostr << bf.GetBit(i);
    }
    return ostr;
}
