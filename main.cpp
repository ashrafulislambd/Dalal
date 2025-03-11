#include <CtrlLib/CtrlLib.h>
#include <Core/Core.h>
#include <plugin/jpg/jpg.h>
#include <plugin/sqlite3/Sqlite3.h>

#define LAYOUTFILE <Dalal/design.lay>
#include <CtrlCore/lay.h>

#include "AuthHandler.h"
#include "User.h"
#include "Vendor.h"
#include "Customer.h"
#include "CategoryManagerSingleton.h"

using namespace Upp;

AuthHandler* authHandler;
CategoryManager* categoryManager;

Sqlite3Session* conn;

class ClientRegistrationDlg : public WithClientRegistration<TopWindow> {
public:
	ClientRegistrationDlg();
};

ClientRegistrationDlg::ClientRegistrationDlg()
{
	CtrlLayout(*this, "");
	
	this->txtPassword.Password(true);
	
	this->btnSubmit.WhenAction = [&] {
		string username = this->txtUserName.GetText().ToString().ToStd();
		string password = this->txtPassword.GetText().ToString().ToStd();
		string name = this->txtName.GetText().ToString().ToStd();
		
		try {
			authHandler->registerCustomer(username, name, password);
			PromptOK("Successfully Registered a new client");
		} catch (AuthHandler::UserAlreadyExistsException) {
			PromptOK("An user with this username already exists. Choose a different one.");
		}
	};
}

class VendorRegistrationDlg : public WithVendorRegistration<TopWindow> {
public:
	VendorRegistrationDlg();
};

VendorRegistrationDlg::VendorRegistrationDlg()
{
	CtrlLayout(*this, "");
	this->txtPassword.Password(true);
	
	this->btnSubmit.WhenAction = [&] {
		string username = this->txtUsername.GetText().ToString().ToStd();
		string password = this->txtPassword.GetText().ToString().ToStd();
		string name = this->txtName.GetText().ToString().ToStd();
		
		try {
			authHandler->registerVendor(username, name, password);
			PromptOK("Successfully Registered a new vendor");
		} catch (AuthHandler::UserAlreadyExistsException) {
			PromptOK("An user with this username already exists. Choose a different one.");
		}
	};
}

class ReviewDlg : public WithReview<TopWindow> {
private:
	Product* product;
public:
	ReviewDlg();
	void setProduct(Product* product);
};

ReviewDlg::ReviewDlg()
{
	CtrlLayout(*this, "");
	
	this->btnAdd.WhenAction = [&] {
		int rating = this->spinRating.GetData();
		string message = this->txtMessage.GetText().ToString().ToStd();
		
		Review* newReview = new Review();
		newReview->setRating(rating);
		newReview->setMessage(message);
		this->product->addReview(newReview);
		this->Close();
		this->Hide();
	};
}

void ReviewDlg::setProduct(Product* product) {
	this->product = product;	
}

class ShopDlg : public WithShop<TopWindow> {
private:
	vector<Product*> loadedProducts;
	Customer* user;
public:
	ShopDlg();
	void loadProducts();
	void loadDetails();
	void setUser(Customer* user);
	void renderCart();
};

void ShopDlg::loadDetails() {
	this->spinQty.SetData(1);	
	Product* currentProduct = loadedProducts[lstProducts.GetCursor()];
	////Upp::PromptOK(currentProduct->imagePath.c_str());
	try {
		Image img;
		img = StreamRaster::LoadFileAny(currentProduct->imagePath.c_str());
		this->imgProduct.SetImage(img);
	} catch (...) {
		PromptOK("Error Loading Image");
	}
	this->lblTitle.SetLabel(currentProduct->name.c_str());
	string reviewText = currentProduct->description;
	
	try {
		Product* currentProduct = loadedProducts[lstProducts.GetCursor()];
		reviewText += "\n\n\n";
		try {
			float avgRating = currentProduct->getAvgRating();
			reviewText += "Average Rating: " + to_string(avgRating) + "\n";
		} catch(Product::NoReviewException) {
			reviewText += "No reviews yet\n";
		}
		
		for(Review* review : currentProduct->getReviews()) {
			reviewText += to_string(review->getRating()) + ": ";
			reviewText += review->getMessage() + "\n";
		}
		
		this->lblDescription.SetLabel(reviewText.c_str());
	} catch(...) {}	
}

