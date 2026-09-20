

#include <cstdlib>
#include <iostream>
#include <thread>
#include <cassert>

int balance_a = 100'000;
int balance_b = 100'000;

void transfer() {
    for (int i = 0; i < 100'000; ++i) {
        int amount = rand() % 10'000;
        bool a_to_b = rand() & 1;

        __transaction_atomic {
            if (a_to_b) {
                if (amount <= balance_a) {
                    balance_a -= amount;
                    balance_b += amount;
                }
            } else {
                if (amount <= balance_b) {
                    balance_a += amount;
                    balance_b -= amount;
                }
            }
        }
    }
}

void reader() {
    for (int i = 0; i < 100'000; ++i) {
        int t;
        __transaction_atomic {
            t = balance_a + balance_b;
        }
        assert(t == 200'000);
    }
}

int main() {
    std::thread a(transfer);
    std::thread b(transfer);
    std::thread c(reader);

    a.join();
    b.join();
    c.join();

    std::cout << balance_a + balance_b << '\n'; // Always 200000.
}
