#include "ProductsClass.h"
#include <iostream>

int main(){
    int k;
    std::cout << "Сколько продуктов будет?\n";
    std::cin >> k;
    std::cout << "Формат ввода: Название Артикул Количество День Месяц Год\n";
    ProductsClass a(k);
    ProductsClass b = a;
    for(int i = 0; i < b.getCount(); i++){
        std::cout << b[i] << '\n';
    }
    std::cout << "------\n";
    Product p;
    p.name = "Сало";
    p.barcode = 12345;
    p.qnty = 100;
    p.expDate.d = 4; p.expDate.m = 7; p.expDate.y = 2020;
    b.addProduct(p);
    for(int i = 0; i < b.getCount(); i++){
        std::cout << b[i] << '\n';
    }
    b.deleteProduct(0);
    std::cout << "------\n";
    for(int i = 0; i < b.getCount(); i++){
        std::cout << b[i] << '\n';
    }
    return 0;
}