void ShopDlg::loadProducts() {
	this->lstProducts.ClearSelection();
	this->lstProducts.Clear();
	this->loadedProducts.clear();
	
	for(User* iuser : authHandler->getUsers()) {
		Vendor* vendor = dynamic_cast<Vendor*>(iuser);
		if(vendor == NULL)
			continue;
		for(Product* iproduct : vendor->getProducts()) {
			if(this->cmbCategory.GetData().ToString().IsEmpty()
				|| this->cmbCategory.GetData().ToString().ToStd() == iproduct->category->name) {
				if(lstProducts.Find(iproduct->name) < 0)
					this->lstProducts.Add(iproduct->name);
				this->loadedProducts.push_back(iproduct);
			}
		}		
	}
}

void ShopDlg::renderCart() {
	auto cartDetails = this->user->getCart();
	this->gridCart.Clear();
	float totalPrice = 0.0f;
	for(auto& [product, cnt] : cartDetails) {
		if(cnt) {
			gridCart.Add(product->name, cnt);
			totalPrice += product->getPrice() * cnt;
		}
	}
	
	this->lblPrice.SetLabel(("Total Price: " + to_string(totalPrice)).c_str());
}

void ShopDlg::setUser(Customer* user) {
	this->user = user;	
}

ShopDlg::ShopDlg()
{
	CtrlLayout(*this, "");
	
	set<Category*> categories = categoryManager->getCategories();
	for(Category* category : categories) {
		PromptOK(category->name.c_str());
		this->cmbCategory.Add(category->name);	
	}
	
	loadProducts();
	
	this->cmbCategory.WhenAction = [&] {
		loadProducts();
	};
	
	this->gridCart.AddColumn("Product");
	this->gridCart.AddColumn("Quantity");
	this->spinQty.SetData(1);
	
	this->lstProducts.WhenAction = [&] {
		loadDetails();
	};
	
	this->btnAddToCart.WhenAction = [&] {
		int qty = this->spinQty.GetData();
		while(qty--)
			this->user->addToCart(loadedProducts[lstProducts.GetCursor()]);
		renderCart();
	};
	
	this->btnPlaceOrder.WhenAction = [&] {
		map<Product*, int> myCart = this->user->getCart();
		if(myCart.empty()) {
			PromptOK("Your cart is empty!");
		} else {
			this->user->makeOrder();
			PromptOK("Your order has been placed");
			renderCart();	
		}
	};
	
	this->btnReviews.WhenAction = [&] {
		ReviewDlg* review = new ReviewDlg();
		review->setProduct(loadedProducts[lstProducts.GetCursor()]);
		review->RunAppModal();
		//PromptOK("Here");
		loadDetails();
	};
}

class AddnewProductDlg : public WithAddnewProduct<TopWindow> {
private:
	Vendor* user;
	
public:
	AddnewProductDlg();
	void setUser(Vendor* user);
};

void AddnewProductDlg::setUser(Vendor* user) {
	this->user = user;
}

AddnewProductDlg::AddnewProductDlg()
{
	CtrlLayout(*this, "");
	
	this->txtImage.Disable();
	
	this->btnImage.WhenAction = [&] {
		FileSel fs;
		fs.Type("Image Files", "*.bmp;*.png;*.jpg;*.jpeg");
		if(fs.ExecuteOpen("Choose Image")) {
			this->txtImage.SetText(~fs);
		}	
	};
	
	this->btnAdd.WhenAction = [&] {
		Product* product = new Product();
		product->name = this->txtName.GetText().ToString();
		product->description = this->txtDescription.GetText().ToString();
		product->imagePath = this->txtImage.GetText().ToString();
		product->setPrice(this->txtPrice.GetData());
		
		
		Category* category = new Category();
		category->name = this->txtCategory.GetText().ToString();
		product->category = category;
		try {
			categoryManager->addCategory(category);
		} catch (...) {}
		user->addProduct(product);
		this->Hide();
		this->Close();
	};
}

class SellDlg : public WithSell<TopWindow> {
private:
	Vendor* user;
	vector<Product*> loadedProducts;
	
public:
	SellDlg();
	
	void setUser(Vendor* user);
	void loadProducts();
	void loadCategories();
};

SellDlg::SellDlg()
{
	CtrlLayout(*this, "");
	
	this->gridProducts.AddColumn("Name");
	this->gridProducts.AddColumn("Description");
	this->gridProducts.AddColumn("Price");
	this->gridProducts.AddColumn("Category");
	
	this->cmbCategory.WhenAction = [&] {
		loadProducts();	
	};
	
	this->btnAdd.WhenAction = [&] {
		AddnewProductDlg* dlgNew = new AddnewProductDlg();
		dlgNew->setUser(this->user);
		dlgNew->RunAppModal();
		loadCategories();
		loadProducts();
	};
}

