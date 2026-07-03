#pragma once
#include <afx.h>

// ==============================
// Model: Book
// ==============================
struct Book
{
    long          ID;
    CString       Name;
    double        Price;
    int           Qty;
    COleDateTime  CreatedDate;

    Book()
        : ID(0), Price(0.0), Qty(0)
    {
    }
};
