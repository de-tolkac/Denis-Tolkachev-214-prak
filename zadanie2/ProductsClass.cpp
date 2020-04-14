#include <iostream>

#include "ProductsClass.h"

std::ostream& operator<<(std::ostream& out, Date &d){ 
    return out << d.d << '.' << d.m << '.' << d.y;
}

std::ostream& operator<<(std::ostream& out, Product& p){
    return out << p.name << " " << p.qnty << " штук до " << p.expDate << ". Артикул: " << p.barcode;
}

ProductsClass::ProductsClass(int k){
    ProductsClass::count = k;
    ProductsClass::products = new Product[ProductsClass::count];
    for(int i = 0; i < ProductsClass::count; i++){
        std::cin >> ProductsClass::products[i].name;
        std::cin >> ProductsClass::products[i].barcode;
        std::cin >> ProductsClass::products[i].qnty;
        std::cin >> ProductsClass::products[i].expDate.d;
        std::cin >> ProductsClass::products[i].expDate.m;
        std::cin >> ProductsClass::products[i].expDate.y;
    }
}

ProductsClass::ProductsClass(const ProductsClass &pr){
    ProductsClass::count = pr.count;
    products = new Product[ProductsClass::count];
    for(int i = 0; i < pr.count; i++){
        ProductsClass::products[i] = pr.products[i];
    }
}

ProductsClass::~ProductsClass(){
    delete [] ProductsClass::products;
}

void ProductsClass::operator=(const ProductsClass &p){
    if(products) delete [] products;
    count = p.count;
    products = new Product[count];
    for(int i = 0; i < count; i++){
        products[i] = p.products[i];
    }
}

Product& ProductsClass::operator[](int k){
    return ProductsClass::products[k];
}

void ProductsClass::addProduct(Product newProduct){
    Product *newArr = new Product[ProductsClass::count + 1];
    for(int i = 0; i < ProductsClass::count; i++){
        newArr[i] = ProductsClass::products[i];
    }
    newArr[ProductsClass::count].name = newProduct.name;
    newArr[ProductsClass::count].qnty = newProduct.qnty;
    newArr[ProductsClass::count].barcode = newProduct.barcode;
    newArr[ProductsClass::count].expDate = newProduct.expDate;
    ProductsClass::count++;
    delete [] ProductsClass::products;
    products = newArr;
}

void ProductsClass::deleteProduct(int num){
    if((num <= ProductsClass::count)){
        Product *newArr = new Product[ProductsClass::count - 1];
        for(int i = 0; i < num; i++){
            newArr[i] = ProductsClass::products[i];
        }
        for(int i = num + 1; i < ProductsClass::count; i++){
            newArr[i - 1] = ProductsClass::products[i];
        }
        ProductsClass::count--;
        delete [] ProductsClass::products;
        products = newArr;
    }else{
        std::cout << "There is no such product\n";
    }
}

Product ProductsClass::findProductByName(std::string name) const{
    Product result;
    for(int i = 0; i < ProductsClass::count; i++){
        if(ProductsClass::products[i].name == name){
            result = ProductsClass::products[i];
            return result;
        }
    }
    return result;
}

int ProductsClass::getCount() const{
    return ProductsClass::count;
}