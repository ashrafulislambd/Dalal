#include "Vendor.h"
#include "CategoryManagerSingleton.h"

#include <plugin/sqlite3/Sqlite3.h>
#include <CtrlLib/CtrlLib.h>
using namespace Upp;

int Vendor::getProductCount() const
{
    return products.size();
}

vector<Product*> Vendor::getProducts() const {
    return products;
}

void Vendor::addProduct(Product* product)
{
    products.push_back(product);
}

int Vendor::getOrderCount() const
{
    return orders.size();
}

float Vendor::getAvgRating() const
{
    float totalRating = 0.0f;
    int count = 0;
    int noRating = 0;
    for(Product* product : products) {
        try {
            totalRating += getAvgRating();
            count++;
        } catch (Product::NoReviewException) {
            noRating++;
        }
    }
    if(count == 0) {
        throw NoReviewException();
    }
    return totalRating / count;
}

vector<Order*> Vendor::getOrders() const {
    return orders;
}

void Vendor::saveData(Sqlite3Session* conn, int vendor_id) {
	for(Product* product : products) {
		Sql	sql(*conn);
		sql.Execute("INSERT INTO product (id, name, description, price, image_path, category, vendor_id) VALUES (NULL, ?, ?, ?, ?, ?, ?)",
				product->name,
				product->description,
				product->getPrice(),
				product->imagePath,
				product->category->name,
				vendor_id);
	}
}

void Vendor::loadData(Sqlite3Session* conn, int vendor_id) {
	Sql sql(*conn);
	sql.Execute("SELECT * FROM product WHERE vendor_id=?", vendor_id);
	
	while(sql.Fetch()) {
		Product* product = new Product();
		product->name = sql[1].ToString().ToStd();
		product->description = sql[2].ToString().ToStd();
		product->setPrice(sql[3]);
		product->imagePath = sql[4].ToString().ToStd();
		
		Category* category = new Category();
		category->name = sql[5].ToString().ToStd();
		
		CategoryManagerSingleton::getInstance()
								->getCategoryManager()
								->addCategory(category);
	
		product->category = category;
		this->addProduct(product);
	}
}