void SellDlg::setUser(Vendor* user) {
	this->user = user;	
}

void SellDlg::loadCategories() {
	set<string> categories;
	loadedProducts.clear();
	this->gridProducts.Clear();
	this->cmbCategory.Clear();
	
	for(Product* product : user->getProducts()) {
		categories.insert(product->category->name);	
	}
	for(string categoryText : categories) {
		this->cmbCategory.Add(categoryText);
	}	
}

void SellDlg::loadProducts() {
	loadedProducts.clear();
	this->gridProducts.Clear();
	
	for(Product* product : user->getProducts()) {
		if(this->cmbCategory.GetData().ToString().IsEmpty()
				|| this->cmbCategory.GetData().ToString().ToStd() == product->category->name) {
			loadedProducts.push_back(product);
			this->gridProducts.Add(product->name, product->description, 
				product->getPrice(), product->category->name);
		}
	}
}

class LogInFormDlg : public WithLogInForm<TopWindow> {
public:
	LogInFormDlg();
};

LogInFormDlg::LogInFormDlg()
{
	CtrlLayout(*this, "");
	
	this->txtPassword.Password(true);
	
	this->btnLogin.WhenAction = [&] {
		string username = this->txtUserName.GetText().ToString().ToStd();
		string password = this->txtPassword.GetText().ToString().ToStd();
		
		try {
			User* user = authHandler->loginUser(username, password);
			Vendor* vendor = dynamic_cast<Vendor*>(user);
			Customer* customer = dynamic_cast<Customer*>(user);
			if(vendor) {
				SellDlg* sell = new SellDlg();
				sell->setUser(vendor);
				sell->loadCategories();
				sell->loadProducts();
				sell->Run();
			} else if(customer) {
				ShopDlg* shop = new ShopDlg();
				shop->setUser(customer);
				shop->Run();
			}
		} catch(AuthHandler::InvalidCredentialsException) {
			Upp::PromptOK("You have provided wrong credentials!");	
		}
	};
	
	this->btnRegisterClient.WhenAction = [&] {
		ClientRegistrationDlg* client = new ClientRegistrationDlg();
		client->Run();	
	};
	
	this->btnRegisterVendor.WhenAction = [&] {
		VendorRegistrationDlg* vendor = new VendorRegistrationDlg();
		vendor->Run();	
	};
}

void testing() {
	try {
		authHandler->registerVendor("wasi", "wasi", "234");
		authHandler->registerCustomer("kasi", "kasi", "234");
	} catch (AuthHandler::UserAlreadyExistsException) {
		PromptOK("User already exists! testing failed");
		//return;	
	}
	Vendor* wasi;
	try {
		wasi = (Vendor*)authHandler->loginUser("wasi", "234");
	} catch (AuthHandler::InvalidCredentialsException) {
		PromptOK("Invalid Credentials! Testing failed");
		return;	
	}
	
	Category* category = new Category();
	category->name = "Car";
	
	Category* category2 = new Category();
	category2->name = "Bike";
	categoryManager->addCategory(category);
	categoryManager->addCategory(category2);
	
	Product* product = new Product();
	product->name = "Car 1";
	product->description = "Best car in Bangladesh";
	product->imagePath = "...";
	product->category = category;
	product->imagePath = "D:\\\\img\\\\s21ultra.jpg";
	product->setPrice(100);
	
	wasi->addProduct(product);
	
	Product* product2 = new Product();
	product2->name = "Honda";
	product2->description = "Best bike in Bangladesh";
	product2->imagePath = "...";
	product2->category = category2;
	product2->imagePath = "D:\\\\img\\\\mac16jpg.jpg";
	product2->setPrice(10);
	
	wasi->addProduct(product2);
}

class MainWindowDlg : public WithMainWindow<TopWindow> {
public:
	MainWindowDlg(){
		CtrlLayout(*this, "Welcome to Dalal");
		
		this->btnLogin.WhenAction = [&] {
			LogInFormDlg* login = new LogInFormDlg();
    		login->Run();
		};
	}
};



GUI_APP_MAIN {
	authHandler = new AuthHandler();
	categoryManager = new CategoryManager();
	CategoryManagerSingleton::getInstance()->setCategoryManager(categoryManager);
	
	conn = new Sqlite3Session();
	
	if(!conn->Open("main.db")) {
		PromptOK("Error loading / creating database.");
	}
	
	authHandler->loadUsers(conn);
	
	//testing();
	
	MainWindowDlg m;
	m.Run();
    
    authHandler->saveUsers(conn);
}
