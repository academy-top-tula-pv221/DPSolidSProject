#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class Report
{
    string text;
public:
    void Create()
    {
        text = "my report";
    }
    void GoBegin()
    {
        cout << "go to begin of report\n";
    }
    void GoEnd()
    {
        cout << "go to end of report\n";
    }
    void GoPage(int page)
    {
        cout << "go to " << page << " page of report\n";
    }
    
    string& Text() { return text; }
};
class ReportPrint
{
public:
    void ConsolePrint(Report report)
    {
        cout << "report: " << report.Text() << "\n";
    }
};

class Product
{
    string title;
    int price;
public:
    string& Title(){ return title; }
    int& Price() { return price; }
    Product(string title, int price) 
        : title{ title }, price{ price } {}
};

class ProductStore
{
    vector<Product> products;
public:
    vector<Product>& Products(){ return products; }
    
    void Process()
    {
        // input product:
        cout << "input title: ";
        string title;
        cin >> title;
        cout << "input price: ";
        int price;
        cin >> price;

        // validation
        if (price < 0)
        {
            cout << "incorrect price!\n";
            return;
        }

        // add to vector
        products.push_back(Product(title, price));
        cout << "product " << title << " add to store\n";

        // save vector to file
            
    }
};

class IProductReader
{
public:
    virtual string* GetData() = 0;
};

class ProductConsoleReader : public IProductReader
{
public:
    string* GetData() override
    {
        string* data = new string[2];
        cout << "input title: ";
        cin >> data[0];
        cout << "input price: ";
        cin >> data[1];
        return data;
    }
};

class IProductCreator
{
public:
    virtual Product GetProduct(string* data) = 0;
};

class ProductSimpleCreate : public IProductCreator
{
public:
    Product GetProduct(string* data) override
    {
        int price;
        try
        {
            price = stoi(data[1]);
        }
        catch (exception e)
        {
            cout << e.what() << "\n";
            throw e;
        }
        return Product(data[0], price);
    }
};

class IProductValidator
{
public:
    virtual bool IsValid(Product product) = 0;
};

class ProductPriceValidator : public IProductValidator
{
public:
    bool IsValid(Product product) override
    {
        return product.Price() >= 0;
    }
};

class IProductSave
{
public:
    virtual void Save(Product product, string name) = 0;
};

class ProductFileSaver : public IProductSave
{
    void Save(Product product, string name) override
    {
        fstream file;
        file.open(name, ios::app);
        file << "product title: " << product.Title() << "\n";
        file << "product price: " << product.Price() << "\n";
        file.close();
    }
};



class ProductStoreGood
{
    vector<Product> products;

public:
    vector<Product>& Products() { return products; }

    IProductReader* Reader;
    IProductCreator* Creator;
    IProductValidator* Validator;
    IProductSave* Saver;

    ProductStoreGood(IProductReader* reader,
        IProductCreator* creator,
        IProductValidator* validator,
        IProductSave* saver)
        : Reader{ reader }, Creator{ creator }, Validator{ validator }, Saver{ saver } {}
    
    void Process()
    {
        string* data = new string[2];
        data = Reader->GetData();
        Product product = Creator->GetProduct(data);

        if (Validator->IsValid(product))
        {
            products.push_back(product);
            Saver->Save(product, "file.dat");
            cout << "product add\n";
        }
        else
            cout << "product not add\n";
    }
};

int main()
{
    ProductStoreGood store(new ProductConsoleReader(), new ProductSimpleCreate(),
                           new ProductPriceValidator(), new ProductFileSaver());

    store.Process();
}
