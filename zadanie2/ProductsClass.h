#ifndef __PRODUCTS_CLASS__
#define __PRODUCTS_CLASS__

#include <string>

struct Date{
    int d;
    int m;
    int y;
    friend std::ostream& operator<<(std::ostream& out, Date& d);
};

struct Product{
    std::string name;
    int barcode;
    int qnty;
    Date expDate;
    friend std::ostream& operator<<(std::ostream& out, Product& p);
};

class ProductsClass{
private:
    int count;
    Product *products;
public:
    ProductsClass(int k);
    ProductsClass(const ProductsClass &stClass);
    Product& operator[](int k);
    void operator=(const ProductsClass &prdct);
    ~ProductsClass();
    void addProduct(Product newProduct);
    void deleteProduct(int num);
    Product findProductByName(std::string name) const;
    int getCount() const;
};
#endif