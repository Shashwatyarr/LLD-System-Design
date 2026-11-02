#include <iomanip>
#include <iostream>
#include <memory>
#include <vector>

class Burger {
public:
    virtual ~Burger() = default;

    virtual const char* name() const = 0;
    virtual double price() const = 0;
    virtual void prepare() const = 0;
};

class BasicBurger : public Burger {
public:
    const char* name() const override {
        return "Basic Burger";
    }

    double price() const override {
        return 5.00;
    }

    void prepare() const override {
        std::cout << "Ingredients: bun, grilled patty, and lettuce\n";
    }
};

class StandardBurger : public Burger {
public:
    const char* name() const override {
        return "Standard Burger";
    }

    double price() const override {
        return 8.50;
    }

    void prepare() const override {
        std::cout << "Ingredients: bun, grilled patty, cheese, lettuce, and tomato\n";
    }
};

class PremiumBurger : public Burger {
public:
    const char* name() const override {
        return "Premium Burger";
    }

    double price() const override {
        return 13.00;
    }

    void prepare() const override {
        std::cout << "Ingredients: brioche bun, double patty, premium cheese, "
                     "caramelized onions, and signature sauce\n";
    }
};

class BurgerFactory {
public:
    virtual ~BurgerFactory() = default;

    virtual std::unique_ptr<Burger> createBurger() const = 0;

    void serveBurger() const {
        const std::unique_ptr<Burger> burger = createBurger();

        std::cout << burger->name() << '\n';
        burger->prepare();
        std::cout << "Price: $" << std::fixed << std::setprecision(2)
                  << burger->price() << "\n\n";
    }
};

class BasicBurgerFactory : public BurgerFactory {
public:
    std::unique_ptr<Burger> createBurger() const override {
        return std::unique_ptr<Burger>(new BasicBurger());
    }
};

class StandardBurgerFactory : public BurgerFactory {
public:
    std::unique_ptr<Burger> createBurger() const override {
        return std::unique_ptr<Burger>(new StandardBurger());
    }
};

class PremiumBurgerFactory : public BurgerFactory {
public:
    std::unique_ptr<Burger> createBurger() const override {
        return std::unique_ptr<Burger>(new PremiumBurger());
    }
};

int main() {
    std::vector<std::unique_ptr<BurgerFactory>> factories;
    factories.push_back(std::unique_ptr<BurgerFactory>(new BasicBurgerFactory()));
    factories.push_back(std::unique_ptr<BurgerFactory>(new StandardBurgerFactory()));
    factories.push_back(std::unique_ptr<BurgerFactory>(new PremiumBurgerFactory()));

    std::cout << "Factory Method Design Pattern - Burger Shop\n";
    std::cout << "--------------------------------------------\n";

    for (const std::unique_ptr<BurgerFactory>& factory : factories) {
        factory->serveBurger();
    }

    return 0;
}
