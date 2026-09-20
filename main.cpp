

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <thread>
#include <cassert>

#ifdef STM_USE_USTM
#include <ustm.h>
#endif

int balance_a = 100'000;
int balance_b = 100'000;

void transfer() {
    for (int i = 0; i < 100'000; ++i) {
        int amount = rand() % 10'000;
        bool a_to_b = rand() & 1;

#ifdef STM_USE_USTM
        ustm::transaction([&] {
            int from = ustm::load(a_to_b ? balance_a : balance_b);
            int to = ustm::load(a_to_b ? balance_b : balance_a);
            if (amount <= from) {
                ustm::store(a_to_b ? balance_a : balance_b, from - amount);
                ustm::store(a_to_b ? balance_b : balance_a, to + amount);
            }
            return true;
        });
#else
        __transaction_atomic {
            int& from = a_to_b ? balance_a : balance_b;
            int& to = a_to_b ? balance_b : balance_a;
            if (amount <= from) {
                from -= amount;
                to += amount;
            }
        }
#endif
    }
}

void reader() {
    for (int i = 0; i < 100'000; ++i) {
#ifdef STM_USE_USTM
        int t = ustm::transaction(
            [&] { return ustm::load(balance_a) + ustm::load(balance_b); }, true);
#else
        int t;
        __transaction_atomic { t = balance_a + balance_b; }
#endif
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
